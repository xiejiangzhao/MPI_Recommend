#include <iostream>
#include <cstdio>
#include <mpi.h>
#include "csv.h"
#include "cross_val.h"
#include "utils.h"
#include <map>
using namespace std;
struct Simi
{
	int i;
	int j;
	float score;
};
vector<vector <string> > Context;
vector<Simi> W;
int N[9126] = { 0 };
int main(int argc, char *argv[]) {
	Context= csvtovector("movies.csv");
	Buildmap(Context);
	Context=csvtovector("ratings.csv");
	vector<vector <string> > train_set = get_train_set(Context);
	vector<vector <string> > test_set = get_test_set(Context);
	vector<pair<int, float>>* movie_mat;
	movie_mat = itembase(train_set);
	for (int i = 0; i < 9126; i++)
	{
		N[i] = movie_mat[i].size();
	}
	for
	int myid, numprocs, source;
	int namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv); //初始化MPI环境
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Get_processor_name(processor_name, &namelen);
	cout << "Hello World!Process " << myid << " of %d on Hello World!Process " << numprocs << " of " << processor_name;
	MPI_Finalize(); //结束MPI环境
	return 0;
}
