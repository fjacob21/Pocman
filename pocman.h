#include <stdio.h>
#include <math.h>
//Message

typedef struct _lstmes
{
	int mess;
	_lstmes *next;
}lstmes;

#define UP    1
#define DOWN  2
#define RIGHT 3
#define LEFT  4

class CFJ_Ctrl
{
	public:
		CFJ_Ctrl();
		~CFJ_Ctrl(){Clear();};
		void operator + (int m);
		int GetMessage(int i);
		void Clear();
	private:
		lstmes *message;

};

CFJ_Ctrl::CFJ_Ctrl()
{
	message = NULL;
}

void CFJ_Ctrl::operator + (int m)
{
	lstmes *pt,*t = message;
	if(message == NULL)
	{
		message = new lstmes;
		message->mess = m;
		message->next = NULL;
	}
	else
	{
		while(t->next!= NULL)
			t= t->next;
		pt = new lstmes;
		pt->mess = m;
		pt->next = NULL;
		t->next = pt;
	}
}
int CFJ_Ctrl::GetMessage(int i)
{
	lstmes *t=message;
	int id=0;
	while(id!=i && t!=NULL)
	{
		t = t->next;
		id++;
	}
	if(t!=NULL)
		return t->mess;
	else
		return -1;
}
void CFJ_Ctrl::Clear()
{
	lstmes *t;
	while(message !=NULL)
	{
		t = message;
		message = message->next;
		delete(t);
	}
}
		
///Sprite

typedef struct _lstimg
{
	char nom[25];
	_lstimg *next;
}lstimg;


class CFJ_Sprite
{
	public:
		CFJ_Sprite();
		~CFJ_Sprite();
		void AddImg(char *nom);
		void operator ++();
		void operator ++(int);
		char *operator()();
	private:
		int nbimg;
		lstimg *imgs;
		int currimg;
};

CFJ_Sprite::CFJ_Sprite()
{
	imgs = NULL;
	nbimg = 0;
}

CFJ_Sprite::~CFJ_Sprite()
{
	while(imgs!=NULL)
	{
		lstimg *t=imgs;
		imgs = imgs->next;
		delete(t);
	}
}
void CFJ_Sprite::AddImg(char *nom)
{

	lstimg *pt,*t = imgs;
	if(imgs == NULL)
	{
		imgs = new lstimg;
		strcpy_s(imgs->nom,25,nom);
		imgs->next = NULL;
	}
	else
	{
		while(t->next!= NULL)
			t= t->next;
		pt = new lstimg;
		strcpy_s(pt->nom,25,nom);
		pt->next = NULL;
		t->next = pt;
	}
	nbimg++;
}

void CFJ_Sprite::operator ++()
{
	currimg++;
	if(currimg > nbimg)
		currimg = 0;
}

void CFJ_Sprite::operator ++(int)
{
	currimg++;
	if(currimg > nbimg)
		currimg = 0;
}

char *CFJ_Sprite::operator()()
{
	lstimg *t = imgs;
	int i=0;
	while(t->next!= NULL && i!=currimg)
	{
		t= t->next;
		i++;
	}
			
	return t->nom;
}

///OBJ
class CFJ_PocOBJ
{
	public:
		CFJ_PocOBJ(){};
		~CFJ_PocOBJ(){};
		void AddImg(char *nom){norm.AddImg(nom);};
		char * GetImg() {return norm();};
		void CLK(){norm++;};
		int x,y;
	protected:
		CFJ_Sprite norm;
		
};

class CFJ_SuperBouf :public CFJ_PocOBJ
{
	public:
		CFJ_SuperBouf(){};
		~CFJ_SuperBouf(){};
	private:
};
////POC
class CFJ_Poc :public CFJ_PocOBJ
{
	public:
		CFJ_Poc();
		~CFJ_Poc(){};
		void ResetBouffe();
		void SetSuperPoc(bool sup);
		void Reset();
		void CLK();
		bool Super(){return superpoc;};
		void operator ++(){nbbouf++;};
		char *operator()();
		int DelVie(){vie--;return vie;};
		int AddVie(){vie++;return vie;};
		int GetVie(){return vie;};
		int dir;
	protected:
		bool superpoc;
		int nbbouf;
		CFJ_Sprite normU,normD,normR,normL;
		CFJ_Sprite sup;
		int vie;
};

