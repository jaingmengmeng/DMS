#include "disk.h"

Disk::Disk() : disk_file("disk") 
{
	ifstream fin(disk_file);
	cout<<disk_file<<endl;
	BLKsize = 0;
	if(!fin) 
	{ 
		cout << "Disk initialization error!" << endl; return; 
	}
	char tmp = 0;
	cout<<sizeof(tmp)<<endl;
	for(int i = 0; i < 2; ++i) 
	{		// read the basic size of a block in the disk
		fin.read((char*)&tmp, sizeof(tmp));
		BLKsize += tmp << (8 * i);		// Little endian
	}
	cout<<BLKsize<<endl;
	fin.close();
}

Disk::~Disk() {
	;
}

bool Disk::GETABLOCK(char buf[], int block_num) 
{
	ifstream fin(disk_file);
	if(!fin) 
	{
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fin.seekg(fin.beg + BLKsize * block_num);
	for(int i = 0; i < BLKsize; ++i)
	{
		fin.read((char*)&buf[i], sizeof(char));
	}
	fin.close();
	return true;
}

boll Disk::GETPBLOCK(char buf[], int block_num)
{
	ifstream fin(disk_file);
	if(!fin) 
	{
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fin.seekg(fin.beg + BLKsize * block_num);
	for(int i = 0; i < sizeof(buf)/sizeof(char); ++i)
	{
		fin.read((char*)&buf[i], sizeof(char));
	}
	fin.close();
	return true;
}

boll Disk::GETPBLOCK(char buf[], int block_num ,int l)
{
	ifstream fin(disk_file);
	if(!fin) 
	{
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fin.seekg(fin.beg + BLKsize * block_num);
	for(int i = 0; i < l; ++i)
	{
		fin.read((char*)&buf[i], sizeof(char));
	}
	fin.close();
	return true;
}

bool Disk::PUTABLOCK(char buf[], int block_num) 
{
	ofstream fout(disk_file, ios::out | ios::in);
	if(!fout) 
	{	// cannot open file
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fout.seekp(fout.beg + BLKsize * block_num);
	for(int i = 0; i < BLKsize; ++i) 
	{
		fout.write((char*)&buf[i], sizeof(char));
	}
	fout.close();
	return true;
}

bool Disk::PUTABLOCK(char buf[], int block_num) 
{
	ofstream fout(disk_file, ios::out | ios::in);
	if(!fout) 
	{	// cannot open file
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fout.seekp(fout.beg + BLKsize * block_num);
	for(int i = 0; i < sizeof(buf)/sizeof(char); ++i) 
	{
		fout.write((char*)&buf[i], sizeof(char));
	}
	fout.close();
	return true;
}

boll Disk::GETPBLOCK(char buf[], int block_num, int l)
{
	ifstream fin(disk_file);
	if(!fin) 
	{
		cout << "Not found diskfile..." << endl;
		return false;
	}
	fin.seekg(fin.beg + BLKsize * block_num);
	for(int i = 0; i < l; ++i)
	{
		fin.read((char*)&buf[i], sizeof(char));
	}
	fin.close();
	return true;
}
