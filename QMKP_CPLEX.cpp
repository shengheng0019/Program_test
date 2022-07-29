// QMKP_CPLEX.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <time.h>
#include <string>
#include <algorithm>
#include <vector>
#include <ilcplex/ilocplex.h>
#include "mkpp.h"


using namespace std;

void ImportData(string file);
void SplitString(const string& s, vector<string>& v, const string& c);
void SolvebyCplex();
void SolvebyCplexTEST();
QMKP inst;

int main()
{
	clock_t start, end;
	start = clock();
	//string file = "SMALL/probT1_0U_R50_T002_M010_N0020_seed10.txt";
	//string file = "C:/Users/admin/Desktop/QMKPData.txt";
	//string file = "D:/科研之路/code/20220703测qmkp/QMKP_CPLEX/QMKP_benchmark/N100_M3_D25/N100_M3_D25_01.txt";
	//string file = "../QMKP_benchmark/N100_M3_D25/N100_M3_D25_01.txt";
	string file = "D:/Desktop/Instances/randomQMKP_30_50_1_1_4.txt"; 
	printf("读入算例\n");

	ImportData(file);//将inst赋值

	SolvebyCplex();
	//SolvebyCplexTEST();

	end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;

	printf("总用时：%f seconds\n", time);

    std::cout << "Hello World!\n";
}

//void ImportData(string file)
//{
//	ifstream inFile(file);
//	string str;
//	if (!inFile)
//	{
//		cout << " Data.txt doesn't exist! " << endl;
//	}
//	else
//	{
//		string flag = "\t";
//		//flag = " ";
//		getline(inFile, str);
//		inst.knap_size = stoi(str);
//		getline(inFile, str);
//		inst.item_size = stoi(str);
//
//
//		vector <string> mkpC;
//		vector <string> Pij;
//
//
//		for (int j = 0; j < inst.knap_size; j++)
//		{
//			getline(inFile, str);
//			inst.C.push_back(stoi(str));
//		}
//
//
//		for (int i = 0; i < inst.item_size; i++)
//		{
//			ITEM item;
//			getline(inFile, str);
//			SplitString(str, mkpC, flag);
//
//			/*for (int i = 0; i < str.size(); i++)
//			{
//				if (str.substr(i, 1) == " ")
//				{
//					mkpC[0] = str.substr(0, i);
//					break;
//				}
//			}
//
//			for (int i = mkpC[0].size(); i < str.size(); i++)
//			{
//				if (str.substr(i, 1) != " ")
//				{
//					mkpC[1] = str.substr(i, str.size());
//					break;
//				}
//			}*/
//
//			item.w = stoi(mkpC[0]);
//			item.p = stoi(mkpC[1]);
//			item.i = i;
//			inst.I.push_back(item);
//		}
//		vector<int> pij_unit = vector<int>(inst.item_size, 0);
//		inst.Pij = vector<vector<int>>(inst.item_size, pij_unit);
//
//		//Pij[0][1]代表第一个和第二个物品的value，Pij[t][t]没有意义。
//		for (size_t i = 0; i < inst.item_size - 1; i++)
//		{
//			getline(inFile, str);
//			SplitString(str, Pij, flag);
//			int idex_pij = 0;
//			for (size_t k = i + 1; k < inst.item_size; k++)
//			{
//				if (idex_pij == Pij.size())
//				{
//					std::cout << "P(ij)数据异常！" << endl;
//					cin.get();
//				}
//				inst.Pij[i][k] = stoi(Pij[idex_pij]);
//				idex_pij++;
//			}
//		}
//	}
//
//	inFile.close();
//}

