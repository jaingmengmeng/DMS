#pragma once
#include <ctime>
#include <iostream>
using namespace std;

class Time
{
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
public:
    Time(int ,int ,int ,int ,int ,int);
    ~Time();
    set_Time();
    show_cur_Time();
    show_Time();
};
