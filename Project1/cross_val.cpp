#include "cross_val.h"

vector<vector<string>> get_train_set(vector<vector<string>>& src)
{
	vector<vector<string>> res;
	for (int i = 0; i < src.size(); i++) {
		if (i % 10 != 0) {
			res.push_back(src[i]);
		}
	}
	return res;
}

vector<vector<string>> get_test_set(vector<vector<string>>& src)
{
	vector<vector<string>> res;
	for (int i = 0; i < src.size(); i++) {
		if (i % 10 == 0) {
			res.push_back(src[i]);
		}
	}
	return res;
}
