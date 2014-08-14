#define INITGUID
#include <dmusici.h>

#define FRONT 1
#define BACK  2

typedef struct _lstsnd
{
	char nom[50];
	IDirectMusicSegment8*     g_pSegment;
	_lstsnd *next;

}lstsnd;
	
class CFJ_DX8Audio
{
	public:
		CFJ_DX8Audio();
		~CFJ_DX8Audio(){End();};
		void Init();
		void Init(char *dir);
		void End();
		int  PlaySound(char *file,bool primary);
		int  PlaySound(char *file,bool prim,int vol);
		int  PlaySound(int i,bool prim,bool loop,int vol);
		void StopSound(int i);
		void LoadSound(char *nom);
		void DeleteSound();
	private:
		IDirectMusicLoader8*      g_pLoader;
		IDirectMusicPerformance8* g_pPerformance;
		lstsnd *snds;


};

CFJ_DX8Audio::CFJ_DX8Audio()
{
	g_pLoader       = NULL;
	g_pPerformance  = NULL;
	snds = NULL;
}

void CFJ_DX8Audio::Init()
{
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,path);
	Init(path);
}

void CFJ_DX8Audio::Init(char *dir)
{
	CoInitialize(NULL);
    
    CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
                     CLSCTX_INPROC, IID_IDirectMusicLoader8,
                     (void**)&g_pLoader);

    CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
                     CLSCTX_INPROC, IID_IDirectMusicPerformance8,
                     (void**)&g_pPerformance );

	g_pPerformance->InitAudio( 
        NULL,                  
        NULL,                  
        NULL,                 
        DMUS_APATH_DYNAMIC_STEREO ,  
        64,                    
        DMUS_AUDIOF_ALL,       
        NULL                   
    );


    CHAR strPath[MAX_PATH];
	//GetWindowsDirectory( strPath, MAX_PATH );
    strcpy( strPath, dir );

	Sleep(1000); 

    // Convert to Unicode.
    WCHAR wstrSearchPath[MAX_PATH];
    MultiByteToWideChar( CP_ACP, 0, strPath, -1, 
                         wstrSearchPath, MAX_PATH );
 
 
    // Set the search directory.
 
    g_pLoader->SetSearchDirectory( 
        GUID_DirectMusicAllTypes,   
        wstrSearchPath,             
        FALSE                       
    );
}

void CFJ_DX8Audio::End()
{
	g_pPerformance->Stop(
        NULL,   
        NULL,   
        0,      
        0       
    );
 
    g_pPerformance->CloseDown();
	
 
    g_pLoader->Release(); 
    g_pPerformance->Release();
	
	CoUninitialize();
}

int CFJ_DX8Audio::PlaySound(char *file,bool primary)
{
	return PlaySound(file,primary,0);
}

int CFJ_DX8Audio::PlaySound(char *file,bool prim,int vol)
{
	IDirectMusicSegment8*     g_pSegment = NULL;
	DWORD flag;
	HRESULT res;

	char wstrFileName[256];
	strcpy(wstrFileName,file);
	WCHAR wstrSearchPath[MAX_PATH];

    MultiByteToWideChar( CP_ACP, 0, wstrFileName, -1, 
                         wstrSearchPath, MAX_PATH );
 
	
	if(prim == true)
		flag = DMUS_SEGF_DEFAULT ;
	else
		flag = DMUS_SEGF_DEFAULT |DMUS_SEGF_SECONDARY ;

    res = g_pLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,IID_IDirectMusicSegment8,
                                  wstrSearchPath,(LPVOID*) &g_pSegment );
    if(res != S_OK)
		return 1;
	g_pSegment->Download( g_pPerformance );

	

	IDirectMusicAudioPath8* p3DAudioPath = NULL;
	g_pPerformance->CreateStandardAudioPath( 
					DMUS_APATH_DYNAMIC_STEREO ,      
					64,                         
					TRUE,                       
					&p3DAudioPath               
					);

	p3DAudioPath->SetVolume(vol,0);
	g_pPerformance->PlaySegmentEx(
        g_pSegment,  
        NULL,        
        NULL,        
        flag,           
        0,          
        NULL,        
        NULL,        
        p3DAudioPath         
    );     
    return 0;
}

int CFJ_DX8Audio::PlaySound(int i,bool prim,bool loop,int vol)
{
	lstsnd *t = snds;
	int id=0;
	DWORD flag;
	HRESULT res;

	while(id != i && t!=NULL)
	{
		t = t->next;
		id++;
	}
	if(t!=NULL)
	{
		
		if(prim == true)
			flag = DMUS_SEGF_DEFAULT ;
		else
			flag = DMUS_SEGF_DEFAULT |DMUS_SEGF_SECONDARY ;
		if(loop == true)
			t->g_pSegment->SetRepeats(DMUS_SEG_REPEAT_INFINITE );
		
		IDirectMusicAudioPath8* p3DAudioPath = NULL;
		res=g_pPerformance->CreateStandardAudioPath( 
					DMUS_APATH_DYNAMIC_STEREO ,     
					64,                         
					TRUE,                       
					&p3DAudioPath              
					);

		p3DAudioPath->SetVolume(vol,0);
		//t->g_pSegment->SetTrackConfig(CLSID_DirectMusicStyleTrack,-1, DMUS_SEG_ALLTRACKS, DMUS_TRACKCONFIG_OVERRIDE_PRIMARY , 0);
		//t->g_pSegment->Download( g_pPerformance );
		res=g_pPerformance->PlaySegmentEx(
										t->g_pSegment,  
										NULL,        
										NULL,        
										flag,           
										0,           
										NULL,        
										NULL,        
										p3DAudioPath        
									);      
		p3DAudioPath->Release();
	}
	return 0;
}

void CFJ_DX8Audio::StopSound(int i)
{
	lstsnd *t = snds;
	int id=0;
	//DWORD flag;

	while(id != i && t!=NULL)
	{
		t = t->next;
		id++;
	}
	g_pPerformance->Stop(t->g_pSegment,NULL,0,DMUS_SEGF_DEFAULT);
}
void CFJ_DX8Audio::LoadSound(char *nom)
{
	lstsnd *pt,*t = snds;
	char wstrFileName[256];
	HRESULT res;
	WCHAR wstrSearchPath[MAX_PATH];
	
	if(snds == NULL)
	{
		snds = new lstsnd;
		strcpy(snds->nom,nom);
		
		strcpy(wstrFileName,nom);
	
		MultiByteToWideChar( CP_ACP, 0, wstrFileName, -1, 
                         wstrSearchPath, MAX_PATH );
 
		res = g_pLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,IID_IDirectMusicSegment8,
                                  wstrSearchPath,(LPVOID*) &snds->g_pSegment );
		snds->g_pSegment->Download( g_pPerformance );
		
		snds->next = NULL;
	}
	else
	{
		while(t->next!= NULL)
			t= t->next;
		pt = new lstsnd;
		strcpy(pt->nom,nom);
		strcpy(wstrFileName,nom);
	
		MultiByteToWideChar( CP_ACP, 0, wstrFileName, -1, 
                         wstrSearchPath, MAX_PATH );
 
		res = g_pLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,IID_IDirectMusicSegment8,
                                  wstrSearchPath,(LPVOID*) &pt->g_pSegment );

		res=pt->g_pSegment->Download( g_pPerformance );

		pt->next = NULL;
		t->next = pt;
	}
}

void CFJ_DX8Audio::DeleteSound()
{
}
