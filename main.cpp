#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <vector>
#include <windows.h>
#include "bitmap.h"
#include "disk.h"
#include "mytime.h"
#include "inode.h"
#include "dir.h"
const int info_num = 17;//默认磁盘superblock信息量
int BLOCK_SIZE = 1024;//一个块的大小为 1024 Byte = 1 KB
int BLOCK_NUM = 1024;//1024 * 1024 Byte = 1 MB
int SUPERBLOCK_ADD = 0;// superblock 从第 0 块开始
int SUPERBLOCK_SIZE = 1;
int INODE_BITMAP_ADD = 1;//inode bitmap 从第 1 块开始
int INODE_BITMAP_SIZE = 1;
int BLOCK_BITMAP_ADD = 2;//block bitmap 从第 2 块开始
int BLOCK_BITMAP_SIZE = 1;
int INODE_SIZE = 64;// 每个 INODE 大小为 64 Byte
int INODE_NUM = 80;//INODE 最多 80 个
int INODE_TABLE_ADD = 3;// INODE 从第 3 块开始
int INODE_TABLE_SIZE = 5;// INODE 一共占用 5 个块
int MAX_BLOCK_PER_FILE = 8;// 每个文件最多 8 个块
int MAX_FILE_NAME = 8;
int DIR_SIZE = 16;// DIR 的大小
int DATA_BLOCK_ADD = 8;// 数据块从第 8 块开始
int DATA_BLOCK_SIZE = 1012;

Bitmap INODE_bitmap(INODE_NUM);//构建INOCDE_bitmap。
Bitmap BLOCK_bitmap(BLOCK_NUM);//构建BLOCK_bitmap。
vector<INODE> INODE_table;//将INODE_TABLE保存在内存里，这样就不用每次读取时临时从磁盘加载。当INODE_table被修改时，将修改的INODE_table更新到磁盘。
vector<DIR> DIR_table;//DIR_TABLE一般存放的是当前目录的目录文件，每次使用时，需要从内存临时加载一整块目录文件。
vector<string> cur_rute;//保存的是当前目录的路径。
vector<string> rute;//用来记录制定的路径，可以是文件，也可以是文件夹
string con;
string name;
Disk mydisk;//disk类，每次与磁盘进行文件交互时都是按块大小进行的。
const string root_str = "Root";//默认的根目录的文件名。
const string file_name = "disk";//默认的模拟磁盘文件是 “ disk ” 。
char buf[1024] = {0};//用来保存一个磁盘块的内容。
int info[info_num]={0};

const int dir_op = 0;
const int ls_op = 1;
const int cd_op = 2;
const int mkdir_op = 3;
const int rmdir_op = 4;
const int touch_op = 5;
const int rmfile_op = 6;
const int move_op = 7;
const int write_op = 8;
const int cat_op = 9;
const int help_op = 10;
const int hault_op = 11;
const int cls_op = 12;
const int copy_op = 13;
const int inode_op = 14;
const int block_op = 15;
const int dire_op = 16;

void dir();
void ls();
void cd();
void mkdir();
void rmdir(vector<string>);
void touch();
void rmfile();
void move(vector<string>);
void write();
void cat();
void help();
void cls();
void copy();

vector<DIR> get_DIR_table(char buf[]);//给定一个磁盘块的字符串，将它转化成DIR_table
int find_DIR_in_DIR_table(vector<DIR>,string,int);
//给定一个DIRtable，以及文件名string，以及文件形式（1文件夹,2普通文件），返回它的INODE索引
int find_free_block();//找到空闲的块
int find_free_INODE();//找到空闲的INODE
void get_superblock();//初始化supperblock
void get_INODE_bitmap();//初始化INODE_bitmap
void get_BLOCK_bitmap();//初始化BLOCK_bitmap
void get_INODE_table();//初始化INODE_table
int get_INODE_index(vector<string> tem_rute);
int get_DIR_index(vector<string>);//给定一个路径，返回更改路径的所指目录的目录文件存储的磁盘快编号
void print_rute(vector<string>);//给定一个路径，按格式打印路径
vector<string> string_2_rute(string);//将string类型转化成路径
void write_DIR(int,int,DIR);//将DIR写入磁盘
void write_INODE(int,INODE);//将INODE写入磁盘
void write_bitmap(Bitmap,int);//将bitmap写入磁盘
void write_DATA(string,int);//将数据写入磁盘
void print_DIR_table();
void print_INODE_bitmap();
void print_BLOCK_bitmap();
void print_INODE_table();
void print_INODE(INODE);
void print_superblock();
void d_delete(int k);
void f_delete(int k);

int input();
void select();
void initial();//doing


int main()
{
    system("cls");
    cur_rute.push_back(root_str);
    initial();//从磁盘读入文件系统的信息。包括上面的那些全局变量并构建INODE_table,DIR_table,INODE_bitmap,block_bitmap;
    select();//输入op及相关参数，解析成命令并执行
    return 0;
}




