#pragma once

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class Disk {
private:
	const string disk_file;
	int BLKsize;		// size of a single block
public:
	Disk();
	~Disk();

	bool GETABLOCK(char buf[], int block_num);
	bool GETPBLOCK(char buf[], int block_num);
	bool GETPBLOCK(char buf[], int block_num, int l);
	bool PUTABLOCK(char buf[], int block_num);
	bool PUTPBLOCK(char buf[], int block_num);
	bool PUTPBLOCK(char buf[], int block_num, int l);
};
