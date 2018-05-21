#include "mytime.h"

Time::Time(int y = 0,int m = 0,int d = 0,int h = 0,int mi = 0,int s = 0)
{
    year = y;
    month = m;
    day = d;
    hour = h;
    minute = mi;
    second = s;
}

void Time::set()
{
    time_t rawtime;
    time(&rawtime);
    struct  tm * timeinfo;
    timeinfo = localtime(&rawtime);

    year = timeinfo->tm_year+1900;
    month = timeinfo->tm_mon+1;
    day = timeinfo->tm_mday;
    hour = timeinfo->tm_hour;
    minute = timeinfo->tm_min;
    second = timeinfo->tm_sec;
}

void Time::show_Time()
{
    cout<<year<<"/"<<month<<"/"<<day<<" "<<hour<<":"<<minute<<":"<<second<<endl;
}