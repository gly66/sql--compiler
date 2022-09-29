#include "grammarLib.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


Grammar::Grammar(string T_filename, string N_filename, string production_filename)
{
	printf("----------�﷨������ʼ��----------\n");
	printf("��ȷ���ѽ��ķ�(grammar.txt)���ս��(T.txt)�����ս���ļ�(N.txt)�����ڵ�ǰ·����\n");
	//�����ս��
	ifstream file;
	string temp;

	file.open(T_filename);
	if (!file) cout << "fail!���ս���ļ�ʧ��" << endl;
	
	while (file >> temp)
	{
		if (temp == "GROUP" || temp == "ORDER") {
			string by;
			file >> by;
			temp = temp + " " + by;
			Ts.push_back(temp);
		}
		else Ts.push_back(temp);
	}
	file.close();
	//������ս��
	file.open(N_filename);
	if (!file) cout << "fail!�򿪷��ս���ļ�ʧ��" << endl;

	while (file >> temp)
	{
		Ns.push_back(temp);
	}
	file.close();
	//�������ʽ
	file.open(production_filename);
	if (!file) cout << "fail!�򿪲���ʽ�ļ�ʧ��" << endl;

	while (getline(file,temp))
	{
		stringstream ss;
		ss << temp;

		string left, symbol, right;
		Production p_temp;
		ss >> left;
		ss >> symbol;
		if (symbol != "->")
		{
			cout << "productions are wrong !!" << endl;
		}
		else
		{
			p_temp.left = left;
			while (ss >> right)
			{
				if (right == "GROUP" || right == "ORDER") {
					string by;
					ss >> by;
					right = right + " " + by;
					p_temp.right.push_back(right);
				}
				else p_temp.right.push_back(right);
			}
		}

		Ps.push_back(p_temp);
	}
	file.close();
}

void Grammar::init() {
	//�����һ���淶�ؼ�
	set<Project> temp_ps;
	Project temp_p;
	set<string> temp_ss;
	temp_p.Ps_index = 0;
	temp_p.location = 0;
	temp_ss.insert("#");
	temp_p.predict = temp_ss;
	temp_ps.insert(temp_p);

	expandClosure(temp_p);
	for (int i = 0; i < static_vp.size(); i++)
	{
		temp_ps.insert(static_vp[i]);
	}
	static_vp.clear();
	proj_fam.push_back(temp_ps);

	//�������й淶��
	int max_len_closure = 0;
	while (max_len_closure < proj_fam.size()) {
		getClosure(max_len_closure);
		max_len_closure++;
		//cout << max_len_closure << endl;
	}

	//���ɷ�����
	getAnalyzeTable();
}

bool Grammar:: isT(string symbol)
{
	for (int i = 0; i < Ts.size(); i++) {
		if (Ts[i] == symbol) return true;
	}
	return false;
}

set<string> Grammar:: calculateFirstSet(vector<string>& objective)
{
	set<string> first_set;
	int i = 0;

	for (i ; i < objective.size(); i++)
	{
		set<string> first_set_i;
		if (isT(objective[i]) || objective[i] == "$")
		{
			first_set.insert(objective[i]);
			return first_set;
		}
		else
		{
			for (int j = 0; j < Ps.size(); j++)
			{
				if (Ps[j].left == objective[i])
				{
					if (Ps[j].right[0] == objective[i]) {
						cout << "first set is worng" << endl;
						continue;
					}
					else if (Ps[j].right[0] == "$") {
						first_set_i.insert("$");
						/*return first_set;*/
					}
					else
					{
						set<string> temp_first_set;
						temp_first_set = calculateFirstSet(Ps[j].right);
						first_set_i.insert(temp_first_set.begin(), temp_first_set.end());
					}
				}
			}
		}

		if (first_set_i.count("$"))
		{
			first_set_i.erase("$");
			first_set.insert(first_set_i.begin(), first_set_i.end());
		}
		else
		{
			first_set.insert(first_set_i.begin(), first_set_i.end());
			break;
		}
	}

	if (i == objective.size()) first_set.insert("$");

	return first_set;
}


