#include <iostream>
#include <cstdio>
#include <mpi.h>
#include "csv.h"
#include "cross_val.h"
#include "utils.h"
#include <map>
#include <cmath>
#include <time.h>
using namespace std;
vector<vector <string> > Context;
int N[9126] = { 0 }, own_N[9126] = { 0 };
float C[9126][9126], own_C[9126][9126];
float W[9128][9128], own_W[2282][9128];
vector<pair<int, float>> train[672], test[672];
int namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;
int myid, numprocs;
map<string, int>  idmap;
void most_recommand(int userid) {
	if (myid == 0) {
		pair<int, float> rank[0126];
		for (int i = 0; i < 9126; i++)
		{
			rank[i].first = i;
			rank[i].second = 0;
			int sum = 0;
			for (int j = 0; j < train[userid].size(); j++)
			{
				if (train[userid][j].first == i) {
					rank[i].second = 0;
					break;
				}
				else
				{
					rank[i].second += W[i][train[userid][j].first] * train[userid][j].second;
				}
			}
			if (i < 50)
				cout << i << " " << rank[i].second << endl;
		}
	}
}
float get_score(vector<pair<int, float>>& src, int id) {
	for (int i = 0; i < src.size(); i++)
	{
		if (src[i].first == id)
			return src[i].second;
	}
	return -1;
}
void Buildmap(vector<vector<string>> src) {
	for (int i = 0; i < src.size(); i++)
	{
		idmap[src[i][0]] = i;
	}
}
int Bothlike(vector<pair<int, float>> a, vector<pair<int, float>> b) {
	return 0;
}
vector<pair<int, float>>* itembase(vector<vector<string>> src, vector<pair<int, float>>* train)
{
	int userid, movieid;
	float rating;
	pair<int, float> movie;
	for (int i = 0; i < src.size(); i++)
	{
		userid = atoi(src[i][0].c_str());
		movieid = idmap[src[i][1]];
		rating = atof(src[i][2].c_str());
		movie.first = movieid; movie.second = rating;
		train[userid].push_back(movie);
	}
	return train;
}
void output_test() {
	if (myid == 0) {
		for (int i = 0; i < 10; i++)
		{
			cout << "User" << i << endl;
			for (int j = 0; j < test[i].size(); j++)
			{
				int movieid = test[i][j].first;
				float sum = 0;
				for (int k = 0; k < train[i].size(); k++)
				{
					sum += W[train[i][k].first][movieid] * train[i][k].second;
				}
				cout << sum << " User give    " << test[i][j].second << endl;
			}
		}
	}
}
int main(int argc, char *argv[]) {
	//初始化MPI模式
	MPI_Init(&argc, &argv);
	long beg = clock();
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Get_processor_name(processor_name, &namelen);
	cout << "Process " << myid << " of " << numprocs << " Start running on " << processor_name << endl;
	memset(C, 0, sizeof(short) * 9126 * 9126);
	memset(own_C, 0, sizeof(short) * 9126 * 9126);

	//读取文件,转换为容器内容
	cout << "Process " << myid << " :start convert csv to vector " << endl;
	beg = clock();
	Context = csvtovector("movies.csv");
	Buildmap(Context);
	Context = csvtovector("ratings.csv");
	cout << "Process " << myid << " :Finish in " << clock() - beg << endl;

	//为交叉验证做准备
	cout << "Process " << myid << " :start spilt set " << endl;
	beg = clock();
	vector<vector <string> > train_set = get_train_set(Context);
	vector<vector <string> > test_set = get_test_set(Context);
	cout << "Process " << myid << " :Finish in " << clock() - beg << endl;

	//建立用户-物品倒排表
	cout << "Process " << myid << " :start itembase " << endl;
	beg = clock();
	itembase(train_set, train);
	itembase(test_set, test);
	cout << "Process " << myid << " :Finish in " << clock() - beg << endl;

	//计算矩阵C和N
	cout << "Process " << myid << " :start count" << endl;
	beg = clock();
	for (int i = 672 * myid / 4; i < 672 * (myid + 1) / 4; i++)
	{
		for (int j = 0; j < train[i].size(); j++)
		{
			own_N[train[i][j].first] += 1;
			for (int k = 0; k < train[i].size(); k++)
			{
				if (j != k)
					own_C[train[i][j].first][train[i][k].first] += 1.0 / log(1 + train[i].size());
			}
		}
	}
	cout << "Process " << myid << " :Finish in " << clock() - beg << endl;

	//Reduce部分
	cout << "Process " << myid << " :start Reduce" << endl;
	beg = clock();
	MPI_Allreduce(&own_N, &N, 9126, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	MPI_Allreduce(&own_C, &C, 9126 * 9126, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
	cout << "Process " << myid << " :Finish in " << clock() - beg << endl;

	int startpos = 9128 * myid / 4;
	for (int i = startpos; i < startpos+2282; i++)
	{
		for (int j = 0; j < 9128; j++)
		{
			own_W[i- startpos][j] = C[i][j] / sqrt(N[i] * N[j]);
		}
	}
	MPI_Gather(own_W, 2282 * 9128, MPI_FLOAT, W, 2282 * 9128, MPI_FLOAT, 0, MPI_COMM_WORLD);
	//MPI_Reduce(&own_W, &W, 9128 * 9128, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	output_test();
	MPI_Finalize(); //结束MPI环境
	return 0;
}