CFJ_Poc::CFJ_Poc()
{
	vie = 3;
	nbbouf = 0;
	superpoc = false;
	normR.AddImg("poc1.bmp");
	normR.AddImg("poc2.bmp");
	normL.AddImg("poc1L.bmp");
	normL.AddImg("poc2L.bmp");
	normU.AddImg("poc1D.bmp");
	normU.AddImg("poc2U.bmp");
	normD.AddImg("poc1U.bmp");
	normD.AddImg("poc2D.bmp");
	sup.AddImg("monstreR2.bmp");
	sup.AddImg("monstreB2.bmp");
	sup.AddImg("monstreV2.bmp");
	sup.AddImg("monstreO2.bmp");

	dir = LEFT;
}

void CFJ_Poc::ResetBouffe()
{
	nbbouf = 0;
}

void CFJ_Poc::SetSuperPoc(bool sup)
{
	superpoc = sup;
}

void CFJ_Poc::Reset()
{
	vie = 3;
	nbbouf = 0;
	superpoc = false;
}

void CFJ_Poc::CLK()
{
	if(superpoc == true)
		sup++;
	else
	{
		switch(dir)
		{
			case UP:
				normU++;
			case DOWN:
				normD++;
			case RIGHT:
				normR++;
			case LEFT:
				normL++;
		};
	}
}

char *CFJ_Poc::operator()()
{
	if(superpoc == true)
		return sup();
	else
	{
		switch(dir)
		{
			case UP:
				return normU();
			case DOWN:
				return normD();
			case RIGHT:
				return normR();
			case LEFT:
				return normL();
            default:
                return NULL;
		};
	}
}

//level
typedef struct _tablevel
{
	int type;
	CFJ_PocOBJ *ptobj;
	_tablevel *prev;
}tablevel;

typedef struct _lstpath
{
	int x,y;
	float dist;
	_lstpath *next;
}lstpath;

#define VIDE      0
#define MUR       1
#define BOUF      2
#define SUPERBOUF 3
#define MONSTRE   4
#define POC       5
#define PORTE     6

class CFJ_Level
{
	public:
		CFJ_Level();
		~CFJ_Level();
		void Load();
		void AddObj(CFJ_PocOBJ *obj,unsigned int x,unsigned int y,int type);
		void DelObj(CFJ_PocOBJ *obj);
		void DelObj(unsigned int x,unsigned int y);
		int MoveObj(CFJ_PocOBJ *obj,unsigned int x,unsigned int y,bool forced=false);
		int Compte(int type);
		int CLK(CFJ_DX9Screen *sc,CFJ_DX8Audio *sp,CFJ_Ctrl *ct);
		
	private:
		tablevel lev[169];
		CFJ_PocOBJ bouf[51];
		CFJ_SuperBouf supbouf[4];
		CFJ_PocOBJ monstre[5];
		CFJ_Poc pj;
		int cmpt,cmpsup;
		int vitess;
		void Draw(CFJ_DX9Screen *sc);
		void Init();
		void Clear();
};

CFJ_Level::CFJ_Level()
{
	for(int i=0;i<169;i++)
	{
		lev[i].type  = VIDE;
		lev[i].ptobj = NULL;
		lev[i].prev = new tablevel;
		lev[i].prev->type  = VIDE;
		lev[i].prev->ptobj = NULL;
	}
	cmpt = cmpsup = 0;
	vitess=5;
	monstre[0].AddImg("monstreR1.bmp");
	monstre[0].AddImg("monstreR2.bmp");
	monstre[1].AddImg("monstreB1.bmp");
	monstre[1].AddImg("monstreB2.bmp");
	monstre[2].AddImg("monstreV1.bmp");
	monstre[2].AddImg("monstreV2.bmp");
	monstre[3].AddImg("monstreJ1.bmp");
	monstre[3].AddImg("monstreJ2.bmp");
	monstre[4].AddImg("monstreO1.bmp");
	monstre[4].AddImg("monstreO2.bmp");
}

CFJ_Level::~CFJ_Level()
{
	for(int i=0;i<169;i++)
		delete(lev[i].prev);
}

