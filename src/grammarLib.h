#pragma once
#ifndef __GRAMMARLIB_H__
#define __GRAMMARLIB_H__
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <string>


using namespace std;

//产生式
struct Production
{
    string left;
    vector<string> right;
    bool operator<(const Production& b) const
    {
        return (right == b.right) ? ((left < b.left) ? true : false) :
                                    ((right < b.right) ? true : false);
    }
};

//规范项目
struct Project
{
    int Ps_index;
    int location;
    set<string> predict;
    bool operator<(const Project& b) const
    {
        
        /*return (Ps_index == b.Ps_index) ? ((location < b.location) ? true : false) :
                                          ((Ps_index < b.Ps_index) ? true : false);*/
        bool result = false;
        if (Ps_index == b.Ps_index) {
            if (location == b.location) {
                if (predict < b.predict) result = true;
                else result = false;
            }
            else if (location < b.location) result = true;
            else result = false;
        }
        else if (Ps_index < b.Ps_index) result = true;
        else result = false;

        return result;
    }
};

//go
struct GoAction
{
    int closure_id;
    int next_id;
    string input;
};

//分析表项

struct ATItem {
    bool b0, b1;
    int index;
};

class Grammar
{
public:
    vector<string> Ts;
    vector<string> Ns;
    vector<Production> Ps;
    vector<Project> static_vp;
    vector<set<Project>> proj_fam;
    vector<GoAction> go_actions;
    vector<map<string, ATItem>> ATable;
    stack<string> AStack;
    stack<int> SStack;

    Grammar(string T_filename, string N_filename, string production_filename);
    set<string> calculateFirstSet(vector<string> &objective);
    bool isT(string symbol);
    void getClosure(int index);
    int getNextClosure(set<Project> temp_closure);
    set<Project> Closure(string next_s, vector<Project> vp);
    void getAnalyzeTable();
    void expandClosure(Project temp_project);
    void grammarCheck(vector<string> inToken, string filename);
    void init();
    void save_ATable(string ATable_filename);
    void load_ATable(string ATable_filename);
    /*void set_T(string);
    void set_N(string);
    void set_Productions(Production);*/
private:
    
    stack<char> ST;//分析栈 
    string str;//输入串 
    string ccstr;//转换为与输入串对应的字符分析串
    vector<char> M[50][50];//预测分析表
};


#endif // !__GRAMMARLIB_H__
