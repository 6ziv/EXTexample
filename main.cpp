#include "disk.h"
#include<iostream>
#if not defined(_WIN32)
#include<signal.h>
iconv_t ico;
#endif
char tmp[65536];
int i;
int main(){
try{
	initenv();
	if(loaddisk())if(!oe)printf("Ó²ÅÌ¼ÓÔØ³É¹¦\r\n");else exit(0);
	while(1){
		tmp[0]=0;
                       showpath(LOCAL);
                       i=0;
                       while(outputstream[i]!='\n')i++;
                       outputstream[i]=0;
                       if(!oe)printf("%s",outputstream);
                       outputstream[0]=0;
                       if(!oe)printf(">>");
		scanf("%[^\n]",tmp);
		scanf("%*c");
		if(!oe)
			docommand(tmp);
		else break;
	};
}
catch(...){
	exit(0);
}
}
