#pragma once

#include <string>
#include <cstring>
#include <ctime>
#include "mytime.h"
using namespace std;
class DIR  // 16 B
{
public:
    string filename;  	             //储存文件名
    int type; 					     //文件或者文件夹的志标,0表示未被占用，1表示目录，2表示普通文件
    int child;                       //指向它的INODE的Index

    DIR(string,int,int);
    DIR();
    ~DIR();
};