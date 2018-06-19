#include "utils.h"
#include <vector>
#include <map>
using namespace std;
float acu(vector<pair<float, float>>& usertmp) {
	int size = usertmp.size()*(usertmp.size() - 1) / 2;
	float sum = 0;
	for (int i = 0; i < usertmp.size(); i++)
	{
		for (int j = i + 1; j < usertmp.size(); j++)
		{
			if ((usertmp[i].first - usertmp[j].first)*(usertmp[i].second - usertmp[j].second) > 0)
				sum += 1;
		}
	}
	return sum / size;
}
int get_movie_id(map<string, int>&  idmap, int obj) {
	map<string, int>::iterator it;
	for (it = idmap.begin(); it != idmap.end(); it++)
	{
		if (it->second == obj)
			return atoi(it->first.c_str());
	}
}