#include "bitmap.h"

Bitmap::Bitmap(int n=0)
{
	Bitmap_size = n;
    for(int i=0;i<n;i++)
    {
        data.push_back(0);
    }
}

Bitmap::~Bitmap(){}

void Bitmap::set(int i)
{
    if(i>=Bitmap_size)
    {
        Bitmap_size = i;
        for(int j=Bitmap;j<=i;j++)
        {
            if(j==i)data.push_back(1);
            else data.push_back(0);
        }
    }
    else data[i>>SHIFT] |=  (1<<(i & MASK)); 
}

void Bitmap::clr(int i)
{
    if(Bitmap_size<=i)cout<<"clr ERROR."<<endl;
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
        cout<<"test ERROR."<<endl;
        return -1;
    }
    else return data[i>>SHIFT] & (1<<(i & MASK));
}
