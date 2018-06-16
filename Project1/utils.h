#pragma once
#include <vector>
#include <string>
using namespace std;
vector<pair<int, float>>* itembase(vector<vector<string>> src);
int get_score(vector<pair<int, float>>& src, int id);
void Buildmap(vector<vector<string>> src);