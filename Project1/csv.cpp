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
void matrixtocsv(float src[9126][9126])
{
	ofstream outfile;
	outfile.open("W.csv", ios::out);
	for (int i = 0; i < 9126; i++)
	{
		for (int j = 0; j < 9126; j++)
		{
			outfile << src[i][j];
			if (j != 9125)
				outfile << ",";
		}
		outfile << endl;
	}
	outfile.close();
}