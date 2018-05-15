#include <fstream>
#include <string>
#include <iostream>

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
const int DIR_SIZE = 16;// DIR 的大小
const int DIR_NUM = 16;//每个 DIR 表中 DIR 项的数量,所以一个块可以放 4 个 DIR 表
const int DIR_TABLE_NUM = 16;// 一个有 16 个 DIR 表项，即最多有 16 个文件夹
const int DIR_TABLE_ADD = 8;
const int DIR_TABLE_SIZE = 4;
const int DATA_BLOCK_ADD = 12;// 数据块从第 12 块开始
const int DATA_BLOCK_SIZE = 1012;

int main()
{
	int info[20] = {1024,1024,0,1,1,1,2,1,64,80,3,5,8,16,16,16,8,4,12,1012,0};
	ofstream fout("disk");
	if(!fout)cout<<"Open File Error."<<endl;
	else
	{
		for(int i = 0;i<20;i++)
		{
			fout.write((char*)&info[i], sizeof(int));
		}
		for(int i=0;i<1024*1024/4-20;i++)
		{
			fout.write((char*)&info[20],sizeof(int));
		}
	}
	return 0;
}