void ImportData(string file)
{
	ifstream inFile(file);
	string str;
	if (!inFile)
	{
		cout << " Data.txt doesn't exist! " << endl;
	}
	else
	{
		getline(inFile, str);//数据集名字

		string flag = "\t";
		flag = " ";
		getline(inFile, str);
		inst.item_size = stoi(str);
		
		inst.knap_size = 5;

		//初始化
		vector <string> Pij;
		vector <string> Pij_1;
		vector<int> pij_unit = vector<int>(inst.item_size, 0);
		inst.Pij = vector<vector<int>>(inst.item_size, pij_unit);
		for (int i = 0; i < inst.item_size; i++)
		{
			ITEM item;
			item.w = 0;
			item.p = 0;
			item.i = i;
			inst.I.push_back(item);
		}

		vector<string> mkpP_1;
		vector<string> mkpP;
		getline(inFile, str);
		SplitString(str, mkpP_1, flag);
		for (size_t ii = 0; ii < mkpP_1.size(); ii++)//需要处理一下
		{
			if (mkpP_1[ii] != "")
			{
				mkpP.push_back(mkpP_1[ii]);
			}

		}
		for (size_t i = 0; i < inst.item_size; i++)
		{
			inst.I[i].p = stoi(mkpP[i]);
		}

		

		//Pij[0][1]代表第一个和第二个物品的value，Pij[t][t]没有意义。
		for (size_t i = 0; i < inst.item_size - 1; i++)
		{
			Pij.clear();
			Pij_1.clear();
			getline(inFile, str);
			SplitString(str, Pij_1, flag);
			for (size_t ii = 0; ii < Pij_1.size(); ii++)//需要处理一下
			{
				if (Pij_1[ii] != "")
				{
					Pij.push_back(Pij_1[ii]);
				}
			}
			int idex_pij = 0;
			for (size_t k = i + 1; k < inst.item_size; k++)
			{
				if (idex_pij == Pij.size())
				{
					std::cout << "P(ij)数据异常！" << endl;
					cin.get();
				}
				inst.Pij[i][k] = stoi(Pij[idex_pij]);
				idex_pij++;
			}
		}

		//空两行
		getline(inFile, str);
		getline(inFile, str);

		//背包容量
		getline(inFile, str);
		//for (size_t i = 0; i < inst.knap_size; i++)
		//{
		//	inst.C.push_back(stoi(str));
		//}

		int wei_total = 0;
		vector<string> mkpW;
		vector<string> mkpW_1;
		getline(inFile, str);
		SplitString(str, mkpW_1, flag);
		for (size_t ii = 0; ii < mkpW_1.size(); ii++)//需要处理一下
		{
			if (mkpW_1[ii] != "")
			{
				mkpW.push_back(mkpW_1[ii]);
			}
		}
		for (size_t i = 0; i < inst.item_size; i++)
		{
			inst.I[i].w = stoi(mkpW[i]);
			wei_total += inst.I[i].w;
		}


		//背包容量生成原则：
		//每个背包等容量，背包总容量为物品总重量的80%
		int knap_capa;
		knap_capa = wei_total * 0.8 / inst.knap_size;
		for (int j = 0; j < inst.knap_size; j++)
		{
			inst.C.push_back(knap_capa);
		}

	}

	inFile.close();
}
void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	v.clear();//删除原内容
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
void SolvebyCplex()
{
	typedef IloArray<IloIntVarArray> IntVarMatrix;
	IloEnv env;
	IloModel model(env);
	IntVarMatrix x(env);
	IloObjective obj(env);
	IloConstraintArray constraints(env);
	IloCplex solver(env);
	IloNumArray c(env);
	IloCplex cplex(model);

	c = IloNumArray(env, inst.knap_size);
	for (size_t i = 0; i < inst.knap_size; i++)
	{
		c[i] = inst.C[i];
	}

	//变量约束
	//IloIntVarArray x_unit = IloIntVarArray(env, inst.item_size, 0, 1);
	x = IloArray<IloIntVarArray>(env, inst.item_size);
	for (size_t i = 0; i < inst.item_size; i++)
	{
		x[i] = IloIntVarArray(env, inst.knap_size, 0, 1);
	}

	//目标函数
	IloExpr objfunc(env);
	for (size_t j = 0; j < inst.knap_size; j++)
	{
		for (size_t i = 0; i < inst.item_size; i++)
		{
			objfunc += inst.I[i].p * x[i][j] ;
		}
	}
	//目标函数二次项
	for (size_t i = 0; i < inst.item_size; i++)
	{
		for (size_t k = i + 1; k < inst.item_size; k++)
		{
			for (size_t j = 0; j < inst.knap_size; j++)
			{
				objfunc += inst.Pij[i][k] * x[i][j] * x[k][j];
			}
		}
	}

	obj = IloMaximize(env, objfunc);
	model.add(obj);
	objfunc.end();

	//约束
	for (size_t j = 0; j < inst.knap_size; j++)
	{
		IloExpr con(env);
		for (size_t i = 0; i < inst.item_size; i++)
		{
			con += inst.I[i].w * x[i][j];
		}
		IloConstraint constrain = con <= inst.C[j];
		//IloConstraint constrain = x[1][2] < 1;
		constraints.add(constrain);
		con.end();
	}
	for (size_t i = 0; i < inst.item_size; i++)
	{
		IloExpr con(env);
		for (size_t j = 0; j < inst.knap_size; j++)
		{
			con += x[i][j];
		}
		IloConstraint constrain = con <= 1;
		constraints.add(constrain);
		con.end();
	}
	model.add(constraints);






	cplex.solve();
	int optobjvalue = cplex.getObjValue();




	vector<vector<int>> Optimal_Solution;
	vector<int> solution(inst.item_size, -1);
	Optimal_Solution = vector<vector<int>>(inst.knap_size, solution);
	for (size_t i = 0; i < inst.knap_size; i++)
	{
		for (size_t j = 0; j < inst.item_size; j++)
		{
			Optimal_Solution[i][j] = cplex.getValue(x[j][i]);
		}
	}

	for (size_t i = 0; i < inst.knap_size; i++)
	{
		for (size_t j = 0; j < inst.item_size; j++)
		{
			cout << "x" << i + 1 << j + 1 << " = " << Optimal_Solution[i][j] << endl;
		}
	}
	cout << "最优解为：" << optobjvalue << endl;

	int calculate = 0;
	for (size_t i = 0; i < inst.knap_size; i++)
	{
		for (size_t j = 0; j < inst.item_size; j++)
		{
			calculate += inst.I[j].p * Optimal_Solution[i][j];
		}
	}
	for (size_t i = 0; i < inst.item_size; i++)
	{
		for (size_t k = i + 1; k < inst.item_size; k++)
		{
			for (size_t j = 0; j < inst.knap_size; j++)
			{
				calculate += inst.Pij[i][k] * Optimal_Solution[j][i] * Optimal_Solution[j][k];
			}
		}
	}
	cout << "最优解为为：" << calculate << endl;
}