int input()//还要考虑输入参数不够的情况
{
    int k = -1;
    int t = 0;
    string op;
    string str[3];
    print_rute(cur_rute);
    getline(cin,op);
    size_t last = 0;
    size_t found = op.find_first_of(" ");
    while(found != string::npos&&t<2)
    {
        str[t] = op.substr(last,found-last);
        op[found] = '*';
        last = found + 1;
        found = op.find_first_of(" ");
        t++;
    }
    str[t] = op.substr(last);
    /*for(int i=0;i<3;i++)
        cout<<str[i]<<"&";
    cout<<endl;*/
    if(str[0] == "dir")k = dir_op;
    else if(str[0] == "ls")k = ls_op;
    else if(str[0] == "cd")k = cd_op;
    else if(str[0] == "mkdir")k = mkdir_op;
    else if(str[0] == "rmdir")k = rmdir_op;
    else if(str[0] == "touch")k = touch_op;
    else if(str[0] == "rmfile")k = rmfile_op;
    else if(str[0] == "move")k = move_op;
    else if(str[0] == "write")k = write_op;
    else if(str[0] == "cat")k = cat_op;
    else if(str[0] == "help")k = help_op;
    else if(str[0] == "cls")k = cls_op;
    else if(str[0] == "copy")k = copy_op;
    else if(str[0] == "hault")k = hault_op;
    else if(str[0] == "quit")k = hault_op;
    else if(str[0] == "inode")k = inode_op;
    else if(str[0] == "block")k = block_op;
    else if(str[0] == "dire")k = dire_op;
    rute = string_2_rute(str[1]);
    con = str[2];
    /*cout<<str[0]<<endl;
    cout<<str[1]<<endl;
    cout<<con<<endl;*/
    //print_rute();
    return k;
}

void print_rule()
{   
    cout<<"******************************JW-DMS*************************************"<<endl
        <<"[dir]        List directory not distinguishing between files and folders."<<endl
        <<"[ls]         List directory distinguishing betwwen files and folders."<<endl
        <<"[cd]         Switch to the working directory."<<endl
        <<"[mkdir]      Create new folders under the specified directory." << endl
        <<"[rmdir]      Delete folders form the specified directory." << endl
        <<"[touch]      Create files under the specified directory." << endl
        <<"[rmfile]     Delete files from the specified directory." << endl
        <<"[move]       Move files or folders." << endl
        <<"[write]      Wirte files" << endl
        <<"[cat]        View the contents of the specified file." << endl
        <<"[help]       Print the helping menu." << endl
        <<"[cls]        Clear screen." << endl
        <<"[copy]       Copy the specified file."<<endl
        <<"[hault/quit] Exit from the current file system."<<endl
        <<"*************************************************************************"<<endl;
}

void select()
{
	vector<string> dirc;
    //print_rule();
    while(true)
    {
        switch(input())
        {
            case dir_op:    dir();break;
            case ls_op:     ls();break;
            case cd_op:     cd();break;
            case mkdir_op:  name = rute[rute.size()-1];rute.pop_back();mkdir();break;
            case rmdir_op:  rmdir(rute);break;
            case touch_op:  name = rute[rute.size()-1];rute.pop_back();touch();break;
            case rmfile_op: name = rute[rute.size()-1];rute.pop_back();rmfile();break;
            case write_op:  name = rute[rute.size()-1];rute.pop_back();write();break;
            case cat_op:    name = rute[rute.size()-1];rute.pop_back();cat();break;
            case help_op:   print_rule();break;
            case cls_op:    system("cls");break;
            case copy_op:   name = rute[rute.size()-1];rute.pop_back();copy();break;
            case inode_op:  print_INODE_bitmap();break;
            case block_op:  print_BLOCK_bitmap();break;  
            case dire_op:   print_DIR_table();break;
            case hault_op:  cout<<"Thank You!!"<<endl;return;
            case move_op:   dirc = string_2_rute(con);name = rute[rute.size()-1];rute.pop_back();con = dirc[dirc.size()-1];dirc.pop_back();move(dirc);break;
            default: 		cout<<"Please enter the right operation."<<endl;break;
        }
    }
}
void initial() { //initial the INODE_TABLE
    get_superblock();
    get_INODE_bitmap();
    get_BLOCK_bitmap();
    get_INODE_table();
    //print_INODE_bitmap();
    //print_BLOCK_bitmap();
    //print_INODE_table();
    //print_superblock();
}