// ��ȡ��һ��closure
void Grammar:: getClosure(int index) 
{
	set<Project> temp_closure;
	GoAction temp_go;
	map<string, vector<Project>> a_map;
	set<string> next_s;

	//ɸѡ�ù淶��
	auto itor1 = proj_fam[index].begin();
	for (; itor1 != proj_fam[index].end(); itor1++)
	{
		if (itor1->location < Ps[itor1->Ps_index].right.size() && Ps[itor1->Ps_index].right[0] != "$")
		{
			a_map[Ps[itor1->Ps_index].right[itor1->location]].push_back(*itor1);
		}
	}

	//����ɸѡ������¹淶�弯
	auto itor2 = a_map.begin();
	for (; itor2 != a_map.end(); itor2++)
	{
		temp_closure.clear();
		temp_closure = Closure(itor2->first, itor2->second);

		temp_go.closure_id = index;
		temp_go.input = itor2->first;
		temp_go.next_id = getNextClosure(temp_closure);
		go_actions.push_back(temp_go);
	}
}

set<Project> Grammar::Closure(string next_s, vector<Project> vp)
{
	set<Project> temp_closure;
	for (int i = 0; i < vp.size(); i++)
	{	
		vp[i].location++;
		temp_closure.insert(vp[i]);

		if (Ps[vp[i].Ps_index].right.size() > vp[i].location && !isT(Ps[vp[i].Ps_index].right[vp[i].location]))
		{
			expandClosure(vp[i]);
			for (int j = 0; j < static_vp.size(); j++)
			{
				temp_closure.insert(static_vp[j]);
			}
			static_vp.clear();
		}
	}

	return temp_closure;
}

void Grammar::expandClosure(Project project)
{
	Project temp;
	vector<string> ba;
	set<string> first;
	//�������ʽ�ĺ�һ����
	for (int j = project.location + 1; j < Ps[project.Ps_index].right.size(); j++)
	{
		ba.push_back(Ps[project.Ps_index].right[j]);
	}
	//������ĿԤ���,��ȡfirst����
	for (auto k = project.predict.begin(); k != project.predict.end(); k++)
	{
		ba.push_back(*k);
		set<string> temp_ss = calculateFirstSet(ba);
		first.insert(temp_ss.begin(), temp_ss.end());
		ba.pop_back();
	}

	for (auto k1 = first.begin(); k1 != first.end(); k1++)
	{
		if (isT(*k1))
		{
			temp.predict.insert(*k1);
		}
	}

	for (int i = 0; i < Ps.size(); i++)
	{
		if (Ps[i].left == Ps[project.Ps_index].right[project.location])
		{
			temp.Ps_index = i;
			temp.location = 0;

			//�ж��¼���Ĳ���ʽ�Ƿ���static_vp��
			bool is_new = true;
			
			for (int k2 = 0; k2 < static_vp.size(); k2++)
			{
				if (static_vp[k2].Ps_index == temp.Ps_index &&
					static_vp[k2].location == temp.location)
				{
					//��static_vp�У��������µ�Ԥ���
					if (static_vp[k2].predict != temp.predict)
					{
						static_vp[k2].predict.insert(temp.predict.begin(), temp.predict.end());
					}
					is_new = false;
				}
			}

			if(is_new)
				static_vp.push_back(temp);
			if (is_new && !isT(Ps[temp.Ps_index].right[temp.location]))
			{
				expandClosure(temp);
			}
		}
	}
}

int Grammar:: getNextClosure(set<Project> temp_closure)
{
	int i = 0;
	for (; i < proj_fam.size(); i++)
	{
		map<set<Project>, int> mTemp;
		mTemp[proj_fam[i]] = i;

		if (mTemp.find(temp_closure) != mTemp.end()) {
			return i;
		}
	}

	proj_fam.push_back(temp_closure);

	return i;
}

//���������
void Grammar::getAnalyzeTable() {
	ATItem temp_item;
	map<string, ATItem> temp_row;
	int next_go;
	for (int i = 0; i < proj_fam.size(); i++) {
		for (auto j = proj_fam[i].begin(); j != proj_fam[i].end(); j++) {
			if (j->Ps_index == 0 && j->location == 1) {
				temp_item.b0 = 0;
				temp_item.b1 = 0;
				temp_item.index = 0; //acc
				temp_row["#"] = temp_item;
			}
			else if (Ps[j->Ps_index].right.size() == j->location || Ps[j->Ps_index].right[0] == "$") {
				for (auto k = j->predict.begin(); k != j->predict.end(); k++) {
					temp_item.b0 = 1;
					temp_item.b1 = 1;
					temp_item.index = j->Ps_index; //��Լ
					temp_row[*k] = temp_item;

				}
			}
			else {

				for (int k = 0; k < go_actions.size();k++) {
					if (go_actions[k].closure_id == i && go_actions[k].input == Ps[j->Ps_index].right[j->location]) {
						next_go = go_actions[k].next_id;
					}
				}
				if (isT(Ps[j->Ps_index].right[j->location])) {
					temp_item.b0 = 1;
					temp_item.b1 = 0;
					temp_item.index = next_go; //�ƽ�
					temp_row[Ps[j->Ps_index].right[j->location]] = temp_item;
				}
				else {
					temp_item.b0 = 0;
					temp_item.b1 = 1;
					temp_item.index = next_go; //go�ƽ�
					temp_row[Ps[j->Ps_index].right[j->location]] = temp_item;
				}
			}		
		}

		ATable.push_back(temp_row);
		temp_row.clear();
	}
}

