#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <cmath>
#include "mytime.h"
#define INFO_NUM 17
using namespace std;

const int BLOCK_SIZE = 1024;//一个块的大小为 1024 Byte = 1 KB
const int BLOCK_NUM = 1024;//1024 * 1024 Byte = 1 MB
const int SUPERBLOCK_ADD = 0;// superblock 从第 0 块开始
const int SUPERBLOCK_SIZE = 1;
const int INODE_BITMAP_ADD = 1;//inode bitmap 从第 1 块开始
const int INODE_BITMAP_SIZE = 1;
const int BLOCK_BITMAP_ADD = 2;//block bitmap 从第 2 块开始
const int BLOCK_BITMAP_SIZE = 1;
const int INODE_SIZE = 64;// 每个 INODE 大小为 64 Byte
const int INODE_NUM = 80;//INODE 最多 80 个
const int INODE_TABLE_ADD = 3;// INODE 从第 3 块开始
const int INODE_TABLE_SIZE = 5;// INODE 一共占用 5 个块
const int MAX_BLOCK_PER_FILE = 8;// 每个文件最多 8 个块
const int MAX_FILE_NAME = 8;
const int DIR_SIZE = 16;// DIR 的大小
const int DATA_BLOCK_ADD = 8;// 数据块从第 8 块开始
const int DATA_BLOCK_SIZE = 1012;
ofstream fout("disk");
void blockbitmap_initial()
{//0,1,2,3,4,5,6,7,8
	int k = 511;
	fout.seekp(fout.beg + BLOCK_SIZE * BLOCK_BITMAP_ADD );
	fout.write((char*)&k,sizeof(int));
}
int main()
{
	int info[INFO_NUM] = {1024,1024,0,1,1,1,2,1,64,80,3,5,8,8,16,8,1012};
	if(!fout)cout<<"Open File Error."<<endl;
	else
	{
		for(int i = 0;i<INFO_NUM;i++)
		{
			fout.write((char*)&info[i], sizeof(int));
		}
		for(int i=0;i<1024*1024/4-INFO_NUM;i++)
		{
			fout.write((char*)&info[2],sizeof(int));
		}
	}
	blockbitmap_initial();
	return 0;
}