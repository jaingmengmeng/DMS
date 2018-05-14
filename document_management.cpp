#include "document_manager.h"

DIR::DIR(string name = nullptr,int tp = 0,int next = 0,int s = 0)
{
	filename = name;
	type = tp;
	nextsector = next;
	size = s;
}
DIR::~DIR(){}

/*DIR_TABLE::DIR_TABLE(int n = 0,int s = 0)
{
	num = n;
	size = s;
	for(int i=0;i<n;i++)
	{
		list.push_back(DIR(nullptr,0,0,0));
	}
}

DIR_TABLE::~DIR_TABLE(){}*/

INODE::INODE(int n,int s)
{
	create_Time.set_Time();
	last_Time.set_Time();
	num = n;
	size = s;
	for(int i=0;i<n;i++)
	{
		point.push_back(0);
	}
}

INODE::~INODE(){}

void INODE::put(int n,int k)
{
	point[n] = k;
}

/*INODE_TABLE::INODE_TABLE(int n = 0,int s = 0)
{
	num = 0;
	s = 0;
	for(int i=0;i<n;i++)
	{
		list.push_back(INODE(0,0));
	}
}

INODE_TABLE::~INODE_TABLE(){}*/
