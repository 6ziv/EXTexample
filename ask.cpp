#if defined(_WIN32)
#include<conio.h>
#else 
#include <curses.h>
#endif
#include<stdio.h>
#include<stdlib.h>
FILE *file;
char ch;
int main(){
	file=fopen("switches.h","w+");
	#if not defined(_WIN32)
	initscr();
	#endif
	printf("是否在程序启动时检查算法正确性（调试用）？[Y/N]\r\n");
	do{
	#if defined(_WIN32)
		ch=getch();
	#else
		ch=getchar();
	#endif
	}while((ch!='Y')&&(ch!='y')&&(ch!='N')&&(ch!='n'));
	if((ch=='Y')||(ch=='y')){
		fwrite("#define _SELF_TEST_\r\n",20,1,file);		
	}
	printf("是否在新建硬盘时进行随机存取测试（调试用）？[Y/N]\r\n");
	do{
	#if defined(_WIN32)
		ch=getch();
	#else 
		ch=getchar();
	#endif
	}while((ch!='Y')&&(ch!='y')&&(ch!='N')&&(ch!='n'));
	if((ch=='Y')||(ch=='y'))
		fwrite("#define _CHECK_DISK_\r\n",21,1,file);
	printf("是否启动硬盘篡改检查？（会降低程序启动速度）[Y/N]\r\n");
	do{
	#if defined(_WIN32)
		ch=getch();
	#else 
		ch=getchar();
	#endif
	}while((ch!='Y')&&(ch!='y')&&(ch!='N')&&(ch!='n'));
	if((ch=='N')||(ch=='n'))
		fwrite("#define _NO_KEY_\r\n",17,1,file);	
	fclose(file);
	#if not defined(_WIN32)
	endwin();
	#endif
}
