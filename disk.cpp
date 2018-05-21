#include "disk.h"

Disk::Disk() : disk_file("disk") 
{
	ifstream fin(disk_file);
	blocksize = 0;
	if(!fin) 
	{ 
		cout << "Disk initialization error!" << endl; return; 
	}
	char tmp = 0;
	for(int i = 0; i < 4; ++i) 
	{
		fin.read((char*)&tmp, sizeof(tmp));
		blocksize += tmp << (8 * i);
	}
	fin.close();
}

Disk::~Disk() {}

bool Disk::GETBLOCK(char buf[], int block_num) 
{
	ifstream fin(disk_file);
	if(!fin) 
	{
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fin.seekg(fin.beg + blocksize * block_num);
	for(int i = 0; i < blocksize; ++i)
	{
		fin.read((char*)&buf[i], sizeof(char));
	}
	fin.close();
	return true;
}

bool Disk::PUTBLOCK(char buf[], int block_num) 
{
	ofstream fout(disk_file, ios::out | ios::in);
	if(!fout) 
	{
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fout.seekp(fout.beg + blocksize * block_num);
	for(int i = 0; i < blocksize; ++i) 
	{
		fout.write((char*)&buf[i], sizeof(char));
	}
	fout.close();
	return true;
}