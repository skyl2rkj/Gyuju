#include<process_copy.h>
void process_wait(void)
{
	pid_t zpid;
	while((zpid=wait(NULL))>0){
		printf("parent wait child success,zpid %d\n",zpid);

}
}
