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
int N[9126] = { 0 };
int own_N[9126] = { 0 };
short C[9126][9126];
short own_C[9126][9126];
float W[9126][9126];
float own_W[9126][9126];
map<string, int>  idmap;
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
vector<pair<int, float>>* itembase(vector<vector<string>> src, vector<pair<int, float>>* res)
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
		res[userid].push_back(movie);
	}
	return res;
}
int main(int argc, char *argv[]) {	
	int myid, numprocs, source;
	int namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Status status;
	MPI_Init(&argc, &argv); //初始化MPI环境
	long beg = clock();
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	//MPI_Bcast(&xjz, 4, MPI_INT,0, MPI_COMM_WORLD);
	MPI_Get_processor_name(processor_name, &namelen);
	cout << "Hello Wor!Process " << myid << " of %d on Hello World!Process " << numprocs << " of " << processor_name << endl;
	vector<pair<int, float>> res[672];
	vector<pair<int, float>> test[672];
	memset(C, 0, sizeof(short) * 9126 * 9126);
	memset(own_C, 0, sizeof(short) * 9126 * 9126);
	cout << "Process " << myid << " startcsvtovector" << endl;
	beg = clock();
	Context= csvtovector("movies.csv");
	Buildmap(Context);
	Context=csvtovector("ratings.csv");
	cout << "Process " << myid << " use " << clock()-beg <<endl;
	cout << "Process " << myid << " start spilt set" << endl;
	beg = clock();
	vector<vector <string> > train_set = get_train_set(Context);
	vector<vector <string> > test_set = get_test_set(Context);
	cout << "Process " << myid << " use " << clock() - beg << endl;
	cout << "Process " << myid << " start itembase " << endl;
	beg = clock();
	itembase(train_set,res);
	itembase(test_set, test);
	cout << "Process " << myid << " use " << clock() - beg << endl;
	cout << "Process " << myid << " start count" << endl;
	beg = clock();
	for (int i = 672*myid/4; i < 672 * (myid+1) / 4; i++)
	{
		for (int j = 0; j < res[i].size(); j++)
		{
			own_N[res[i][j].first] += 1;
			for (int k = 0; k < res[i].size(); k++)
			{
				if(j!=k)
					own_C[res[i][j].first][res[i][k].first] += 1;
			}
		}
	}
	cout << "Process " << myid << "use " << clock() - beg << endl;
	MPI_Allreduce(&own_N, &N, 9126, MPI_INT, MPI_SUM,  MPI_COMM_WORLD);
	MPI_Allreduce(&own_C, &C, 9126*9126, MPI_SHORT, MPI_SUM,MPI_COMM_WORLD);
	for (int i = 9126 * myid / 4; i < 9126 * (myid + 1) / 4; i++)
	{
		for (int j = 0; j < 9126; j++)
		{
			own_W[i][j] = C[i][j] / sqrt(N[i]*N[j]);
		}
	}
	MPI_Reduce(&own_W, &W, 9126*9126, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
	if (myid ==0) {
		for (int i = 0; i < 10; i++)
		{
			cout << N[i] << endl;
		}
		for (int i = 0; i < 10; i++)
		{
			cout << "User" << i << endl;
			for (int j = 0; j < test[i].size(); j++)
			{
				int movieid = test[i][j].first;
				float sum = 0;
				for (int k = 0; k < res[i].size(); k++)
				{
					sum += W[res[i][k].first][movieid] * res[i][k].second;
				}
				cout << sum << " User give    " << test[i][j].second << endl;
			}
		}
	}
	MPI_Finalize(); //结束MPI环境
	return 0;
}
