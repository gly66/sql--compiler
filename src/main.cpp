#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <cstring>
#include<fstream>
#include <sstream>
#include "grammarLib.h"

using namespace std;

int main()
{
	string T_filename = "T.txt";
	string N_filename = "N.txt";
	string production_filename = "productions.txt";
	string ATable_filename = "ATable.txt";
	string out_filename = "23graA.tsv";
	bool is_init = 0;
	Grammar grammar(T_filename, N_filename, production_filename);

	if (is_init) {
		grammar.init();
		grammar.save_ATable(ATable_filename);
	}
	else {
		grammar.load_ATable(ATable_filename);
	}
	vector<string> KW;
	vector<string> OP;
	vector<string> SE;
	vector<string> inToken;
	for (int i = 0; i < 51; i++) {
		if (i <= 31)
			KW.push_back(grammar.Ts[i]);
		else if (i <= 47)
			OP.push_back(grammar.Ts[i]);
		else
			SE.push_back(grammar.Ts[i]);
	}

	string temp;
	ifstream file;
	file.open("23lexA.tsv");
	while (getline(file, temp)) {
		istringstream is(temp);
		string left, right;
		is >> left;
		if (left == "GROUP" || left == "ORDER")
		{
			string by;
			is >> by;
			left = left + " " + by;
		}
		is >> right;
		right.erase(0, 1);
		int pos = right.find(">");
		right.erase(pos, 1);
		int len = size(right);
		int pos1 = right.find(",");
		string type, value;
		type = right.substr(0, pos1);
		value = right.substr(pos1 + 1, len - pos1);

		if (type == "KW") {
			int a = atoi(value.c_str());
			inToken.push_back(KW[a - 1]);
		}
		else if (type == "OP") {
			int a = atoi(value.c_str());
			inToken.push_back(OP[a - 1]);
		}
		else if (type == "SE") {
			int a = atoi(value.c_str());
			inToken.push_back(SE[a - 1]);
		}
		else {
			inToken.push_back(type);
		}

	}
	file.close();
	/*inToken.push_back("SELECT");
	inToken.push_back("IDN");
	inToken.push_back(".");
	inToken.push_back("IDN");
	inToken.push_back("FROM");
	inToken.push_back("IDN");
	inToken.push_back("WHERE");
	inToken.push_back("IDN");
	inToken.push_back(".");
	inToken.push_back("IDN");
	inToken.push_back(">");
	inToken.push_back("INT");*/

	/*inToken.push_back("id");
	inToken.push_back("=");
	inToken.push_back("id");*/

	/*inToken.push_back("a");
	inToken.push_back("b");
	inToken.push_back("a");
	inToken.push_back("b");*/

	/*inToken.push_back("SELECT");
	inToken.push_back("IDN");
	inToken.push_back("AS");
	inToken.push_back("IDN");
	inToken.push_back(",");
	inToken.push_back("IDN");
	inToken.push_back("AS");
	inToken.push_back("IDN");
	inToken.push_back("FROM");
	inToken.push_back("IDN");
	inToken.push_back("ORDER BY");
	inToken.push_back("IDN");*/

	/*inToken.push_back("SELECT");
	inToken.push_back("*");
	inToken.push_back("FROM");
	inToken.push_back("IDN");
	inToken.push_back("WHERE");
	inToken.push_back("(");
	inToken.push_back("IDN");
	inToken.push_back("<");
	inToken.push_back("FLOAT");
	inToken.push_back(")");
	inToken.push_back("AND");
	inToken.push_back("!");
	inToken.push_back("(");
	inToken.push_back("IDN");
	inToken.push_back("<");
	inToken.push_back("INT");
	inToken.push_back(")");*/

	grammar.grammarCheck(inToken,out_filename);
	cout << "hddhh" << endl;
	return 0;
}