#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"
#include "disk.h"
#include "mytime.h"
#include "document_manager.h"

int BLOCK_SIZE;
int BLOCK_NUM;
int BLOCK_ADD;
int INODE_SIZE;
int INODE_NUM;
int INODE_ADD;
int MAX_BLOCK_PER_FILE;
int DIR_SIZE;
int DIR_NUM;
int DIR_ADD;
int SUPERBLOCK_ADD;
const int dir_op = 0;
const int ls_op = 1;
const int cd_op = 2;
const int mkdir_op = 3;
const int rmdir_op = 4;
const int mv_op = 5;
const int touch_op = 6;
const int rm_op = 7;
const int open_op = 8;
const int close_op = 9;
const int read_op = 10;
const int write_op = 11;
const int hault_op = 12;

int input();
void select();
void initial();
int main()
{
    Bitmap INODE_bitmap();
    Bitmap block_bitmap();
    vector<INODE> INODE_table;
    vector<DIR> DIR_table;
    initial();//从磁盘读入文件系统的信息。包括上面的那些全局变量并构建INODE_table,DIR_table,INODE_bitmap,block_bitmap;
    select();
    return 0;
}

void initial()
{
    ifstream fin.open("disk.txt");
}

int input()
{
    int k = -1;
    char op[16] = {0};
    memset(op,0,sizeof(op));
    printf(">>");
    scanf("%s",op);
    if(strcmp(op,"dir")==0)k = dir_op;
    else if(strcmp(op,"ls")==0)k = ls_op;
    else if(strcmp(op,"cd")==0)k = cd_op;
    else if(strcmp(op,"mkdir")==0)k = mkdir_op;
    else if(strcmp(op,"rmdir")==0)k = rmdir_op;
    else if(strcmp(op,"mv")==0)k = mv_op;
    else if(strcmp(op,"touch")==0)k = touch_op;
    else if(strcmp(op,"rm")==0)k = rm_op;
    else if(strcmp(op,"open")==0)k = open_op;
    else if(strcmp(op,"close")==0)k = close_op;
    else if(strcmp(op,"read")==0)k = read_op;
    else if(strcmp(op,"write")==0)k = write_op;
    else if(strcmp(op,"hault")==0)k = hault_op;
    return k;
}

void select()
{
    print_rule();
    while(true)
    {
        switch(input())
        {
            case dir_op:    dir();break;
            case ls_op:     ls();break;
            case cd_op:     scanf("%s",name);cd(name);break;
            case mkdir_op:  scanf("%s",name);mkdir(name);break;
            case rmdir_op:  scanf("%s",name);rmdir(name);break;
            case touch_op:  scanf("%s",name);touch(name);break;
            case rm_op:     scanf("%s",name);rm(name);break;
            case open_op:   scanf("%s",name);open(name);break;
            case close_op:  scanf("%s",name);close(name);break;
            case read_op:   scanf("%s",name);read(name);break;
            case write_op:  scanf("%s",name);write(name);break;
            case hault_op:  cout<<"Thank You!!"<<endl;return 0;
            default:        cout<<"Please enter the right operation."<<endl;break;
        }
    }
}
