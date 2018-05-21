#pragma once

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class Disk {
private:
	const string disk_file;
	int blocksize;
public:
	Disk();
	~Disk();

	bool GETBLOCK(char buf[], int block_num);
	bool PUTBLOCK(char buf[], int block_num);
};