void CFJ_Level::Load()
{

	AddObj(&pj,12,5,POC);
	AddObj(&monstre[0],6,4,MONSTRE);
	AddObj(&monstre[1],5,5,MONSTRE);
	AddObj(&monstre[2],7,5,MONSTRE);
	AddObj(&monstre[3],6,6,MONSTRE);
	AddObj(&monstre[4],6,5,MONSTRE);

	AddObj(NULL,1,1,SUPERBOUF);
	AddObj(NULL,1,9,SUPERBOUF);
	AddObj(NULL,11,1,SUPERBOUF);
	AddObj(NULL,11,9,SUPERBOUF);
	
	AddObj(NULL,2,1,MUR);AddObj(NULL,2,2,MUR);AddObj(NULL,1,2,MUR);AddObj(NULL,10,1,MUR);
	AddObj(NULL,10,2,MUR);AddObj(NULL,11,2,MUR);AddObj(NULL,1,8,MUR);AddObj(NULL,2,8,MUR);
	AddObj(NULL,2,9,MUR);AddObj(NULL,10,8,MUR);AddObj(NULL,10,9,MUR);AddObj(NULL,11,8,MUR);
	AddObj(NULL,1,4,MUR);AddObj(NULL,1,5,MUR);AddObj(NULL,1,6,MUR);AddObj(NULL,2,5,MUR);
	AddObj(NULL,10,5,MUR);AddObj(NULL,11,4,MUR);AddObj(NULL,11,5,MUR);AddObj(NULL,11,6,MUR);
	AddObj(NULL,4,3,MUR);AddObj(NULL,4,4,MUR);AddObj(NULL,5,3,MUR);AddObj(NULL,7,3,MUR);
	AddObj(NULL,8,3,MUR);AddObj(NULL,8,4,MUR);AddObj(NULL,4,6,MUR);AddObj(NULL,4,7,MUR);
	AddObj(NULL,5,7,MUR);AddObj(NULL,7,7,MUR);AddObj(NULL,8,6,MUR);AddObj(NULL,8,7,MUR);
	AddObj(NULL,4,1,MUR);AddObj(NULL,5,1,MUR);AddObj(NULL,6,1,MUR);AddObj(NULL,7,1,MUR);AddObj(NULL,8,1,MUR);
	AddObj(NULL,4,9,MUR);AddObj(NULL,5,9,MUR);AddObj(NULL,6,9,MUR);AddObj(NULL,7,9,MUR);AddObj(NULL,8,9,MUR);
	
	AddObj(NULL,5,4,PORTE);AddObj(NULL,6,4,PORTE);AddObj(NULL,7,4,PORTE);
	AddObj(NULL,5,5,PORTE);AddObj(NULL,6,5,PORTE);AddObj(NULL,7,5,PORTE);
	AddObj(NULL,5,6,PORTE);AddObj(NULL,6,6,PORTE);AddObj(NULL,7,6,PORTE);
	AddObj(NULL,6,3,PORTE);AddObj(NULL,4,5,PORTE);AddObj(NULL,6,7,PORTE);AddObj(NULL,8,5,PORTE);

	for(int i=0;i<11;i++)
	{
		for(int j=0;j<13;j++)
		{
			if(lev[i*13+j].type == VIDE)
				AddObj(NULL,j,i,BOUF);
		}
	}
}

void CFJ_Level::AddObj(CFJ_PocOBJ *obj,unsigned int x,unsigned int y,int type)
{
	if(x < 13 && y <13)
	{
		if(lev[y*13+x].type  == VIDE)
		{
			lev[y*13+x].type = type;
			lev[y*13+x].ptobj = obj;
			if(obj != NULL)
			{
				obj->x = x;
				obj->y = y;
			}
		}
	}
}

void CFJ_Level::DelObj(CFJ_PocOBJ *obj)
{
	lev[obj->y*13+obj->x].ptobj = lev[obj->y*13+obj->x].prev->ptobj;
	lev[obj->y*13+obj->x].type  = lev[obj->y*13+obj->x].prev->type;
}

void CFJ_Level::DelObj(unsigned int x,unsigned int y)
{
	lev[y*13+x].ptobj = lev[y*13+x].prev->ptobj;
	lev[y*13+x].type  = lev[y*13+x].prev->type;
}