//�ķ�����
void Grammar::grammarCheck(vector<string> inToken, string filename) {
	SStack.push(0);
	AStack.push("#");
	inToken.push_back("#");
	ofstream file;
	file.open(filename);
	if (!file) cout << "fail!�򿪽���ļ�ʧ��" << endl;

	int nowState = 0;
	int inIndex = 0;
	int outSeq = 1;
	string nowSymbol;
	map<string, ATItem>::iterator it_item;
	ATItem temp_item;

	while (inIndex < inToken.size()) {
		/*if (outSeq == 5)
			cout << "this is wrong" << endl;*/
		nowState = SStack.top();
		nowSymbol = inToken[inIndex];
		it_item = ATable[nowState].find(nowSymbol);
		if (it_item == ATable[nowState].end()) {
			cout << "Wrong!!!" << endl;
			return;
		}
		temp_item = it_item->second;

		if (temp_item.b0 && temp_item.b1) {
			if (Ps[temp_item.index].right[0] != "$") {
				int rl = Ps[temp_item.index].right.size();
				for (int i = 0; i < rl; i++) {
					SStack.pop();
					//���һ��ATable������û
					if (Ps[temp_item.index].right[rl - i - 1] == AStack.top()) AStack.pop();
					else {
						cout << "ATable is wrong!!!" << endl;
						return;
					}
				}
			}
			AStack.push(Ps[temp_item.index].left);
			SStack.push(ATable[SStack.top()][Ps[temp_item.index].left].index);
			file << outSeq << "	" << temp_item.index << "	" << AStack.top() << "#" << nowSymbol << "	reduction" << endl;
		}
		else if (temp_item.b0 || temp_item.b1) {
			SStack.push(temp_item.index);
			AStack.push(nowSymbol);
			inIndex++;
			file << outSeq << "	/	" << AStack.top() << "#" << nowSymbol << "	move" << endl;
		}
		else {
			file << outSeq << "	/	" <<"#"<<"	" << "accept" << endl;
			return;
		}

		outSeq++;
	}
}

void Grammar::save_ATable(string ATable_filename) {
	ofstream file;
	file.open(ATable_filename);
	if (!file) cout << "fail!�򿪽���ļ�ʧ��" << endl;
	else {
		file << ATable.size() << endl;
		for (int i = 0; i < ATable.size(); i++) {
			file << ATable[i].size() << endl;
			for (auto j = ATable[i].begin(); j != ATable[i].end(); j++) {
				file << j->first << " " << j->second.b0 << " " << j->second.b1 << " " << j->second.index << endl;
			}
		}
	}
}

void Grammar::load_ATable(string ATable_filename) {
	ifstream file;
	string temp;
	int size = 0;
	string map_first;
	ATItem item;
	string b0, b1, index;
	map<string, ATItem> temp_map;

	file.open(ATable_filename);
	if (!file) {
		cout << "fail!�򿪽���ļ�ʧ��" << endl;
		return;
	}

	file >> temp;
	size = stoi(temp);
	for (int i = 0; i < size; i++) {
		file >> temp;
		int temp_size = stoi(temp);
		for (int j = 0; j < temp_size; j++) {
			file >> map_first;
			if (map_first == "ORDER" || map_first == "GROUP") {
				string by;
				file >> by;
				map_first = map_first + " " + by;
			}
			file>> b0 >> b1 >> index;
			item.b0 = stoi(b0);
			item.b1 = stoi(b1);
			item.index = stoi(index);
			temp_map[map_first] = item;
		}

		ATable.push_back(temp_map);
		temp_map.clear();
	}
}