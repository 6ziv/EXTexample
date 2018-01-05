#include"disk.h"
bool setup;
f_data nothing;
const plarr nullplarr={-1,-1,-1,-1,-1};
void cpy(unsigned char *a,const unsigned char *b){
	for(int i=0;i<252;i++)
		a[i]=b[i];
	return;
}
bool equ(const unsigned char *a,const unsigned char *b){
	int i;
	i=0;
	while((a[i]!=0)&&(i<252)){if(a[i]!=b[i])return false;i++;}
	if(i==252)return true;
	if(b[i]!=0)return false;else return true;
}
void switchtoinode(const inodestruct x,inodedata *y){
	y->highpart[0]=(x.filesize>>24)&(0xFF);
	y->highpart[1]=(x.filesize>>16)&(0xFF);
	y->highpart[2]=(x.filesize>>8)&(0xFF);
	y->highpart[3]=x.filesize&(0xFF);
	y->highpart[4]=(x.attribute>>24)&(0xFF);
	y->highpart[5]=(x.attribute>>16)&(0xFF);
	y->highpart[6]=(x.attribute>>8)&(0xFF);
	y->highpart[7]=x.attribute&(0xFF);
	y->highpart[8]=(x.owner>>24)&(0xFF);
	y->highpart[9]=(x.owner>>16)&(0xFF);
	y->highpart[10]=(x.owner>>8)&(0xFF);
	y->highpart[11]=x.owner&(0xFF);
	y->highpart[12]=(x.createtime>>24)&(0xFF);
	y->highpart[13]=(x.createtime>>16)&(0xFF);
	y->highpart[14]=(x.createtime>>8)&(0xFF);
	y->highpart[15]=x.createtime&(0xFF);
	y->lowpart[0]=(x.lastwritetime>>24)&(0xFF);
	y->lowpart[1]=(x.lastwritetime>>16)&(0xFF);
	y->lowpart[2]=(x.lastwritetime>>8)&(0xFF);
	y->lowpart[3]=x.lastwritetime&(0xFF);
	y->lowpart[4]=(x.lastaccesstime>>24)&(0xFF);
	y->lowpart[5]=(x.lastaccesstime>>16)&(0xFF);
	y->lowpart[6]=(x.lastaccesstime>>8)&(0xFF);
	y->lowpart[7]=x.lastaccesstime&(0xFF);
	y->lowpart[8]=(x.places[0]>>24)&(0xFF);
	y->lowpart[9]=(x.places[0]>>16)&(0xFF);
	y->lowpart[10]=(x.places[0]>>8)&(0xFF);
	y->lowpart[11]=x.places[0]&(0xFF);
	y->lowpart[12]=(x.places[1]>>24)&(0xFF);
	y->lowpart[13]=(x.places[1]>>16)&(0xFF);
	y->lowpart[14]=(x.places[1]>>8)&(0xFF);
	y->lowpart[15]=x.places[1]&(0xFF);
}
void switchtostruct(const inodedata y,inodestruct *x){
	x->filesize=(((int)y.highpart[0]<<24)&(0xFF000000))|(((int)y.highpart[1]<<16)&(0x00FF0000))|(((int)y.highpart[2]<<8)&(0x0000FF00))|(((int)y.highpart[3])&(0x000000FF));
	x->attribute=(((int)y.highpart[4]<<24)&(0xFF000000))|(((int)y.highpart[5]<<16)&(0x00FF0000))|(((int)y.highpart[6]<<8)&(0x0000FF00))|(((int)y.highpart[7])&(0x000000FF));
	x->owner=(((int)y.highpart[8]<<24)&(0xFF000000))|(((int)y.highpart[9]<<16)&(0x00FF0000))|(((int)y.highpart[10]<<8)&(0x0000FF00))|(((int)y.highpart[11])&(0x000000FF));
	x->createtime=(((int)y.highpart[12]<<24)&(0xFF000000))|(((int)y.highpart[13]<<16)&(0x00FF0000))|(((int)y.highpart[14]<<8)&(0x0000FF00))|(((int)y.highpart[15])&(0x000000FF));
	x->lastwritetime=(((int)y.lowpart[0]<<24)&(0xFF000000))|(((int)y.lowpart[1]<<16)&(0x00FF0000))|(((int)y.lowpart[2]<<8)&(0x0000FF00))|(((int)y.lowpart[3])&(0x000000FF));
	x->lastaccesstime=(((int)y.lowpart[4]<<24)&(0xFF000000))|(((int)y.lowpart[5]<<16)&(0x00FF0000))|(((int)y.lowpart[6]<<8)&(0x0000FF00))|(((int)y.lowpart[7])&(0x000000FF));
	x->places[0]=(((int)y.lowpart[8]<<24)&(0xFF000000))|(((int)y.lowpart[9]<<16)&(0x00FF0000))|(((int)y.lowpart[10]<<8)&(0x0000FF00))|(((int)y.lowpart[11])&(0x000000FF));
	x->places[1]=(((int)y.lowpart[12]<<24)&(0xFF000000))|(((int)y.lowpart[13]<<16)&(0x00FF0000))|(((int)y.lowpart[14]<<8)&(0x0000FF00))|(((int)y.lowpart[15])&(0x000000FF));
}
void writeinodedata(const int id,const inodedata x){
	inodedata tmp;
	put(INODE+(id*2),x.highpart);
	put(INODE+(id*2)+1,x.lowpart);
	get(INODE+(id*2),tmp.highpart);
	get(INODE+(id*2)+1,tmp.lowpart);
}
void writeinode(const int id,const inodestruct y){
	inodedata x;
	switchtoinode(y,&x);
	put(INODE+(id*2),x.highpart);
	put(INODE+(id*2)+1,x.lowpart);
}
void readinodedata(const int id,inodedata x){
	get(INODE+(id*2),x.highpart);
	get(INODE+(id*2)+1,x.lowpart);
}
void readinode(const int id,inodestruct *y){
	inodedata x;
	data qqq;
	get(INODE+(id*2),x.highpart);
	get(INODE+(id*2)+1,x.lowpart);
	switchtostruct(x,y);
}
void getinodeattribute(const int id,const unsigned int mask,int* rtn){
	data x;
	int tmp;
	if(mask<16)get(INODE+(id*2),x);else	get(INODE+(id*2)+1,x);
	tmp=mask&(0x0F);
	*rtn=(((int)x[tmp]<<24)&(0xFF000000))+(((int)x[tmp+1]<<16)&(0xFF0000))+(((int)x[tmp+2]<<8)&(0xFF00))+(x[tmp+3]&(0xFF));
}
void setinodeattribute(const int id,const unsigned int mask,const int dat){
	data x;
	int tmp;
	tmp=mask&(0x0F);
	if(mask<16)get(INODE+(id*2),x);else	get(INODE+(id*2)+1,x);
	x[tmp+3]=(unsigned char)(dat&(0x000000FF));
	x[tmp+2]=(unsigned char)((dat>>8)&(0x000000FF));
	x[tmp+1]=(unsigned char)((dat>>16)&(0x000000FF));
	x[tmp+0]=(unsigned char)((dat>>24)&(0x000000FF));
	if(mask<16)put(INODE+(id*2),x);else	put(INODE+(id*2)+1,x);
}
void getinodeplaces(const int id,plarr rtn){
	data tttmpx;
	int tmp;
	int i;
	getinodeattribute(id,MASK_FILESIZE,&tmp);
	get(INODE+(id*2)+1,tttmpx);
	rtn[0]=tttmpx[8]*16+tttmpx[9]/16;rtn[1]=tttmpx[9]%16*256+tttmpx[10];
	rtn[2]=tttmpx[11]*16+tttmpx[12]/16;rtn[3]=tttmpx[12]%16*256+tttmpx[13];
	rtn[4]=tttmpx[14]*16+tttmpx[15]/16;i=tttmpx[15]%16;
	while(i<5){
		rtn[i]=-1;i++;
	}
	i=0;
}
void writeinodeplaces(const int id,const plarr rtn){
	data tttmpx;
	int tmp;
	get(INODE+(id*2)+1,tttmpx);int i=0;while((rtn[i]!=-1)&&(i<5))i++;
	tttmpx[8]=rtn[0]/16;tttmpx[9]=rtn[0]%16*16+rtn[1]/256;tttmpx[10]=rtn[1]%256;
	tttmpx[11]=rtn[2]/16;tttmpx[12]=rtn[2]%16*16+rtn[3]/256;tttmpx[13]=rtn[3]%256;
	tttmpx[14]=rtn[4]/16;tttmpx[15]=rtn[4]%16*16+i;
	put(INODE+(id*2)+1,tttmpx);
}
int count(unsigned char i){
	return((i&1)+((i>1)&1)+((i>2)&1)+((i>3)&1)+((i>4)&1)+((i>5)&1)+((i>6)&1)+((i>7)&1));
}
int freeinodespace(){
	data x;
	int sum=0;
	for(int i=0;i<32;i++){
		get(i,x);
		for(int j=0;j<16;j++)sum+=count(x[j]); 
	}
	return sum;
}
int freediskspace(){
	data x;
	int sum=0;
	for(int i=0;i<32;i++){
		get(i|(0x20),x);
		for(int j=0;j<16;j++)sum+=count(x[j]); 
	}
	return sum;
}
int gethighest(const unsigned char x){
	if((~(x|0x7F))&(0xFF))return 0;
	if((~(x|0xBF))&(0xFF))return 1;
	if((~(x|0xDF))&(0xFF))return 2;
	if((~(x|0xEF))&(0xFF))return 3;
	if((~(x|0xF7))&(0xFF))return 4;
	if((~(x|0xFB))&(0xFF))return 5;
	if((~(x|0xFD))&(0xFF))return 6;
	if((~(x|0xFE))&(0xFF))return 7;
	return -1;
}
int findfreeinode(){
	data x;
	int k,t;
	for(int i=0;i<32;i++){
		get(i,x);
		for(int j=0;j<16;j++){
			if(x[j]==255)continue;
			k=0;t=1;
			while(t&x[j]){
				t=t*2;
				k=k+1;
			}
			if(k<8)return(k+(8*j)+(128*i));
		}
	}
	return DEF_ERROR_DISK_FULL;
}
int findfreedisk(){
	data x;
	int k,t;
	for(int i=0;i<32;i++){
		get(i+32,x);
		for(int j=0;j<16;j++){
			if(x[j]==255)continue;
			k=0;t=1;
			while(t&x[j]){
				t=t*2;
				k=k+1;
			}
			if(k<8)return(k+(8*j)+(128*i));
		}
	}
/*		if (!isdiskoccupied(i))
			return i;*/
	return DEF_ERROR_DISK_FULL;
}
bool isinodeoccupied(const int id){
	data x;
	int i,j,k,t;
	i=id/128;
	j=id%128;
	j=j/8;
	k=id%8;
	t=1;
	while(k>0){
		t=t*2;k=k-1;
	}
	get(i,x);
	return(x[i]&t);
}
bool isdiskoccupied(const int id){
	data x;
	int i,j,k,t;
	i=id/128+32;
	j=id%128;
	j=j/8;
	k=id%8;
	t=1;
	while(k>0){
		t=t*2;k=k-1;
	}
	get(i,x);
	return(x[i]&t);
}
void occupyinode(const int id){
	data x;
	int i,j,k,t;
	i=id/128;
	j=id%128;
	j=j/8;
	k=id%8;
	t=1;
	while(k>0){
		t=t*2;k=k-1;
	}
	get(i,x);
	if(!(x[j]&t)){
		x[j]=x[j]+t;
		put(i,x);
	}
	return;
}
void occupydisk(const int id){
	data x;
	int i,j,k,t;
	i=id/128+32;
	j=id%128;
	j=j/8;
	k=id%8;
	t=1;
	while(k>0){
		t=t*2;k=k-1;
	}
	get(i,x);
	if(!(x[j]&t)){
		x[j]=x[j]+t;
		put(i,x);
	}
	return;
}
void freeinode(const int id){
	data x;
	int i,j,k,t;
	i=id/128;
	j=id%128;
	j=j/8;
	k=id%8;
	t=1;
	while(k>0){
		t=t*2;k=k-1;
	}
	get(i,x);
	if(x[j]&t){
		x[j]=x[j]-t;
		put(i,x);
	}
	return;
}
void freedisk(const int id){
	data x;
	int i,j,k,t;
	i=id/128+32;
	j=id%128;
	j=j/8;
	k=id%8;
	t=1;
	while(k>0){
		t=t*2;k=k-1;
	}
	get(i,x);
	if(x[j]&t){
		x[j]=x[j]-t;
		put(i,x);
	}
	return;
}

