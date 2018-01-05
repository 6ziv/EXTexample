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
 GetConsoleScreenBufferInfo(hOut, &bInfo);     /*��õ�ǰ���λ��*/
 cursorPos.X=0;                  /*������һ����������ݳ��ȣ�����������������ú���������λ�õ�X����*/
 cursorPos.Y=bInfo.dwCursorPosition.Y+100;        /*���ú������ʱ����Y����*/
 SetConsoleCursorPosition( hOut, cursorPos ); /*����궨λ�������λ��*/
 cursorPos.Y=bInfo.dwCursorPosition.Y;
 SetConsoleCursorPosition( hOut, cursorPos ); /*����궨λ�������λ��*/
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
		sprintf(outputstream,"%s��Ч��·��.\n",outputstream);
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
	if(!hasreadpri(i)){sprintf(outputstream,"%sû��Ȩ�ޡ�\n",outputstream);return ACCESS_DENIED;}
	if(!hasexepri(i)){sprintf(outputstream,"%sû��Ȩ�ޡ�\n",outputstream);return ACCESS_DENIED;}
	if(!isdir(i)){sprintf(outputstream,"%s�Ƿ�·����\n",outputstream);return INVALID_PARAMETERS;}
	LOCAL=i;
	showpath(i);
	return ERROR_SUCCESS;
}
int cd(const char *str){
	int temp;
	temp=fffind(str);
	if(temp<0){sprintf(outputstream,"%s·��������\n",outputstream);return temp;}
	return cd(temp);
}
int ls(const char *str){
	if(str[0]==0)return ls(LOCAL);
	int temp;
	temp=fffind(str);
	if(temp<0){sprintf(outputstream,"%s�Ҳ����ļ�\n",outputstream);return temp;}
	return ls(temp);
}
int mmkdir(const char *str){
	char tmp[65536];
	int i,j,k;
	int temp;
	i=0;
	while(str[i]){
		if((str[i]<='\t')){sprintf(outputstream,"%s�Ƿ��ļ���\n",outputstream);return INVALID_PARAMETERS;}
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
			if(j==ACCESS_DENIED) {sprintf(outputstream,"%sû��Ȩ�ޡ�\n",outputstream);return ACCESS_DENIED;}
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
	if(isdir(id)){sprintf(outputstream,"%s������Ŀ¼��Ч��\n",outputstream);return INVALID_PARAMETERS;}
	removesinglef(id,pare);
}
int rm(const char *str){
	int i,p;
	char tmp[65536];
	char c;
	i=0;
	while(str[i]){tmp[i]=str[i];i++;}
	i--;
	if(str[i]=='/'){sprintf(outputstream,"%s������Ŀ¼��Ч��\n",outputstream);return INVALID_PARAMETERS;}
	while((str[i]!='/')&&(i>=0))i--;
	if(i==-1)p=LOCAL;else{
		c=tmp[i+1];
		tmp[i+1]=0;
		p=fffind(tmp);
		tmp[i+1]=c;
		if(p<0){sprintf(outputstream,"%s·����Ч��\n",outputstream);return INVALID_PARAMETERS;}
		if(!isdir(p)){sprintf(outputstream,"%s·����Ч��\n",outputstream);return INVALID_PARAMETERS;}
	}
	int temp;
	temp=fffind(str);
	if(temp<0)return temp;
	if(!haswritepri(temp)){sprintf(outputstream,"%sû��Ȩ�ޡ�\n",outputstream);return ACCESS_DENIED;} 
	return rm(temp,p);
}
int kerrmdir(int id,int pare){
	int temp;
	int items;
	dir_list tmp;
	if(id==LOCAL){sprintf(outputstream,"%s�޷�ɾ����ǰĿ¼��\n",outputstream);return FILE_IN_USE;}
	readfolder(id,tmp,&temp);
	if(!haswritepri(pare)){sprintf(outputstream,"%sû��Ȩ�ޡ�\n",outputstream);return ACCESS_DENIED;}
	if(isdir(id))
		for(int i=2;i<temp;i++)
			kerrmdir(tmp[i].inode_id,id);
	return removesinglef(id,pare);
}
int rmdirid(int id){
	int temp;
	temp=find(id,parent);
	if(temp<0){sprintf(outputstream,"%s�Ҳ����ļ���\n",outputstream);return temp;}
	if(id==0){sprintf(outputstream,"%s��ֹɾ����Ŀ¼��\n",outputstream);return INVALID_PARAMETERS;} 
	if(!isdir(id)){sprintf(outputstream,"%sֻ��ɾ��Ŀ¼��\n",outputstream);return NOT_A_DIRECTORY;} 
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
	if(!hasreadpri(id)){if(!oe)printf("û��Ȩ�ޡ�\n");return ACCESS_DENIED;}
	if(!haswritepri(id)){if(!oe)printf("û��Ȩ�ޡ�\n");return ACCESS_DENIED;}
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
	if(!oe)printf("��Ч���ļ���\n");
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
		if(fname[j]=='/'){if(!oe)printf("����дĿ¼��\n");return INVALID_PARAMETERS;}
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
		if(k==ACCESS_DENIED) {if(!oe)printf("û��Ȩ�ޡ�\n");return ACCESS_DENIED;}
		if(k<0)return k;
		setupfile(k);
		i=k;
	}
	if(isdir(i)){if(!oe)printf("����дĿ¼��\n");return NOT_A_FILE;}
	if(!haswritepri(i)){if(!oe)printf("û��Ȩ�ޡ�\n");return ACCESS_DENIED;}
	if(attach&&(getsize(i)+filesize>5*4096))return FILE_TOO_LARGE;
	return echo(content,i,filesize,attach);
}
int type(const int id){
	int i,j;
	if(isdir(id)){sprintf(outputstream,"%s�Ƿ���Ŀ�ꡣ\n",outputstream);return INVALID_PARAMETERS;}
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
	if(i<0){sprintf(outputstream,"%s�޷������ļ����������ļ������ڡ�\n",outputstream);return i;}
	if(!hasreadpri(i)){sprintf(outputstream,"%sû��Ȩ�ޡ�\n",outputstream);return ACCESS_DENIED;}
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
	if(k<0)sprintf(outputstream,"%s�й�ĳ���������ϸ��Ϣ�������help������\n",outputstream);
	if((k==27)||(k<0))sprintf(outputstream,"%sbash\t\tִ��һ��������\n",outputstream);
	if((k==3)||(k<0))sprintf(outputstream,"%scat\t\t��ʾ�ı��ļ�������\n",outputstream);
	if((k==2)||(k<0))sprintf(outputstream,"%scd\t\t�л�Ŀ¼\n",outputstream);
	if((k==19)||(k<0))sprintf(outputstream,"%schgrp\t\t�����ļ������û���\n",outputstream);
	if((k==16)||(k<0))sprintf(outputstream,"%schmod\t\t�����ļ�Ȩ��\n",outputstream);
	if((k==18)||(k<0))sprintf(outputstream,"%schown\t\t�����ļ�������\n",outputstream);
	if((k==28)||(k<0))sprintf(outputstream,"%sclear\t\t�����Ļ\n",outputstream);
	if((k==24)||(k<0))sprintf(outputstream,"%scp\t\t�����ļ���Ŀ¼\n",outputstream);
	if((k==4)||(k<0))sprintf(outputstream,"%secho\t\t����һ����Ϣ\n",outputstream);
	if((k==8)||(k<0))sprintf(outputstream,"%sexit\t\t�˳�����\n",outputstream);
	if((k==21)||(k<0))sprintf(outputstream,"%sgpasswd\t\t��ʵֻ��gpasswd���û������Ƴ��û��Ĺ�����\n",outputstream);
	if((k==11)||(k<0))sprintf(outputstream,"%sgroupadd\t���û���ӵ��û�����\n",outputstream);
	if((k==22)||(k<0))sprintf(outputstream,"%sgroupdel\tɾ��һ�����û���\n",outputstream);
	if((k==13)||(k<0))sprintf(outputstream,"%sgroups\t\t��ʾ�����û���\n",outputstream);
	if((k==26)||(k<0))sprintf(outputstream,"%shelp\t\t��ʾ�����б�\n",outputstream);
	if((k==0)||(k<0))sprintf(outputstream,"%sls\t\t�г�Ŀ¼�е��ļ�\n",outputstream);
	if((k==5)||(k<0))sprintf(outputstream,"%smkdir\t\t�½��ļ���\n",outputstream);
	if((k==25)||(k<0))sprintf(outputstream,"%smv\t\t�ƶ��ļ����ļ���\n",outputstream);
	if((k==1)||(k<0))sprintf(outputstream,"%spwd\t\t��ʾ��ǰ����Ŀ¼\n",outputstream);
	if((k==17)||(k<0))sprintf(outputstream,"%spasswd\t\t�����û�������\n",outputstream);
	if((k==7)||(k<0))sprintf(outputstream,"%srm\t\tɾ��һ���ļ�\n",outputstream);
	if((k==6)||(k<0))sprintf(outputstream,"%srmdir\t\tɾ��һ��Ŀ¼�����е�����\n",outputstream);
	if((k==14)||(k<0))sprintf(outputstream,"%ssu\t\t�ڵ�ǰĿ¼���л��û�\n",outputstream);
	if((k==23)||(k<0))sprintf(outputstream,"%suser\t\t���û���ݵ�½\n",outputstream);
	if((k==9)||(k<0))sprintf(outputstream,"%suseradd\t\t�½�һ���û�\n",outputstream);
	if((k==10)||(k<0))sprintf(outputstream,"%suserdel\t\t�Ƴ�һ���û�\n",outputstream);
	if((k==20)||(k<0))sprintf(outputstream,"%susermod\t\t���û���ӵ�һ���û�����\n",outputstream);
	if((k==12)||(k<0))sprintf(outputstream,"%susers\t\t�г������û�\n",outputstream);
	if((k==15)||(k<0))sprintf(outputstream,"%swhoami\t\t��ʾ��ǰ�û�\n",outputstream);
	if(k==0){
		sprintf(outputstream,"%sls Ŀ¼\n�磺ls /etc\n����·��Ӧ����/��ͷ\n��ʾ��ǰĿ¼��Ŀʱ���Բ�������\n",outputstream);
	}
	if(k==1){
		sprintf(outputstream,"%s������������ʾ��ǰ����Ŀ¼\n",outputstream);
	}
	if(k==2){
		sprintf(outputstream,"%scd Ŀ¼\n�磺cd /usr/root/\n����·��Ӧ����/��ͷ\n",outputstream);
	}
	if(k==3){
		sprintf(outputstream,"%scat �ļ�\n�磺cat /etc/passwd\n����·��Ӧ����/��ͷ��\n",outputstream);
	}
	if(k==4){
		sprintf(outputstream,"%secho Ҫ��ʾ���ı�\n�磺echo HelloWorld\n��Ҫ������ļ�ʱ����>��>>��������ļ�����>��>>ǰӦ���пո�\n>�������ļ�ԭ���ݣ���>>��ʾ׷�ӵ��ļ���\n��Ҫ����������ʱ���ڷ���ǰ��\\��\\����ַ����ᱻ�ı�\n���� echo \\ \\>\\\\�Ľ����' >\\'\n�½��ļ�ʱ���ļ�����֧�����֡�Ӣ����ĸ��.\n",outputstream);
	}
	if(k==5){
		sprintf(outputstream,"%smkdir Ŀ¼��\n�磺mkdir myfiles/homework\n����·��Ӧ����/��ͷ����֧�����֡�Ӣ����ĸ��.\n",outputstream);
	}
	if(k==6){
		sprintf(outputstream,"%srmdir Ŀ¼��\n�磺rmdir myfiles/\n����·��Ӧ����/��ͷ��\n",outputstream);
	}
	if(k==7){
		sprintf(outputstream,"%srm �ļ�����ɾ��һ���ļ���\n",outputstream);
	}
	if(k==8){
		sprintf(outputstream,"%s�����������˳��˳��򡣹�������ctrl+c��\n",outputstream);
	}
	if(k==9){
		sprintf(outputstream,"%suseradd �û���\n�û�����֧��Ӣ����ĸ�����֡�\n��Ҫ��ǰ�û���root�û����С�",outputstream);
	}
	if(k==10){
		sprintf(outputstream,"%suserdel �û���\n��Ҫ��ǰ�û���root�û�����\n",outputstream);
	}
	if(k==11){
		sprintf(outputstream,"%sgroupadd �û�����\n��Ҫ��ǰ�û���root�û�����\n",outputstream);
	}
	if(k==12){
		sprintf(outputstream,"%s�����������г������û�\n",outputstream);
	}
	if(k==13){
		sprintf(outputstream,"%s�����������г������û���\n",outputstream);
	}
	if(k==14){
		sprintf(outputstream,"%ssu �û���\n���ı乤��Ŀ¼\n",outputstream);
	}
	if(k==15){
		sprintf(outputstream,"%s������������ʾ��ǰ�û���\n",outputstream);
	}
	if(k==16){
		sprintf(outputstream,"%schmod �ļ� ��λ�߽�����\n��ָ���ļ���Ȩ���޸�Ϊ������λ�߽�������ʾ��Ȩ�ޡ�\n�������λ��ʾ�ļ����������е�Ȩ�ޣ��θ�λ��ʾ�ļ���������û����е�Ȩ�ޣ�ĩλ��ʾ��ͨ�û����е�Ȩ��\nÿ���߽������Ƕ�ȡ��4��д��:2��ִ�У�1�����\n",outputstream);
	}
	if(k==17){
		sprintf(outputstream,"%spasswd �û���\n�޸�ָ���û�������\npasswd -d �û��� ɾ��ָ���û������롣\n��Ҫ��ǰ�û��Ǳ��޸ĵ��û�����root�û����С�",outputstream);
	}
	if(k==18){
		sprintf(outputstream,"%schown �û��� �ļ���\n�����ļ�������\n��Ҫ�ļ�����Ȩ��ǰ�û���root�û�����\n",outputstream);
	}
	if(k==19){
		sprintf(outputstream,"%schown �û��� �ļ���\n�����ļ�������\n��Ҫ�ļ�����Ȩ��ǰ�û���root�û�����\n",outputstream);
	}
	if(k==20){
		sprintf(outputstream,"%susermod -g �û����� �û���\n�����û��������û���\n��Ҫ��ǰ�û���root�û�����\n",outputstream);
		sprintf(outputstream,"%susermod -G �û����� �û���\n���û���ӵ��û���\n��Ҫ��ǰ�û���root�û�����\n",outputstream);
	}
	if(k==21){
		sprintf(outputstream,"%sgpasswd -d �û����� �û���\n���û������Ƴ��û�\n��Ҫ��ǰ�û���root�û�����\n���ܴӸ��û������û������Ƴ����û���\n",outputstream);
	}
	if(k==22){
		sprintf(outputstream,"%sgroupdel �û�����\nɾ��һ���յ��û���\n��Ҫ��ǰ�û���root�û�����\n",outputstream);
	}
	if(k==23){
		sprintf(outputstream,"%suser �û���\n��Ϊĳ���û���¼\n����½�ɹ����л������û�����Ŀ¼\n������Ŀ¼���������л�����Ŀ¼��\n",outputstream);
		
	}
	if(k==24){
		sprintf(outputstream,"%scp Դ Ŀ���ļ���\nĿ���ļ����в������ļ����ļ�����Դ����\n����Դ�в����ļ���Ȩ���ʣ�����һ�����ļ��Ḵ��ʧ��\n",outputstream);
	}
	if(k==25){
		sprintf(outputstream,"%smv Դ Ŀ���ļ���\n�������Ժ�cpһ��\n��Ϊ����cp��rm�����\n",outputstream);	
	}
	if(k==26){
		sprintf(outputstream,"%shelp ������\n��ʾ����İ���\nҲ���Բ�����������ʾ�����б�\n",outputstream);			
	}
	if(k==27){
		sprintf(outputstream,"%sbash �������ļ�\n���ж��벢ִ��\n",outputstream);			
	}
	if(k==28){
		sprintf(outputstream,"%s������������ҳ����ͬѧ������cls˵����˲���linux..��\n",outputstream);			
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
	if(i<0){sprintf(outputstream,"%s�޷����ļ�\n",outputstream);return;}
	if(!hasexepri(i)){sprintf(outputstream,"%sȨ�޲��㣡\n",outputstream);return;}
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
	if(k==-1){if(!oe)printf("δ��ʶ������!\n");return;}
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
	if(k==26){help(temp+l);if(!oe)printf("%s",outputstream);}else if(k==8)exit();else if(k==28){b=false;cclear();}else if(k==23){fuser(temp+l);if(ME==-1){if(!oe)printf("��½ʧ��\n");}else {if(!oe)printf("��½�ɹ�����Ϊ%s\n",USERS[finduser(ME)].username);}}else {if(!oe)printf("���ȵ�¼��\n");}	
}
	outputstream[0]=0;
}

