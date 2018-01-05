#include"disk.h"
int ME;
user_list USERS;
int users;
group_list GROUPS;
int groups;
const char hhex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int todec(char c){
	if((c>='0')&&(c<='9'))return c-'0';
	if((c>='A')&&(c<='F'))return c-'A'+10;
	if((c>='a')&&(c<='f'))return c-'a'+10;
	return -1;
}
int loaduserinfo(){
	int file;
	f_data tmp;
	int filesize; 
	int i,j,k;
	file=fffind("/etc/passwd");
	if(file<0){
		file=fffind("/etc/");
		if(file<0){
			file=mmkdir("/etc");
			if(file<0)return file;
			file=fffind("/etc/");
			if(file<0)return file;
		}
		file=newf(file,"passwd");
		if(file<0)return file;
		file=fffind("/etc/passwd");
		if(file<0)return file;
		setupfile(file);
		tmp[0]='r';tmp[1]='o';tmp[2]='o';tmp[3]='t';tmp[4]=':';
		tmp[5]='/';tmp[6]='u';tmp[7]='s';tmp[8]='r';tmp[9]='/';
		tmp[10]='r';tmp[11]='o';tmp[12]='o';tmp[13]='t';tmp[14]=':';
		tmp[15]='0';tmp[16]=':';tmp[17]='0';tmp[18]=':';tmp[19]='\n';
		writefile(file,tmp,20);
	}
	readfile(file,tmp,&filesize);
	i=0;j=0;k=0;
	while(i<filesize){
		if(i<filesize)
		while(tmp[i]!=':'){
			USERS[j].username[i-k]=tmp[i];
			i++;
			if(i>=filesize)break;
		}
		USERS[j].username[i-k]=0;
		i++;
		k=i;
		if(i<filesize)
		while(tmp[i]!=':'){
			USERS[j].path[i-k]=tmp[i];
			i++;
			if(i>=filesize)break;
		}
		USERS[j].path[i-k]=0;
		i++;
		USERS[j].id=0;
		if(i<filesize)
		while(tmp[i]!=':'){
			USERS[j].id=USERS[j].id*10+tmp[i]-'0';
			i++;
			if(i>=filesize)break;
		}
		i++;
		USERS[j].gid=0;
		if(i<filesize)
		while(tmp[i]!=':'){
			USERS[j].gid=USERS[j].gid*10+tmp[i]-'0';
			i++;
			if(i>=filesize)break;
		}
		i++;
		k=i;
		if(((tmp[i]<'0')||(tmp[i]>'9'))&&((tmp[i]<'A')||(tmp[i]>'F')))
			USERS[j].havepass=false;
			else 
			USERS[j].havepass=true;
		if(i<filesize)
		while(!(((tmp[i]<'0')||(tmp[i]>'9'))&&((tmp[i]<'A')||(tmp[i]>'F')))){
			USERS[j].pass[(i-k)/2]=16*todec(tmp[i])+todec(tmp[i+1]);
			i=i+2;
			if(i>=filesize)break;
		}
		i++;
		k=i;
		j++;
	}
	users=j;
	return ERROR_SUCCESS;
}
int writeuserinfo(){
	f_data tmp;

	int file;
	file=fffind("/etc/passwd");
	if(file<0){
		file=fffind("/etc/");
		if(file<0){
			file=mmkdir("/etc");
			if(file<0)return file;
			file=fffind("/etc/");
			if(file<0)return file;
		}
		newf(file,"passwd");
		file=fffind("/etc/passwd");
		if(file<0)return file;
		setupfile(file);
		tmp[0]='r';tmp[1]='o';tmp[2]='o';tmp[3]='t';tmp[4]=':';
		tmp[5]='/';tmp[6]='u';tmp[7]='s';tmp[8]='r';tmp[9]='/';
		tmp[10]='r';tmp[11]='o';tmp[12]='o';tmp[13]='t';tmp[14]=':';
		tmp[15]='0';tmp[16]=':';tmp[17]='0';tmp[18]=':';tmp[19]='\n';
		writefile(file,tmp,20);
	}
	int i,j,k;
	long long l;
	j=0;k=0;
	for(i=0;i<users;i++){
		while(USERS[i].username[j-k]){
			tmp[j]=USERS[i].username[j-k];
			j++;
		}
		tmp[j]=':';
		j++;
		k=j;
		while(USERS[i].path[j-k]){
			tmp[j]=USERS[i].path[j-k];
			j++;
		}
		tmp[j]=':';
		j++;
		l=1;
		while(l<=USERS[i].id)l=l*10;
		if(l>1)l=l/10;
		while(l>0){
			tmp[j]=((USERS[i].id%(10*l))/l)+'0';
			j++;
			l=l/10;
		}
		tmp[j]=':';
		j++;
		l=1;
		while(l<=USERS[i].gid)l=l*10;
		if(l>1)l=l/10;
		while(l>0){
			tmp[j]=((USERS[i].gid%(10*l))/l)+'0';
			j++;
			l=l/10;
		}
		tmp[j]=':';
		j++;
		if(USERS[i].havepass)
		for(k=0;k<16;k++){
			tmp[j]=hhex[USERS[i].pass[k]/16];
			tmp[j+1]=hhex[USERS[i].pass[k]%16];
			j=j+2;
		}
		tmp[j]='\n';
		j++;
		k=j;
	}
	writefile(file,tmp,j);
	return ERROR_SUCCESS;
}
int loadgroupinfo(){
	int file;
	f_data tmp;
	file=fffind("/etc/group");
	if(file<0){

		file=fffind("/etc/");

		if(file<0){
			file=mmkdir("/etc");
			if(file<0)return file;
			file=fffind("/etc/");
			if(file<0)return file;
		}
		newf(file,"group");
		file=fffind("/etc/group");
		if(file<0)return file;
		setupfile(file);
		tmp[0]='r';tmp[1]='o';tmp[2]='o';
		tmp[3]='t';tmp[4]=':';tmp[5]='0';
		tmp[6]=':';tmp[7]='0';tmp[8]='\n';
		writefile(file,tmp,8);
	}
	int i,j,k,l,m,filesize;
	char uname[256];
	readfile(file,tmp,&filesize);
	i=0;j=0;
	while(i<filesize){
		m=0;
		k=i;
		if(i<filesize)
		while(tmp[i]!=':'){
			GROUPS[j].groupname[i-k]=tmp[i];
			i++;
			if(i>=filesize)break;
		}
		GROUPS[j].groupname[i-k]=0;
		i++;
		GROUPS[j].id=0;

		if(i<filesize)
		while(tmp[i]!=':'){
			GROUPS[j].id=GROUPS[j].id*10+tmp[i]-'0';
			i++;
			if(i>=filesize)break;
		}
		i++;
		k=i;
		if(i<filesize){

			while((tmp[i]>='0')&&(tmp[i]<='9')){
				GROUPS[j].users[m]=0;
				l=i;
				while((tmp[i]>='0')&&(tmp[i]<='9')){
					GROUPS[j].users[m]=GROUPS[j].users[m]*10+tmp[i]-'0';
					i++;
				}
				if(tmp[i]==',')i++;
				if(finduser(GROUPS[j].users[m])<0)continue;
				if(USERS[finduser(GROUPS[j].users[m])].username[0]=='#')continue;
				m++;
			}
			GROUPS[j].users[m]=-1;
		}
		j++;
		i++;
	}
	groups=j;
	return ERROR_SUCCESS;
}
int writegroupinfo(){
	int file;
	f_data tmp;
	file=fffind("/etc/group");
	if(file<0){
		file=fffind("/etc/");
		if(file<0){
			file=mmkdir("/etc");
			if(file<0)return file;
			file=fffind("/etc/");
			if(file<0)return file;
		}
		newf(file,"group");
		file=fffind("/etc/group");
		if(file<0)return file;
		setupfile(file);
		tmp[0]='r';tmp[1]='o';tmp[2]='o';
		tmp[3]='t';tmp[4]=':';tmp[5]='0';
		tmp[6]=':';tmp[7]='0';tmp[8]='\n';
		writefile(file,tmp,8);
	}
	int i,j,k,m;
	long long l;
	j=0;k=0;
	for(i=0;i<groups;i++){
		while(GROUPS[i].groupname[j-k]){
			tmp[j]=GROUPS[i].groupname[j-k];
			j++;
		}
		tmp[j]=':';
		j++;
		l=1;
		while(l<=GROUPS[i].id)l=l*10;
		if(l>1)l=l/10;
		while(l>0){
			tmp[j]=((GROUPS[i].id%(10*l))/l)+'0';
			j++;
			l=l/10;
		}
		tmp[j]=':';
		j++;
		m=0;
		if(GROUPS[i].users[0]!=-1){
		while(GROUPS[i].users[m]!=-1){ 
			l=1;
			while(l<=GROUPS[i].users[m])l=l*10;
			if(l>1)l=l/10;
			while(l>0){
				tmp[j]=((GROUPS[i].users[m]%(10*l))/l)+'0';
				j++;
				l=l/10;
			}
			tmp[j]=',';
			j++;
			m++;
		}
		j--;
		}
		tmp[j]='\n';
		j++;
		k=j;
	}
	writefile(file,tmp,j);
	return ERROR_SUCCESS;
}
int finduser(const char *user){
	int i,j;
	for(i=0;i<users;i++){
		j=0;
		if(USERS[i].username[0]=='#')continue;
		while(user[j]){
			if(USERS[i].username[j]!=user[j])break;
			if(USERS[i].username[j]==0)break;
			j++;
		}
		if(USERS[i].username[j]!=user[j])continue;
		return USERS[i].id;
	}
	return NO_SUCH_USER;
}
int finduser(const int userid){
	int i;
	for(i=0;i<users;i++){
		if(USERS[i].username[0]=='#')continue;
		if(USERS[i].id==userid)return i;
	}
	return NO_SUCH_USER;
}
int findgroup(const char *group){
	int i,j;
	for(i=0;i<groups;i++){
		j=0;
		if(GROUPS[i].groupname[0]=='#')continue;
		while(group[j]){
			if(GROUPS[i].groupname[j]!=group[j])break;
			if(GROUPS[i].groupname[j]==0)break;
			j++;
		}
		if(GROUPS[i].groupname[j]!=group[j])continue;
		return GROUPS[i].id;
	}
	return NO_SUCH_GROUP;
}
int findgroup(const int groupid){
	int i;
	for(i=0;i<groups;i++){
		if(GROUPS[i].groupname[0]=='#')continue;
		if(GROUPS[i].id==groupid)return i;
	}
	return NO_SUCH_GROUP;
}

