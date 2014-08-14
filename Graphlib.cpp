#include <windows.h>
#include <d3d9.h>
#include <D3dx9math.h>

typedef struct _lsttex
{
	char nom[25];
	LPDIRECT3DTEXTURE9 pTexture;
	_lsttex *next;
}lsttex;

class CFJ_DX9Screen
{
	public:
		CFJ_DX9Screen();
		~CFJ_DX9Screen(){EndDX();};
		bool InitDX(HWND hWnd,bool windowed,int width,int height);
		void EndDX();
		void BeginDraw();
		void EndDraw();
		void DrawImage(char *img,int x,int y);
		void DrawImage(int i,int x,int y);
		void LoadImage(char *img);
		void DeleteImage(int i);
		void DrawForm();
		void DrawText(char *text,char *font,int gr,RECT pos,int r=255,int g=255,int b=255);
		void Clear(int r,int g,int b);
		void Show();

	private:
		LPDIRECT3D9             pD3D;
		LPDIRECT3DDEVICE9       pd3dDevice;
		LPD3DXSPRITE            sprite;
		lsttex                  *imgs;
		bool                    draw;
		
};

CFJ_DX9Screen::CFJ_DX9Screen()
{
	pD3D		= NULL;
	pd3dDevice	= NULL;
	imgs		= NULL;
	draw		= false;
}

bool CFJ_DX9Screen::InitDX(HWND hWnd,bool windowed,int width,int height)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp; 
	HRESULT res;

	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return false;
	 
    if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
        DestroyWindow( hWnd );
	 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferCount = 0;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE ;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd ;
    d3dpp.Windowed = windowed;
    d3dpp.EnableAutoDepthStencil;
    d3dpp.AutoDepthStencilFormat;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT ;
    //d3dpp. = D3DPRESENT_INTERVAL_DEFAULT ;


	res = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &pd3dDevice );
	pd3dDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE,TRUE);
	D3DXCreateSprite(pd3dDevice,&sprite);
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,125,0), 1.0f, 0 );
	return true;
}

void CFJ_DX9Screen::EndDX()
{
	if( pd3dDevice != NULL) 
       pd3dDevice->Release();

    if( pD3D != NULL)
        pD3D->Release();

    if( sprite != NULL)
        sprite->Release();

	while(imgs!=NULL)
	{
		lsttex *t=imgs;
		imgs = imgs->next;
        t->pTexture->Release();
		delete(t);
	}

}

void CFJ_DX9Screen::BeginDraw()
{
	pd3dDevice->BeginScene();
	draw = true;
}

void CFJ_DX9Screen::EndDraw()
{
	pd3dDevice->EndScene();
	draw = false;
}

void CFJ_DX9Screen::DrawImage(char *img,int x,int y)
{
	LPDIRECT3DTEXTURE9 g_pTexture;
	D3DXIMAGE_INFO desc;
	HRESULT res = D3DXCreateTextureFromFileEx(pd3dDevice,img,0,0,0,0,
		                   D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,D3DX_FILTER_NONE ,
						   D3DX_FILTER_NONE,D3DCOLOR_RGBA(255,0,255,255),
						   &desc,NULL,&g_pTexture);

	D3DXVECTOR3 v1,v2;
	v1.x = 1.0;
	v1.y = 1.0;
	v2.x = (float)x;
	v2.y = (float)y;
	sprite->Begin( D3DXSPRITE_ALPHABLEND );
	sprite->Draw(g_pTexture,NULL,&v1,&v2,D3DCOLOR_RGBA(255,255,255,255));
	sprite->End();
	g_pTexture->Release();
}

void CFJ_DX9Screen::DrawImage(int i,int x,int y)
{
	lsttex *t = imgs;
	int id=0;

	while(id != i && t!=NULL)
	{
		t = t->next;
		id++;
	}
	if(t!=NULL)
	{
		D3DXVECTOR3 v1,v2;
		v1.x = 1.0;
		v1.y = 1.0;
		v2.x = (float)x;
		v2.y = (float)y;
		HRESULT res = sprite->Begin(D3DXSPRITE_ALPHABLEND);
		res = sprite->Draw(t->pTexture,NULL,&v1,&v2,D3DCOLOR_RGBA(255,255,255,255));
		res = sprite->End();
	}
}

void CFJ_DX9Screen::LoadImage(char *img)
{
	D3DXIMAGE_INFO desc;
	lsttex *pt,*t = imgs;
	if(imgs == NULL)
	{
		imgs = new lsttex;
		strcpy_s(imgs->nom,25,img);
		D3DXCreateTextureFromFileEx(pd3dDevice,img,0,0,0,0,
		                   D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,D3DX_FILTER_NONE ,
						   D3DX_FILTER_NONE,D3DCOLOR_RGBA(255,0,255,255),
						   &desc,NULL,&imgs->pTexture); 
		imgs->next = NULL;
	}
	else
	{
		while(t->next!= NULL)
			t= t->next;
		pt = new lsttex;
		strcpy_s(pt->nom,25,img);
		D3DXCreateTextureFromFileEx(pd3dDevice,img,0,0,0,0,
		                   D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,D3DX_FILTER_NONE ,
						   D3DX_FILTER_NONE,D3DCOLOR_RGBA(255,0,255,255),
						   &desc,NULL,&pt->pTexture);
		pt->next = NULL;
		t->next = pt;
	}
	
}

void CFJ_DX9Screen::DeleteImage(int i)
{
	lsttex *pt,*t = imgs;
	int id=0;

	while(t!= NULL && id!=i)
	{
		pt = t;
		t= t->next;	
		id++;
	}
	pt->next =t->next;
	t->pTexture->Release();
	delete(t);
}

void CFJ_DX9Screen::DrawForm()
{
}

void CFJ_DX9Screen::DrawText(char *text,char *font,int gr,RECT pos,int r,int g,int b)
{
	LPD3DXFONT d3dfont;

	HRESULT res =D3DXCreateFont(pd3dDevice,
        gr,					// logical height of font 
		0,					// logical average character width 
        FW_THIN,			// font weight 
		0,					// Miplevel 
        FALSE,				// italic attribute flag 
        DEFAULT_CHARSET,	// character set identifier 
		OUT_TT_PRECIS,		// output precision 
		ANTIALIASED_QUALITY,		// output quality 
        FF_SWISS,
		font		// pointer to typeface name string
		,&d3dfont);
	if(res  == E_OUTOFMEMORY)
		::MessageBox(NULL,"erreur","mem",MB_OK);
	else
	{
		d3dfont->DrawTextA(NULL,text,-1,&pos,DT_LEFT ,D3DCOLOR_RGBA(r,g,b,255));
		d3dfont->Release();
	}
	//DeleteObject(hf);
}

void CFJ_DX9Screen::Clear(int r,int g,int b)
{
	D3DCOLOR c = D3DCOLOR_XRGB(r,g,b);
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, c, 1.0f, 0 );
}

void CFJ_DX9Screen::Show()
{
	HRESULT res = pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
