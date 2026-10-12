#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

int main(int argc, char** argv){
int sfd;
int dfd;
int offset=atoi(argv[3]);
int blocksize = atoi(argv[4]);
if((sfd= open(argv[1],O_RDONLY))==-1)
	perror("open srcfile, failed");
if((dfd=open(argv[2],O_WRONLY|O_CREAT,0664))==-1)
	perror("open srcfile, failed");
lseek(sfd,offset,SEEK_SET);
lseek(dfd,offset,SEEK_SET);

char buffer[blocksize];
int len;
printf("child_p %d execl copy,offset %d ,blocksize %d\n",getpid(),offset,blocksize);
//通过len返回值判定是否读取完毕，可能循环读取
len=read(sfd,buffer,sizeof(buffer));
write(dfd,buffer,len);
close(sfd); close(dfd);
return 0;
}

