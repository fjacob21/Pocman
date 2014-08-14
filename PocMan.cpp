#include <windows.h>
#include "graphlib.cpp"
#include "audiolib.cpp"
#include "game.h"
#include "pocman.h"

//variable hyperglobal
HINSTANCE hInst;
HWND hWnd;
CFJ_Level l;
CFJ_DX9Screen sc;
CFJ_DX8Audio  sp;
CFJ_Ctrl ctrl;

LRESULT CALLBACK WindowProc
(HWND      hWnd,
 UINT      uMsgId,
 WPARAM    wParam,
 LPARAM    lParam
 );


int PASCAL WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR pszCmdLine,int nCmdShow)
{
	MSG   msg;
	WNDCLASS  wndClass; 
	char nomwin[7]="PocMan";
	CFJ_Timer t;
	int fin;
	
	
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance,"#107");
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = nomwin; 
	if (RegisterClass(&wndClass) == 0)
		return 0;

	hWnd= CreateWindow 
	(	nomwin,
		nomwin,
		WS_OVERLAPPEDWINDOW,
		0,0,800,600,NULL,
		NULL,hInstance,NULL);

	
	if (hWnd == 0) 
		return 0;
	
	hInst=hInstance;
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	sc.InitDX(hWnd,true,800,600);
	sc.LoadImage("CUBEB.BMP");
	sc.LoadImage("bouf.bmp");
	sc.LoadImage("fond.bmp");
	sc.LoadImage("titre.bmp");
	sp.Init();
	sp.LoadSound("bouf.mid");
	sp.LoadSound("poc.mid");
	sp.PlaySound(1,true,true,-500);
	t.Start();
	l.Load();
	while(true)
	{	
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
		if(t() > 0.1)
		{
			fin = l.CLK(&sc,&sp,&ctrl);
			t.Start();
			ctrl.Clear();
			if(fin == 1 )
			{
				break;
			}
		}
	}
	sp.StopSound(1);
	return msg.wParam;
}
 
LRESULT CALLBACK WindowProc
(HWND      hWnd,
 UINT      uMsgId,
 WPARAM    wParam,
 LPARAM    lParam
 )
{
	
	switch(uMsgId)
	{
		case WM_COMMAND:
			/*switch(wParam)
			{	 

			}*/
		return 0;
		break;
		case WM_LBUTTONUP:
				DestroyWindow( hWnd );
			return 0;
			break;
		case WM_KEYDOWN:
			switch( wParam )
			{
				
			case VK_ESCAPE:	
			case VK_F12:
					DestroyWindow( hWnd );
			break;
			case VK_UP:	
				ctrl + UP;
			break;
			case VK_DOWN:
				ctrl + DOWN;
			break;
			case VK_RIGHT:
				ctrl + RIGHT;
			break;
			case VK_LEFT:
				ctrl + LEFT;
			break;
			case VK_RETURN:
					
			break;
			}
		return 0;
		break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
		break;

		default:
			return DefWindowProc(hWnd,uMsgId,wParam,lParam);

	}
}