void get_superblock() {
    //cout<<"superblock"<<endl;
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,0);//get the supperblock
    for(int i=0; i<info_num; i++) {//get 20 integers store the information of the DMS
        for(int j=0; j<sizeof(int); j++) {
            if(buf[sizeof(int)*i+j]>0)info[i] += buf[sizeof(int)*i+j]<<(j*8);
            else if(buf[sizeof(int)*i+j]<0 && j!=3)info[i] += (buf[sizeof(int)*i+j]+256)<<(j*8);
            else if(buf[sizeof(int)*i+j]<0 && j==3)info[i] =(-info[i])+(buf[sizeof(int)*i+j])<<(j*8);
        }
    }
    BLOCK_SIZE = info[0];
    BLOCK_NUM = info[1];
    SUPERBLOCK_ADD = info[2];
    SUPERBLOCK_SIZE = info[3];
    INODE_BITMAP_ADD = info[4];
    INODE_BITMAP_SIZE = info[5];
    BLOCK_BITMAP_ADD = info[6];
    BLOCK_BITMAP_SIZE = info[7];
    INODE_SIZE = info[8];
    INODE_NUM = info[9];
    INODE_TABLE_ADD = info[10];
    INODE_TABLE_SIZE = info[11];
    MAX_BLOCK_PER_FILE = info[12];
    MAX_FILE_NAME = info[13];
    DIR_SIZE = info[14];
    DATA_BLOCK_ADD = info[15];
    DATA_BLOCK_SIZE = info[16];
}

void get_INODE_bitmap() {
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,INODE_BITMAP_ADD);
    for(int i=0; i < ceil(INODE_BITMAP_SIZE/32.0); i++) {
        int temp = 0;
        for(int j=0; j<sizeof(int); j++) {
            if(buf[sizeof(int)*i+j]>0)temp+= buf[sizeof(int)*i+j]<<(j*8);
            else if(buf[sizeof(int)*i+j]<0 && j!=3)temp += (buf[sizeof(int)*i+j]+256)<<(j*8);
            else if(buf[sizeof(int)*i+j]<0 && j==3)temp =(-info[i])+(buf[sizeof(int)*i+j])<<(j*8);
        }
        INODE_bitmap.data[i] = temp;
    }
}

void get_BLOCK_bitmap() {
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,BLOCK_BITMAP_ADD);
    for(int i=0; i < ceil(BLOCK_BITMAP_SIZE/32.0); i++) {
        int temp = 0;
        for(int j=0; j<sizeof(int); j++) {
            if(buf[sizeof(int)*i+j]>=0)temp += buf[sizeof(int)*i+j]<<(j*8);
            else if(buf[sizeof(int)*i+j]<0 && j!=3)temp += (buf[sizeof(int)*i+j]+256)<<(j*8);
            else if(buf[sizeof(int)*i+j]<0 && j==3)temp =(-info[i])+(buf[sizeof(int)*i+j])<<(j*8);
        }
        BLOCK_bitmap.data[i] = temp;
    }
}

void get_INODE_table() {
    int tem[info_num] = {0};
    INODE_table.clear();
    //cout<<"get_INODE_table:"<<INODE_TABLE_ADD<<" "<<DATA_BLOCK_ADD<<endl;
    for(int i=INODE_TABLE_ADD;i<DATA_BLOCK_ADD;i++) {
        memset(buf,0,sizeof(buf));
        mydisk.GETBLOCK(buf,i);
        //cout<<"get_INODE_table:"<<BLOCK_SIZE<<" "<<INODE_SIZE<<endl;
        for(int j=0;j<BLOCK_SIZE/INODE_SIZE;j++) {
            for(int k = 0;k<16;k++) {
                int temp = 0;
                for(int m=0;m<4;m++) {
                    if(buf[INODE_SIZE*j+k*4+m]>=0)temp += buf[INODE_SIZE*j+k*4+m]<<(m*8);
                    else if(buf[INODE_SIZE*j+k*4+m]<0 && j!=3)temp += (buf[INODE_SIZE*j+k*4+m]+256)<<(m*8);
                    else if(buf[INODE_SIZE*j+k*4+m]<0 && j==3)temp =(-temp)+(buf[INODE_SIZE*j+k*4+m])<<(m*8);
                }
                tem[k] = temp;
                //cout<<temp<<" ";
            }
            //cout<<endl;
            Time tem_time(tem[0],tem[1],tem[2],tem[3],tem[4],tem[5]);
            INODE tem_INODE(tem_time,tem[6],tem[7]);
            for(int k=8;k<16;k++)
                tem_INODE.point.push_back(tem[k]);
            INODE_table.push_back(tem_INODE);
        }
    }
    return;
}

void print_DIR_table() {
    for(int i=0;i<DIR_table.size();i++)
    {
        cout<<DIR_table[i].filename<<" "<<DIR_table[i].type<<" "<<DIR_table[i].child<<endl;
    }
}
void print_INODE_bitmap() {
    cout<<"INODE_bitmap"<<endl;
    //for(int i=0;i<INODE_bitmap.data.size();i++)
    //    cout<<INODE_bitmap.data[i]<<" ";
    //cout<<endl;
    for(int i=0;i<INODE_bitmap.Bitmap_size;i++)
        if(INODE_bitmap.test(i)==1)cout<<i<<" ";
    cout<<endl;
}

