#include "disk.h"
int wheretoput;
data whattoput;
bool howtoput;
data key;
FILE* fil; 
AES aesaes;
MD5 md5md5;
void clean(data x){
	for(int i=0;i<16;i++)
		x[i]=0;
	
	return;
}
void print(const data x){	
	for(int i=0;i<16;i++)
		if(!oe)printf("%02X",x[i]);
	if(!oe)printf("\r\n");
	
	return;
}
void copy(const data x,data y){
	for(int i=0;i<16;i++)
		y[i]=x[i];
	return;
}
void aesinit(const data key){
	aesaes.init(key);
	return;
}
void aes(const data input,data output){
	aesaes.Cipher(input,output);
	return;
}
void md5(const data input,data output){
	md5md5.settext(input);
	md5md5.md5(output);
	return;
}

bool equal(const data x,const data y){
	for(int i=0;i<16;i++)
		if(x[i]!=y[i]){return false;}
	return true;
} 
void xxor(const data a,data b){
	for(int i=0;i<16;i++)
		b[i]=a[i]^b[i];
	return;
}
void getkey(const int x,data rtn){
	data t,tmp;
	for(int i=0;i<16;i++){
		if(i%4==0)t[i]=(x>>24)%256;
		if(i%4==1)t[i]=(x>>16)%256;
		if(i%4==2)t[i]=(x>>8)%256;
		if(i%4==3)t[i]=x%256;
	}
	md5(t,tmp);
	aes(tmp,rtn);
	clean(t);clean(tmp);
	return;
}
void chdata(const int seed,const data a,data b){
	data tmp;
	getkey(seed,tmp);
	for(int i=0;i<16;i++)
		b[i]=a[i]^tmp[i];
	clean(tmp);
	return;
}
void get(const int offset,data rtn){
	data tmp;
	fseek(fil,offset*16,0);
	for(int i=0;i<16;i++) 
		tmp[i]=fgetc(fil);
	chdata(offset,tmp,rtn);
	clean(tmp);
	return;
}
void put(const int offset,const data dat) {
	put(offset,dat,true);
}
void put(const int offset,const data dat,const bool mac) {
	wheretoput=offset;
	copy(dat,whattoput);
	howtoput=mac;
running=true;
	data tmp;
	data tmp1;
	get(offset,tmp1);
	if(mac){
		if(offset!=disksize){
			get(disksize,tmp);
			for(int i=0;i<16;i++)
				tmp[i]=tmp[i]^dat[i]^tmp1[i];
			put(disksize,tmp,false);
		}
	}
	chdata(offset,dat,tmp);
	fseek(fil,offset*16,0);
	for(int i=0;i<16;i++) fputc(tmp[i],fil);
	fflush(fil);
	clean(tmp);
running=false;	 
	return;
}
bool checkdisk(){
	bool tmp;
#ifndef _NO_KEY_
	data a,b,c,tttmp;
	for(int i=0;i<16;i++) b[i]=0;
	for(int i=0;i<16;i++) c[i]=0;
	for(int i=0;i<disksize;i++){
		get(i,tttmp);
		xxor(tttmp,b);
		if((i+1)%2048==0)
			if(!oe)printf("Integrity test %d/16916480\r",16*(i+1));
	}
	if(!oe)printf("Integrity test finish              \r\n");
	get(disksize,a);
	tmp=equal(b,a);
	clean(a);
	clean(b);
	clean(c);
	clean(tttmp);
	if(tmp){
		return true;
		tmp=false;
	}else{
		return false;
		tmp=false;
	}
#else
	return true;
#endif
}
bool disktest(){
	data a,b;
	data x;
	bool booboo;
	int q,t;
	q=0;
	for(int i=0;i<16;i++)x[i]=0;
	srand(time(NULL));
	for(int i=0;i<65536;i++){
		booboo=false;
		for(int j=0;j<16;j++)
			a[j]=rand()%256;
			t=rand()%256;			
				put(t,a,true);
				get(t,b);
				if(equal(a,b))q++;
				put(t,x,true);
				if(!oe)printf("I/O test %d/65536\r",i+1);
	}
	if(!oe)printf("%d/65536",q);
	clean(a);
	clean(b);
	clean(x);
	t=0;
	booboo=false;	
	if(q==65536){
		q=0;
		return true;
	}else{
		q=0;
		return false;
	}
}

