#include<process_copy.h>

int check_pram(int argc,const char* srcfile, int pronum){
	//实现代码
	if(argc<3)//./process_copy 1.png 2.png 10
	{
		printf("process_copy argv[]参数数量异常\n");
		exit(0);
	}
	if((access(srcfile,F_OK))!=0){
		printf("process_copy srcfile 不存在\n");
		exit(0);
	}
	if(pronum <=3||pronum>=100){
		printf("process_copy srcfile 进程数量异常\n");
		exit(0);
	}
	return 0;
}