void print_BLOCK_bitmap() {
    cout<<"BLOCK_bitmap"<<endl;
    //cout<<BLOCK_bitmap.Bitmap_size<<endl;
    //for(int i=0;i<BLOCK_bitmap.data.size();i++)
    //    cout<<BLOCK_bitmap.data[i]<<" ";
    //cout<<endl;
    for(int i=0;i<BLOCK_bitmap.Bitmap_size;i++)
        //cout<<BLOCK_bitmap.test(i)<<" ";
        if(BLOCK_bitmap.test(i)==1)cout<<i<<" ";
    cout<<endl;
}

void print_INODE(INODE tem_i) {
    //cout<<tem_i.create_time.year<<"/"<<tem_i.create_time.month<<"/"<<tem_i.create_time.day<<" "<<tem_i.create_time.hour<<":"<<tem_i.create_time.minute<<":"<<tem_i.create_time.second<<" ";
    tem_i.create_time.show_Time();
    cout<<" "<<tem_i.point_num<<" "<<tem_i.index<<endl;
    for(int i=0;i<tem_i.point.size();i++)
        if(tem_i.point[i])cout<<i<<":"<<tem_i.point[i]<<endl;
}

void print_INODE_table() {
    cout<<"INODE_table"<<endl;
    for(int i=0;i<INODE_table.size();i++) {
        if(INODE_table[i].create_time.year) {
            cout<<i<<" : ";
            print_INODE(INODE_table[i]);
        }
    }
}

void print_superblock()
{
    cout<<"superblock"<<endl
    <<BLOCK_SIZE<<endl
    <<BLOCK_NUM<<endl
    <<SUPERBLOCK_ADD<<endl
    <<SUPERBLOCK_SIZE<<endl
    <<INODE_BITMAP_ADD<<endl
    <<INODE_BITMAP_SIZE<<endl
    <<BLOCK_BITMAP_ADD<<endl
    <<BLOCK_BITMAP_SIZE<<endl
    <<INODE_SIZE<<endl
    <<INODE_NUM<<endl
    <<INODE_TABLE_ADD<<endl
    <<INODE_TABLE_SIZE<<endl
    <<MAX_BLOCK_PER_FILE<<endl
    <<MAX_FILE_NAME<<endl
    <<DIR_SIZE<<endl
    <<DATA_BLOCK_ADD<<endl
    <<DATA_BLOCK_SIZE<<endl;
}

void dir()
{
    int index = get_DIR_index(cur_rute);
    //cout<<"DIR index:"<<index<<endl;
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    //print_DIR_table();
    int sum = 0;
    for(int i = 0; i < DIR_table.size(); i++)
    {
        if(DIR_table[i].type != 0) {
            cout<<setw(10)<<DIR_table[i].filename;
            sum++;
        }
        if(sum%5==4)cout<<endl;
    }
    cout<<endl;
}

void ls()
{
    int index = get_DIR_index(cur_rute);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    int sum = 0;
    for(int i = 0; i < DIR_table.size(); i++)
    {
        if(DIR_table[i].type == 1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
            cout<<setw(10)<<DIR_table[i].filename;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED); 
            sum++;
        }
        else if(DIR_table[i].type == 2){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
            cout<<setw(10)<<DIR_table[i].filename;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED); 
        }
        if(sum%5==4)cout<<endl;
    }
    cout<<endl;
}

void cd()
{
    int index = get_DIR_index(rute);
    string select;
    if(index == -1)
    {
        cout<<"There is no directory named "<<rute[rute.size()-1]<<"."<<endl;
        /*    <<"Do you want to build a new directory on this rute? [ y / n ]"<<endl;
        getline(cin,select);
        size_t found = select.find_first_of("yY");
        //if(select[0] == 'y' || select[0] == 'Y')
        if(found != string::npos)
        {
            vector<string> tem;
            int i = 0;
            tem.push_back(root_str);
            int fd = DATA_BLOCK_ADD;//得到根目录文件的索引
            memset(buf,0,sizeof(buf));
            mydisk.GETBLOCK(buf,fd);//获取磁盘块内容
            DIR_table.clear();
            DIR_table = get_DIR_table(buf);//获得根目录的目录文件
            int index;
            i++;
            while(true) 
            {
                if(i>=tem_rute.size())break;
                index = find_DIR_in_DIR_table(DIR_table,tem_rute[i],0);//在根目录下找到级二级索引
                //cout<<"get_DIR_index:index:"<<index<<endl;
                if(index == -1)break;
                tem.push_back(rute[i]);
                fd = INODE_table[index].point[0];//得到目录文件索引
                //cout<<"get_DIR_index:fd:"<<fd<<endl;
                memset(buf,0,sizeof(buf));
                mydisk.GETBLOCK(buf,fd);//找到下一级目录的目录文件
                DIR_table.clear();
                DIR_table = get_DIR_table(buf);//将目录文件转成DIR_table
                i++;
            }
            for(int j = i;j<rute.size();j++)
            {
                tem.push_back(rute[j]);

            }
        }*/
    }
    else cur_rute = rute;
    return;
}

