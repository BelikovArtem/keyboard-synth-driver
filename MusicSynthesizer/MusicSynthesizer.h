#pragma once

#include "framework.h"
#include "resource.h"
#include <mmeapi.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <string>
#include <cmath>

#define VK_Z        0x5A
#define VK_S        0x53
#define VK_X        0x58
#define VK_D        0x44
#define VK_C        0x43
#define VK_V        0x56
#define VK_G        0x47
#define VK_B        0x42
#define VK_H        0x48
#define VK_N        0x4E
#define VK_J        0x4A
#define VK_M        0x4D
#define VK_Q        0x51
#define VK_2        0x32
#define VK_W        0x57
#define VK_3        0x33
#define VK_E        0x45
#define VK_R        0x52
#define VK_5        0x35
#define VK_T        0x54
#define VK_6        0x36
#define VK_Y        0x59
#define VK_7        0x37
#define VK_U        0x55
#define VK_LEFT     0x25
#define VK_RIGHT    0x27

const int KEY_COUNT = 24;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                       Дані для програми                                                                 //  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct kbData {
    HWND hWnd;        // Дескриптор батьківського вікна
    HINSTANCE hInst;  // Дескриптор виконуваної програми
    HMIDIOUT hMidi;   // Дескриптор пристрою MIDI 
    HDC memDC;        // Контекст пристрою
    HFONT hFont;      // Дескриптор використовуваного шрифту
    long instrument;  // Обраний інструмент (0 - 127)
    long chPlay;      // Канал від &H90 до &H9F, де ch &H99 = перкусія
    long chStop;      // Канал від &H80 до &H8F, де ch &H89 = перкусія
    long chControl;   // Канал від &HB0 до &HBF, зміна параметрів
    long chInstr;     // Канал від &HC0 до &HCF, зміна інструменту
    long chBend;      // Канал від &HE0 до &HEF, висота тону
    bool KeyText;     // Включення/виключення опису на клавішах
    bool NoteText;    // Включення/виключення опису нот на клавішах
    long note;        // Нота для програвання (0 - 127)
    long octave;      // Основна октава (1 октава = 12 нот) припустимі значення: 0 - 9
    long pitch;       // Висота звуку
    long volume;      // Гучність звуку
    long balance;     // Правий/Лівий баланс звуку
    long vibrato;     // Модуляція/вібрато
};

// Дані для масиву клавіш
struct pKey {
    long Key;         // Клавіша
    long nt;          // Нота (0 - 127)
    bool dn;          // Прапорець натиснутої клавіши (натиснуто, відпущено), 1/0
    RECT rc;          // Координати клавіш для відрисовки
};

WCHAR KEY1[] = { L"ZSXDCVGBHNJMQ2W3ER5T6Y7U" };  // Масив 24 клавіш для програвання нот
kbData g_kbd;
pKey pk[KEY_COUNT];