void SolvebyCplexTEST()
{
	typedef IloArray<IloNumVarArray> IntVarMatrix;
	IloEnv env;
	IloModel model(env);
	IntVarMatrix x(env);
	IloObjective obj(env);
	IloConstraintArray constraints(env);
	IloCplex solver(env);
	IloNumArray c(env);
	IloCplex cplex(model);

	c = IloNumArray(env, inst.knap_size);
	for (size_t i = 0; i < inst.knap_size; i++)
	{
		c[i] = inst.C[i];
	}

	//变量约束
	x = IloArray<IloNumVarArray>(env, inst.item_size);
	for (size_t i = 0; i < inst.item_size; i++)
	{
		x[i] = IloNumVarArray(env, inst.knap_size, 0, 1);
	}

	//目标函数
	IloExpr objfunc(env);
	for (size_t j = 0; j < inst.knap_size; j++)
	{
		for (size_t i = 0; i < inst.item_size; i++)
		{
			objfunc += inst.I[i].p * x[i][j];
		}
	}
	obj = IloMaximize(env, objfunc);
	model.add(obj);

	//约束
	for (size_t j = 0; j < inst.knap_size; j++)
	{
		IloExpr con(env);
		for (size_t i = 0; i < inst.item_size; i++)
		{
			con += inst.I[i].w * x[i][j];
		}
		IloConstraint constrain = con <= inst.C[j];
		constraints.add(constrain);
		con.end();
	}
	for (size_t i = 0; i < inst.item_size; i++)
	{
		IloExpr con(env);
		for (size_t j = 0; j < inst.knap_size; j++)
		{
			con += x[i][j];
		}
		IloConstraint constrain = con <= 1;
		constraints.add(constrain);
		con.end();
	}
	model.add(constraints);






	cplex.solve();
	int optobjvalue = cplex.getObjValue();




	vector<vector<float>> Optimal_Solution;
	vector<float> solution(inst.item_size, -1);
	Optimal_Solution = vector<vector<float>>(inst.knap_size, solution);
	for (size_t i = 0; i < inst.knap_size; i++)
	{
		for (size_t j = 0; j < inst.item_size; j++)
		{
			Optimal_Solution[i][j] = cplex.getValue(x[j][i]);
		}
	}

	for (size_t i = 0; i < inst.knap_size; i++)
	{
		for (size_t j = 0; j < inst.item_size; j++)
		{
			cout << "x" << i + 1 << j + 1 << " = " << Optimal_Solution[i][j] << endl;
		}
	}
	cout << "最优解为：" << optobjvalue << endl;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