void mkdir()//在当前目录下加一项，新建一个INODE，新建一个DIR目录文件
{
    if(name.size()>MAX_FILE_NAME) {
        cout<<"Sorry!!! The filename is too long."<<endl;
        return;
    }
    else if(name.size()<MAX_FILE_NAME) {
        for(int i=name.size();i<MAX_FILE_NAME;i++)
            name.push_back(' ');
    }
    if(name == "        ")
    {
    	cout<<"There folder's name can't be empty."<<endl;
    	return;
    }
    int index = get_DIR_index(rute);
    //cout<<"DIR index:"<<index<<endl;
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    for(int i=0;i<DIR_table.size();i++) {//判断重名的情况
        if(DIR_table[i].filename == name && DIR_table[i].type == 1) {
            cout<<"The folder named "<<name<<" has already existed."<<endl;
            return;
        }
    }
    int fd = find_free_INODE();// free INODE ---> fd
    int ind = find_free_block();// free block ---> index
    if(fd == -1 || ind == -1)return;// 不满足条件时直接返回
    INODE_bitmap.set(fd);// 记录 bitmap
    DIR temp_d(name,1,fd);// 新建DIR项
    //cout<<"new INODE:"<<name<<"  INODE index:"<<fd<<"  BLOCK index:"<<ind<<endl;
    write_DIR(index,DIR_table.size(),temp_d);//write_DIR
    DIR_table.push_back(temp_d);
    Time c_time;
    c_time.set();
    INODE temp_i(c_time,1,fd);//新建INODE
    BLOCK_bitmap.set(ind);
    write_bitmap(BLOCK_bitmap,BLOCK_BITMAP_ADD);
    write_bitmap(INODE_bitmap,INODE_BITMAP_ADD);
    temp_i.point.push_back(ind);
    for(int i=0;i<MAX_BLOCK_PER_FILE-1;i++)
        temp_i.point.push_back(0);
    temp_i.point_num = 1;
    temp_i.index = fd;
    INODE_table[fd] = temp_i;//修改INODE_table
    write_INODE(fd,temp_i);//向磁盘写入INODE
}

void touch()//在当前目录下加一项，新建一个INODE
{
    if(name.size()>MAX_FILE_NAME) {
        cout<<"Sorry!!! The filename is too long."<<endl;
        return;
    }
    else if(name.size()<MAX_FILE_NAME) {
        for(int i=name.size();i<MAX_FILE_NAME;i++)
            name.push_back(' ');
    }
    if(name == "        ")
    {
    	cout<<"There file's name can't be empty."<<endl;
    	return;
    }
    int index = get_DIR_index(rute);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    for(int i=0;i<DIR_table.size();i++) {//判断重名的情况
        if(DIR_table[i].filename == name && DIR_table[i].type == 2) {
            cout<<"The dirtory named "<<name<<" has already existed."<<endl;
            return;
        }
    }
    int fd = find_free_INODE();// free INODE ---> fd
    if(fd == -1 )return;// 不满足条件时直接返回
    INODE_bitmap.set(fd);// 记录 bitmap
    DIR temp_d(name,2,fd);// 新建DIR项
    write_DIR(index,DIR_table.size(),temp_d);//write_DIR
    DIR_table.push_back(temp_d);
    Time c_time;
    c_time.set();
    INODE temp_i(c_time,0,fd);//新建INODE
    write_bitmap(BLOCK_bitmap,BLOCK_BITMAP_ADD);
    write_bitmap(INODE_bitmap,INODE_BITMAP_ADD);
    for(int i=0;i<MAX_BLOCK_PER_FILE;i++)
        temp_i.point.push_back(0);
    temp_i.point_num = 0;
    temp_i.index = fd;
    INODE_table[fd] = temp_i;//修改INODE_table
    write_INODE(fd,temp_i);//向磁盘写入INODE
}