const WCHAR* allInstruments[] = {
    L"1 Acoustic Grand Piano",
    L"2 Bright Acoustic Piano",
    L"3 Electric Grand Piano",
    L"4 Honky-tonk Piano",
    L"5 Rhodes Piano",
    L"6 Chorused Piano",
    L"7 Harpsichord",
    L"8 Clavinet",
    L"9 Celesta",
    L"10 Glockenspiel",
    L"11 Music Box",
    L"12 Vibraphone",
    L"13 Marimba",
    L"14 Xylophone",
    L"15 Tubular Bells",
    L"16 Dulcimer",
    L"17 Hammond Organ",
    L"18 Percussive Organ",
    L"19 Rock Organ",
    L"20 Church Organ",
    L"21 Reed Organ",
    L"22 Accordion",
    L"23 Harmonica",
    L"24 Tango Accordion",
    L"25 Acoustic Guitar (nylon)",
    L"26 Acoustic Guitar (steel)",
    L"27 Electric Guitar (jazz)",
    L"28 Electric Guitar (clean)",
    L"29 Electric Guitar (muted)",
    L"30 Overdriven Guitar",
    L"31 Distortion Guitar",
    L"32 Guitar Harmonics",
    L"33 Acoustic Bass",
    L"34 Electric Bass (finger)",
    L"35 Electric Bass (pick)",
    L"36 Fretless Bass",
    L"37 Slap Bass 1",
    L"38 Slap Bass 2",
    L"39 Synth Bass 1",
    L"40 Synth Bass 2",
    L"41 Violin",
    L"42 Viola",
    L"43 Cello",
    L"44 Contrabass",
    L"45 Tremolo Strings",
    L"46 Pizzicato String",
    L"47 Orchestral Harp",
    L"48 Timpani",
    L"49 String Ensemble",
    L"50 String Ensemble",
    L"51 SynthStrings 1",
    L"52 SynthStrings 2",
    L"53 Choir Aahs",
    L"54 Voice Oohs",
    L"55 Synth Voice",
    L"56 Orchestra Hit",
    L"57 Trumpet",
    L"58 Trombone",
    L"59 Tuba",
    L"60 Muted Trumpet",
    L"61 French Horn",
    L"62 Brass Section",
    L"63 Synth Brass 1",
    L"64 Synth Brass 2",
    L"65 Soprano Sax",
    L"66 Alto Sax",
    L"67 Tenor Sax",
    L"68 Baritone Sax",
    L"69 Oboe",
    L"70 English Horn",
    L"71 Bassoon",
    L"72 Clarinet",
    L"73 Piccolo",
    L"74 Flute",
    L"75 Recorder",
    L"76 Pan Flute",
    L"77 Bottle Blow",
    L"78 Shakuhachi",
    L"79 Whistle",
    L"80 Ocarina",
    L"81 Lead 1 (square)",
    L"82 Lead 2 (sawtooth",
    L"83 Lead 3 (calliope lead)",
    L"84 Lead 4 (chiff lead)",
    L"85 Lead 5 (charang)",
    L"86 Lead 6 (voice)",
    L"87 Lead 7 (fifths)",
    L"88 Lead 8 (bass + lead)",
    L"89 Pad 1 (new age)",
    L"90 Pad 2 (warm)",
    L"91 Pad 3 (polysynth)",
    L"92 Pad 4 (choir)",
    L"93 Pad 5 (bowed)",
    L"94 Pad 6 (metallic)",
    L"95 Pad 7 (halo)",
    L"96 Pad 8 (sweep)",
    L"97 FX 1 (rain)",
    L"98 FX 2 (soundtrack)",
    L"99 FX 3 (crystal)",
    L"100 FX 4 (atmosphere)",
    L"101 FX 5 (brightnes)",
    L"102 FX 6 (goblins)",
    L"103 FX 7 (echoes)",
    L"104 FX 8 (sci-fi)",
    L"105 Sitar",
    L"106 Banjo",
    L"107 Shamisen",
    L"108 Koto",
    L"109 Kalimba",
    L"110 Bagpipe",
    L"111 Fiddle",
    L"112 Shanai",
    L"113 Tinkle BelL",
    L"114 Agogo",
    L"115 Steel Drums",
    L"116 Woodblock",
    L"117 Taiko Drum",
    L"118 Melodic Tom",
    L"119 Synth Drum",
    L"120 Reverse Cymbal",
    L"121 Guitar Fret Noise",
    L"122 Breath Noise",
    L"123 Seashore",
    L"124 Bird Tweet",
    L"125 Telephone Ring",
    L"126 Helicopter",
    L"127 Applause",
    L"128 Gunshot"
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Функції для взаємодії з MIDI                                                            //  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Встановити потрібний інструмент, подія/канал 0xB0 до 0xBF
MMRESULT SetInstrument(HMIDIOUT hMidi, long Event, long Instrument) {
    DWORD message = Event + Instrument * 0x100;
    return midiOutShortMsg(hMidi, message);
}

// Встановити модуляцію/вібрато для вибраного каналу. Modulation приймає значення від 1 до 127
MMRESULT SetModulation(HMIDIOUT hMidi, long Event, long Modulation) {
    DWORD message = Event + 0x1 * 0x100 + Modulation * 0x10000;
    return midiOutShortMsg(hMidi, message);
}

// Висота тону +/- 2 ноти, 0 = максимальне нижнє значення, 64 = звичайна нота, 127 = максимальне високе значення
MMRESULT SetPitch(HMIDIOUT hMidi, long Event, long Pitch) {
    DWORD message = Event + Pitch * 0x10000;
    return midiOutShortMsg(hMidi, message);
}

// Встановити баланс для вибраного каналу. 0 = лівий канал, 64 = центр, 127 = правий канал
MMRESULT SetChannelBalance(HMIDIOUT hMidi, long Event, long Balance) {
    DWORD message = Event + 0xA * 0x100 + Balance * 0x10000;
    return midiOutShortMsg(hMidi, message);
}

// Встановити сустейн
MMRESULT SetSustain(HMIDIOUT hMidi, long Event, long Sustain) {
    DWORD message = Event + 0x40 * 0x100 + Sustain * 0x10000;
    return midiOutShortMsg(hMidi, message);
}

// Програти обрану ноту
MMRESULT PlayNote(HMIDIOUT hMidi, long Event, long Note, long Volume) {
    DWORD message = Event + Note * 0x100 + Volume * 0x10000;
    return midiOutShortMsg(hMidi, message);
}

// Припинити програвати обрану ноту
MMRESULT StopNote(HMIDIOUT hMidi, long Event, long Note) {
    DWORD message = Event + (Note * 0x100);
    return midiOutShortMsg(hMidi, message);
}

// Припинити програвати всі ноти
MMRESULT StopAllNotes(HMIDIOUT hMidi, long Event) {
    DWORD message = Event + 0x7B * 0x100;
    return midiOutShortMsg(hMidi, message);
}

// Перелічити всі девайси
void ListDevices(HWND hList) {
    DWORD msg1 = 0, msg2 = 0;

    MIDIOUTCAPS MidiCaps = { 0 };
    SendMessage(hList, CB_RESETCONTENT, 0, 0);
    for (UINT dev = 0; dev < midiOutGetNumDevs(); dev++) {
        midiOutGetDevCaps(dev, &MidiCaps, sizeof(MIDIOUTCAPS));
        SendMessage(hList, CB_ADDSTRING, NULL, (LPARAM)MidiCaps.szPname);
    }

    SendMessage(hList, CB_SETCURSEL, NULL, NULL);
}

// Встановити обраний девайс
void SetDevice(HWND hList) {
    if (g_kbd.hMidi) midiOutClose(g_kbd.hMidi);

    long c = (long)SendMessage(hList, CB_GETCURSEL, NULL, NULL);

    if (midiOutOpen(&g_kbd.hMidi, (UINT)c, 0, 0, 0) != MMSYSERR_NOERROR) {
        MessageBox(g_kbd.hWnd, L"midiOutOpen functiond Failed", L"Error message!", MB_OK);
        midiOutOpen((LPHMIDIOUT)g_kbd.hMidi, -1, 0, 0, 0);
    }

    SetInstrument(g_kbd.hMidi, g_kbd.chInstr, g_kbd.instrument);    // встановити інструмент 
    midiOutSetVolume(g_kbd.hMidi, g_kbd.volume);
    SetChannelBalance(g_kbd.hMidi, g_kbd.chControl, g_kbd.balance); // встановити баланс між каналами
    SetSustain(g_kbd.hMidi, g_kbd.chControl, 0);                    // встановити сустейн
    SetFocus(g_kbd.hWnd);                                           // встановити фокус вводу на головному вікні
}

// Перелічити всі патчі
void ListPatches(HWND hList) {

    SendMessage(hList, CB_RESETCONTENT, 0, 0);

    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Piano");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Chromatic Percussion");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Organ");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Guitar");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Bass");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Strings");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Ensamble");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Brass");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Reed");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Pipe");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Synth Lead");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Synth Pad");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Synth Effects");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Ethnic");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Percussive");
    SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)L"Sound effects");
    SendMessage(hList, CB_SETCURSEL, NULL, NULL);
}

