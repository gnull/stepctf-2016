#include <windows.h>
#include <vector>

/*
00 01 02 03 04
05 06 07 08 09
10 11 12 13 14
15 16 17 18 19
20 21 22 23 24
*/
unsigned int a1 [] = {1,0,5,10,15,20,21};
unsigned int a2 [] = {17,12,7,2,1,2,3};
unsigned int a3 [] = {1,0,5,10,15,20,15,10,11};
unsigned int a4 [] = {0,5,10,15,20};
#pragma section(".data",read,write)
__declspec(allocate(".data")) char key [] = "abcdefghijklmnopqrstuvwxyz";

std::vector<int> track;
int state = 0;
bool testTrack1(){
	if (track.size() < 7)
		return false;
	for (int i=0; i<7;i++){
		if (a1[i] != track[track.size()-7+i])
			return false;
	}
	for (int i=0; i<7;i++){
		char tmp = key[a1[i]];
		key[a1[i]] = key[a1[i]+1];
		key[a1[i]+1] = tmp;
	}
	return true;
}
bool testTrack2(){
	if (track.size() < 7)
		return false;
	for (int i=0; i<7;i++){
		if (a2[i] != track[track.size()-7+i])
			return false;
	}
	for (int i=0; i<7;i++){
		char tmp = key[a2[i]];
		key[a2[i]] = key[a2[i]+1];
		key[a2[i]+1] = tmp;
	}
	return true;
}
bool testTrack3(){
	if (track.size() < 9)
		return false;
	for (int i=0; i<9;i++)
		if (a3[i] != track[track.size()-9+i])
			return false;
	for (int i=0; i<9;i++){
		char tmp = key[a3[i]];
		key[a3[i]] = key[a3[i]+1];
		key[a3[i]+1] = tmp;
	}
	return true;
}
bool testTrack4(){
	if (track.size() < 5)
		return false;
	for (int i=0; i<5;i++)
		if (a4[i] != track[track.size()-5+i])
			return false;
	for (int i=0; i<5;i++){
		char tmp = key[a4[i]];
		key[a4[i]] = key[a4[i]+1];
		key[a4[i]+1] = tmp;
	}
	return true;
}
int main (){
	int xres = GetSystemMetrics(SM_CXSCREEN);
	int yres = GetSystemMetrics(SM_CYSCREEN);
	int cubes_pix = yres/5;
	POINT Pos;
	int lastcub = -1,curcub = 0;
	bool lp=true;
	while (lp){
		GetCursorPos(&Pos);
		int xcub = Pos.x / cubes_pix;
		int ycub = Pos.y / cubes_pix;
		curcub = ycub * 5 + xcub;
		if (lastcub != curcub){
			lastcub = curcub;
			track.push_back(lastcub);
			switch (state){
				case 0:
					if (testTrack1())
						state=1;
				break;
				case 1:
					if (testTrack2())
						state=2;
				break;
				case 2:
					if (testTrack3())
						state=3;
				break;
				case 3:
					if (testTrack4())
						state=4;
				break;
				case 4:
					printf("\nOKAY! STCTF#%s#\n",key);
					return 0;
				break;
				default:
				break;
			};
			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\blast:%02d state:%02d",lastcub,state);
		}
		Sleep(200);
	}
}
// STCTF#cbdeagfihjnlkmopqsrtvwuxyz#
// CTFI