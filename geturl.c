#include<regex.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(){
	
   char *regstr = "<a[^>]*href=\"\\([^\"]\\+\\)\"[^>]*>\\([^<]\\+\\)</a>";
    regex_t reg;
    regcomp(&reg, regstr, 0);
    int fd = open("url.txt", O_RDONLY);
    int size = lseek(fd, 0, SEEK_END);
    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    int regnum = 3;
    regmatch_t match[regnum];
    char *p = data;
    while (regexec(&reg, p, regnum, match, 0) == 0) {
        int len1 = match[1].rm_eo - match[1].rm_so;
        char link[1024];
        strncpy(link, p + match[1].rm_so, len1);
        link[len1] = '\0';

        int len2 = match[2].rm_eo - match[2].rm_so;
        char title[1024];
        strncpy(title, p + match[2].rm_so, len2);
        title[len2] = '\0';

        printf("title=%s  link=%s\n", title, link);

       
        p += match[0].rm_eo;
    }
    munmap(data, size);
    regfree(&reg);
    return 0;



}
