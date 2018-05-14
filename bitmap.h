#pragma once
#include <string>
#include <vector>
#define SHIFT 5
#define MASK 0x1F

struct Bitmap
{
private:
    vector<int> data;
    int Bitmap_size;
public:
    void set(int);
    void clr(int);
    void clear();
    int test(int);
    Bitmap(int);
    ~Bitmap();
};