int newuser(const char *user){
	int i=0;
	int folder;
	char temp[300];
	while(user[i]){
		if((user[i]>='a')&&(user[i]<='z')){i++;continue;}
		if((user[i]>='A')&&(user[i]<='Z')){i++;continue;}
		if((user[i]>='0')&&(user[i]<='9')){i++;continue;}
		return INVALID_PARAMETERS;
	}
	i=newgroup(user);
//	if(i<0)return i;
	i=findgroup(user);
	if(i<0)return i;
	addtogroup(users,i);
	if(finduser(user)>=0)return USER_EXISTS;
	if(USERS[users].id>=65536)return TOO_MANY_USERS;
	if(users>0)USERS[users].id=USERS[users-1].id+1;else USERS[users].id=0;
	USERS[users].havepass=false;
	USERS[users].gid=i;
	i=0;
	while(user[i]){
		USERS[users].username[i]=user[i];
		i++;
	}
	USERS[users].username[i]=0;
	sprintf(temp,"/usr/%s",user);
	folder=fffind(temp);
	if(folder<0){
		bool bootmp;
		bootmp=setup;
		setup=true;
		folder=mmkdir(temp);
		setup=bootmp;
		folder=fffind(temp);
		if(folder<0)folder=0;else{
			setowner(folder,(USERS[users].id%65536)*65536+(USERS[users].gid%65536));
		}
	}
	sprintf(USERS[users].path,"%s",temp);
	users++;
	return writeuserinfo();
}
int deluser(const char *user){
	int id;
	int i,j,k;
	k=finduser(user);
	id=finduser(k);
	if(id<0){sprintf(outputstream,"%s找不到用户！\n",outputstream);return id;}
/*	users--;
	for(i=id;i<users;i++){
	USERS[i].id=USERS[i+1].id;
	USERS[i].havepass=USERS[i+1].havepass;
	for(j=0;j<16;j++)USERS[i].pass[j]=USERS[i+1].pass[j];
	j=0;
	while(USERS[i+1].username[j]){USERS[i].username[j]=USERS[i+1].username[j];j++;}
	USERS[i].username[j]=0;
	}*/
	USERS[id].username[0]='#';
	for(i=0;i<groups;i++){
		j=0;
		while((GROUPS[i].users[j]!=-1)&&(GROUPS[i].users[j]!=k))j++;
		while(GROUPS[i].users[j]!=-1)GROUPS[i].users[j]=GROUPS[i].users[j+1];
	}
	loadgroupinfo();
	writegroupinfo();
	return writeuserinfo();
}
void showusers(){
	int i,j,k;
	char temp[256];
	for(i=0;i<users;i++){
		if(USERS[i].username[0]=='#')continue;
		j=USERS[i].gid;
		j=findgroup(j);
		if(j<0){temp[0]='*';temp[1]=0;};
		k=0;
		while(GROUPS[j].groupname[k]){
			temp[k]=GROUPS[j].groupname[k];k++;
		}
		temp[k]=0;
		sprintf(outputstream,"%s%d\t%s\t%s\n",outputstream,USERS[i].id,USERS[i].username,temp);
	}
}
void showgroups(){
	int i;
	for(i=0;i<groups;i++){
		if(GROUPS[i].groupname[0]=='#')continue;
		sprintf(outputstream,"%s%d %s\n",outputstream,GROUPS[i].id,GROUPS[i].groupname);
	}
}

