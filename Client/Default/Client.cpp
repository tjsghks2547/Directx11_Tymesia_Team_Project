// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "pch.h" 
#include "Client.h"

#include "../Public/MainApp.h"
#include "GameInstance.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND	g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.



// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/* �����Ϸ��� ������ �ۼ��� �ڵ�(���� �ڵ�)�� ���ؼ� ȣ�����ִ� �Լ�(�������Լ� or �����Լ�). */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	CMainApp*			pMainApp = { nullptr };



	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	/* �̱����� ��쿡�� �Ҵ��� ��ü�� �ּҸ� ������ ��ü�� ����(m_pInstance) �Ѵ�. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);	

	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Default"))))
		return FALSE;
	if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_240"))))
		return FALSE;

	_float		fTimerAcc = {};

	// �⺻ �޽��� �����Դϴ�.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		pGameInstance->Compute_TimeDelta(TEXT("Timer_Default"));

		fTimerAcc += pGameInstance->Get_TimeDelta(TEXT("Timer_Default"));
		
		if (fTimerAcc >= 1.f / 240.f /*1�ʿ� 60���� Ʈ��� ����*/)
		{
			pGameInstance->Compute_TimeDelta(TEXT("Timer_240"));

			pMainApp->Update(pGameInstance->Get_TimeDelta(TEXT("Timer_240")));
			pMainApp->Render();			

			fTimerAcc = 0.f;
		}		
	}

	Safe_Release(pGameInstance);

	if (0 != Safe_Release(pMainApp))
	{
		MSG_BOX("Failed to Release : CMainApp");
		return FALSE;
	}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   RECT		rcWindows = { 0, 0, g_iWinSizeX, g_iWinSizeY };

   AdjustWindowRect(&rcWindows, WS_OVERLAPPEDWINDOW, true);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rcWindows.right - rcWindows.left, rcWindows.bottom - rcWindows.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_F9: 
            DestroyWindow(g_hWnd);  
            break;  
        }
        break;
#ifdef _DEBUG
    //case WM_CREATE:
    //    AllocConsole();
    //    SetConsoleTitle(TEXT("�׽�Ʈ�� �ܼ�"));
    //    /*AllocConsole �Լ��� ȣ���Ͽ� �ܼ�â�� ����ٸ� freopen �Լ��� �⺻ ����� ��ġ�� �����ؾ� �մϴ�.*/
    //    FILE* fp;  // ���� ������ ����
    //    _wfreopen_s(&fp, L"CONOUT$", L"w", stdout);  // stdout ����
    //    _wfreopen_s(&fp, L"CONERR$", L"w", stderr);  // stderr ����
    //    _wfreopen_s(&fp, L"CONIN$", L"r", stdin);    // stdin ����

    //    /* setlocale �Լ��� �⺻ ����¿� ���� �������� �����մϴ�. */
    //    _tsetlocale(LC_ALL, _T(""));

    //    break;
#endif
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