// Перелічити всі інструменти для обраного патчу
void ListInstruments(HWND hList, long Patches) {
    long c = 0, Start = 0, Stop = 0;
    DWORD msg1 = 0, msg2 = 0;
    WCHAR szText[64];

    SendMessage(hList, LB_RESETCONTENT, 0, 0);

    switch (Patches) {  // Перелічити інструменти за категоріями
    case 0: Start = 1;    Stop = 8;  // Piano
        break;
    case 1: Start = 9;    Stop = 16;  // Chromatic Percussion
        break;
    case 2: Start = 17;   Stop = 24;  // Organ
        break;
    case 3: Start = 25;   Stop = 32;  // Guitar
        break;
    case 4: Start = 33;   Stop = 40;  // Bass
        break;
    case 5: Start = 41;   Stop = 48;  // Strings
        break;
    case 6: Start = 49;   Stop = 56;  // Ensamble
        break;
    case 7: Start = 57;   Stop = 64;  // Brass
        break;
    case 8: Start = 65;   Stop = 72;  // Reed
        break;
    case 9: Start = 73;   Stop = 80;  // Pipe
        break;
    case 10: Start = 81;  Stop = 88;  // Synth Lead
        break;
    case 11: Start = 89;  Stop = 96;  // Synth Pad
        break;
    case 12: Start = 97;  Stop = 104;  // Synth Effects
        break;
    case 13: Start = 105; Stop = 112;  // Ethnic
        break;
    case 14: Start = 113; Stop = 119;  // Percussive
        break;
    case 15: Start = 120; Stop = 128;  // Sound effects
        break;
    }

    for (c = Start; c <= Stop; c++) {
        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)allInstruments[c - 1]);
        SendMessage(hList, LB_SETITEMDATA, c - Start, c - 1);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Функції створення GUI та роботи з клавіатурою                                            //  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HFONT MakeFontEx(HDC hdc, const WCHAR* szFontName, long PointSize, long fAngle, long fBold, long fItalic, long fUnderline) {
    LPARAM charSet = 0;
    long CyPixels = 0;

    if (hdc == 0) {
        hdc = GetDC(HWND_DESKTOP);
        CyPixels = GetDeviceCaps(hdc, LOGPIXELSY);
        ReleaseDC(HWND_DESKTOP, hdc);
    }
    else {
        CyPixels = GetDeviceCaps(hdc, LOGPIXELSY);
    }

    PointSize = 0 - (PointSize * CyPixels) / 72;

    return CreateFont(PointSize, 0, fAngle, fAngle, fBold, fItalic, fUnderline, FALSE,
        DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, szFontName);
}

