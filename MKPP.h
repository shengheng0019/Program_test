#pragma once
#include <vector>


using namespace std;
#define EPS 1e-3


struct ITEM
{
	int i;
	int w;
	int p;

};

struct QMKP
{
	//背包容量
	vector<int> C;
	vector<ITEM> I;
	int knap_size;
	int item_size;
	vector<vector<int>> Pij;
};

extern QMKP inst; //声明外部变量，使得这个变量在当前文件中可用，该变量是在别的文件中定义的