void rmdir( vector<string> dir_rute)
{
    if(cur_rute == dir_rute) {
        cout<<"You can't remove the current directory."<<endl;
        return;
    }
    int fd = get_INODE_index(dir_rute);
    if(fd == -1){
        cout<<"There is no directory named "<<dir_rute[dir_rute.size()-1]<<endl;
        return;
    }
    INODE_bitmap.clr(fd);
    write_bitmap(INODE_bitmap,INODE_BITMAP_ADD);
    int index = get_DIR_index(dir_rute);
    BLOCK_bitmap.clr(index);
    write_bitmap(BLOCK_bitmap,BLOCK_BITMAP_ADD);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    for(int i=0;i<DIR_table.size();i++)
    {
        if(DIR_table[i].type == 1)
        {
            vector<string> tem_rute = dir_rute;
            tem_rute.push_back(DIR_table[i].filename);
            rmdir(tem_rute);
        }
        else if(DIR_table[i].type == 2)
        {
            f_delete(DIR_table[i].child);
        }
    }
    string wei = dir_rute[dir_rute.size()-1];//获得最后一个文件名
    dir_rute.pop_back();
    index = get_DIR_index(dir_rute);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    fd = find_DIR_in_DIR_table(DIR_table,wei,1);
    for(int j = fd;j<DIR_table.size()-1;j++)
    {
        DIR temp_d;
        temp_d = DIR_table[j+1];
        write_DIR(index,j,temp_d);//write_DIR
    }
    DIR temp_d("",0,0);
    write_DIR(index,DIR_table.size()-1,temp_d);
    DIR_table.erase(DIR_table.begin()+fd);
    //print_DIR_table();
    return;
}
void f_delete(int t)
{
    INODE_bitmap.clr(t);
    write_bitmap(INODE_bitmap,INODE_BITMAP_ADD);
    for(int i=0;i<INODE_table[t].point.size();i++)
    {
        if(INODE_table[t].point[i]!=0)BLOCK_bitmap.clr(i);
    }
    write_bitmap(BLOCK_bitmap,BLOCK_BITMAP_ADD);
}
void rmfile()
{
    if(name.size()<MAX_FILE_NAME) {
        for(int i=name.size();i<MAX_FILE_NAME;i++)
            name.push_back(' ');
    }
    int index = get_DIR_index(rute);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    for(int i=0;i<DIR_table.size();i++)
    {
        if(DIR_table[i].filename == name && DIR_table[i].type == 2)
        {
            for(int j = i;j<DIR_table.size()-1;j++)
            {
                DIR temp_d;
                temp_d = DIR_table[j+1];
                write_DIR(index,j,temp_d);//write_DIR
            }
            DIR temp_d("",0,0);
            write_DIR(index,DIR_table.size()-1,temp_d);
            DIR_table.erase(DIR_table.begin()+i);
            //print_DIR_table();
            f_delete(DIR_table[i].child);//给定文件的INODE索引，删除文件的所有块
            return;
        }
    }
    cout<<"There is no file named "<<name<<"."<<endl;
    return;
}


void copy()
{
    char str;
    ifstream fin(con);
    con = "";
    if(!fin)cout<<"OPEN FILE ERROR."<<endl;
    else {
        while(fin>>str) {
            con.push_back(str);
        }
        write();
    }
}

void move(vector<string> dirc)
{
    int index = get_DIR_index(rute);
    int ind = get_DIR_index(dirc);
    if(index == -1)
    {
    	cout<<"There is no folder named "<<rute[rute.size()-1]<<endl;
    	return;
    }
    if(ind == -1)
    {
    	cout<<"There is no folder named "<<dirc[dirc.size()-1]<<endl;
    	return;
    }
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    int fd = find_DIR_in_DIR_table(DIR_table,name,0);
    if(fd == -1)
    {
    	cout<<"There is no item named "<<name<<" under the folder "<<rute[rute.size()-1]<<endl;
    	return;
    }
    DIR temp = DIR_table[fd];
    for(int j = fd;j<DIR_table.size()-1;j++)
    {
        DIR temp_d;
        temp_d = DIR_table[j+1];
        write_DIR(index,j,temp_d);//write_DIR
    }
    DIR temp_d("",0,0);
    write_DIR(index,DIR_table.size()-1,temp_d);
    DIR_table.erase(DIR_table.begin()+fd);

    if(con.size()<MAX_FILE_NAME)
    {
    	for(int i=con.size();i<MAX_FILE_NAME;i++)
    		con.push_back(' ');
    }
    if(con == "        ")
    {
    	cout<<"There item's name can't be empty."<<endl;
    	return;
    }
    temp.filename = con;
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,ind);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    int k = find_DIR_in_DIR_table(DIR_table,temp.filename,temp.type);
   	if(k = -1)write_DIR(ind,DIR_table.size(),temp);
   	else
   	{
   		if(temp.type == 1)
   			cout<<"There already existed a folder named "<<con<<endl;
   		else cout<<"There already existed a file named "<<con<<endl;
   	}
}

void write()
{
	cout<<con<<endl;
    string filename = name;
    string str;
    if(filename.size()<MAX_FILE_NAME) {
        for(int i=filename.size();i<MAX_FILE_NAME;i++)
            filename.push_back(' ');
    }
    int index = get_DIR_index(rute);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    for(int i=0;i<DIR_table.size();i++)
    {
        if(DIR_table[i].filename == filename && DIR_table[i].type == 2)
        {
            int fd;
            int k = DIR_table[i].child;
            int t=0;
            for(int i=0;i<INODE_table[k].point.size();i++)
                INODE_table[k].point[i] = 0;
            while(con.size()>BLOCK_SIZE)
            {
                fd = find_free_block();
                if(fd == -1) 
                {
                	cout<<"There is no free block."<<endl;
                	return;
                }
                BLOCK_bitmap.set(fd);//设置bitmap
                str = con.substr(0,BLOCK_SIZE);
                con = con.substr(BLOCK_SIZE,con.size()-BLOCK_NUM);
                INODE_table[k].point[t] = fd;
                write_DATA(str,fd);
                t++;
            }
            fd = find_free_block();
            if(fd == -1)
            {
            	cout<<"There is no free block."<<endl;
            	return;
            }
            BLOCK_bitmap.set(fd);
            INODE_table[k].point[t] = fd;
            //cout<<fd<<" "<<con<<endl;
            write_DATA(con,fd);
            write_INODE(k,INODE_table[k]);
            return;
        }
    }
    cout<<"There is no file named "<<filename<<"."<<endl;
    return;
}

