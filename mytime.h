#pragma once

#include <ctime>
#include <iostream>

using namespace std;

class Time
{
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    
    Time(){}
    ~Time(){}
    Time(int a,int b,int c,int d,int e,int f);
    void set();
    void show_Time();
};