#include "factory.h"
//处理各种数据库操作，各种命令的处理函数

int Compute_file_md5(char *file_path, char *md5_str);

//得到MD5值
int GetMd5Data(char *file_name,char *md5_str)
{
	int ret;
	char file_path[30] ="./";
    strcat(file_path,file_name);
 
	ret = Compute_file_md5(file_path, md5_str);
    ERROR_CHECK(ret,-1,"compute md5");
	return 0;
}

int Makesalt(char *str)
{
    int i,flag;
    str[0] = '$';
    str[1] = '6';
    str[2] = '$';
    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。 for(i = 0; i < STR_LEN; i ++)
    for(i = 3; i < 11; i++)
    {
        flag = rand()%3; switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a';
            break;
        case 1:
            str[i] = rand()%26 + 'A';
            break; 
        case 2:
            str[i] = rand()%10 + '0';
            break;
        }
    }
    return 0; 
}

//打开数据库
int mySqlInit(MYSQL **conn,char *query)
{
    char* server="localhost";//需要在调用函数前准备好数据库三件套，query填入要执行的mysql语句，conn为传入传出 参数返回执行后的值
    char* user="root";//若使用了MYSQL_RES 需要记得free_result 和closs conn
    char* password="peng1997601";
    char* database="net_disk";//要访问的数据库名称
    int t;
    *conn=mysql_init(NULL);
    if(!mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(*conn));
        return -1;
    }
    t=mysql_query(*conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(*conn));
        mysql_close(*conn);
        return -1;
    }
    else
    {
        return 0;
    }
}

//登陆记录
int userlogin_note(char *username)
{
    MYSQL *conn;
    char query[300];
    sprintf(query,"insert into userlogin(ID,Username,loginTime) value (0,'%s',now());",username) ;
    int ret = mySqlInit(&conn,query);
    if(!ret)
        mysql_close(conn);
    else{
        return 0;
    }
}

int usercmd_note(char *username,char *usercmd,char *cmdpath)
{
    MYSQL *conn;
    char query[300]={0};
    sprintf(query,"insert into usercmd (ID,username,usercmd,cmdpath,cmdtime) VALUE (0,'%s','%s','%s',now());",username,usercmd,cmdpath);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        printf("cmd note writed\n");
        mysql_close(conn);
    }else{
        return 0;
    }
}

int find_ID(char *username,int *pID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select * from userlist;" ;
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {	
                if(strcmp(row[1],username)==0)//匹配用户名成功,则取ID  
                {
                    *pID=atoi(row[0]);
                    break;   
                }
            }
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    else{
        return -1;
    }
}

int find_ID2(char *username,int *pID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    bzero(query,sizeof(query));
    sprintf(query,"select * from userlist where username='%s';",username);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {	
                *pID=atoi(row[0]);
                break;   
            }
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    else{
        return -1;
    }
}

//判断用户名是否存在，匹配成功获取salt
int checkAddress(char *usrA,char *salt)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select * from userlist;" ;
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {	
                //printf("num=%d\n",mysql_num_fields(res));//列数
                if(strcmp(row[1],usrA)==0)//匹配用户名成功,则取盐值  
                {
                    strcpy(salt,row[2]);
                    break;   
                }
            }

            if(row == NULL)//没匹配到
            {           
                printf("none of this usr Account\n");
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    else{
        return -1;
    }
}

int checkPwd(char *usrA,char *pwd)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]={0};
    sprintf(query,"select * from userlist where Username='%s';",usrA);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            char a[200]={0};
            char b[200]={0};
            sprintf(a,"%s",row[3]);//row[3]里放的就是该用户名加密后的密码
            sprintf(b,"%s",pwd);
            if(strcmp(a,b)==0)//匹配密码成功 返回0  
            {
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else 
            {
                printf("login failed\n");
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }
        }
    }
}

//用户名查重,没有重复返回0，重复返回-1
int check_duplicates(char* usrA)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select * from userlist;" ;
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    //遍历每一行
    while((row=mysql_fetch_row(res))!=NULL)
    {	
        if(strcmp(row[1],usrA)==0)//用户名已存在
        {           
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}

//文件夹名查重，不重复返回0，重复返回-1
int check_dirname_duplicates(char *dirname,int precode,int belongID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where precode=%d and belongID=%d and filename='%s';",precode,belongID,dirname);
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))!=NULL)//找到了，既有同名文件夹
    {
        mysql_free_result(res);
        mysql_close(conn);
        return -1;
    }
    //没找到，既没有同名文件夹
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}