void cat()
{
    if(name.size()<MAX_FILE_NAME) {
        for(int i=name.size();i<MAX_FILE_NAME;i++)
            name.push_back(' ');
    }
    int index = get_DIR_index(rute);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//得到了当前目录的目录文件
    for(int i=0;i<DIR_table.size();i++)
    {
        if(DIR_table[i].filename == name && DIR_table[i].type == 2)
        {
            int k = DIR_table[i].child;
            //cout<<"cat:"<<k<<endl;
            int t = 0;
            //cout<<"cat:"<<INODE_table[k].point.size()<<endl;
            while(t<INODE_table[k].point.size()&& INODE_table[k].point[t]!=0)
            {
                //cout<<"cat:"<<t<<" "<<INODE_table[k].point[t]<<endl;
                memset(buf,0,sizeof(buf));
                mydisk.GETBLOCK(buf,INODE_table[k].point[t]);
                for(int j=0;j<sizeof(buf);j++)
                {
                    if(buf[j])cout<<buf[j];
                }
                cout<<endl;
                t++;
            }
            return;
        }
    }
    cout<<"There is no file named "<<name<<"."<<endl;
    return;
}


int find_DIR_in_DIR_table(vector<DIR> DIR_t,string directory,int type)  {//type = 0 表示不在意文件类型，1 文件夹，2 普通文件
    int res = -1;
    if(directory.size()<MAX_FILE_NAME)
        for(int i=directory.size();i<MAX_FILE_NAME;i++)
            directory.push_back(' ');
    for(int i=0;i<DIR_t.size();i++) {
        if(DIR_t[i].filename == directory&&(DIR_t[i].type == type | type == 0))return DIR_t[i].child;
    }
    return res;
}

int find_free_block()
{
    for(int i=DATA_BLOCK_ADD;i<BLOCK_NUM;i++)
    {
        int res = BLOCK_bitmap.test(i);
        if(res == 0)return i;
    }
    //cout<<"All blocks have already been occupied."<<endl;
    cout<<"There is no free block."<<endl;
    return -1;
}

int find_free_INODE()
{
    for(int i=0;i<INODE_NUM;i++)
    {
        int res = INODE_bitmap.test(i);
        if(res == 0)return i;
    }
    cout<<"All INODEs have already been occupied."<<endl;
    return -1;
}

vector<DIR> get_DIR_table(char buf[]) {//将磁盘块文件解析成DIR_table
    //cout<<buf<<endl;
    vector<DIR> DIR_t;//要返回的DIR_table
    char temp;
    string filename;
    int child = 0;
    int type = 0;
    for(int i=0;i<BLOCK_SIZE/DIR_SIZE;i++) {
        filename = "";
        child = 0;
        type = 0;
        for(int j=0;j<DIR_SIZE;j++) {
            int loc = i*DIR_SIZE+j;
            temp = buf[loc];
            if(j<8&&temp)filename.push_back(temp);
            else if(j<12)type += temp<<(8*(j-8));
            else child += temp<<(8*(j-12));
        }
        DIR tem(filename,type,child);
        if(type!=0)DIR_t.push_back(tem);
    }
    return DIR_t;
}

int get_INODE_index(vector<string> tem_rute) {//返回指定filefilename的INODE索引，路径可能是指向一个文件或一个目录
    string str = tem_rute[tem_rute.size()-1];//获得最后一个名字
    tem_rute.pop_back();
    int index = get_DIR_index(tem_rute);//获得这个文件的上一级目录的目录文件所在的磁盘块号
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);//获得上一级目录的目录文件
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//获得DIR_table
    int fd = find_DIR_in_DIR_table(DIR_table,str,0);//从DIR_table中找出制定的文件，0表示不在意文件类型
    return fd;
}

int get_DIR_index(vector<string> tem_rute) {//返回路径所指目录的索引（即返回其目录文件的块号）,保证是一个目录,不存在则返回-1
    int i = 0;
    int fd = DATA_BLOCK_ADD;//得到根目录文件的索引
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,fd);//获取磁盘块内容
    DIR_table.clear();
    DIR_table = get_DIR_table(buf);//获得根目录的目录文件
    int index;
    i++;
    while(true) {
        if(i>=tem_rute.size())break;
        index = find_DIR_in_DIR_table(DIR_table,tem_rute[i],0);//在根目录下找到级二级索引
        //cout<<"get_DIR_index:index:"<<index<<endl;
        if(index == -1)return -1;
        fd = INODE_table[index].point[0];//得到目录文件索引
        //cout<<"get_DIR_index:fd:"<<fd<<endl;
        memset(buf,0,sizeof(buf));
        mydisk.GETBLOCK(buf,fd);//找到下一级目录的目录文件
        DIR_table.clear();
        DIR_table = get_DIR_table(buf);//将目录文件转成DIR_table
        i++;
    }
    return fd;
}

