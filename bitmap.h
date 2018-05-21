#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#define SHIFT 5
#define MASK 0x1F
using namespace std;
struct Bitmap
{
public:
    vector<int> data;
    int Bitmap_size;//bitmap 的大小，是指要映射的bit数

	Bitmap();
	Bitmap(int n);
    ~Bitmap();

    void set(int n);
    void clr(int n);
    void clear();
    int test(int n);
};