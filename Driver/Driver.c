#include <ntddk.h>

// Структура даних, пов'язана з об'єктом пристрою
typedef struct {
    PDEVICE_OBJECT LowerKbdDevice;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

// Структура даних, яка відображає введену інформацію з клавіатури
typedef struct _KEYBOARD_INPUT_DATA {
    USHORT UnitId;
    USHORT MakeCode;
    USHORT Flags;
    USHORT Reserved;
    USHORT ExtraInformation;
} KEYBOARD_INPUT_DATA, * PKEYBOARD_INPUT_DATA;

// Об'єкт пристрою
PDEVICE_OBJECT myKbdDevice;

// Кількість необроблених клавіш
ULONG pendingKey = 0;

// Процедура вивантаження драйвера
VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
    LARGE_INTEGER interval = { 0 };

    PDEVICE_OBJECT DeviceObject = DriverObject->DeviceObject;

    interval.QuadPart = -10 * 1000 * 1000;

    IoDetachDevice(((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->LowerKbdDevice);

    while (pendingKey) {
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }

    IoDeleteDevice(myKbdDevice);
    KdPrint(("Driver unload called successfully"));
}

 // Процедура завершення обробки IRP-пакета IRP_MJ_READ
NTSTATUS ReadComplete(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context) {
    UNREFERENCED_PARAMETER(Context);
    UNREFERENCED_PARAMETER(DeviceObject);

    CHAR* keyFlag[4] = { "KeyDown", "KeyUp", "E0", "E1" };
    PKEYBOARD_INPUT_DATA Keys = (PKEYBOARD_INPUT_DATA)Irp->AssociatedIrp.SystemBuffer;

    if (Irp->IoStatus.Status == STATUS_SUCCESS) {
        for (int i = 0; i < Irp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA); i++) {
            KdPrint(("The scan code of pressed button is: %x (%s)\n", Keys[i].MakeCode, keyFlag[Keys[i].Flags]));
        }
    }

    if (Irp->PendingReturned) {
        IoMarkIrpPending(Irp);
    }

    pendingKey--;
    return Irp->IoStatus.Status;
}

// Процедура обробки пакетів IRP_MJ_READ
NTSTATUS DispatchRead(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    IoCopyCurrentIrpStackLocationToNext(Irp);

    IoSetCompletionRoutine(Irp, ReadComplete, NULL, TRUE, TRUE, TRUE);

    pendingKey++;

    return IoCallDriver(((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->LowerKbdDevice, Irp);
}

// Процедура обробки всіх пакетів IRP, окрім IRP_MJ_READ, IRP_MJ_CREATE, IRP_MJ_CLOSE
NTSTATUS DispatchPass(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    IoCopyCurrentIrpStackLocationToNext(Irp);
    return IoCallDriver(((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->LowerKbdDevice, Irp);
}

// Ініціалізація пристрою
NTSTATUS MyAttachDevice(PDRIVER_OBJECT DriverObject) {
    NTSTATUS status;
    UNICODE_STRING higherDevice = RTL_CONSTANT_STRING(L"\\Device\\HigherKeyboardClass0");
    UNICODE_STRING TargetDevice = RTL_CONSTANT_STRING(L"\\Device\\KeyboardClass0");

    status = IoCreateDevice(DriverObject, sizeof(DEVICE_EXTENSION), &higherDevice, 32769, 0, FALSE, &myKbdDevice);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    myKbdDevice->Flags |= DO_BUFFERED_IO;
    myKbdDevice->Flags &= ~DO_DEVICE_INITIALIZING;

    RtlZeroMemory(myKbdDevice->DeviceExtension, sizeof(DEVICE_EXTENSION));

    status = IoAttachDevice(myKbdDevice, &TargetDevice, &((PDEVICE_EXTENSION)myKbdDevice->DeviceExtension)->LowerKbdDevice);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(myKbdDevice);
        return status;
    }

    return STATUS_SUCCESS;
}

// Процедура обробки пакетів IRP_MJ_CREATE
NTSTATUS OnCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    KdPrint(("OnCreate completed successfully\n"));
    return STATUS_SUCCESS;
}

// Процедура обробки пакетів IRP_MJ_CLOSE
NTSTATUS OnClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    KdPrint(("OnClose completed successfully\n"));
    return STATUS_SUCCESS;
}

// Вхідна точка у драйвер
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;
    int i;
    DriverObject->DriverUnload = DriverUnload;

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++) {
        DriverObject->MajorFunction[i] = DispatchPass;
    }

    DriverObject->MajorFunction[IRP_MJ_READ] = DispatchRead;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = OnCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = OnClose;

    status = MyAttachDevice(DriverObject);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Attaching device is failling with status 0x%X", status));
    }
    else {
        KdPrint(("Attaching device succeeds"));
    }

    KdPrint(("Driver has been loaded successfully"));
    return STATUS_SUCCESS;
}