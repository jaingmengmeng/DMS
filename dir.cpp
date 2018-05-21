#include "dir.h"
DIR::DIR(){}
DIR::DIR(string name,int tp,int next)
{
	filename = name;
	type = tp;
	child = next;
}
DIR::~DIR(){}