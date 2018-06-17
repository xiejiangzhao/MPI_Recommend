#include <iostream>
#include <cstdio>
#include <mpi.h>
#include "csv.h"
using namespace std;
vector<vector <string> > Context;
int main(int argc, char *argv[]) {
	Context=csvtovector("movies.csv");
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
