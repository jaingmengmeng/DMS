#include "bitmap.h"

Bitmap::Bitmap()
{
    Bitmap_size = 0;
}

Bitmap::Bitmap(int n)
{
	Bitmap_size = n;
    for(int i=0;i<ceil(n/32.0);i++)
    {
        data.push_back(0);
    }
}

Bitmap::~Bitmap(){}

void Bitmap::set(int i)
{
    if(i>=Bitmap_size)
    {
        cout<<"ERROR."<<endl;
    }
    else data[i>>SHIFT] |=  (1<<(i & MASK)); 
}

void Bitmap::clr(int i)
{
    if(Bitmap_size<=i)cout<<"ERROR."<<endl;
    else data[i>>SHIFT] &= ~(1<<(i & MASK));
}

void Bitmap::clear()
{
    for(int i=0;i<Bitmap_size;i++)
    {
        data[i] = 0;
    }
}

int Bitmap::test(int i)
{
    if(Bitmap_size<=i)
    {
        cout<<"ERROR."<<endl;
        return -1;
    }
    else if(data[i>>SHIFT] & (1<<(i & MASK)))return 1;
    else return 0;
}