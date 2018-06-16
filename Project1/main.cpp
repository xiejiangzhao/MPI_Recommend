#include <iostream>
#include <cstdio>
#include <mpi.h>
#include "csv.h"
#include "cross_val.h"
#include "utils.h"
using namespace std;
struct Simi
{
	int i;
	int j;
	float score;
};
vector<vector <string> > Context;
int N[164951];
vector<Simi> W;
int main(int argc, char *argv[]) {
	Context=csvtovector("ratings.csv");
	vector<vector <string> > train_set = get_train_set(Context);
	vector<vector <string> > test_set = get_test_set(Context);
	vector<pair<int, float>>* movie_mat;
	movie_mat = itembase(train_set);
	vector<int> have_data;
	for (int i = 0; i < 163951; i++)
	{
		if (movie_mat[i].size() != 0)
			have_data.push_back(i);
	}
	int i, j;
	for (int m = 0; m < have_data.size(); m++)
	{
		i = have_data[m];
		cout << i << endl;
		int n = have_data[m + 1];
		for (int n = m + 1; n < have_data.size(); n++) {
			j = have_data[n];
			if (movie_mat[i].size()&movie_mat[j].size()) {
				Simi tmp;
				tmp.i = i, tmp.j = j; tmp.score = 0;
				W.push_back(tmp);
				continue;
			}
			else{
				int sum_x = 0, sum_y = 0, sum_x2 = 0, sum_xy = 0, sum_y2 = 0, count = 0, score;
				for (int k = 0; k < movie_mat[i].size(); k++)
				{
					score = get_score(movie_mat[j], movie_mat[i][k].first) != -1;
					if (score!=-1) {
						count++;
						sum_x += movie_mat[i][k].second;
						sum_y += score;
						sum_x2 += movie_mat[i][k].second*movie_mat[i][k].second;;
						sum_y2 += score * score;
						sum_xy += movie_mat[i][k].second*score;
					}
				}
				Simi tmp;
				tmp.i = i, tmp.j = j;
				if (count != 0)
					tmp.score = (sum_xy - sum_x * sum_y / count) / sqrt((sum_x2 - sum_x * sum_x / count) + (sum_xy - sum_y * sum_y / count));
				else
					tmp.score = 0;
				W.push_back(tmp);
			}
		}
	}
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
