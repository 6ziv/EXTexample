#include "disk.h"
const data zeromatrix={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
data waste;
bool running;
bool oe;
#if not defined(_WIN32)
extern bool hhhhidden;
#endif
void beforeexit(){
if(oe)return;
oe=true;
#if not defined(_WIN32)
if(hhhhidden){
endwin();
hhhhidden=false;
}
#endif
printf("Quiting program:Exiting active writing procedure.\r\n");
put(wheretoput,whattoput,howtoput);
printf("Quiting program:Cleaning up.\r\n");
aesinit(zeromatrix);
aes(zeromatrix,waste);
md5(zeromatrix,waste);
clean(waste);
fclose(fil);
fil=NULL;
printf("Quiting program:Finish.\r\n");
printf("Thanks for usage.");
PrintTime();
#if defined(_WIN32)
Sleep(2000);
#else
sleep(2);
#endif
exit(0);
return;
}
void beforeexit(int i){
	beforeexit();
}
void initenv(){
	oe=false;
	setup=false;
	running=false;
#if not defined(_WIN32)
	hhhhidden=false;
#endif
	if (atexit(beforeexit)!=0){
		printf("Protection initialization failed.");
	}

    signal(SIGTERM,beforeexit);
    signal(SIGINT,beforeexit);
#if not defined(_WIN32)
    signal(SIGPIPE,SIG_IGN);
    signal(SIGBUS,beforeexit);     // 总线错误
#endif
    signal(SIGSEGV,beforeexit);    // SIGSEGV，非法内存访问
    signal(SIGFPE,beforeexit);       // SIGFPE，数学相关的异常，如被0除，浮点溢出，等等
    signal(SIGABRT,beforeexit);     // SIGABRT，由调用abort函数产生，进程非正常退出
	pretest();
}
