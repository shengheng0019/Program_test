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
	//��������
	vector<int> C;
	vector<ITEM> I;
	int knap_size;
	int item_size;
	vector<vector<int>> Pij;
};

extern QMKP inst; //�����ⲿ������ʹ����������ڵ�ǰ�ļ��п��ã��ñ������ڱ���ļ��ж����