//插入新用户信息
int new_user_insert(char* Username,char* salt,char* UserPasswd)
{
    MYSQL *conn;
    char query[300]={0};
    sprintf(query,"insert into userlist(ID,Username,salt,UserPasswd) value (0,'%s','%s','%s');",Username,salt,UserPasswd);
    //printf("%s\n",query);
    int ret = mySqlInit(&conn,query); 
    mysql_close(conn);
    if(!ret)
    {
	    return 0;
    }
    else
    {
        return -1;
    }
}

int send_in_path(int new_fd,int code,int belongID)
{
    train_state_t train;
    char row_buf[300];
    char row_buf0[100];//precode
    char row_buf1[100];//code
    char row_buf2[100];//filename
    char row_buf3[100];//belongID
    char row_buf4[100];//filetype
    char row_buf5[200];
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where precode=%d and belongID=%d;",code,belongID);
    int ret= mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {
                bzero(row_buf5,sizeof(row_buf5));
                sprintf(row_buf5,"%s\n",row[5]);
                bzero(row_buf0,sizeof(row_buf0));
                strcpy(row_buf0,row[0]);//precode
                bzero(row_buf1,sizeof(row_buf1));
                strcpy(row_buf1,row[1]);//code
                bzero(row_buf2,sizeof(row_buf2));
                strcpy(row_buf2,row[2]);//filename
                bzero(row_buf3,sizeof(row_buf3));
                strcpy(row_buf3,row[3]);//belongID
                bzero(row_buf4,sizeof(row_buf4));
                strcpy(row_buf4,row[4]);//filetype
                // bzero(row_buf0,sizeof(row_buf0));
                // memcpy(row_buf0,row[0],100);//precode
                // bzero(row_buf1,sizeof(row_buf1));
                // memcpy(row_buf1,row[1],100);//code
                // bzero(row_buf2,sizeof(row_buf2));
                // memcpy(row_buf2,row[2],100);//filename
                // bzero(row_buf3,sizeof(row_buf3));
                // memcpy(row_buf3,row[3],100);//belongID
                // bzero(row_buf4,sizeof(row_buf4));
                // memcpy(row_buf4,row[4],100);//filetype
                // bzero(row_buf5,sizeof(row_buf5));
                // memcpy(row_buf5,row[5],200);//md5
                sprintf(row_buf,"%-10s %-10s %-20s %-10s %-10s %-100s\n",row_buf0,row_buf1,row_buf2,row_buf3,row_buf4,row_buf5); 
                set_train(&train,row_buf);
                send_train(new_fd,train);
            }
            //到头了就发个空火车
            if(row == NULL)//没匹配到
            {           
                send_end_train(new_fd);
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }
        }
        mysql_free_result(res);
        mysql_close(conn);
        return 0;    
    }
    else
    {
        return -1;
    }
}

int find_file_code(char *file_name,int belongID,int precode)
{
    int code;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where filename='%s' and belongID=%d and filetype='d' and precode=%d ;",file_name,belongID,precode);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(row==NULL)//没匹配到
            {
                mysql_free_result(res);
                mysql_close(conn);
                return -1;
            }else{
                code=atoi(row[1]);
                mysql_free_result(res);
                mysql_close(conn);
                return code;
            }
        }
    }
}

int find_precode(int code)
{
    long precode;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where code=%d;",code);
    int ret=mySqlInit(&conn,query);
    if(!ret)
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            char buf[1000];
            bzero(buf,sizeof(buf));
            strcpy(buf,row[0]);
            precode=atoi(buf);
            mysql_free_result(res);
            mysql_close(conn);
            return precode;
        }
    }
    else
    {
        return -1;
    }
}

//插入新文件夹名
insert_new_dir(int precode,char *dirname,int belongID)
{
    MYSQL *conn;
    char query[300];
    sprintf(query,"insert into filelist(precode,code,filename,belongID,filetype) value (%d,0,'%s',%d,'d');",precode,dirname,belongID);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        mysql_close(conn);
    }
    else{
        return 0;
    }
}

//找是否有目标文件夹
int find_target_dir(int precode,char* dirname,int belongID,long* targetcode)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where precode=%d and belongID=%d and filename='%s' and filetype='d';",precode,belongID,dirname);
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))==NULL)
    {
        mysql_free_result(res);
        mysql_close(conn);
        return -1;
    }
    *targetcode=atoi(row[1]);
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}