int CFJ_Level::MoveObj(CFJ_PocOBJ *obj,unsigned int x,unsigned int y,bool forced)
{
	if(x < 13 && y <11)
	{
		if(lev[y*13+x].type  == VIDE || forced == true)
		{
			lev[y*13+x].prev->ptobj = lev[y*13+x].ptobj;
			lev[y*13+x].prev->type = lev[y*13+x].type;
			lev[y*13+x].type = lev[obj->y*13+obj->x].type;
			lev[y*13+x].ptobj = obj;
			lev[obj->y*13+obj->x].ptobj = lev[obj->y*13+obj->x].prev->ptobj;
			lev[obj->y*13+obj->x].type  = lev[obj->y*13+obj->x].prev->type;
			lev[obj->y*13+obj->x].prev->ptobj = NULL;
			lev[obj->y*13+obj->x].prev->type  = VIDE;
			obj->x = x;
			obj->y = y;
		}
		else
			return lev[y*13+x].type;
	}
	return -1;
}

int CFJ_Level::Compte(int type)
{
	int nb=0;
	for(int i=0;i<169;i++)
		if(lev[i].type == type)
			nb++;
	return nb;
}

int CFJ_Level::CLK(CFJ_DX9Screen *sc,CFJ_DX8Audio *sp,CFJ_Ctrl *ct)
{
	int i,j,idmess=0;
	
	//deplace pj selon table d'action
	while(ct->GetMessage(idmess) != -1)
		idmess++;
	if(idmess != 0)
		pj.dir = ct->GetMessage(idmess-1);
		switch(pj.dir)
		{
			case UP:
				i=MoveObj(&pj,pj.x,pj.y-1);
				if(i == BOUF || i == SUPERBOUF)
				{
					DelObj(pj.x,pj.y-1);
					MoveObj(&pj,pj.x,pj.y-1);
					sp->PlaySound(0,false,false,-500);
					if(i == SUPERBOUF)
					{
						pj.SetSuperPoc(true);
						cmpsup=0;
					}
				}
			break;
			case DOWN:
				i=MoveObj(&pj,pj.x,pj.y+1);
				if(i == BOUF|| i == SUPERBOUF)
				{
					DelObj(pj.x,pj.y+1);
					MoveObj(&pj,pj.x,pj.y+1);
					sp->PlaySound(0,false,false,-500);
					if(i == SUPERBOUF)
					{
						pj.SetSuperPoc(true);
						cmpsup=0;
					}
				}
			break;
			case RIGHT:
				i=MoveObj(&pj,pj.x+1,pj.y);
				if(i == BOUF|| i == SUPERBOUF)
				{
					DelObj(pj.x+1,pj.y);
					MoveObj(&pj,pj.x+1,pj.y);
					sp->PlaySound(0,false,false,-500);
					if(i == SUPERBOUF)
					{
						pj.SetSuperPoc(true);
						cmpsup=0;
					}
				}
				if(pj.x == 12 && pj.y == 5)
				{
					i=MoveObj(&pj,0,5);
					if(i == BOUF|| i == SUPERBOUF)
					{
						DelObj(0,5);
						MoveObj(&pj,0,5);
						sp->PlaySound(0,false,false,-500);
						if(i == SUPERBOUF)
						{
							pj.SetSuperPoc(true);
							cmpsup=0;
						}
					}
				}

			break;
			case LEFT:
				i=MoveObj(&pj,pj.x-1,pj.y);
				if(i == BOUF|| i == SUPERBOUF)
				{
					DelObj(pj.x-1,pj.y);
					MoveObj(&pj,pj.x-1,pj.y);
					sp->PlaySound(0,false,false,-500);
					if(i == SUPERBOUF)
					{
						pj.SetSuperPoc(true);
						cmpsup=0;
					}
				}
				if(pj.x == 0 && pj.y == 5)
				{
					i=MoveObj(&pj,12,5);
					if(i == BOUF|| i == SUPERBOUF)
					{
						DelObj(12,5);
						MoveObj(&pj,12,5);
						sp->PlaySound(0,false,false,-500);
						if(i == SUPERBOUF)
						{
							pj.SetSuperPoc(true);
							cmpsup=0;
						}
					}
				}
			break;
		};
	if(pj.Super() == true)
	{
		cmpsup++;
		if(cmpsup > 20)
		{
			pj.SetSuperPoc(false);
		}
	}
	pj.CLK();
	//deplace monstre
	lstpath posibilite[8];lstpath t;
	int x,y,cx=pj.x,cy=pj.y,pluspetit,typ;
	cmpt++;
	if(cmpt > vitess)
	{
		for(int id=0;id<5;id++)
		{
			//remplir le tab des path possibles
			x= monstre[id].x;
			y= monstre[id].y;
			pluspetit=0;

			for(i=y-1;i<=y+1&&i<10 ;i++)
				for(j=x-1;j<=x+1&& i<10&&i>=0 ;j++)
				{
					if(!(i == y && j == x)&& j>=0 && i<11 && j <13)
					{
						posibilite[pluspetit].dist = (float)(max(abs(cx-j),abs(cy-i)) + sqrt(pow((double)abs(cy-i),2)+pow((double)abs(cx-j),2)));
						posibilite[pluspetit].x = j;
						posibilite[pluspetit].y = i;
						pluspetit++;
					}
				}

			//trier le tab de path selon la dist
			for(i=0;i<pluspetit;i++)
				for(j=i+1;j<pluspetit;j++)
				{
					if(posibilite[j].dist < posibilite[i].dist)
					{
					
					t.dist = posibilite[i].dist;
					t.x  = posibilite[i].x;
					t.y  = posibilite[i].y;
				
					posibilite[i].dist = posibilite[j].dist;
					posibilite[i].x  = posibilite[j].x;
					posibilite[i].y  = posibilite[j].y;

					posibilite[j].dist = t.dist;
					posibilite[j].x  = t.x;
					posibilite[j].y  = t.y;
					}
				}

			for(i=0;i<pluspetit;i++)
			{
				typ = MoveObj(&monstre[id],posibilite[i].x,posibilite[i].y);
				if(typ == PORTE || typ == BOUF|| typ == SUPERBOUF)
					typ=MoveObj(&monstre[id],posibilite[i].x,posibilite[i].y,true);
				if(typ == -1)
					break;
				if(typ == POC && pj.Super()!=true)
				{
					if(pj.DelVie()==0)
					{
						pj.SetSuperPoc(true);
						CFJ_Timer tend;
						tend.Start();
						i=0;
						while(i<200)
						{
							if(tend()> 0)
							{
								Draw(sc);
								pj.CLK();
								tend.Start();
								i++;
							}
						}
				
						return 1;
					}
					Init();
					id = 5;
					break;
				}
			}
			monstre[id].CLK();
		}
		cmpt=0;
	}
	if(Compte(BOUF) == 0)
	{
		Clear();
		Load();
		pj.AddVie();
		vitess--;
		sp->StopSound(1);
		if(vitess <1)
			return 1;
	}
	//affiche ecran
	Draw(sc);
	return 0;
}

