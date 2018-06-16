#include "utils.h"
#include <vector>
using namespace std;
vector<pair<int, float>> res[163951];
vector<pair<int, float>>* itembase(vector<vector<string>> src)
{
	int userid, movieid;
	float rating;
	pair<int, float> movie;
	for (int i = 0; i < src.size(); i++)
	{
		userid = atoi(src[i][0].c_str());
		movieid = atoi(src[i][1].c_str());
		rating = atof(src[i][2].c_str());
		movie.first = userid; movie.second = rating;
		res[movieid].push_back(movie);
	}
	return res;
}
int get_score(vector<pair<int, float>>& src, int id) {
	for (int i = 0; i < src.size(); i++)
	{
		if (src[i].first == id)
			return src[i].second;
	}
	return -1;
}