//被remove的文件linknum减1
void linknum_reduce(char *filemd5)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"update filepool set linknum=linknum-1 where filemd5='%s';",filemd5);
    //printf("%s\n",query);
    int ret = mySqlInit(&conn,query);
    mysql_close(conn);
}

//找是否有目标文件，有就取md5值
int find_target_file(int precode,char* filename,int belongID,char* filemd5)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where precode=%d and belongID=%d and filename='%s' and filetype='f';",precode,belongID,filename);
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))==NULL)//不存在
    {
        mysql_free_result(res);
        mysql_close(conn);
        return -1;
    }
    strcpy(filemd5,row[5]);
    //strncpy(filemd5,row[5],sizeof(filemd5));
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}

//找文件夹内是否有子文件，有返回-1，没有返回0
int find_sons_file(int code,int belongID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filelist where precode=%d and belongID=%d;",code,belongID);
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))!=NULL)//找到
    {
        mysql_free_result(res);
        mysql_close(conn);
        return -1;
    }
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}

//删除目标文件夹
int delete_dir(int precode,char* dirname,int belongID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"delete from filelist where precode=%d and belongID=%d and filename='%s' and filetype='d';",precode,belongID,dirname);
    printf("%s\n",query);
    int ret = mySqlInit(&conn,query);
    mysql_close(conn);
    return 0;
}

//删除目标文件
int delete_file(int precode,char* filename,int belongID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"delete from filelist where precode=%d and belongID=%d and filename='%s' and filetype='f';",precode,belongID,filename);
    //printf("%s\n",query);
    int ret = mySqlInit(&conn,query);
    mysql_close(conn);
    return 0;
}

//删除文件的文件池表项
void delete_filepool(char *filemd5)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query1[300];
    char query2[300];
    sprintf(query1,"select * from filepool where filemd5='%s' and linknum=0;",filemd5);
    int ret = mySqlInit(&conn,query1);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))!=NULL)//找到
    {
        char deletepath[1000];
        sprintf(deletepath,"file_pool/%s",filemd5);
        remove(deletepath);
        printf("\nA file source has been deleted!!\n");
    }
    mysql_free_result(res);
    mysql_close(conn);
    
    sprintf(query2,"delete from filepool where filemd5='%s' and linknum=0;",filemd5);
    //printf("%s\n",query2);
    ret = mySqlInit(&conn,query2);
    if(ret!=-1)
    {
        printf("\nA file_pool member delete.\n");
    }
    mysql_close(conn);
}

//根据tokrn得到username，如果salt是对的返回0，否则返回-1
int get_salt_to_username(char* username,char* salt)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from userlist where salt='%s';",salt);
    printf("%s\n",query);
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))!=NULL)//salt存在,获取username
    {
        strncpy(username,row[1],sizeof(username));
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    //不存在的话
    mysql_free_result(res);
    mysql_close(conn);
    return -1;
}

//查MD5是否存在，存在返回0，不存在返回-1
int check_md5_exist(char* md5)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"select * from filepool where filemd5='%s';",md5);
    printf("%s\n",query);
    int ret = mySqlInit(&conn,query);
    res=mysql_use_result(conn);
    if((row=mysql_fetch_row(res))!=NULL)//MD5存在，返回0
    {
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }
    //不存在的话
    mysql_free_result(res);
    mysql_close(conn);
    return -1;
}

//插入文件池项
void insert_filepool(char* md5)
{
    MYSQL *conn;
    char query[300];
    sprintf(query,"insert into filepool (ID,filemd5,linknum) value (0,'%s',1);",md5);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        mysql_close(conn);
    }
}

//文件linknum加1
void linknum_add(char* filemd5)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"update filepool set linknum=linknum+1 where filemd5='%s';",filemd5);
        //printf("%s\n",query);
    int ret = mySqlInit(&conn,query);
    mysql_close(conn);
}

//插入一个文件项
void insert_file(int precode,char* filename,int belongID,char* md5)
{
    MYSQL *conn;
    char query[300];
    sprintf(query,"insert into filelist(precode,code,filename,belongID,filetype,md5sum) value (%d,0,'%s',%d,'f','%s');",precode,filename,belongID,md5);
    int ret = mySqlInit(&conn,query);
    if(!ret)
    {
        mysql_close(conn);
    }
}