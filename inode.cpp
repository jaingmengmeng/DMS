#include "inode.h"

INODE::INODE(){}

INODE::INODE(Time tem,int n,int s)
{
	this->create_time = tem;
	this->point_num = n;
	this->index = s;
}

INODE::~INODE(){}

void INODE::put(int n,int k)
{
	this->point[n] = k;
	return;
}

void INODE::new_index(int n)
{
	index = n;
	return;
}