void CFJ_Level::Draw(CFJ_DX9Screen *sc)
{
	RECT r={0,0,300,40};
	char bou[10];

    sc->BeginDraw();
	sc->Clear(0,125,0);
	sc->DrawImage(3,275,15);
	sc->DrawImage(2,190,130);
	sprintf(bou,"Bouf: %d",Compte(BOUF));
	sc->DrawText(bou,"Comic Sans MS",30,r);
	r.top =40;
	r.bottom=80;
	sprintf(bou,"Vie: %d",pj.GetVie());
	sc->DrawText(bou,"Comic Sans MS",30,r);

	for(int i=0;i<11;i++)
	{
		for(int j=0;j<13;j++)
		{
			if(lev[i*13+j].type == MUR)
				sc->DrawImage(0,j*30+195,i*30+135);
			if(lev[i*13+j].type == BOUF)
				sc->DrawImage(1,j*30+195,i*30+135);
			if(lev[i*13+j].type == SUPERBOUF)
				sc->DrawImage("supbouf.bmp",j*30+195,i*30+135);
			if(lev[i*13+j].type == MONSTRE)
				sc->DrawImage(lev[i*13+j].ptobj->GetImg(),j*30+195,i*30+135);
			if(lev[i*13+j].type == POC)
				sc->DrawImage(pj(),j*30+195,i*30+135);
			
		}
	}
    sc->EndDraw();
	sc->Show();
}

void CFJ_Level::Init()
{
	MoveObj(&monstre[0],6,4,true);
	MoveObj(&monstre[1],5,5,true);
	MoveObj(&monstre[2],7,5,true);
	MoveObj(&monstre[3],6,6,true);
	MoveObj(&monstre[4],6,5,true);
	MoveObj(&pj,12,5);
}

void CFJ_Level::Clear()
{
	DelObj(&monstre[0]);
	DelObj(&monstre[1]);
	DelObj(&monstre[2]);
	DelObj(&monstre[3]);
	DelObj(&monstre[4]);
	DelObj(&pj);
}