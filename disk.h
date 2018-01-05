#ifndef _DISKP_12021017_H_
#define _DISKP_12021017_H_
#include "switches.h"
#include "aes.h"
#include "md5.h"
#include "errorconstances.h"
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include<signal.h>
#if defined(_WIN32) 
#include<io.h>
#include<windows.h>
#include<conio.h>
#else
#include<curses.h>
#include <memory.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <fcntl.h> 
#include <iconv.h> 
#endif
using namespace std;
typedef unsigned char data[16];
typedef struct inodedata{
	data highpart;
	data lowpart; 
}inodedata;
typedef int plarr[5];
typedef struct inode{
	int filesize;
	int attribute;
	int owner;
	int createtime;
	int lastwritetime;
	int lastaccesstime;
	int places[2];
}inodestruct;
typedef struct user_info{
	char username[256];
	char path[256];
	int id;
	int gid;
	data pass;
	bool havepass;
}user_info;
typedef struct group_info{
	char groupname[256];
	int id;
	int users[256];
}group_info;
typedef user_info user_list[256];
typedef group_info group_list[256];
typedef struct entry{
	unsigned char name[252];
	int inode_id;
}entry;
typedef unsigned char data_block[4096];
typedef unsigned char file_block[4096];
typedef unsigned char f_data[4096*5];
typedef entry dir_block[16];
typedef entry dir_list[16*5];
#if not defined(_WIN32)
extern iconv_t ico;
#endif
extern bool oe;
extern int wheretoput;
extern data whattoput;
extern bool howtoput;
extern char outputstream[65536*256];
extern bool running; 
extern FILE* fil;
extern bool setup;
extern int diskid;
extern int ME; 
extern int LOCAL; 
extern int users;
extern user_list USERS;
extern int groups;
extern group_list GROUPS;
const int INODE_BITMAP=0;
const int BLOCK_BITMAP=4096/16;
const int INODE=8192/16;
const int BLOCK=139264/16;
const int END=1057280/16;
const int MASK_FILESIZE=0;
const int MASK_ATTRIBUTE=4;
const int MASK_OWNER=8;
const int MASK_CREATETIME=12;
const int MASK_LASTWRITETIME=16;
const int MASK_LASTACCESSTIME=20;
const int MASK_PLACES=24;
const int disksize=1057280;
void beforeexit();
void beforeexit(int i);
int mmkdir(const char *str);
int rm(const char *str);
int mv(char *str);
int cp(char *str);
int cp(const int src,const int dest,const char *name);
bool isroot();
void fuser(char *str);
int groupdel(const char *group);
int gpasswd(const char *str);
int usermod(const char *str);
int chmod(char *str);
int chown(int id,int usr);
int chown(const char *str);
int chgrp(int id,int grp);
int chgrp(const char *str);
int passwd(const char *user);
bool hasreadpri(const int id);
bool haswritepri(const int id);
bool hasexepri(const int id);
bool isingroup(const int user,const int group);
int readpri(const int id);
void setpri(const int id,const int pri);
int loaduserinfo();
int writeuserinfo();
int finduser(const int userid);
int loadgroupinfo();
int writegroupinfo();
int findgroup(const int groupid);
int newgroup(const char *group);
int findgroup(const char *group);
int delgroup(const char *group);
int addtogroup(const int user,const int group);
int removefromgroup(int user,int group);
void startup();
void showusers();
void showgroups();
void su(char *str);
void whoami();
int newuser(const char *user);
int finduser(const char *user);
int deluser(const char *user);
int echo(const char *content,const int id,const int filesize,bool attach);
int echo(const char *content,const char *filename,const int filesize,bool attach);
int type(const int id);
int pwd();
int getsize(const int id);
int cat(const char *str);
int isdir(char *str);
void docommand(const char *cmdline);
int removesinglef(int id,int pare);
int kerrmdir(int id,int pare);
int rmdirid(int id);
int rmdirname(const char *str);
int cd(int i);
int cd(const char *str);
int ls(const char *str);
int showname(int id);
int showpath(int id);
int fffind(const char *t);
bool isdir(const int id);
int ls(int id);
int attachtofile(int id,f_data dat,int size);
int setupfolder(const int id,const int parent);
int setupfile(const int id);
bool fileexist(int dir,const unsigned char *fname);
bool fileexist(int dir,const char *fname);
bool fileexistex(int dir,const unsigned char *fname);
bool fileexistex(int dir,const char *fname);
void setsize(const int id,int size);
int newf(int dir,const unsigned char *fname);
int newf(int dir,const char *fname);
int delf(int dir,int inode);
int find(int dir,const unsigned char *str);
int find(int dir,const char *str);
int findex(int dir,const unsigned char *str);
int findex(int dir,const char *str);
int getowner(const int id);
void setowner(const int id,const int owner);
int readfolder(int dir,dir_list rtn,int *items);
int writefolder(const int dir,dir_list dat,const int items);
int writefile(const int id,const f_data dat,const int filesize);
int readfile(const int id,f_data rtn,int *filesize);
void switchtoinode(const inodestruct,inodedata);
void switchtostruct(const inodedata,inodestruct *);
void writeinodedata(const int,const inodedata);
void writeinode(const int,const inodestruct);
void readinodedata(const int,inodedata);
void readinode(const int,inodestruct *);
void getinodeattribute(const int,const unsigned int,int *);
void setinodeattribute(const int,const unsigned int,const int);
void getinodeplaces(const int,plarr);
void writeinodeplaces(const int,const plarr);
int count(unsigned char);
int freeinodespace();
int freediskspace();
int gethighest(const unsigned char);
int findfreeinode();
int findfreedisk();
bool isinodeoccupied(const int);
bool isdiskoccupied(const int);
void occupyinode(const int);
void occupydisk(const int);
void freeinode(const int);
void freedisk(const int);
int Now();
void cmod(int);
void PrintTime(int);
void PrintTime();
void clean(data);
void print(const data);
void copy(const data,data);
void aesinit(const data);
void aes(const data,data);
void md5(const data,data);
bool equal(const data,const data);
void xxor(const data,data);
void getkey(const int,data);
void chdata(const int,const data,data);
void get(const int,data);
void put(const int,const data);
void put(const int,const data,const bool);
bool checkdisk();
bool disktest();
void loadkey(data);
void createdisk();
bool aestest();
bool md5test();
bool pretest();
bool loaddisk();
void cleardisk();
void initenv();
#endif