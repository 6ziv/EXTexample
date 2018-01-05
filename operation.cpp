#include"disk.h"
int LOCAL;
unsigned char self[2]={'.',0};
unsigned char parent[3]={'.','.',0};
char commandtable[256][16]={"ls","pwd","cd","cat","echo","mkdir","rmdir","rm","exit","useradd","userdel","groupadd","users","groups","su","whoami","chmod","passwd","chown","chgrp","usermod","gpasswd","groupdel","user","cp","mv","help","bash","clear"};
const int commands=29;
char switches[256][16];
char outputstream[65536*256];
void cclear()
{
#if defined(_WIN32)
 COORD cursorPos;
 HANDLE hOut  ;
 CONSOLE_SCREEN_BUFFER_INFO bInfo;
 hOut=GetStdHandle(STD_OUTPUT_HANDLE);
 GetConsoleScreenBufferInfo(hOut, &bInfo);     /*获得当前光标位置*/
 cursorPos.X=0;                  /*计算上一行输出的内容长度，并根据这个长度设置后续输出光标位置的X坐标*/
 cursorPos.Y=bInfo.dwCursorPosition.Y+100;        /*设置后续输出时光标的Y坐标*/
 SetConsoleCursorPosition( hOut, cursorPos ); /*将光标定位到新输出位置*/
 cursorPos.Y=bInfo.dwCursorPosition.Y;
 SetConsoleCursorPosition( hOut, cursorPos ); /*将光标定位到新输出位置*/
#else
 if(!oe)printf("\033[2J\n");
#endif
 return;
}
int fffind(const char *t){
	unsigned char str[65536];
	int i,j;
	i=0;
	while(t[i]){
		str[i]=t[i];
		i++;
	}
	str[i]=0;
	i=0;
	while((str[i]==' ')&&(str[i]))i++;
	if(str[i]==0)return INVALID_PARAMETERS;
	j=0;
	while(str[j]){
		str[j]=str[j+i];
		j++;
	}
	i=0;
	while(str[i]){
		if((str[i]=='/')&&(str[i-1]=='/')){
			j=i;
			while(str[j]){
				str[j]=str[j+1];
				j++;
			}
		}
		i++;
	}
	i--;
	while(str[i]==' '){
		str[i]=0;
		i--;
	}
	if(str[0]=='/')return findex(0,str);else return findex(LOCAL,str);
}
char w(const int i,const char x){
	if(i==0)return '-';else return x;
}
int ls(int id){
	if(!hasreadpri(id))return ACCESS_DENIED;
	dir_list tmp;
	int sz; 
	int pri,ow;
	int tmptime0,tmptime1,tmptime2;
	char nul[2]="*";
	if(!isdir(id)){
		sprintf(outputstream,"%s无效的路径.\n",outputstream);
		return INVALID_PARAMETERS;
	}
	readfolder(id,tmp,&sz);
	for(int i=0;i<sz;i++){
		getinodeattribute(tmp[i].inode_id,MASK_CREATETIME,&tmptime0);
		getinodeattribute(tmp[i].inode_id,MASK_LASTWRITETIME,&tmptime1);
		getinodeattribute(tmp[i].inode_id,MASK_LASTACCESSTIME,&tmptime2);
		pri=readpri(tmp[i].inode_id);
		ow=getowner(tmp[i].inode_id);
		char uname[256];
		char gname[256];
		if(finduser(ow/65536)>=0)
			if(USERS[finduser(ow/65536)].username[0]=='#')
				sprintf(uname,"*");
			else
				sprintf(uname,"%s",USERS[finduser(ow/65536)].username);
		else
			sprintf(uname,"*");
		if(findgroup(ow/65536)>=0)
			if(GROUPS[findgroup(ow/65536)].groupname[0]=='#')
				sprintf(gname,"*");
			else
				sprintf(gname,"%s",GROUPS[findgroup(ow/65536)].groupname);
		else
			sprintf(gname,"*");
char tmp0[256];char tmp1[256];char tmp2[256];
int timecheck;
time_t timetemp;
timetemp=tmptime0;
sprintf(tmp0,"%s",ctime(&timetemp));
timecheck=0;
while(tmp0[timecheck]){if(tmp0[timecheck]=='\n')tmp0[timecheck]=0;timecheck++;}
timetemp=tmptime1;
sprintf(tmp1,"%s",ctime(&timetemp));
timecheck=0;
while(tmp1[timecheck]){if(tmp1[timecheck]=='\n')tmp1[timecheck]=0;timecheck++;}
timetemp=tmptime2;
sprintf(tmp2,"%s",ctime(&timetemp));
timecheck=0;
while(tmp2[timecheck]){if(tmp2[timecheck]=='\n')tmp2[timecheck]=0;timecheck++;}

		sprintf(outputstream,"%s%c%c%c%c%c%c%c%c%c%c\t%s\t%s\t%d\t%s\t%s\t%s\t%s\n",outputstream,isdir(tmp[i].inode_id)?'d':'-',w(pri&256,'r'),w(pri&128,'w'),w(pri&64,'x'),w(pri&32,'r'),w(pri&16,'w'),w(pri&8,'x'),w(pri&4,'r'),w(pri&2,'w'),w(pri&1,'x'),uname,gname,tmp[i].inode_id,tmp[i].name,tmp0,tmp1,tmp2);

	}
	sprintf(outputstream,"%s\n",outputstream);
}

