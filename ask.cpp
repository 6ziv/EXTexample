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
	printf("�Ƿ��ڳ�������ʱ����㷨��ȷ�ԣ������ã���[Y/N]\r\n");
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
	printf("�Ƿ����½�Ӳ��ʱ���������ȡ���ԣ������ã���[Y/N]\r\n");
	do{
	#if defined(_WIN32)
		ch=getch();
	#else 
		ch=getchar();
	#endif
	}while((ch!='Y')&&(ch!='y')&&(ch!='N')&&(ch!='n'));
	if((ch=='Y')||(ch=='y'))
		fwrite("#define _CHECK_DISK_\r\n",21,1,file);
	printf("�Ƿ�����Ӳ�̴۸ļ�飿���ή�ͳ��������ٶȣ�[Y/N]\r\n");
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
