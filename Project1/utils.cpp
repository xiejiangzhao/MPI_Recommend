#include "utils.h"
#include <vector>
#include <map>
using namespace std;
vector<pair<int, float>> res[9126];
map<string, int>  idmap;
vector<pair<int, float>>* itembase(vector<vector<string>> src)
{
	int userid, movieid;
	float rating;
	pair<int, float> movie;
	for (int i = 0; i < src.size(); i++)
	{
		userid = atoi(src[i][0].c_str());
		movieid = idmap[src[i][1]];
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
void Buildmap(vector<vector<string>> src) {
	for (int i = 0; i < src.size(); i++)
	{
		idmap[src[i][0]] = i;
	}
}
int Bothlike(vector<pair<int, float>> a, vector<pair<int, float>> b) {

}