int showname(int id){
	int temp;
	int items;
	if(id==0)return ERROR_SUCCESS;
	temp=find(id,parent);
	if(temp<0)return temp;
	dir_list tmp;
	readfolder(temp,tmp,&items);
	for(int j=0;j<items;j++)
		if(tmp[j].inode_id==id){
			sprintf(outputstream,"%s%s",outputstream,tmp[j].name);
			return ERROR_SUCCESS;
		}
	return FILE_NOT_FOUND;
}
int showpath(int id){
	int i;
	int a[4096];
	i=0;
	while((i<4096)&&(id!=0)){
		a[i]=id;
		i++;
		id=find(id,parent);
		if(id<0)return id;	
	}
	sprintf(outputstream,"%s/",outputstream);
	while(i>0){
		i--;
		showname(a[i]);
		sprintf(outputstream,"%s/",outputstream);
	}
	sprintf(outputstream,"%s\n",outputstream);
	return ERROR_SUCCESS;
}
int cd(int i){
	if(!hasreadpri(i)){sprintf(outputstream,"%s没有权限。\n",outputstream);return ACCESS_DENIED;}
	if(!hasexepri(i)){sprintf(outputstream,"%s没有权限。\n",outputstream);return ACCESS_DENIED;}
	if(!isdir(i)){sprintf(outputstream,"%s非法路径！\n",outputstream);return INVALID_PARAMETERS;}
	LOCAL=i;
	showpath(i);
	return ERROR_SUCCESS;
}
int cd(const char *str){
	int temp;
	temp=fffind(str);
	if(temp<0){sprintf(outputstream,"%s路径不存在\n",outputstream);return temp;}
	return cd(temp);
}
int ls(const char *str){
	if(str[0]==0)return ls(LOCAL);
	int temp;
	temp=fffind(str);
	if(temp<0){sprintf(outputstream,"%s找不到文件\n",outputstream);return temp;}
	return ls(temp);
}
int mmkdir(const char *str){
	char tmp[65536];
	int i,j,k;
	int temp;
	i=0;
	while(str[i]){
		if((str[i]<='\t')){sprintf(outputstream,"%s非法文件名\n",outputstream);return INVALID_PARAMETERS;}
		i++;
	}
	temp=LOCAL;
	i=0;j=0;
	if(str[0]=='/'){temp=0;while(str[i]=='/')i++;}
	while(str[i]){
		while((str[i]!='/')&&(str[i]!=0)){
			tmp[j]=str[i];
			i++;j++;
		}
		while(str[i]=='/')i++;
		tmp[j]=0;
		if(fileexist(temp,tmp)){
			j=find(temp,tmp);
			if(j<0)return j;
			temp=j;
		}else{
			j=newf(temp,tmp);
			if(j==ACCESS_DENIED) {sprintf(outputstream,"%s没有权限。\n",outputstream);return ACCESS_DENIED;}
			if(j<0)return j;
			k=setupfolder(j,temp);

			if(k!=0)return k;
			temp=j;
		}
		j=0;
	}
	return 0;	
}
int pwd(){
	return showpath(LOCAL);
}
int isdir(char *str){
	int tmp;
	tmp=fffind(str);
	if(tmp<0)return tmp;
	if(isdir(tmp))return ISDIR_DIRECTORY;else return ISDIR_FILE;
}
int removesinglef(int id,int pare){
	int temp;
	int items;
	if(id==0)return INVALID_PARAMETERS; 
	//if(!fileexist(pare))
	if(isdir(id)){
		dir_list tmp;
		readfolder(id,tmp,&temp);
		if(temp>2)
			return FOLDER_NOT_EMPTY; 
	}
	temp=delf(pare,id);
	return temp;
}
int rm(int id,int pare){
	if(isdir(id)){sprintf(outputstream,"%s操作对目录无效。\n",outputstream);return INVALID_PARAMETERS;}
	removesinglef(id,pare);
}
int rm(const char *str){
	int i,p;
	char tmp[65536];
	char c;
	i=0;
	while(str[i]){tmp[i]=str[i];i++;}
	i--;
	if(str[i]=='/'){sprintf(outputstream,"%s操作对目录无效。\n",outputstream);return INVALID_PARAMETERS;}
	while((str[i]!='/')&&(i>=0))i--;
	if(i==-1)p=LOCAL;else{
		c=tmp[i+1];
		tmp[i+1]=0;
		p=fffind(tmp);
		tmp[i+1]=c;
		if(p<0){sprintf(outputstream,"%s路径无效。\n",outputstream);return INVALID_PARAMETERS;}
		if(!isdir(p)){sprintf(outputstream,"%s路径无效。\n",outputstream);return INVALID_PARAMETERS;}
	}
	int temp;
	temp=fffind(str);
	if(temp<0)return temp;
	if(!haswritepri(temp)){sprintf(outputstream,"%s没有权限。\n",outputstream);return ACCESS_DENIED;} 
	return rm(temp,p);
}
int kerrmdir(int id,int pare){
	int temp;
	int items;
	dir_list tmp;
	if(id==LOCAL){sprintf(outputstream,"%s无法删除当前目录。\n",outputstream);return FILE_IN_USE;}
	readfolder(id,tmp,&temp);
	if(!haswritepri(pare)){sprintf(outputstream,"%s没有权限。\n",outputstream);return ACCESS_DENIED;}
	if(isdir(id))
		for(int i=2;i<temp;i++)
			kerrmdir(tmp[i].inode_id,id);
	return removesinglef(id,pare);
}
int rmdirid(int id){
	int temp;
	temp=find(id,parent);
	if(temp<0){sprintf(outputstream,"%s找不到文件。\n",outputstream);return temp;}
	if(id==0){sprintf(outputstream,"%s禁止删除根目录！\n",outputstream);return INVALID_PARAMETERS;} 
	if(!isdir(id)){sprintf(outputstream,"%s只能删除目录。\n",outputstream);return NOT_A_DIRECTORY;} 
	kerrmdir(id,temp);
}
int rmdirname(const char *str){
	int temp;
	temp=fffind(str);
	if(temp<0)return temp;
	return rmdirid(temp);
}
int echo(const char *content,const int id,const int filesize,bool attach){
	int i;
	f_data filetmp;
	if(filesize>5*4096)return FILE_TOO_LARGE;
	if(attach&&(getsize(id)+filesize>5*4096))return FILE_TOO_LARGE;
	if(filesize==-1){if(!oe)printf("%s",content);return ERROR_SUCCESS;}
	for(i=0;i<filesize;i++)
		filetmp[i]=content[i];
	if(id<0)return INVALID_PARAMETERS;
	if(isdir(id))return NOT_A_FILE;
	if(!hasreadpri(id)){if(!oe)printf("没有权限。\n");return ACCESS_DENIED;}
	if(!haswritepri(id)){if(!oe)printf("没有权限。\n");return ACCESS_DENIED;}
	if(!attach)return writefile(id,filetmp,filesize);else return attachtofile(id,filetmp,filesize);
}
int echo(const char *content,const char *filename,const int filesize,bool attach){
	int i,j,k;
	i=0;
	while(filename[0]==' ')filename++;
	while(filename[i]){
	if((filename[i]>='a')&&(filename[i]<='z')){i++;continue;} 
	if((filename[i]>='A')&&(filename[i]<='Z')){i++;continue;} 
	if((filename[i]>='0')&&(filename[i]<='9')){i++;continue;} 
	if(filename[i]=='.'){i++;continue;}
	if(filename[i]=='/'){i++;continue;}
	if(!oe)printf("无效的文件名\n");
	return INVALID_PARAMETERS;
	}
	char fname[65536];
	char c;
	if(filesize>5*4096)return FILE_TOO_LARGE;
	if(filesize==-1){if(!oe)printf("%s",content);return ERROR_SUCCESS;}
	i=fffind(filename);
	if((i<0)&&(i!=FILE_NOT_FOUND))return i;
	if(i==FILE_NOT_FOUND){
		j=0;
		while(filename[j]){fname[j]=filename[j];j++;}
		j--;
		if(fname[j]=='/'){if(!oe)printf("不能写目录！\n");return INVALID_PARAMETERS;}
		while((fname[j]!='/')&&(j>=0))j--;
		if(j!=-1){
			c=fname[j+1];
			fname[j+1]=0;
			k=fffind(fname);
			if(k<0){
				mmkdir(fname);
				k=fffind(fname);
				if(k<0)return k;
			} 
			fname[j+1]=c;
		}else k=LOCAL;
		k=newf(k,fname+j+1);
		if(k==ACCESS_DENIED) {if(!oe)printf("没有权限。\n");return ACCESS_DENIED;}
		if(k<0)return k;
		setupfile(k);
		i=k;
	}
	if(isdir(i)){if(!oe)printf("不能写目录！\n");return NOT_A_FILE;}
	if(!haswritepri(i)){if(!oe)printf("没有权限。\n");return ACCESS_DENIED;}
	if(attach&&(getsize(i)+filesize>5*4096))return FILE_TOO_LARGE;
	return echo(content,i,filesize,attach);
}
int type(const int id){
	int i,j;
	if(isdir(id)){sprintf(outputstream,"%s非法的目标。\n",outputstream);return INVALID_PARAMETERS;}
	f_data tmp;
	j=readfile(id,tmp,&i);
	if(j!=ERROR_SUCCESS)return j;
	for(j=0;j<i;j++)sprintf(outputstream,"%s%c",outputstream,tmp[j]);
	sprintf(outputstream,"%s\n",outputstream);
	return ERROR_SUCCESS;
}
int cat(const char *str){
	int i,j;
	i=fffind(str);
	if(i<0){sprintf(outputstream,"%s无法访问文件：可能是文件不存在。\n",outputstream);return i;}
	if(!hasreadpri(i)){sprintf(outputstream,"%s没有权限。\n",outputstream);return ACCESS_DENIED;}
	return type(i);
}
int findcommand(const char *cmdline){
	int i,j;
	while(cmdline[0]==' ')cmdline++;
	if(cmdline[0]==0)return -2;
	for(i=0;i<commands;i++){
		j=0;
		while(commandtable[i][j]){
			if(commandtable[i][j]!=cmdline[j])break;
			j++;
		}
		if((!commandtable[i][j])&&((cmdline[j]==' ')||(cmdline[j]==0)))return i;
	}
	return -1;
}
void echoex(char *str){
	int i;
	i=0;
	while(str[i]){
		sprintf(outputstream,"%s%c",outputstream,str[i]);
		i++;
	}
		sprintf(outputstream,"%s\n",outputstream);
	
	return;
}
void help(const char *str){
	int k;
	k=findcommand(str);
	if(k<0)sprintf(outputstream,"%s有关某个命令的详细信息，请键入help命令名\n",outputstream);
	if((k==27)||(k<0))sprintf(outputstream,"%sbash\t\t执行一段批处理\n",outputstream);
	if((k==3)||(k<0))sprintf(outputstream,"%scat\t\t显示文本文件的内容\n",outputstream);
	if((k==2)||(k<0))sprintf(outputstream,"%scd\t\t切换目录\n",outputstream);
	if((k==19)||(k<0))sprintf(outputstream,"%schgrp\t\t更改文件所属用户组\n",outputstream);
	if((k==16)||(k<0))sprintf(outputstream,"%schmod\t\t更改文件权限\n",outputstream);
	if((k==18)||(k<0))sprintf(outputstream,"%schown\t\t更改文件所有者\n",outputstream);
	if((k==28)||(k<0))sprintf(outputstream,"%sclear\t\t清空屏幕\n",outputstream);
	if((k==24)||(k<0))sprintf(outputstream,"%scp\t\t复制文件或目录\n",outputstream);
	if((k==4)||(k<0))sprintf(outputstream,"%secho\t\t回显一段消息\n",outputstream);
	if((k==8)||(k<0))sprintf(outputstream,"%sexit\t\t退出程序\n",outputstream);
	if((k==21)||(k<0))sprintf(outputstream,"%sgpasswd\t\t其实只有gpasswd从用户组里移除用户的功能啦\n",outputstream);
	if((k==11)||(k<0))sprintf(outputstream,"%sgroupadd\t将用户添加到用户组中\n",outputstream);
	if((k==22)||(k<0))sprintf(outputstream,"%sgroupdel\t删除一个空用户组\n",outputstream);
	if((k==13)||(k<0))sprintf(outputstream,"%sgroups\t\t显示所有用户组\n",outputstream);
	if((k==26)||(k<0))sprintf(outputstream,"%shelp\t\t显示帮助列表\n",outputstream);
	if((k==0)||(k<0))sprintf(outputstream,"%sls\t\t列出目录中的文件\n",outputstream);
	if((k==5)||(k<0))sprintf(outputstream,"%smkdir\t\t新建文件夹\n",outputstream);
	if((k==25)||(k<0))sprintf(outputstream,"%smv\t\t移动文件或文件夹\n",outputstream);
	if((k==1)||(k<0))sprintf(outputstream,"%spwd\t\t显示当前所在目录\n",outputstream);
	if((k==17)||(k<0))sprintf(outputstream,"%spasswd\t\t设置用户的密码\n",outputstream);
	if((k==7)||(k<0))sprintf(outputstream,"%srm\t\t删除一个文件\n",outputstream);
	if((k==6)||(k<0))sprintf(outputstream,"%srmdir\t\t删除一个目录和其中的内容\n",outputstream);
	if((k==14)||(k<0))sprintf(outputstream,"%ssu\t\t在当前目录下切换用户\n",outputstream);
	if((k==23)||(k<0))sprintf(outputstream,"%suser\t\t以用户身份登陆\n",outputstream);
	if((k==9)||(k<0))sprintf(outputstream,"%suseradd\t\t新建一个用户\n",outputstream);
	if((k==10)||(k<0))sprintf(outputstream,"%suserdel\t\t移除一个用户\n",outputstream);
	if((k==20)||(k<0))sprintf(outputstream,"%susermod\t\t将用户添加到一个用户组中\n",outputstream);
	if((k==12)||(k<0))sprintf(outputstream,"%susers\t\t列出所有用户\n",outputstream);
	if((k==15)||(k<0))sprintf(outputstream,"%swhoami\t\t显示当前用户\n",outputstream);
	if(k==0){
		sprintf(outputstream,"%sls 目录\n如：ls /etc\n绝对路径应当以/开头\n显示当前目录条目时可以不带参数\n",outputstream);
	}
	if(k==1){
		sprintf(outputstream,"%s不带参数，显示当前工作目录\n",outputstream);
	}
	if(k==2){
		sprintf(outputstream,"%scd 目录\n如：cd /usr/root/\n绝对路径应当以/开头\n",outputstream);
	}
	if(k==3){
		sprintf(outputstream,"%scat 文件\n如：cat /etc/passwd\n绝对路径应当以/开头。\n",outputstream);
	}
	if(k==4){
		sprintf(outputstream,"%secho 要显示的文本\n如：echo HelloWorld\n需要输出到文件时，用>或>>连接输出文件名，>或>>前应当有空格\n>代表覆盖文件原内容，而>>表示追加到文件后。\n需要输出特殊符号时请在符号前加\\，\\后的字符不会被改变\n例如 echo \\ \\>\\\\的结果是' >\\'\n新建文件时，文件名仅支持数字、英文字母和.\n",outputstream);
	}
	if(k==5){
		sprintf(outputstream,"%smkdir 目录名\n如：mkdir myfiles/homework\n绝对路径应当以/开头。仅支持数字、英文字母和.\n",outputstream);
	}
	if(k==6){
		sprintf(outputstream,"%srmdir 目录名\n如：rmdir myfiles/\n绝对路径应当以/开头。\n",outputstream);
	}
	if(k==7){
		sprintf(outputstream,"%srm 文件名，删除一个文件。\n",outputstream);
	}
	if(k==8){
		sprintf(outputstream,"%s不带参数，退出此程序。功能类似ctrl+c。\n",outputstream);
	}
	if(k==9){
		sprintf(outputstream,"%suseradd 用户名\n用户名仅支持英文字母和数字。\n需要当前用户在root用户组中。",outputstream);
	}
	if(k==10){
		sprintf(outputstream,"%suserdel 用户名\n需要当前用户在root用户组中\n",outputstream);
	}
	if(k==11){
		sprintf(outputstream,"%sgroupadd 用户组名\n需要当前用户在root用户组中\n",outputstream);
	}
	if(k==12){
		sprintf(outputstream,"%s不带参数，列出所有用户\n",outputstream);
	}
	if(k==13){
		sprintf(outputstream,"%s不带参数，列出所有用户组\n",outputstream);
	}
	if(k==14){
		sprintf(outputstream,"%ssu 用户名\n不改变工作目录\n",outputstream);
	}
	if(k==15){
		sprintf(outputstream,"%s不带参数，显示当前用户名\n",outputstream);
	}
	if(k==16){
		sprintf(outputstream,"%schmod 文件 三位七进制数\n将指定文件的权限修改为由这三位七进制数表示的权限。\n其中最高位表示文件所有者享有的权限，次高位表示文件所在组的用户享有的权限，末位表示普通用户享有的权限\n每个七进制数是读取：4、写入:2、执行：1的组合\n",outputstream);
	}
	if(k==17){
		sprintf(outputstream,"%spasswd 用户名\n修改指定用户的密码\npasswd -d 用户名 删除指定用户的密码。\n需要当前用户是被修改的用户或在root用户组中。",outputstream);
	}
	if(k==18){
		sprintf(outputstream,"%schown 用户名 文件名\n更改文件所有者\n需要文件所有权或当前用户在root用户组中\n",outputstream);
	}
	if(k==19){
		sprintf(outputstream,"%schown 用户名 文件名\n更改文件所有者\n需要文件所有权或当前用户在root用户组中\n",outputstream);
	}
	if(k==20){
		sprintf(outputstream,"%susermod -g 用户组名 用户名\n更改用户所属主用户组\n需要当前用户在root用户组中\n",outputstream);
		sprintf(outputstream,"%susermod -G 用户组名 用户名\n将用户添加到用户组\n需要当前用户在root用户组中\n",outputstream);
	}
	if(k==21){
		sprintf(outputstream,"%sgpasswd -d 用户组名 用户名\n从用户组中移除用户\n需要当前用户在root用户组中\n不能从该用户的主用户组中移除该用户。\n",outputstream);
	}
	if(k==22){
		sprintf(outputstream,"%sgroupdel 用户组名\n删除一个空的用户组\n需要当前用户在root用户组中\n",outputstream);
	}
	if(k==23){
		sprintf(outputstream,"%suser 用户名\n作为某个用户登录\n若登陆成功会切换至此用户工作目录\n若工作目录不存在则切换至根目录。\n",outputstream);
		
	}
	if(k==24){
		sprintf(outputstream,"%scp 源 目标文件夹\n目标文件夹中不能有文件或文件夹与源重名\n若对源中部分文件无权访问，则这一部分文件会复制失败\n",outputstream);
	}
	if(k==25){
		sprintf(outputstream,"%smv 源 目标文件夹\n各种特性和cp一样\n因为就是cp和rm的组合\n",outputstream);	
	}
	if(k==26){
		sprintf(outputstream,"%shelp 命令名\n显示命令的帮助\n也可以不带参数，显示命令列表。\n",outputstream);			
	}
	if(k==27){
		sprintf(outputstream,"%sbash 批处理文件\n逐行读入并执行\n",outputstream);			
	}
	if(k==28){
		sprintf(outputstream,"%s不带参数，翻页。（同学不让用cls说清空了不够linux..）\n",outputstream);			
	}
}
void exit(){
	exit(0);
}
void bash(const char *file){
	int i,j,fs;
	char temp[65536];
	f_data tmp;
	i=fffind(file);
	if(i<0){sprintf(outputstream,"%s无法打开文件\n",outputstream);return;}
	if(!hasexepri(i)){sprintf(outputstream,"%s权限不足！\n",outputstream);return;}
	readfile(i,tmp,&fs);
	i=0;j=0;
	while(j<fs){
		if((tmp[j]==0)||(tmp[j]=='\n')){
			temp[i]=0;
			if(i<65536)docommand(temp);
			i=0;
		}else{
			if(i<65536)temp[i]=tmp[j];
			i++;
		}
		j++;
	}
	return;
}
void docommand(const char *cmdline){
	int i,j,k,l,tmp,p,q;
	char temp[65536];bool a,b;
	i=0;
	while(cmdline[0]==' ')cmdline++;
	while(cmdline[i]){temp[i]=cmdline[i];i++;
	}
	temp[i]=0;
	while(temp[i]==' ')i++;
	i=0;
	if(temp[i]==0)return;
	j=i;
	while((temp[i]!=0)&&(temp[i]!=' '))i++;
	tmp=temp[i];
	temp[i]=0;
	k=findcommand(temp+j);
	temp[i]=tmp;
	if(k==-2)return;
	if(k==-1){if(!oe)printf("未能识别命令!\n");return;}
	l=j+strlen(commandtable[k]);
	while(temp[l]==' ')l++;
	while((temp[i])&&(!((temp[i]==' ')&&(temp[i+1]=='>'))))i++;
	if(temp[i])
		if(temp[i+2]=='>'){
			a=true;b=true;
			j=i+3;
		}
		else {a=false;j=i+2;b=true;}
	else b=false;
	temp[i]=0;
	temp[i+1]=0;
	outputstream[0]=0;
	p=l;
	while(temp[p]){
		if(temp[p]=='\\'){
			q=p;
			while(temp[q]){
				temp[q]=temp[q+1];
				q++;
			}
		}
		p++;
	}
if(ME!=-1){
	if(k==0)ls(temp+l);
	if(k==1)pwd();
	if(k==2)cd(temp+l);
	if(k==3)cat(temp+l);
	if(k==4)echoex(temp+l);
	if(k==5)mmkdir(temp+l);
	if(k==6)rmdirname(temp+l);
	if(k==7)rm(temp+l);
	if(k==8)exit();
	if(k==9)newuser(temp+l);
	if(k==10)deluser(temp+l);
	if(k==11)newgroup(temp+l);
	if(k==12)showusers();	
	if(k==13)showgroups();
	if(k==14)su(temp+l);
	if(k==15)whoami();
	if(k==16)chmod(temp+l);
	if(k==17)passwd(temp+l);
	if(k==18)chown(temp+l);
	if(k==19)chgrp(temp+l);
	if(k==20)usermod(temp+l);
	if(k==21)gpasswd(temp+l);
	if(k==22)groupdel(temp+l);
	if(k==23)fuser(temp+l);
	if(k==24)cp(temp+l);
	if(k==25)mv(temp+l);
	if(k==26)help(temp+l);
	if(k==27)bash(temp+l);
	if(k==28){b=false;cclear();}
	if(b)
		echo(outputstream,temp+j,strlen(outputstream),a);
	else {if(!oe)printf("%s",outputstream);}
}else{
	b=false;
	if(k==26){help(temp+l);if(!oe)printf("%s",outputstream);}else if(k==8)exit();else if(k==28){b=false;cclear();}else if(k==23){fuser(temp+l);if(ME==-1){if(!oe)printf("登陆失败\n");}else {if(!oe)printf("登陆成功，作为%s\n",USERS[finduser(ME)].username);}}else {if(!oe)printf("请先登录。\n");}	
}
	outputstream[0]=0;
}

