#include <Windowsx.h>
#include <time.h>

//RANDOM
class CFJ_Random
{
	public:
		CFJ_Random(){ordinal = 1;srand((int)time(NULL));};
		CFJ_Random(int ord){ordinal = ord;srand((int)time(NULL));};
		~CFJ_Random(){};
		int operator () (){return rand()%ordinal+1;};
		int operator () (int ord){return rand()%ord+1;};
	private:
		int ordinal;
		
};

CFJ_Random d4(4);
CFJ_Random d6(6);
CFJ_Random d8(8);
CFJ_Random d10(10);
CFJ_Random d12(12);
CFJ_Random d20(20);
CFJ_Random d100(100);
CFJ_Random dd;

//TIMER

void Wait(int t)
{
	_int64 start, end, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	QueryPerformanceCounter((LARGE_INTEGER*)&start);    // the main message loop begins here
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	while((end-start)/freq < t)
		QueryPerformanceCounter((LARGE_INTEGER*)&end);
}

class CFJ_Timer
{
	public:
		CFJ_Timer(){QueryPerformanceFrequency((LARGE_INTEGER*)&freq);set=false;};
		~CFJ_Timer(){};
		void Start();
		void Stop();
		float operator()();
	private:
		_int64 start, freq;
		bool set;
};

void CFJ_Timer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
	set = true;
}

void CFJ_Timer::Stop()
{
	set = false;
}

float CFJ_Timer::operator() ()
{
	_int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);

	return (float)(now-start)/freq;
}