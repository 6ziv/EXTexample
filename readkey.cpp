#include "disk.h"
extern data key;
static data k,tmp;
static unsigned char ch;
static char kkk[64];
static int i,j,q;
static bool shown;
#if not defined(_WIN32)
bool hhhhidden;
#endif
void loadkey(data rtn){
#ifndef _NO_KEY_

	shown=false;
#if defined(_WIN32)
	if(!oe)printf("Please input key.Press ctrl+c or hold F1 to switch between hide/show,enter to finish.\r\n>> ");
#else
	if(!oe)printf("As I do not have a PC with linux,development on Linux version is hard.Thus,this password input area has a lot of problems.\r\nI sincerely apology for that.Press F2 to switch display mode.\r\n>> ");
#endif
	for(i=0;i<16;i++)k[i]=0;
	i=0;
	q=0;
	#if not defined(_WIN32)
		hhhhidden=true;	
		initscr();
	#endif
	do{
#if defined(_WIN32)
	  	ch=getch();
#else
		ch=getchar();
#endif
		if(ch=='\r')break;
		if(ch=='\n')break;
	#if defined(_WIN32)
		if(ch=='\b'){
	#else
		if(ch==127){
	#endif
			if(q==0)continue;
			if(!oe)printf("\b \b");
			for(j=q;j<i;j++){
				kkk[j-1]=kkk[j];
			if(shown){if(!oe)printf("%c",kkk[j]);}else printf("*");
			}
			if(!oe)printf(" \b");
			for(j=q;j<i;j++)if(!oe)printf("\b");
			i--;q--;
		}else
	#if defined(_WIN32)
		{
			if(ch==0){
	  	ch=getch();
				

				if(ch==59){
					shown=!shown;
					if(shown){
						if(!oe)printf("\r>> ");
						for(j=0;j<i;j++)
							if(!oe)printf("%c",kkk[j]);
						for(j=q;j<i;j++)
							if(!oe)printf("\b");
					}else{
						if(!oe)printf("\r>> ");
						for(j=0;j<i;j++)
							if(!oe)printf("*");
						for(j=q;j<i;j++)
							if(!oe)printf("\b");
					}
					do{}while(GetKeyState(0x70)<0);
	  				while (kbhit())getch();
					shown=!shown;
					if(shown){
						if(!oe)printf("\r>> ");
						for(j=0;j<i;j++)
							if(!oe)printf("%c",kkk[j]);
						for(j=q;j<i;j++)
							if(!oe)printf("\b");
					}else{
						if(!oe)printf("\r>> ");
						for(j=0;j<i;j++)
							if(!oe)printf("*");
						for(j=q;j<i;j++)
							if(!oe)printf("\b");
					}
				}

				continue;
			}

			if(ch==3){
				shown=!shown;
				if(shown){
					if(!oe)printf("\r>> ");
					for(j=0;j<i;j++)
						if(!oe)printf("%c",kkk[j]);
					for(j=q;j<i;j++)
						if(!oe)printf("\b");
				}else{
					if(!oe)printf("\r>> ");
					for(j=0;j<i;j++)
						if(!oe)printf("*");
					for(j=q;j<i;j++)
						if(!oe)printf("\b");
				}
				continue;
			}
			if(ch==224){
				ch=getch();
				if(ch==75)
					if(q>0){
						if(!oe)printf("\b");
						q--;
					}
				if(ch==77)
					if(q<i){
						if(shown)
							{if(!oe)printf("%c",kkk[q]);}
						else {if(!oe)printf("*");}
						q++;
						}
				if(ch==83)
					if(q<i){
						for(j=q;j<i-1;j++){
							kkk[j]=kkk[j+1];
							if(shown)
								{if(!oe)printf("%c",kkk[j]);}
							else {if(!oe)printf("*");}
						}
						if(!oe)printf(" \b");
						for(j=q;j<i-1;j++)
							if(!oe)printf("\b");
						i--;
					}
					continue;
				}
			if(ch<32)continue;
			if(ch>126){
				getch();
				continue;
			}
			if(i>=64)
				continue;
#else
{
	   		if(ch==27){
			  	ch=getchar();
				if(ch==79){		
				  	ch=getchar();
					if(ch==81){
						shown=!shown;
						if(shown){
							if(!oe)printf("\r>> ");
							for(j=0;j<i;j++)
								if(!oe)printf("%c",kkk[j]);
							for(j=q;j<i;j++)
								if(!oe)printf("\b");
						}else{
							if(!oe)printf("\r>> ");
							for(j=0;j<i;j++)
								if(!oe)printf("*");
							for(j=q;j<i;j++)
								if(!oe)printf("\b");
						}
					continue;
					}
					continue;
				}else if(ch==91){
					ch=getchar();
					if(ch==67){
						if(q<i){
							if(shown)
								{if(!oe)printf("%c",kkk[q]);}
							else {if(!oe)printf("*");}
							q++;
						continue;
						}						
					}else if(ch==68){
						if(q>0){
							if(!oe)printf("\b");
							q--;
						}
					continue;
					}else if(ch==51){
						ch=getchar();
						if(ch==126){
							if(q<i){
								for(j=q;j<i-1;j++){
									kkk[j]=kkk[j+1];
									if(shown)
										{if(!oe)printf("%c",kkk[j]);}
									else {if(!oe)printf("*");}
								}
								if(!oe)printf(" \b");
								for(j=q;j<i-1;j++)
									if(!oe)printf("\b");
								i--;
							}
						continue;
						}
					}else continue;
				}else continue;
			}


#endif

			for(j=i;j>q;j--)
				kkk[j]=kkk[j-1];
			kkk[q]=ch;
			for(j=q;j<=i;j++)
				if(shown)
					{if(!oe)printf("%c",kkk[j]);}
				else {if(!oe)printf("*");}
			for(j=q;j<i;j++)
				if(!oe)printf("\b");
			i++;q++;
		}
	}
	while(ch!='\n');
	#if not defined(_WIN32)
		hhhhidden=false;
		endwin();
	#endif

	for(j=0;j<i;j++){
		k[j%16]^=kkk[j];
		j++;
		if(j%16==0){
			md5(k,tmp);
			copy(tmp,k);
		}
	}
	md5(k,tmp);
	copy(tmp,key);
	aesinit(key);
	if(!oe)printf("\r>> ");
	for(j=0;j<i;j++)
		if(!oe)printf(" ");
	if(!oe)printf("\r   ");
	if(!oe)printf("Locked.\r\n");
	clean(k);
	clean(tmp);
	for(j=0;j<64;j++)
		kkk[i]=0;
	ch=0;
	i=0;j=0;q=0;
	shown=false;
	copy(key,rtn);
	clean(key);
#else
	clean(key);
	md5(key,rtn);
	aesinit(rtn);
	if(!oe)printf("Key defined as:");
	print(rtn);
#endif
}