const WCHAR* GetNoteText(int Note) {
    switch (Note % 12) {
    case 0: return L"С";
    case 1: return L"C#";
    case 2: return L"D";
    case 3: return L"D#";
    case 4: return L"E";
    case 5: return L"F";
    case 6: return L"F#";
    case 7: return L"G";
    case 8: return L"G#";
    case 9: return L"A";
    case 10: return L"A#";
    case 11: return L"B";
    }
}

HWND WINAPI CreateTrackbar(
    HWND hWnd,          // Дескриптор батьківського вікна
    UINT iMin,          // Мінімальне значення
    UINT iMax,          // Максимальне значення
    UINT TicFreqWparam, // Частота ділення
    UINT TicFreqLparam, //
    UINT pos,           // Початкова позиція
    int x,              // Позиція по х
    int y,              // Позиція по у
    int width,          // Ширина
    int height,         // Висота
    DWORD dwStyles      // Стилі трекбару
)
{
    HWND hTrackbar = CreateWindowEx(NULL, TRACKBAR_CLASS, NULL, dwStyles, x, y, width, height, hWnd, NULL, g_kbd.hInst, NULL);

    SendMessage(hTrackbar, TBM_SETRANGE, TRUE, MAKELONG(iMin, iMax));
    SendMessage(hTrackbar, TBM_SETTICFREQ, TicFreqWparam, TicFreqLparam);
    SendMessage(hTrackbar, TBM_SETPOS, TRUE, pos);
    SendMessage(hTrackbar, WM_SETFONT, (WPARAM)MakeFontEx(0, L"Arial", 10, 0, FW_BOLD, 0, 0), NULL);
    return hTrackbar;
}