bool isdir(const int id){
	int x; 
	getinodeattribute(id,MASK_ATTRIBUTE,&x);
	return ((x>>31)&1);
}
int getsize(const int id){	
	int x; 
	getinodeattribute(id,MASK_FILESIZE,&x);
	if(x>4096*5)x=0;
	return (x);
}
void setsize(const int id,int size){	
	setinodeattribute(id,MASK_FILESIZE,size);
	return;
}
int readfile(const int id,f_data rtn,int *filesize){
	plarr tmp;
	getinodeplaces(id,tmp);
	int i,j,k;
	i=0;
	data t;
	*filesize=getsize(id);
	if((*filesize<0)||(*filesize>5*4096)){
		*filesize=0;
		return FILE_TOO_LARGE;
	}
	while(tmp[i]!=-1){
		for(j=0;j<256;j++){
			get(INODE+tmp[i]*256+j,t);
			for(k=0;k<16;k++){
				*(rtn+i*4096+j*16+k)=*(t+k);
			}
		}
		i++;
	}
	setinodeattribute(id,MASK_LASTACCESSTIME,Now());
	return ERROR_SUCCESS;
}
int writefile(const int id,const f_data dat,const int filesize){
	plarr tmp;
	getinodeplaces(id,tmp);
	int i,j,k;
	i=0;
	data t;
	if(filesize>4096*5)return FILE_TOO_LARGE;
	if(filesize>0)
	while(i<(filesize-1)/4096+1){
		if(tmp[i]==-1){
			k=findfreedisk();
			if(k==DEF_ERROR_DISK_FULL)return DEF_ERROR_DISK_FULL;
			occupydisk(k);
			tmp[i]=k;
		}
	i++;
	}
	if(i<5)
		do{
			if(tmp[i]!=-1)freedisk(tmp[i]); 
			tmp[i]=-1;
			i++;
		}while(i<5);
	
		writeinodeplaces(id,tmp);
	for(i=0;i<(filesize-1)/4096+1;i++){
		for(j=0;j<256;j++){
			for(k=0;k<16;k++)
				t[k]=dat[i*4096+j*16+k];
			put(INODE+tmp[i]*256+j,t);
		}
	}
	setsize(id,filesize);
	setinodeattribute(id,MASK_LASTWRITETIME,Now());
	setinodeattribute(id,MASK_LASTACCESSTIME,Now());
	return ERROR_SUCCESS;
}
int readfolder(int dir,dir_list rtn,int *items){
	int bytes;
	int ret;
	f_data tmp;
	ret=readfile(dir,tmp,&bytes);
	if(ret!=ERROR_SUCCESS)return ret;
	*items=bytes/256;
	if(*items>50){*items=0;return TOO_MANY_ITEMS;}
	for(int i=0;i<*items;i++){
		for(int j=0;j<252;j++)rtn[i].name[j]=tmp[256*i+j];
		rtn[i].inode_id=256*tmp[256*i+254]+tmp[256*i+255];
	}
	return ERROR_SUCCESS;
}
int writefolder(const int dir,dir_list dat,const int items){
	int bytes;
	int ret;
	f_data tmp;
	for(int i=0;i<items;i++){
		for(int j=0;j<252;j++)tmp[256*i+j]=dat[i].name[j];
		tmp[256*i+255]=dat[i].inode_id%256;
		tmp[256*i+254]=(dat[i].inode_id/256)%256;
	}	
	bytes=items*256;
	ret=writefile(dir,tmp,bytes);
	if(ret!=ERROR_SUCCESS)return ret;
	return ERROR_SUCCESS;
}
int newf(int dir,const unsigned char *fname){
int i;
i=0;
while(fname[i]){
	if((fname[i]>='a')&&(fname[i]<='z')){i++;continue;}
	if((fname[i]>='A')&&(fname[i]<='Z')){i++;continue;}
	if((fname[i]>='0')&&(fname[i]<='9')){i++;continue;}
	if(fname[i]=='.'){i++;continue;}
	sprintf(outputstream,"%s非法文件名：仅支持数字和字母\n",outputstream);
	return INVALID_PARAMETERS;
	i++;
}
	dir_list tmp;
	int items;
	int rtn;
	if(fileexist(dir,fname))return FILE_EXISTS;
	rtn=readfolder(dir,tmp,&items);
	if(rtn!=ERROR_SUCCESS)return rtn;
	if(items>49)return TOO_MANY_ITEMS;
	if(!haswritepri(dir))return ACCESS_DENIED; 
	rtn=findfreeinode();
	if(rtn<0)return rtn;
	occupyinode(rtn);
	writeinodeplaces(rtn,nullplarr);
	tmp[items].inode_id=rtn;
	cpy(tmp[items].name,fname);
	items=items+1;
//	setsize(rtn,0);
	writefolder(dir,tmp,items);
	return rtn;
}
int newf(int dir,const char *fname){
	int i;
	unsigned char tmp[256];
	while(fname[i]){
	if((fname[i]>='a')&&(fname[i]<='z')){i++;continue;}
	if((fname[i]>='A')&&(fname[i]<='Z')){i++;continue;}
	if((fname[i]>='0')&&(fname[i]<='9')){i++;continue;}
	if(fname[i]=='.'){i++;continue;}
	sprintf(outputstream,"%s非法文件名-仅支持数字与字母！\n",outputstream);
	return INVALID_PARAMETERS;
	i++;
	}
	i=0;
	while(fname[i]){
		tmp[i]=fname[i];
		i++;
	}
	tmp[i]=0;
	return newf(dir,tmp);
}
bool fileexist(int dir,const unsigned char *fname){
	int rtn;
	rtn=find(dir,fname);
	if(rtn!=FILE_NOT_FOUND)
		return true;else return false;
}
bool fileexist(int dir,const char *fname){
	unsigned char tmp[256];
	int rtn;
	int i;
	i=0;
	while(fname[i]){
		tmp[i]=fname[i];
		i++;
	}
	tmp[i]=0;
	rtn=find(dir,tmp);
	if(rtn!=FILE_NOT_FOUND)
		return true;else return false;
}
bool fileexistex(int dir,const unsigned char *fname){
	int rtn;
	rtn=findex(dir,fname);
	if(rtn!=FILE_NOT_FOUND)
		return true;else return false;
}
bool fileexistex(int dir,const char *fname){
	int rtn;
	unsigned char tmp[256];
	int i;
	i=0;
	while(fname[i]){
		tmp[i]=fname[i];
		i++;
	}
	tmp[i]=0;
	rtn=findex(dir,tmp);
	if(rtn!=FILE_NOT_FOUND)
		return true;else return false;
}
int delf(int dir,int inode){
	dir_list tmp;
	int items;
	int rtn;
	rtn=readfolder(dir,tmp,&items);
	if(rtn!=ERROR_SUCCESS)return rtn;
	rtn=0;
	while((tmp[rtn].inode_id!=inode)&&(rtn<items))rtn++;
	if(rtn==items)return FILE_NOT_FOUND;
	while(rtn<items){
		tmp[rtn].inode_id=tmp[rtn+1].inode_id;
		cpy(tmp[rtn].name,tmp[rtn+1].name);
		rtn++;
	}
	items--;
	rtn=writefile(inode,nothing,0);
	rtn=writefolder(dir,tmp,items);
	if(rtn==ERROR_SUCCESS)freeinode(inode);
	return rtn;
}
int find(int dir,const unsigned char *str){
	dir_list tmp;
	int items;
	int rtn;
	rtn=readfolder(dir,tmp,&items);
	if(rtn!=ERROR_SUCCESS)return rtn;
	rtn=0;
	while((!equ(str,tmp[rtn].name))&&(rtn<items))rtn++;
	if(rtn==items)return FILE_NOT_FOUND;
	return tmp[rtn].inode_id;
}
int find(int dir,const char *str){
	unsigned char tmp[256];
	int i;
	i=0;
	while(str[i]){
		tmp[i]=str[i];
		i++;
	}
	tmp[i]=0;
	return find(dir,tmp);
}
int findex(int dir,const unsigned char *str){
	unsigned char temp[252];
	cpy(temp,str);
	int i,j;
	i=0;j=0;
	unsigned char tmp;
	while((temp[i]=='/')&&(temp[i]!=0))i++;
	while(temp[i]){
		j=i;
		while((temp[i]!='/')/*&&(temp[i]!='\\')*/&&(temp[i]!=0))i++;
		if(temp[i]==0){
			temp[i+1]=0;
			temp[i]='/';
		}
		tmp=temp[i];
		if((temp[i]=='/')/*||(temp[i]=='\\')*/)temp[i]=0;
		dir=find(dir,temp+j);
		temp[i]=tmp;
		while((temp[i]=='/')/*||(temp[i]=='\\')*/)i++;
		j=i;
		if(dir<0)return dir;
	}
	return dir;
}
int findex(int dir,const char *str){
	unsigned char tmp[65536];
	int i;
	i=0;
	while(str[i]){
		tmp[i]=str[i];
		i++;
	}
	tmp[i]=0;
	return findex(dir,tmp);
}
int getowner(const int id) {
	int x;
	getinodeattribute(id,MASK_OWNER,&x);
	return (x);
}
void setowner(const int id,const int owner){	
	setinodeattribute(id,MASK_OWNER,owner);
	return;
}

