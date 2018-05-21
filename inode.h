#pragma once

#include <vector>
#include "mytime.h"

using namespace std;

class INODE // 64 B
{
public:
    Time create_time;//INODE所属文件的创建时间
    int point_num;//存了多少个块
    int index;//
    vector<int> point;//INODE文件的内容存放地址

    INODE();
    INODE(Time tem,int p,int i);
    ~INODE();
    void put(int n,int k);//修改point的第n个为k
    void new_index(int n);//修改index
};