void PrintErrorInMessageBox(DWORD error, const WCHAR* errorMessage) {
    WCHAR szTest[10];
    swprintf_s(szTest, 10, L"%d", error);
    MessageBox(NULL, szTest, errorMessage, MB_OK);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Функції для взаємодії з драйвером                                                        //  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Add() {
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL) {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"OpenSCManager failed");
        return;
    }

    SC_HANDLE schService = CreateService(
        hSCM,
        L"driver_1",                    // service name
        L"driver_1",                    // symbolic link name
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_CRITICAL,
        L"C:\\driver\\Driver.sys",      // file path
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (schService != NULL) {
        MessageBox(g_kbd.hWnd, L"Service succesfully created", L"Success", MB_OK);
    }
    else {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"OpenSCManager failed");
        return;
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(hSCM);
}

void Remove() {
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL) {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"OpenSCManager failed");
        return;
    }

    SC_HANDLE openService = OpenService(
        hSCM,
        L"driver_1",
        SC_MANAGER_ALL_ACCESS
    );

    if (openService != NULL) {
        if (DeleteService(openService)) {
            MessageBox(g_kbd.hWnd, L"Service deleted sucessfully!", L"Success", MB_OK);
        }
        else {
            DWORD error = GetLastError();
            PrintErrorInMessageBox(error, L"Service delete failed");
            return;
        }

    }
    else {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"Service opening error!");
        return;
    }

    CloseServiceHandle(openService);
    CloseServiceHandle(hSCM);
}

void Start() {
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL) {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"OpenSCManager failed");
        return;
    }

    SC_HANDLE openService = OpenService(
        hSCM,
        L"driver_1",
        SC_MANAGER_ALL_ACCESS
    );
    if (openService != NULL) {
        if (StartService(openService, 0, 0)) {
            MessageBox(g_kbd.hWnd, L"Driver started successfully!", L"Success", MB_OK);
        }
        else {
            DWORD error = GetLastError();
            PrintErrorInMessageBox(error, L"Driver start failed!");
        }
    }
    else {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"Service opening error!");
    }
}

void Stop() {
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL) {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"OpenSCManager failed");
        return;
    }
    else {
        SC_HANDLE openService = OpenService(
            hSCM,
            L"driver_1",
            SC_MANAGER_ALL_ACCESS
        );
        if (openService != NULL) {
            _SERVICE_STATUS status;
            if (ControlService(openService, SERVICE_CONTROL_STOP, &status)) {
                MessageBox(g_kbd.hWnd, L"Driver stoped successfully!", L"Success", MB_OK);
            }
            else {
                DWORD error = GetLastError();
                PrintErrorInMessageBox(error, L"Driver stop failed!");
            }
        }
        else {
            DWORD error = GetLastError();
            PrintErrorInMessageBox(error, L"Service opening error!");
        }
    }
}

HANDLE Open() {
    HANDLE hDeviceNeither = CreateFile(
        L"\\\\.\\GLOBALROOT\\Device\\HigherKeyboardClass0",
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hDeviceNeither == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        PrintErrorInMessageBox(error, L"Device create failed!");
    }
    else {
        MessageBox(g_kbd.hWnd, L"Device created sucessfully", L"Success", MB_OK);
        return hDeviceNeither;
    }
    return INVALID_HANDLE_VALUE;
}

void Close(HANDLE hDevice) {
    if (hDevice != NULL && CloseHandle(hDevice)) {
        MessageBox(g_kbd.hWnd, L"Device closed successfully", L"Success", MB_OK);
    }
    else {
        PrintErrorInMessageBox(GetLastError(), L"Device close failed!");
    }
}
