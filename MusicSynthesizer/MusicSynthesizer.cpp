#include "MusicSynthesizer.h"

#pragma comment(lib, "WinMM.Lib")

#define MAX_LOADSTRING 100

// Глобальні змінні:
HINSTANCE hInst;                                // поточний екземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст рядка заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // ім'я класу головного вікна
HWND hPiano;
HANDLE hDevice;

// Надіслати оголошення функцій, включених до цього модуль коду:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Ініціалізація глобальних рядків:
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MUSICSYNTHESIZER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Виконати ініціалізацію програми:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MUSICSYNTHESIZER));

    MSG msg;

    // Цикл основного повідомлення:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦІЯ: MyRegisterClass()
//
//  ЦІЛЬ: Реєструє клас вікна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(130));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(235, 235, 235));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MUSICSYNTHESIZER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(130));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦІЯ: InitInstance(HINSTANCE, int)
//
//   ЦІЛЬ: Зберігає маркер екземпляра та створює головне вікно
//
//   КОМЕНТАРІ:
//
//        У цій функції маркер екземпляра зберігається в глобальній змінній, а також
//        створюється та виводиться головне вікно програми.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Зберегти маркер екземпляра у глобальній змінній

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        550, 330, 735, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ФУНКЦІЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦІЛЬ: Обробляє повідомлення у головному вікні.
//
//  WM_COMMAND  - обробити меню програми
//  WM_PAINT    - Відображення головного вікна
//  WM_DESTROY  - надіслати повідомлення про вихід та повернутися
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Групбокси
    static HWND hGroupBoxOptions;
    static HWND hGroupBoxPatchesAndIntruments;
    static HWND hGroupBoxParameters;

    // Випадаючі списки
    static HWND hDropDownListDevices;
    static HWND hDropDownListPatches;

    // Чекбокси
    static HWND hCheckboxShowNotes;
    static HWND hCheckboxShowKeys;

    // Листбокси
    static HWND hListboxInstruments;

    // Вікна статичного тексту
    static HWND hStaticOctave;
    static HWND hStaticCurrentOctave;
    static HWND hStaticVolume;
    static HWND hStaticBalance;
    static HWND hStaticVibrato;
    static HWND hStaticPitch;

    // Трекбари
    static HWND hTrackbarVolume;
    static HWND hTrackbarBalance;
    static HWND hTrackbarVibrato;
    static HWND hTrackbarPitch;

    // Кнопки
    static HWND hButtonConnectToDriver;
    static HWND hButtonDisconnectDriver;

    switch (message)
    {

    case WM_CREATE:
    {

        if (midiOutOpen(&g_kbd.hMidi, -1, 0, 0, 0) != MMSYSERR_NOERROR) {
            MessageBox(hWnd, L"midiOutOpen Failed!", L"Error", MB_OK);
            return 0;
        }

        g_kbd.hWnd = hWnd;
        g_kbd.hInst = hInst;
        g_kbd.KeyText = 1;
        g_kbd.NoteText = 1;
        g_kbd.octave = 4;
        g_kbd.volume = 127;
        g_kbd.instrument = 0;
        g_kbd.pitch = 0;
        g_kbd.vibrato = 0;
        g_kbd.balance = 64;
        g_kbd.chStop = 0x80;
        g_kbd.chPlay = 0x90;
        g_kbd.chControl = 0xB0;
        g_kbd.chInstr = 0xC0;
        g_kbd.chBend = 0xE0;
        g_kbd.NoteText = false;
        g_kbd.KeyText = false;
        SetInstrument(g_kbd.hMidi, g_kbd.chInstr, g_kbd.instrument);
        midiOutSetVolume(g_kbd.hMidi, g_kbd.volume);
        SetChannelBalance(g_kbd.hMidi, g_kbd.chControl, g_kbd.balance);
        SetSustain(g_kbd.hMidi, g_kbd.chControl, 0);

        g_kbd.hFont = MakeFontEx(0, L"Arial", 12, 0, FW_BOLD, 0, 0);
        SendMessage(hWnd, WM_SETFONT, (WPARAM)g_kbd.hFont, NULL);

        hGroupBoxOptions = CreateWindowEx(NULL, L"button", L"Options", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            10, 3, 235, 276, hWnd, NULL, hInst, NULL);
        SendMessage(hGroupBoxOptions, WM_SETFONT, (WPARAM)g_kbd.hFont, NULL);

        hCheckboxShowNotes = CreateWindow(L"button", L"Show note text", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
            20, 110, 135, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hCheckboxShowNotes, WM_SETFONT, (WPARAM)g_kbd.hFont, NULL);

        hCheckboxShowKeys = CreateWindow(L"button", L"Show key text", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
            20, 145, 130, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hCheckboxShowKeys, WM_SETFONT, (WPARAM)g_kbd.hFont, NULL);

        hDropDownListDevices = CreateWindowEx(NULL, L"combobox", NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
            20, 30, 220, 160, hWnd, NULL, hInst, NULL);
        SendMessage(hDropDownListDevices, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);
        ListDevices(hDropDownListDevices);

        hGroupBoxPatchesAndIntruments = CreateWindowEx(NULL, L"button", L"Patches/Instruments", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            250, 3, 200, 276, hWnd, NULL, hInst, NULL);
        SendMessage(hGroupBoxPatchesAndIntruments, WM_SETFONT, (WPARAM)g_kbd.hFont, NULL);

        hDropDownListPatches = CreateWindowEx(NULL, L"combobox", NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
            260, 30, 180, 290, hWnd, NULL, hInst, NULL);
        SendMessage(hDropDownListPatches, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);
        ListPatches(hDropDownListPatches);

        hListboxInstruments = CreateWindowEx(WS_EX_CLIENTEDGE, L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
            260, 60, 180, 200, hWnd, NULL, hInst, NULL);
        SendMessage(hListboxInstruments, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);
        ListInstruments(hListboxInstruments, 0);

        hGroupBoxParameters = CreateWindowEx(NULL, L"button", L"Audio parameters", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            455, 3, 255, 276, hWnd, NULL, hInst, NULL);
        SendMessage(hGroupBoxParameters, WM_SETFONT, (WPARAM)g_kbd.hFont, NULL);

        hStaticOctave = CreateWindowEx(NULL, L"static", L"Press '<' or '>' to change octave", WS_CHILD | WS_VISIBLE | SS_CENTER,
            475, 30, 210, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hStaticOctave, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

        hStaticCurrentOctave = CreateWindowEx(NULL, L"static", L"Current selected octave: 4 to 5", WS_CHILD | WS_VISIBLE | SS_CENTER,
            475, 50, 210, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hStaticCurrentOctave, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

        hStaticVolume = CreateWindowEx(NULL, L"static", L"Volume", WS_CHILD | WS_VISIBLE,
            470, 85, 55, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hStaticVolume, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

        hTrackbarVolume = CreateTrackbar(hWnd, 0, 10, 1, 1, 10, 530, 80, 120, 30, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TOOLTIPS | TBS_ENABLESELRANGE);
        SendMessage(hTrackbarVolume, TBM_SETSEL, FALSE, MAKELONG(0, 10));

        hStaticBalance = CreateWindowEx(NULL, L"static", L"Balance", WS_CHILD | WS_VISIBLE,
            470, 120, 55, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hStaticBalance, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

        hTrackbarBalance = CreateTrackbar(hWnd, -10, 10, 5, 1, 0, 530, 120, 120, 30, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TOOLTIPS);

        hStaticVibrato = CreateWindowEx(NULL, L"static", L"Vibrato", WS_CHILD | WS_VISIBLE,
            470, 160, 55, 20, hWnd, NULL, hInst, NULL);

        hTrackbarVibrato = CreateTrackbar(hWnd, 0, 10, 1, 1, 0, 530, 160, 120, 30, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TOOLTIPS | TBS_ENABLESELRANGE);

        hStaticPitch = CreateWindowEx(NULL, L"static", L"Pitch", WS_CHILD | WS_VISIBLE,
            470, 200, 55, 20, hWnd, NULL, hInst, NULL);
        SendMessage(hStaticPitch, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

        hTrackbarPitch = CreateTrackbar(hWnd, -64, 64, 10, 1, 0, 530, 200, 120, 30, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TOOLTIPS);

        hButtonConnectToDriver = CreateWindowEx(NULL, L"button", L"Connect to driver", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 180, 140, 25, hWnd, NULL, hInst, NULL);
        SendMessage(hButtonConnectToDriver, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

        hButtonDisconnectDriver = CreateWindowEx(NULL, L"button", L"Disconnect driver", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 215, 140, 25, hWnd, NULL, hInst, NULL);
        SendMessage(hButtonConnectToDriver, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);

    }
    break;

    case WM_KEYDOWN:
    {
        long c = -1;
        switch (wParam) {
        case VK_Z: c = 0;  break;
        case VK_S: c = 1;  break;
        case VK_X: c = 2;  break;
        case VK_D: c = 3;  break;
        case VK_C: c = 4;  break;
        case VK_V: c = 5;  break;
        case VK_G: c = 6;  break;
        case VK_B: c = 7;  break;
        case VK_H: c = 8;  break;
        case VK_N: c = 9;  break;
        case VK_J: c = 10; break;
        case VK_M: c = 11; break;
        case VK_Q: c = 12; break;
        case VK_2: c = 13; break;
        case VK_W: c = 14; break;
        case VK_3: c = 15; break;
        case VK_E: c = 16; break;
        case VK_R: c = 17; break;
        case VK_5: c = 18; break;
        case VK_T: c = 19; break;
        case VK_6: c = 20; break;
        case VK_Y: c = 21; break;
        case VK_7: c = 22; break;
        case VK_U: c = 23; break;

        case VK_LEFT:
            if (g_kbd.octave >= 1) {
                StopAllNotes(g_kbd.hMidi, g_kbd.chControl);
                g_kbd.octave--;

                wchar_t szText[50];
                wcscpy_s(szText, 26, L"Current selected octave: ");
                std::wstring leftOctave = std::to_wstring(g_kbd.octave);
                wcscat_s(szText, leftOctave.c_str());
                wcscat_s(szText, L" to ");
                std::wstring rightOctave = std::to_wstring(g_kbd.octave + 1);
                wcscat_s(szText, rightOctave.c_str());
                SetWindowText(hStaticCurrentOctave, szText);
            }
            break;

        case VK_RIGHT:
            if (g_kbd.octave <= 7) {
                StopAllNotes(g_kbd.hMidi, g_kbd.chControl);
                g_kbd.octave++;

                wchar_t szText[50];
                wcscpy_s(szText, 26, L"Current selected octave: ");
                std::wstring leftOctave = std::to_wstring(g_kbd.octave);
                wcscat_s(szText, leftOctave.c_str());
                wcscat_s(szText, L" to ");
                std::wstring rightOctave = std::to_wstring(g_kbd.octave + 1);
                wcscat_s(szText, rightOctave.c_str());
                SetWindowText(hStaticCurrentOctave, szText);
            }
            break;
        }

        if (c >= 0 && !pk[c].dn) {
            pk[c].dn = true;
            g_kbd.note = g_kbd.octave * 12 + pk[c].nt;
            PlayNote(g_kbd.hMidi, g_kbd.chPlay, g_kbd.note, g_kbd.volume);
            break;
        }
    }
    break;

    case WM_KEYUP:
    {
        long c = -1;
        switch (wParam) {
        case VK_Z: c = 0;  break;
        case VK_S: c = 1;  break;
        case VK_X: c = 2;  break;
        case VK_D: c = 3;  break;
        case VK_C: c = 4;  break;
        case VK_V: c = 5;  break;
        case VK_G: c = 6;  break;
        case VK_B: c = 7;  break;
        case VK_H: c = 8;  break;
        case VK_N: c = 9;  break;
        case VK_J: c = 10; break;
        case VK_M: c = 11; break;
        case VK_Q: c = 12; break;
        case VK_2: c = 13; break;
        case VK_W: c = 14; break;
        case VK_3: c = 15; break;
        case VK_E: c = 16; break;
        case VK_R: c = 17; break;
        case VK_5: c = 18; break;
        case VK_T: c = 19; break;
        case VK_6: c = 20; break;
        case VK_Y: c = 21; break;
        case VK_7: c = 22; break;
        case VK_U: c = 23; break;
            break;
        }
        if (c >= 0 && pk[c].dn) {
            pk[c].dn = false;
        }
    }
    break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    {
        POINT p = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        for (size_t i = 0; i < KEY_COUNT; i++) {
            if (PtInRect(&pk[i].rc, p)) {
                g_kbd.note = g_kbd.octave * 12 + pk[i].nt;
                PlayNote(g_kbd.hMidi, g_kbd.chPlay, g_kbd.note, g_kbd.volume);
                break;
            }
        }
        SetFocus(hWnd);
    }
    break;

    case WM_COMMAND:
    {
        if (lParam == (LPARAM)hCheckboxShowNotes) {
            LRESULT res = SendMessage(hCheckboxShowNotes, BM_GETCHECK, NULL, NULL);

            if (res == BST_CHECKED) {
                g_kbd.NoteText = true;
            }
            else {
                g_kbd.NoteText = false;
            }
            InvalidateRect(hWnd, NULL, FALSE);
            SetFocus(hWnd);
        }
        if (lParam == (LPARAM)hCheckboxShowKeys) {
            LRESULT res = SendMessage(hCheckboxShowKeys, BM_GETCHECK, NULL, NULL);

            if (res == BST_CHECKED) {
                g_kbd.KeyText = true;
            }
            else {
                g_kbd.KeyText = false;
            }
            InvalidateRect(hWnd, NULL, FALSE);
            SetFocus(hWnd);
        }
        if (lParam == (LPARAM)hDropDownListDevices) {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                SetDevice(hDropDownListDevices);
            }
            else if (LOWORD(wParam) == CBN_CLOSEUP) {
                SetFocus(hWnd);
            }
        }
        if (lParam == (LPARAM)hDropDownListPatches) {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                // Отримуємо поточний обраний патч
                long c = SendMessage(hDropDownListPatches, CB_GETCURSEL, 0, 0);
                // Виводимо відповідний перелік інструментів
                ListInstruments(hListboxInstruments, c);
            }
            else if (LOWORD(wParam) == CBN_CLOSEUP) {
                SetFocus(hWnd);
            }
        }
        if (lParam == (LPARAM)hListboxInstruments) {
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                StopAllNotes(g_kbd.hMidi, g_kbd.chControl);
                // Встановити обраний інструмент
                long c = (long)SendMessage(hListboxInstruments, LB_GETCURSEL, 0, 0);
                c = (long)SendMessage(hListboxInstruments, LB_GETITEMDATA, c, 0);
                g_kbd.instrument = c;
                SetInstrument(g_kbd.hMidi, g_kbd.chInstr, g_kbd.instrument);
                if (lParam) {
                    SetFocus(hWnd);
                }
            }
        }
        if (lParam == (LPARAM)hButtonConnectToDriver) {
            if (!hDevice) {
                Add();
                Start();
                hDevice = Open();
            }
            else {
                MessageBox(hWnd, L"Driver already connected!", L"Warning", MB_OK);
            }
        }
        if (lParam == (LPARAM)hButtonDisconnectDriver) {
            if (hDevice) {
                Close(hDevice);
                MessageBox(hWnd, L"Press any key to remove driver...", L"Warning", MB_OK);
                Stop();
                Remove();
                hDevice = NULL;
            }
            else {
                MessageBox(hWnd, L"Driver is not connected!", L"Warning", MB_OK);
            }
        }

        int wmId = LOWORD(wParam);
        // Розібрати вибір у меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_HSCROLL:    // Викликається при прокрутці трекбарів
        if (lParam == (LPARAM)hTrackbarVolume) {    // Зміна гучності
            if (LOWORD(wParam) == TB_THUMBTRACK) {
                StopAllNotes(g_kbd.hMidi, g_kbd.chControl);
                long c = (long)SendMessage(hTrackbarVolume, TBM_GETPOS, NULL, NULL);

                g_kbd.volume = min(127, c * 13);
                midiOutSetVolume(g_kbd.hMidi, g_kbd.volume);    // Встановлення нової гучності
                SendMessage(hTrackbarVolume, TBM_SETSEL, TRUE, MAKELONG(0, c));
                SetFocus(hWnd);
            }
        }
        if (lParam == (LPARAM)hTrackbarBalance) {   // Зміна балансу стерео
            if (LOWORD(wParam) == TB_THUMBTRACK) {
                long c = (long)SendMessage(hTrackbarBalance, TBM_GETPOS, NULL, NULL);

                g_kbd.balance = min(127, c * 6.4f + 64);
                SetChannelBalance(g_kbd.hMidi, g_kbd.chControl, g_kbd.balance);
                SetFocus(hWnd);
            }
        }
        if (lParam == (LPARAM)hTrackbarVibrato) {   // Зміна модуляції (вібрато)
            if (LOWORD(wParam) == TB_THUMBTRACK) {
                long c = (long)SendMessage(hTrackbarVibrato, TBM_GETPOS, NULL, NULL);

                g_kbd.vibrato = min(127, c * 13);
                SetModulation(g_kbd.hMidi, g_kbd.chControl, g_kbd.vibrato);
                SendMessage(hTrackbarVibrato, TBM_SETSEL, TRUE, MAKELONG(0, c));
                SetFocus(hWnd);
            }
        }
        if (lParam == (LPARAM)hTrackbarPitch) {   // Зміна висоти звуку
            if (LOWORD(wParam) == TB_THUMBTRACK) {
                long c = (long)SendMessage(hTrackbarPitch, TBM_GETPOS, NULL, NULL);

                g_kbd.pitch = 64 - c;
                SetPitch(g_kbd.hMidi, g_kbd.chBend, g_kbd.pitch);
                SetFocus(hWnd);
            }
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rt;

        SelectObject(hdc, g_kbd.hFont);

        GetClientRect(hWnd, &rt);
        if (g_kbd.memDC) DeleteDC(g_kbd.memDC);
        g_kbd.memDC = CreateCompatibleDC(hdc);

        HBRUSH whiteBrush = CreateSolidBrush(RGB(244, 244, 244));
        SelectObject(hdc, whiteBrush);

        int xWhite = 0;
        int xBlack = 45;

        for (size_t i = 0; i < KEY_COUNT; i++) {
            // Намалювати білі клавіши
            if (i != 1 && i != 3 && i != 6 && i != 8 && i != 10 &&
                i != 13 && i != 15 && i != 18 && i != 20 && i != 22) {
                RECT rc;
                rc.left = rt.left + 10 + xWhite;
                rc.top = rt.top + 330;
                pk[i].dn = false;
                pk[i].nt = i;
                pk[i].Key = KEY1[i];
                rc.right = xWhite + 60;
                rc.bottom = rt.bottom - 10;
                pk[i].rc = rc;
                RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 5, 5);
                xWhite += 50;

                if (g_kbd.NoteText) {
                    SetBkColor(hdc, RGB(244, 244, 244));
                    SetTextColor(hdc, RGB(128, 128, 128));
                    SetTextAlign(hdc, TA_BASELINE);
                    DrawText(hdc, GetNoteText(i), 2, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
                }

                if (g_kbd.KeyText) {
                    rc.bottom -= 30;
                    SetBkColor(hdc, RGB(244, 244, 244));
                    SetTextColor(hdc, RGB(160, 160, 160));
                    SetTextAlign(hdc, TA_BASELINE);
                    DrawText(hdc, &KEY1[i], 1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
                }
            }
        }

        HBRUSH blackBrush = CreateSolidBrush(RGB(64, 64, 64));
        SelectObject(hdc, blackBrush);
        DeleteObject(whiteBrush);

        for (int i = 0; i < KEY_COUNT; i++) {
            // Намалювати чорні клавіши
            if (i == 1 || i == 3 || i == 13 || i == 15) {
                RECT rc;
                rc.left = rt.left + xBlack;
                rc.top = rt.top + 330;
                rc.right = xBlack + 30;
                rc.bottom = rt.bottom - 100;
                pk[i].dn = false;
                pk[i].nt = i;
                pk[i].rc = rc;
                pk[i].Key = KEY1[i];
                RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 5, 5);

                if (g_kbd.NoteText) {
                    SetBkColor(hdc, RGB(64, 64, 64));
                    SetTextColor(hdc, RGB(128, 128, 128));
                    SetTextAlign(hdc, TA_BASELINE);
                    DrawText(hdc, GetNoteText(i), 2, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
                }

                if (g_kbd.KeyText) {
                    rc.bottom -= 30;
                    SetBkColor(hdc, RGB(64, 64, 64));
                    SetTextColor(hdc, RGB(160, 160, 160));
                    SetTextAlign(hdc, TA_BASELINE);
                    DrawText(hdc, &KEY1[i], 1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
                }

                xBlack += 50;
            }
            else if (i == 6 || i == 8 || i == 10 || i == 18 || i == 20 || i == 22) {
                xBlack += 50;
                RECT rc;
                rc.left = rt.left + xBlack;
                rc.top = rt.top + 330;
                rc.right = xBlack + 30;
                rc.bottom = rt.bottom - 100;
                pk[i].dn = false;
                pk[i].nt = i;
                pk[i].rc = rc;
                pk[i].Key = KEY1[i];
                RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 5, 5);

                if (g_kbd.NoteText) {
                    SetBkColor(hdc, RGB(64, 64, 64));
                    SetTextColor(hdc, RGB(128, 128, 128));
                    SetTextAlign(hdc, TA_BASELINE);
                    DrawText(hdc, GetNoteText(i), 2, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
                }

                if (g_kbd.KeyText) {
                    rc.bottom -= 30;
                    SetBkColor(hdc, RGB(64, 64, 64));
                    SetTextColor(hdc, RGB(160, 160, 160));
                    SetTextAlign(hdc, TA_BASELINE);
                    DrawText(hdc, &KEY1[i], 1, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
                }
            }

            if (i == 11) {
                xBlack += 100;
            }
        }

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        if (g_kbd.hMidi) midiOutClose(g_kbd.hMidi);
        if (g_kbd.memDC) DeleteDC(g_kbd.memDC);


        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обробник повідомлень для вікна "Про програму".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
