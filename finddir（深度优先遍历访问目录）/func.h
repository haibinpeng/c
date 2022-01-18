#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>

#define ARGS_CHECK(argc,num){if(argc!=num){printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,func_name){if(ret==retval){perror(func_name);return -1;}}
