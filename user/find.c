#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"


char *fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;
 
    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
 
    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    buf[strlen(p)] = '\0';
    return buf;
}

void 
find(char *path, char *target)
{
	char buf[512], *p;
  	int fd;
  	struct dirent de;
  	struct stat st;

  	if((fd = open(path, 0)) < 0){
    		fprintf(2, "find: cannot open %s\n", path);
    		return;
  	}

  	if(fstat(fd, &st) < 0){
    		fprintf(2, "find: cannot stat %s\n", path);
    		close(fd);
    		return;
  	}

  	switch(st.type){
  		case T_FILE:
			//printf("%s\n", fmtname(path));
			if(strcmp(fmtname(path), target) == 0)
			{
				//printf("1\n");
    				printf("%s\n", path);
			}
    			break;

  		case T_DIR:
    			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
     				 printf("find: path too long\n");
      				break;
    			}
    			strcpy(buf, path);
   			p = buf+strlen(buf);
    			*p++ = '/';
   			 while(read(fd, &de, sizeof(de)) == sizeof(de)){
     				 if((de.inum == 0) || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0))
        				continue;
				 memmove(p, de.name, DIRSIZ);
     				 p[DIRSIZ] = 0;
     				 if(stat(buf, &st) < 0){
       					 printf("ls: cannot stat %s\n", buf);
       					 continue;
      				}	
				find(buf, target);
    			}
    			break;
  	}
  	close(fd);
}
int
main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("usage: find dir file\n");
		exit(0);
	}
	find(argv[1], argv[2]);
	exit(0);
}
