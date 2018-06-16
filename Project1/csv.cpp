#include "csv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
vector<vector<string>> csvtovector(string file)
{
	vector<vector <string> > Context;    //�ļ�����
	string line, field;    //lineΪÿ�����ݣ�fieldΪÿ���ֶ�
	ifstream in;
	in.open(file);
	if (in.is_open()) {
		getline(in, line);
		cout << "open succeess" << endl;
		while (getline(in,line))
		{
			istringstream stream(line);
			vector<string> RowContext;
			while (getline(stream, field, ',')) {
				RowContext.push_back(field);
			}
			Context.push_back(RowContext);
		}
	}
	else
	{
		cout << "File open failed" << endl;
	}
	return Context;
}