void createdisk(){
	data k1,k2;
	do{ 
		if(!oe)printf("Please enter key for your new disk\r\n");loadkey(k1);
		if(!oe)printf("Please re-enter your key\r\n");loadkey(k2);
		if(equal(k1,k2)){
			if(!oe)printf("Key Confirmed.\r\n");
			break;
		}else if(!oe)printf("Keys do not match.Please check and enter again.\r\n");
	}while(!equal(k1,k2));
	clean(k1);
	clean(k2);	
	fil=fopen("mydisk.disk","wb+");
	fclose(fil); 
	fil=fopen("mydisk.disk","rb+");
	data c,x;
	for(int i=0;i<16;i++)c[i]=0;
	for(int i=0;i<=disksize;i++){
		put(i,c,false);
		if(i%1024==0)
			if(!oe)printf("Init:%d/1057280\r",i);
	}
	if(!oe)printf("\r\n");

#ifdef _CHECK_DISK_
	if(!oe)printf("Disk created!Starting random I/O test.       \r\n");
	if(!disktest()){
		if(!oe)printf("I/O test failed,please Retry!\r\n");
	}else 
		if(!oe)printf("I/O test passed!\r\n");
#endif
setup=true;
ME=0; 
	newf(0,(unsigned char *)"");
	setupfolder(0,0);
	setpri(0,493);
	startup();
	setpri(1,256+64+32+8+4+1);
	setpri(2,0);
	setpri(3,0);
	mmkdir("/usr");
	setpri(4,256+64+32+8+4+1);
	mmkdir("/usr/root/");
	setpri(5,484);
	setup=false;
	fclose(fil);
	if(!oe)printf("The new virtual disk is ready.\r\n");
} 
bool aestest(){
	data a,b,c;
	int q,t;
	q=0;
	srand(time(NULL));
	for(int i=0;i<65536;i++){
		for(int j=0;j<16;j++)
			a[j]=rand()%256;
		aesinit(a);
		for(int j=0;j<16;j++)
			a[j]=rand()%256;
		t=rand();
		chdata(t,a,b);
		chdata(t,b,c);
		if(equal(a,c))
			q++;
		if(i%1024==0)
			if(!oe)printf("AES test %d/65536\r",i);
	}
	if(!oe)printf("AES test finished!Passed %d/65536 tests\r\n",q);
	clean(a);clean(b);clean(c);t=0;
	if(q==65536)
		return true;
	else 
		return false;
}
bool md5test(){
	data a,b,c;
	int q;
	q=0;
	srand(time(NULL));
	for(int i=0;i<65536;i++){
		for(int j=0;j<16;j++)
			a[j]=rand()%256;
		md5(a,b);
		md5(a,c);
		if(equal(b,c))q++;
		if(i%1024==0)
			if(!oe)printf("MD5 test %d/65536\r",i);
	}
	if(!oe)printf("MD5 test finished!Passed %d/65536 tests\r\n",q);
	clean(a);
	clean(b);
	clean(c);
	if(q==65536)
		return true;
	else
		return false;
}

bool pretest(){
#ifdef _SELF_TEST_
	return aestest()&&md5test();
#else
	return true;
#endif
}

bool loaddisk(){
#if defined(_WIN32)
	if((_access("mydisk.disk",0))==-1){
#else
	if((access("mydisk.disk",0))<0){
#endif
		if(!oe)printf("Disk not found!Now creating.\r\n");
		createdisk();
	}
#if defined(_WIN32)
	if((_access("mydisk.disk",6))==-1){
#else
	if((access("mydisk.disk",6))<0){
#endif
		if(!oe)printf("Access Denied!\r\n");
		system("pause>nul");		
		return false;
	}
	fil=fopen("mydisk.disk","rb+");
	data key;
	while(1){
		if(!oe)printf("Please input your key for decryption.");loadkey(key);clean(key);
#ifndef _NO_KEY_
		if(checkdisk()){
			if(!oe)printf("Secure.\r\n");
			break;
		}else if(!oe)printf("Modified Or Password Error.\r\nPlease retry.\r\n");
#else
                 break;
#endif
	}
	startup();
	
	return true;
}
void cleardisk(){
	data c;	
	for(int i=0;i<16;i++)c[i]=0;
	for(int i=0;i<=disksize;i++){
		put(i,c,false);
		if(i%1024==0)
			if(!oe)printf("Init:%d/1057280\r",i);
	}
}
