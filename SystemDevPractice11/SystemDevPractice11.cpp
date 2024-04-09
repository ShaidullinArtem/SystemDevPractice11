#include "framework.h"
#include "SystemDevPractice11.h"
#include <commctrl.h>
#include <regex>
#include <commdlg.h>
#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100
#define OnSaveIpButtonClick 1
#define ID_FIND 2
#define ID_PRINT 3

#define IDC_LISTBOX 102
#define WC_LISTBOXCAL L"ListBox"

#define TextBufferSize 256

HWND hIpEditControl;
HWND hIpEditErrorControl;
HWND hListbox;

void SaveData(LPCSTR path);
void LoadData(LPCSTR path);

char Buffer[TextBufferSize];


HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];           

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

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SYSTEMDEVPRACTICE11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSTEMDEVPRACTICE11));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSTEMDEVPRACTICE11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SYSTEMDEVPRACTICE11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void SaveData(LPCSTR path) {
    HANDLE FileToSave = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    int saveLen = GetWindowTextLength(hIpEditControl);
    char* data = new char[saveLen];

    saveLen = GetWindowTextA(hIpEditControl, data, saveLen) + 1;

    DWORD bytesIterated;
    WriteFile(FileToSave, data, saveLen, &bytesIterated, NULL);
    CloseHandle(FileToSave);
};

void LoadData(LPCSTR path) {
    HANDLE FileToLoad = CreateFileA(
        path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    DWORD bytesIterated;
    bool _ = ReadFile(FileToLoad, Buffer, TextBufferSize, &bytesIterated, NULL);


    CloseHandle(FileToLoad);
}


void MainWinAddWidgets(HWND hWnd) {
    CreateWindowA("static", "Enter IP-Address", WS_VISIBLE | WS_CHILD, 8, 16, 210, 20, hWnd, NULL, NULL, NULL);
    hIpEditControl = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 8, 35, 210, 20, hWnd, NULL, NULL, NULL);
    hIpEditErrorControl = CreateWindowA("static", "", WS_VISIBLE | WS_CHILD, 8, 60, 210, 20, hWnd, NULL, NULL, NULL);
    CreateWindowA("button", "Save", WS_VISIBLE | WS_CHILD, 8, 90, 130, 25, hWnd, (HMENU)OnSaveIpButtonClick, NULL, NULL);
    CreateWindowEx(0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE, 230, 16, 210, 30, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);
    
    hListbox = CreateWindowEx(0, WC_LISTBOXCAL, L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_COMBOBOX,
        8, 150, 150, 50,
        hWnd, (HMENU)IDC_LISTBOX,
        GetModuleHandle(NULL), NULL);
    
    SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)L"Первый элемент");
    SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)L"Второй элемент");

}

void PrintDialog(HWND hwnd)
{
    PRINTDLG pd = { sizeof(PRINTDLG) };
    pd.Flags = PD_ALLPAGES | PD_RETURNDC;
    pd.hwndOwner = hwnd;

    if (PrintDlg(&pd))
    {
        MessageBox(hwnd, TEXT("Печать настроена"), TEXT("Печать"), MB_OK);
        // Здесь можно добавить код для печати
        // Например, вызов функции StartDoc, StartPage, TextOut, EndPage, EndDoc
    }
    else
    {
        if (CommDlgExtendedError() != 0)
        {
            MessageBox(hwnd, TEXT("Ошибка диалогового окна печати"), TEXT("Ошибка"), MB_OK);
        }
    }
}

void FindDialog(HWND hwnd)
{
    FINDREPLACE fr = { sizeof(FINDREPLACE) };
    fr.lStructSize = sizeof(FINDREPLACE);
    fr.hwndOwner = hwnd;
    fr.lpstrFindWhat = (LPWSTR)L"Первый элемент";
    fr.wFindWhatLen = 256;
    fr.Flags = FR_HIDEWHOLEWORD;

    if (FindText(&fr))
    {
        MessageBox(hwnd, TEXT("Текст найден"), TEXT("Найти"), MB_OK);
        // Здесь можно добавить код для выполнения действий после нахождения текста
    }
    else
    {
        if (CommDlgExtendedError() != 0)
        {
            MessageBox(hwnd, TEXT("Ошибка диалогового окна поиска"), TEXT("Ошибка"), MB_OK);
        }
    }
}

void MainWinAddMenu(HWND hWnd) {
    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();

    // Добавление пунктов в подменю
    AppendMenu(hSubMenu, MF_STRING, ID_FIND, TEXT("Найти"));
    AppendMenu(hSubMenu, MF_STRING, ID_PRINT, TEXT("Печать"));

    // Добавление подменю в главное меню
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, TEXT("Пункты"));

    // Установка меню для окна
    SetMenu(hWnd, hMenu);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            std::wregex ip_regex(L"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
            switch (wmId)
            {
            case OnSaveIpButtonClick:
                TCHAR szIP[128];
                GetWindowText(hIpEditControl, szIP, ARRAYSIZE(szIP));

                if (regex_match(szIP, ip_regex))
                {
                    SaveData("C:\\Users\\mrshe\\source\\repos\\SystemDevPractice11\\output.txt");
                    SetWindowTextA(hIpEditControl, "");
                }
                else
                {
                    SetWindowTextA(hIpEditErrorControl, "Неверный форат ip");
                }

                break;
            case ID_PRINT:
                PrintDialog(hWnd);
                break;
            case ID_FIND:
                FindDialog(hWnd);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CREATE:
        MainWinAddMenu(hWnd);
        MainWinAddWidgets(hWnd);
        break;

    case WM_CHAR:
        MessageBox(hWnd, L"Событие 1 - Нажатие на БУКВУ ИЛИ ЦИФРУ", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_KEYDOWN:
        MessageBox(hWnd, L"Событие 2 - Была нажата несистемная клавиша", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
        break;

    case WM_MBUTTONDBLCLK:
        MessageBox(hWnd, L"Событие 3 - Пользователь дважды щелкнул среднюю кнопку мыши", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_MBUTTONDOWN:
        MessageBox(hWnd, L"Событие 4 - Пользователь щелкнул среднюю кнопку мыши", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_MBUTTONUP:
        MessageBox(hWnd, L"Событие 5 - Средняя кнопка мыши была отпущена", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
        break;
    //case WM_MOUSEMOVE:
    //    MessageBox(hWnd, L"Событие 6 - Окно переместилось", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
    //    break;
    case WM_MOVE:
        MessageBox(hWnd, L"Событие 7 - Окно закончило перемещение", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}