int newgroup(const char *group){
	int i=0;
	while(group[i]){
		if((group[i]>='a')&&(group[i]<='z')){i++;continue;}
		if((group[i]>='A')&&(group[i]<='Z')){i++;continue;}
		if((group[i]>='0')&&(group[i]<='9')){i++;continue;}
		return INVALID_PARAMETERS;
	}
	if(findgroup(group)>=0)return GROUP_EXISTS;
	if(GROUPS[groups-1].id>=65536)return TOO_MANY_GROUPS;
	if(groups>0)GROUPS[groups].id=GROUPS[groups-1].id+1;else GROUPS[groups].id=0;
//	USERS[groups].havepass=false;
	i=0;
	while(group[i]){
		GROUPS[groups].groupname[i]=group[i];
		i++;
	}
	GROUPS[groups].groupname[i]=0;
	GROUPS[groups].users[0]=-1;
	groups++;
	return writegroupinfo();
}
int addtogroup(const int user,const int group){
	int i,j;
	i=0;
	j=findgroup(group);
	if(j<0)return j;
	while(GROUPS[j].users[i]!=-1){if(GROUPS[j].users[i]==user)return ERROR_SUCCESS;i++;}
	GROUPS[j].users[i]=user;
	GROUPS[j].users[i+1]=-1;
	writegroupinfo();
}
int removefromgroup(int user,int group){
	int i,j;
	i=0;
	j=findgroup(group);
	if(j<0)return j;
	while(GROUPS[j].users[i]!=user){if(GROUPS[j].users[i]==-1)return NO_SUCH_USER;i++;}
	while(GROUPS[j].users[i]!=-1){
		GROUPS[j].users[i]=GROUPS[j].users[i+1];
		i++;
	}
	writegroupinfo();	
}
bool isingroup(const int user,const int group){
	int i,j;
	i=0;
	j=findgroup(group);
	if(j<0)return false;
	while(GROUPS[j].users[i]!=-1){if(GROUPS[j].users[i]==user)return true;i++;}
}
void startup(){
	loadgroupinfo();
	loaduserinfo();
	if(!setup)ME=-1;
}
void su(char *str){
	data key;
	int i,j;
	i=0;
	while(str[i]&&(str[i]!=' ')&&(str[i]!='\t')){
		if((str[i]<='9')&&(str[i]>='0')){i++;continue;}
		if((str[i]<='z')&&(str[i]>='a')){i++;continue;}
		if((str[i]<='Z')&&(str[i]>='A')){i++;continue;}
		sprintf(outputstream,"%s参数错误\n",outputstream);
		return;
	}
	i=finduser(finduser(str));
	if(i<0)return;
	if(USERS[i].havepass){
	loadkey(key);
	for(j=0;j<16;j++)
		if(key[j]!=USERS[i].pass[j]){
			sprintf(outputstream,"%s密码错误\n",outputstream);
			return;
		}
	}
	sprintf(outputstream,"%s已经作为%s登陆\n",outputstream,USERS[i].username);
	ME=USERS[i].id;
	return;
}
void fuser(char *str){
	data key;
	int i,j;
	i=0;
	while(str[i]&&(str[i]!=' ')&&(str[i]!='\t')){
		if((str[i]<='9')&&(str[i]>='0')){i++;continue;}
		if((str[i]<='z')&&(str[i]>='a')){i++;continue;}
		if((str[i]<='Z')&&(str[i]>='A')){i++;continue;}
		sprintf(outputstream,"%s参数错误\n",outputstream);
		return;
	}
	i=finduser(finduser(str));
	if(i<0)return;
	if(USERS[i].havepass){
	loadkey(key);
	for(j=0;j<16;j++)
		if(key[j]!=USERS[i].pass[j]){
			sprintf(outputstream,"%s密码错误\n",outputstream);
			return;
		}
	}
	j=fffind(USERS[i].path);
	ME=USERS[i].id;
	if(j>=0)cd(j);else cd(0);
	return;
}
void whoami(){
	int i;
	i=finduser(ME);
	if(i<0){sprintf(outputstream,"%s您尚未登陆\n",outputstream);return;}
	sprintf(outputstream,"%s%s\n",outputstream,USERS[i].username);
	return;
}
int chmod(char *str){
	int i;
	int temp;
	int p;
	char file[65536];
	i=0;
	while((str[i]!=' ')&&(str[i]!=0)){
		file[i]=str[i];
		i++;
	}
	file[i]=0;
	temp=fffind(file);
	if(temp<0){sprintf(outputstream,"%s路径不存在\n",outputstream);return temp;}
	if((str[i+1]>'7')||(str[i+1]<'0')){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	if((str[i+2]>'7')||(str[i+2]<'0')){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	if((str[i+3]>'7')||(str[i+3]<'0')){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	p=(str[i+1]-'0')*64+(str[i+2]-'0')*8+(str[i+3]-'0');
	if((!isroot())&&(getowner(temp)/65536!=ME)){sprintf(outputstream,"%s权限不足\n",outputstream);return temp;}
	setpri(temp,p);
}
int passwd(const char *user){
	int i,j;
	j=0;

	if((user[j+1])&&(user[j])){
		while((user[j+2])&&((user[j]!='-')||(user[j+1]!='d')||(user[j+2]!=' ')))
			j++;
		if(!user[j+2])
			j=0;
		else{
			j=j+2;
			while(user[j]==' ')j++;
		}
	}
	i=finduser(finduser(user+j));
	if(i<0){sprintf(outputstream,"%s无效用户\n",outputstream);return NO_SUCH_USER;}
if((!isroot())&&(USERS[i].id!=ME)){sprintf(outputstream,"%s权限不足！\n",outputstream);return ACCESS_DENIED;} 
	if(j==0){
	loadkey(USERS[i].pass);
	USERS[i].havepass=true;
	}else USERS[i].havepass=false;
	return writeuserinfo();
}

int chown(const char *str){
	char temp[256];
	int i,j,k;
	i=0;
	while((str[i])&&(str[i]!=' ')&&(i<256)){temp[i]=str[i];i++;}
	if((i>=256)||(!str[i])){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	temp[i]=0;
	while(str[i]==' ')i++;
	if(str[i]==0){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	j=finduser(temp);
	if(j<0){sprintf(outputstream,"%s无效用户\n",outputstream);return NO_SUCH_USER;}
	k=fffind(str+i);
	if(k<0){sprintf(outputstream,"%s找不到文件\n",outputstream);return FILE_NOT_FOUND;}
	k=chown(k,j);
	if(k==ACCESS_DENIED){sprintf(outputstream,"%s没有权限\n",outputstream);return ACCESS_DENIED;}
	return ERROR_SUCCESS;
}
int chgrp(const char *str){
	char temp[256];
	int i,j,k;
	i=0;
	while((str[i])&&(str[i]!=' ')&&(i<256)){temp[i]=str[i];i++;}
	if((i>=256)||(!str[i])){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	temp[i]=0;
	while(str[i]==' ')i++;
	if(str[i]==0){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	j=findgroup(temp);
	if(j<0){sprintf(outputstream,"%s无效用户组\n",outputstream);return NO_SUCH_GROUP;}
	k=fffind(str+i);
	if(k<0){sprintf(outputstream,"%s找不到文件\n",outputstream);return FILE_NOT_FOUND;}
	k=chgrp(k,j);
	if(k==ACCESS_DENIED){sprintf(outputstream,"%s没有权限\n",outputstream);return ACCESS_DENIED;}
}
int usermod(const char *str){
	if(!isroot()){sprintf(outputstream,"%s没有权限\n",outputstream);return ACCESS_DENIED;}
	int i,j,f,u;
	char temp[65536];
	if((str[0]=='-')&&(str[1]=='g')&&(str[2]==' ')){
		f=3;
		while(str[f]==' ')f++;
		i=f;
		while((i<65536)&&(str[i])&&(str[i]!=' ')){temp[i-f]=str[i];i++;}
		if((i>=65536)||(!str[i])){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		temp[i-f]=0;
		while(str[i]==' ')i++;
		if(str[i]==0){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		f=findgroup(temp);
		if(f<0)return f;
		u=finduser(str+i);
		if(u<0)return u;
		u=finduser(u);
		if(u<0)return u;
 		removefromgroup(USERS[u].id,USERS[u].gid);
		//if(j<0)return j;
		USERS[u].gid=f;
		writeuserinfo();
		return addtogroup(u,f);
	}	
	if((str[0]=='-')&&(str[1]=='G')&&(str[2]==' ')){
		f=3;
		while(str[f]==' ')f++;
		i=f;
		while((i<65536)&&(str[i])&&(str[i]!=' ')){temp[i-f]=str[i];i++;}
		if((i>=65536)||(!str[i])){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		temp[i-f]=0;
		while(str[i]==' ')i++;
		if(str[i]==0){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		f=findgroup(temp);
		if(f<0)return f;
		u=finduser(str+i);
		if(u<0)return u;
		return addtogroup(u,f);	
	}
	sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;
}
int gpasswd(const char *str){
	if(!isroot()){sprintf(outputstream,"%s没有权限\n",outputstream);return ACCESS_DENIED;}
	int i,j,f,u;
	char temp[65536];
	if((str[0]=='-')&&(str[1]=='d')&&(str[2]==' ')){
		f=3;
		while(str[f]==' ')f++;
		i=f;
		while((i<65536)&&(str[i])&&(str[i]!=' ')){temp[i-f]=str[i];i++;}
		if((i>=65536)||(!str[i])){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		temp[i-f]=0;
		while(str[i]==' ')i++;
		if(str[i]==0){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		f=findgroup(temp);
		if(f<0)return f;
		u=finduser(str+i);
		if(u<0)return u;
		u=finduser(u);
		if(u<0)return u;
 		//if(j<0)return j;
		if(USERS[u].gid==f){sprintf(outputstream,"%s不能从主用户组中移除\n",outputstream);return IS_MAIN_GROUP;}
		return removefromgroup(USERS[u].id,f);
	}	
	if((str[0]=='-')&&(str[1]=='a')&&(str[2]==' ')){
		f=3;
		while(str[f]==' ')f++;
		i=f;
		while((i<65536)&&(str[i])&&(str[i]!=' ')){temp[i-f]=str[i];i++;}
		if((i>=65536)||(!str[i])){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		temp[i-f]=0;
		while(str[i]==' ')i++;
		if(str[i]==0){sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;}
		f=findgroup(temp);
		if(f<0)return f;
		u=finduser(str+i);
		if(u<0)return u;
		return addtogroup(u,f);	
	}
	sprintf(outputstream,"%s参数无效\n",outputstream);return INVALID_PARAMETERS;
}
int groupdel(const char *group){
	if(!isroot()){sprintf(outputstream,"%s没有权限\n",outputstream);return ACCESS_DENIED;}
	int id;
	int i,j;
	id=findgroup(findgroup(group));
	if(id<0)return id;
	if(GROUPS[id].users[0]!=-1){sprintf(outputstream,"%s不能移除非空用户组\n",outputstream);return GROUP_NOT_EMPTY;} 
	GROUPS[id].groupname[0]='#';
	return writegroupinfo();
}
