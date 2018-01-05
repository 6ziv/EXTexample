#include "disk.h"
static int tmod=0;
int Now(){
	return(time(0)+tmod);
}
void cmod(int t){
	tmod=t;
}
void PrintTime(int ts){
	time_t tmp;
	tmp=ts;
}
void PrintTime(){
	time_t tmp;
	tmp=time(0);
	printf("%s",ctime(&tmp));
}