void print_rute(vector<string> tem) {
    cout<<tem[0]<<":";
    for(int i=1;i<tem.size();i++)
        cout<<"/"<<tem[i];
    cout<<"> ";
}

vector<string> string_2_rute(string name) {//将string解析成路径
    while(true)//去除name后面的"/","\"
    {
        char ch = name[name.size()-1];
        if(name.size() == 1 && (ch == '\\' || ch == '/'))
        {
            rute.clear();
            rute.push_back(root_str);
            return rute;
        }
        else if(ch == '\\' || ch == '/')
        {
            name.pop_back();
        }
        else break;
    }
    vector<string> list;
    list = cur_rute;
    int pos = 0;
    size_t last = 0;
    size_t found = name.find_first_of("/\\");
    while(found != string::npos) {
        if(found == 0){
            list.clear();
            list.push_back(root_str);
        }
        else 
        {
            string str = name.substr(last,found-last);
            if(found-last)list.push_back(str);
            if(str == ".")list.erase(list.end());
            else if(str == "..") {
                list.erase(list.end());
                if(!list.empty())list.erase(list.end());
            }
            else if(str == root_str && last == 0 ) {
                list.clear();
                list.push_back(root_str);
            }
        }
        name[found] = '*';
        last = found+1;
        found = name.find_first_of("/\\");
    }
    string str = name.substr(last,name.size());
    list.push_back(str);
    if(str == ".")list.erase(list.end());
    else if(str == "..") {
        list.erase(list.end());
        if(list.size()>=2)list.erase(list.end());
    }
    else if(str == root_str && last == 0 ) {
        list.clear();
        list.push_back(root_str);
    }
    return list;
}

void write_DIR(int index,int k,DIR temp_d)//将目录项temp_d写入目录文件的k位置中,其中目录文件保存在index中
{
    //cout<<"write_DIR:"<<index<<" "<<k<<endl;
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,index);
    int base = k * DIR_SIZE;
    for(int i=0;i<temp_d.filename.size();i++)
    {
        buf[base+i] = temp_d.filename[i];
    }
    base += 8;
    for(int i=0;i<4;i++)
    {
        buf[base+i] = ((temp_d.type>>(i*8)) & 0xFF);
        //cout<<((temp_d.type>>(i*8)) & 0xFF)<<endl;
    }
    base += 4;
    for(int i=0;i<4;i++)
    {
        buf[base+i] = ((temp_d.child>>(i*8)) & 0xFF);
        //cout<<((temp_d.child>>(i*8)) & 0xFF)<<endl;
    }
    //cout<<"write_DIR:"<<buf<<endl;
    mydisk.PUTBLOCK(buf,index);
}

void write_INODE(int index,INODE temp_i) {//将INODE项temp_i写入INODE_table中，其中index是空位
    //cout<<"write_INODE:"<<endl;
    int k = index / (BLOCK_SIZE/INODE_SIZE);
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,k+INODE_TABLE_ADD);
    int pos = index % (BLOCK_SIZE/INODE_SIZE);
    int base = pos * INODE_SIZE;
    int tem[16]={0};
    tem[0]=temp_i.create_time.year;
    tem[1]=temp_i.create_time.month;
    tem[2]=temp_i.create_time.day;
    tem[3]=temp_i.create_time.hour;
    tem[4]=temp_i.create_time.minute;
    tem[5]=temp_i.create_time.second;
    tem[6]=temp_i.point_num;
    tem[7]=temp_i.index;
    tem[8]=temp_i.point[0];
    tem[9]=temp_i.point[1];
    tem[10]=temp_i.point[2];
    tem[11]=temp_i.point[3];
    tem[12]=temp_i.point[4];
    tem[13]=temp_i.point[5];
    tem[14]=temp_i.point[6];
    tem[15]=temp_i.point[7];
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<4;j++)
        {
            buf[base+i*4+j] = ((tem[i]>>(j*8)) & 0xFF);
        }
    }
    mydisk.PUTBLOCK(buf,k+INODE_TABLE_ADD);
}

void write_bitmap(Bitmap B,int k)//将更新后的Bitmap写入磁盘，其中k表示磁盘块的编号
{
    memset(buf,0,sizeof(buf));
    mydisk.GETBLOCK(buf,k);
    for(int i=0;i<B.data.size();i++)
    {
        for(int j=0;j<4;j++)
        {
            buf[i*4+j] = ((B.data[i]>>(j*8)) & 0xFF);
        }
    }
    mydisk.PUTBLOCK(buf,k);
}
void write_DATA(string data,int k)//把一段string类型放到k块磁盘中
{
    memset(buf,0,sizeof(buf));
    for(int i=0;i<data.size();i++)
    {
        buf[i] = data[i];
        //cout<<"write_DATA:"<<data[i];
    }
    mydisk.PUTBLOCK(buf,k);
}