int attachtofile(int id,f_data dat,int size){
	f_data tmp;
	int rtn;
	int filesize;
	rtn=readfile(id,tmp,&filesize);
	if(rtn!=ERROR_SUCCESS)return rtn;
	if(filesize+size>4096*5)return FILE_TOO_LARGE;
	for(int i=0;i<size;i++)
		tmp[filesize+i]=dat[i];
	filesize=filesize+size;
	rtn=writefile(id,tmp,filesize);
	if(rtn!=ERROR_SUCCESS)return rtn;
	return ERROR_SUCCESS;
}
int setupfolder(const int id,const int parent){
	setinodeattribute(id,MASK_ATTRIBUTE,1<<31);
	setpri(id,493);
	dir_list tmp;
	setowner(id,(ME%65536)*65536+(USERS[finduser(ME)].gid%65536));
	tmp[0].inode_id=id;
	tmp[0].name[0]='.';
	tmp[0].name[1]=0;
	tmp[1].inode_id=parent;
	tmp[1].name[0]='.';
	tmp[1].name[1]='.';
	tmp[1].name[2]=0;
	setinodeattribute(id,MASK_CREATETIME,Now());
	return writefolder(id,tmp,2);
}
int setupfile(const int id){
	setinodeattribute(id,MASK_ATTRIBUTE,0);
	setpri(id,420);	
	setsize(id,0);
	setowner(id,(ME%65536)*65536+(USERS[finduser(ME)].gid%65536));
	writeinodeplaces(id,nullplarr);
	setinodeattribute(id,MASK_CREATETIME,Now());
	return ERROR_SUCCESS;
}
int createfile(const int dir,unsigned char *fname){
	int id;
	id=newf(dir,fname);
	if(id<0)return id;
	return setupfile(id);
}
int readpri(const int id){
	int x;
	getinodeattribute(id,MASK_ATTRIBUTE,&x);
	return(x&511);
}
void setpri(const int id,const int pri){
	int x;
	getinodeattribute(id,MASK_ATTRIBUTE,&x);
	x=x-(x&511);
	x=x|(pri&511);
	setinodeattribute(id,MASK_ATTRIBUTE,x);
	return;
}
bool hasreadpri(const int id){
	if(setup)return true;
//	if(isroot())return true;
	int o,p;
	o=getowner(id);
	p=readpri(id);
	if(p&4)return true;
	if((p&32)&&isingroup(ME,o%65536))return true;
	if((p&256)&&(ME==o/65536))return true;
	return false;
}
bool haswritepri(const int id){
	if(setup)return true;
//	if(isroot())return true;
	int o,p;
	o=getowner(id);
	p=readpri(id);
	if(p&2)return true;
	if((p&16)&&isingroup(ME,o%65536))return true;
	if((p&128)&&(ME==o/65536))return true;
	return false;
}
bool hasexepri(const int id){
	if(setup)return true;
//	if(isroot())return true;
	int o,p;
	o=getowner(id);
	p=readpri(id);
	if(p&1)return true;
	if((p&8)&&isingroup(ME,o%65536))return true;
	if((p&64)&&(ME==o/65536))return true;
	return false;
}
bool isroot(){
	if(setup)return true;
	return(isingroup(ME,0));
}
int chown(int id,int usr){
	int x;
	if((getowner(id)/65536!=ME)&&(!isroot()))return ACCESS_DENIED;
	x=getowner(id);
	x=(x%65536)+(usr*65536);
	setowner(id,x);
	return ERROR_SUCCESS;
}
bool dirisance(const int id1,const int id2){
	if(id1==0)return true;
	if(id1==id2)return true;
	int tmp;
	if(!isdir(id2))return true;
	tmp=id2;
	while(tmp!=0){tmp=find(tmp,"..");if(tmp<0)return true;if(tmp==id1)return true;}
	return false;
}
int chgrp(int id,int grp){
	int x;
	if((getowner(id)/65536!=ME)&&(!isroot()))return ACCESS_DENIED;
	x=getowner(id);
	x=(x/65536)*65536+(grp%65536);
	setowner(id,x);
	return ERROR_SUCCESS;	
}
int copydata(const int src,const int dest){
	f_data tmp;
	int fs;int flag;
	if(!hasreadpri(src)){sprintf(outputstream,"%s权限不足\n",outputstream);return ACCESS_DENIED;}
	flag=readfile(src,tmp,&fs);
	if(flag<0)return flag;
	return writefile(dest,tmp,fs);
}
int cp(const int src,const int dest,const char *name){
	int i;
	int flag;
	if(!hasreadpri(src)){sprintf(outputstream,"%s没有权限\n",outputstream);return ACCESS_DENIED;}
	if(!isdir(dest)){sprintf(outputstream,"%s路径出错\n",outputstream);return NOT_A_DIRECTORY;}
	if(dirisance(src,dest)){sprintf(outputstream,"%s错误：循环复制\n",outputstream);return ERROR_LOOP;}
	if(!isdir(src)){
		if(fileexist(dest,name)){sprintf(outputstream,"%s文件已存在\n",outputstream);return FILE_EXISTS;}
		flag=newf(dest,name);
		if(flag<0){
			if(flag==ACCESS_DENIED)sprintf(outputstream,"%s没有权限\n",outputstream);
			else if(flag==FILE_EXISTS)sprintf(outputstream,"%s文件已存在\n",outputstream);
			else if(flag==DEF_ERROR_DISK_FULL)sprintf(outputstream,"%s磁盘空间不足\n",outputstream); 
			else if(flag==INVALID_PARAMETERS)sprintf(outputstream,"%s无效的参数\n",outputstream); 
			else sprintf(outputstream,"%s未知错误\n",outputstream); 
			return flag;
		}
		i=find(dest,name);
		if(i<0){sprintf(outputstream,"%s新建文件失败。。\n",outputstream);return i;}
		setupfile(i);
		flag=copydata(src,i); 
		return flag;
	}else{
		if(fileexist(dest,name))return FILE_EXISTS;
		flag=newf(dest,name);		if(flag<0){
			if(flag==ACCESS_DENIED)sprintf(outputstream,"%s权限不足\n",outputstream);
			else if(flag==FILE_EXISTS)sprintf(outputstream,"%s文件已存在\n",outputstream);
			else if(flag==DEF_ERROR_DISK_FULL)sprintf(outputstream,"%s磁盘已满\n",outputstream); 
			else if(flag==INVALID_PARAMETERS)sprintf(outputstream,"%s参数错误\n",outputstream); 
			else sprintf(outputstream,"%s未知错误\n",outputstream); 
			return flag;
		}
		i=find(dest,name);
		if(i<0){sprintf(outputstream,"%s新建文件失败。。\n",outputstream);return i;}
		setupfolder(i,dest);
		dir_list temp;
		int items,loo;
		char tmp[256];
		flag=readfolder(src,temp,&items);
		if(flag<0)return flag;
		for(flag=2;flag<items;flag++){
			loo=0;
			while(temp[flag].name[loo]){tmp[loo]=temp[flag].name[loo];loo++;}
			tmp[loo]=0;
			cp(temp[flag].inode_id,i,tmp);
		}
		return ERROR_SUCCESS;
	}
}
int cp(char *str){
	char temp[65536];
	int i,j,k,l;
	i=0;
	while((str[i])&&(str[i]!=' ')){
		temp[i]=str[i];
		i++;
	}
	temp[i]=0;
	while(str[i]==' ')i++;
	l=i;
	if(!str[i]){sprintf(outputstream,"%s参数错误\n",outputstream);return INVALID_PARAMETERS;}
	j=fffind(temp);
	if(j<0){sprintf(outputstream,"%s找不到文件\n",outputstream);return j;}
	k=fffind(str+i);
	if(k<0){
		i=0;
		while((str[i+l]!=0)&&(str[i+l]!=' ')){
			temp[i]=str[i+l];
			i++;
		}
		temp[i]=0;
		while((i>=0)&&(temp[i]!='/'))i--;
		if(i<=0)k=LOCAL;
			else{k=fffind(temp);temp[i]=0;}
		return cp(j,k,temp+i+1);
	}
	if(!isdir(k)){
		sprintf(outputstream,"%s文件已存在\n",outputstream);
		return j;
	}
	while((i>=0)&&(temp[i]!='/'))i--;
	i++;
	return cp(j,k,temp+i);
}
int mv(char *str){
	int i,j,k,l;
	char temp[65536];
	i=0;
	while((str[i])&&(str[i]!=' ')){
		temp[i]=str[i];
		i++;
	}
	temp[i]=0;
	i=fffind(temp);
	if(i<0)return i;	
	j=cp(str);
	if(j<0)return i;
	if(isdir(i))return rmdirname(temp);else return rm(temp);
}
