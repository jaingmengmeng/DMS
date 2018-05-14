#pragma once
#include <string>
#include <cstring>
#include <ctime>
#include "mytime.h"

class DIR
{
    private:
    string filename;  	             //储存文件名
   	int type; 						 //文件或者文件夹的标志
    int nextsector;  				 //指向第一个块的地址
    int size;                        //DIR_ITEM的大小,单位Byte
    public:
    DIR(string,int,int,int);
    ~DIR();
};

class INODE
{
    private:
    Time create_time;//INODE所属文件的创建时间
    Time last_time;//INODE所属文件的最后访问时间
    vector<int> point;//INODE文件的内容存放地址
    int num;//ponit的数量
    int size;//INODE的大小
    public:
    INODE(int,int);
    ~INODE();
    void put(int,int);//修改point的第n个为k
};
