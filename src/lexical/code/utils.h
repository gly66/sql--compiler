#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <map>
// #include <unordered_set>

const int Num_edge = 20;
const int Num_status = 56;
int Dfa_status;

using std:: cout; using std:: endl;
using std:: vector; using std:: string;
using std:: set; using std:: stringstream;
using std:: map; 

vector<string> KeyWord = {"SELECT", "FROM", "WHERE", "AS","*","INSERT", "INTO", "VALUES","VALUE", "DEFAULT","UPDATE", "SET","DELETE", "JOIN", "LEFT", "RIGHT", "ON","MIN", "MAX","AVG", "SUM", "UNION", "ALL", "GROUP BY", "HAVING", "DISTINCT","ORDER BY", "TRUE", "FALSE", "UNKNOWN", "IS","NULL"};

vector<string> OP = {"=", ">", "<", ">=", "<=", "!=", "<=>", "AND", "&&", "OR", "||", "XOR", "NOT", "!", "-", "."};

vector<string> SE = {"(", ")", ","};

struct dfa_status{
    int no;
    int term;
};

vector<dfa_status> dfa_all_status; 

vector<int> nfa_matrix[Num_status][Num_edge];

int dfa_martix[Num_status][Num_edge];

int dfa_min_martix[Num_status][Num_edge];

// typedef enum EdgeType {};

// map<int, int> stop_status = {{2,-2}, {7,-3}, {9,-4},{13,-5},{18,-6},{20, -7},{24, -8}};

set<int> stop_status = {4, 12, 14, 24, 31, 32, 33, 35, 37, 39, 42, 44, 46, 47, 48, 49, 50, 54, 55, 56};
// map<char, int> edges = {{'!',0}, {'"', 1}, {'(',2}, {')',3}, {',',4}, {'.',5}, {'<',6}, {'=',7}, {'>',8}, 
//                         {'B',9}, {'Y',10}, {'_',11}, {'a',12}, {'c',13}, {'m',14}, {'n',15}, {'s',16}};

map<char, int> edges;

void build_nfa(std::ifstream& nfa){
    string line;
    string temp1; char temp; string temp2;
    int s1,s2;
    int counter_token = 0;
    while(std::getline(nfa, line)){
        stringstream input(line);
        input >> temp1; input>> temp; input >> temp2;
        if(edges.count(temp) == 0){
            edges.insert(std::make_pair(temp, counter_token));
            counter_token++;
        }
        s1 = std:: stoi(temp1); s2 = std:: stoi(temp2);
        // if(stop_status.find(s2) != stop_status.end())
        //     s2 = stop_status[s2];
        // cout << s1 << " "<< s2 <<endl;
        // cout << edges[temp]<<endl;
        nfa_matrix[s1-1][edges[temp]].push_back(s2);
    }
    // map<char, int>::iterator iter;
    // iter = edges.begin();
    // while(iter != edges.end()) {
    //     cout << iter->first << " : " << iter->second << endl;
    //     iter++;
    // }
    return;
}

set<int> moveia(set<int> init, int input)
{
    set<int> outcome;
    // ???????????????????????????????????????????????????
    // cout <<input<<endl;
    set<int>::iterator it;
    for(it=init.begin(); it !=init.end(); ++it){
        for (int j = 0; j < nfa_matrix[(*it)-1][input].size(); ++j){
            outcome.insert(nfa_matrix[(*it)-1][input][j]);
        }
    }
    // set<int>::iterator it2;
    // cout<<"moveia"<<endl;
    // for(it2=outcome.begin(); it2!=outcome.end(); ++it2){
    //     cout<<*it2<<" ";
    // }
    // cout<<endl;
    
    return outcome;
}

set<int> closuer(set<int> init, int input)
{
    // ???init??????????????????, ?????????@????????????
    // ????????????
    // ????????????????????????????????????, ???????????????
    set<int> temp = moveia(init, input);
    // int test;
    // std::cin>>test;
    set<int>::iterator it;
    for(it =temp.begin(); it!=temp.end(); ++it){
        // for(int m =0; m <nfa_matrix[(*it)-1][12].size(); ++m)
        //     temp.insert(nfa_matrix[(*it)-1][12][m]);
        for(int n=0; n < nfa_matrix[(*it)-1][1].size(); ++n)
            temp.insert(nfa_matrix[(*it)-1][1][n]);
    }
    return temp;
}

void nfa2dfa()
{
    set<int> init= {1};
    vector< set<int> > all_states;
    all_states.push_back(init);
    dfa_all_status.push_back({1,false});
    int current = 0;
    while(true){
        // ??????????????????, ?????????????????????????????????????????????
        for(int input =0; input< Num_edge; ++input){
            if (input == 1) continue;
            set<int> outcome = closuer(all_states[current], input);
            if(outcome.size()==0) continue; // ??????????????????, ???????????????
            if(find(all_states.begin(), all_states.end(), outcome)==all_states.end())
            {
                // ??????????????????????????????????????????, ???????????????, ???????????????????????????.
                all_states.push_back(outcome);
                set<int>::iterator it;
                bool isterminate=false;
                for(it= outcome.begin(); it!=outcome.end(); ++it){
                    if(stop_status.find(*it)!=stop_status.end()){
                        isterminate = true;
                        break;
                    }
                }
                // ???????????????
                dfa_all_status.push_back({(int)all_states.size(),isterminate});
                dfa_martix[current][input] = isterminate ? -1*all_states.size() : all_states.size();
            }else{
                int i;
                // set<int>::iterator it;
                // for(it=outcome.begin(); it!=outcome.end();++it)
                //     cout<<*it<<" ";
                //     cout<<endl;
                for(i=0; i<all_states.size(); ++i){
                    if(all_states[i]==outcome)
                        break;
                }
                dfa_martix[current][input] = dfa_all_status[i].term ? -1*(i+1) : (i+1);
            }
        }
        current++;
        if(current == all_states.size())
            break;
    }
    Dfa_status = all_states.size();
}

void dfa_min()
{
    set<int> nt; set<int> t;
    for(int i=0; i< dfa_all_status.size(); ++i){
        if(dfa_all_status[i].term==false)
            nt.insert(dfa_all_status[i].no);
        else t.insert(dfa_all_status[i].no);
    }
    vector<set<int>> all;
    all.push_back(nt); all.push_back(t);
    
    for(int j=0; j<Num_edge; ++j){
        // ????????????????????????.
        for(int i = 0; i< all.size(); ++i){
            // ??????????????????
            // cout <<"test1\n";
            if(all[i].size()<=1) continue;
            set<int>::iterator its;
            set<int> new_set; // ????????????
            for(its=all[i].begin(); its!= all[i].end(); ++its){
                // ?????????????????????
                // cout <<"test2\n";
                int to = std::abs(dfa_martix[(*its)-1][j]); //???????????????????????????
                // cout <<"test3\n";
                if(all[i].find(to)!=all[i].end())
                    new_set.insert(*its);
            }
            if(new_set.size()<all[i].size()){
                for(its=new_set.begin(); its!=new_set.end(); ++its)
                    all[i].erase(all[i].find(*its));
            }else{
                new_set.clear();
            }
            // ???????????????
            // cout <<"test5\n";
            if(new_set.size()>0)
                all.insert(all.begin()+i+1, new_set);
        }
    }
    // ????????????dfa??????????????????
    
    for(int i=0; i< all.size(); ++i){
        set<int>::iterator it;
        // ????????????????????????????????????
        if(all[i].size() > 1){
            it = all[i].begin();
            int temp = *it;
            // ??????????????????????????????????????????????????????
            for(; it!=all[i].end(); ++it){
                for(int m=0; m< Dfa_status; ++m){
                    for(int n=0; n< Num_edge; ++n){
                        if (all[i].count(m)!=0){
                            // ????????????????????????????????????, ?????????????????????????????????0
                            if(std::abs(dfa_min_martix[m][n])==*it)
                                dfa_min_martix[m][n]= 0;
                            if(dfa_min_martix[temp-1][n]==0)
                                dfa_min_martix[temp-1][n] = dfa_min_martix[m][n];
                        }
                        else if(std::abs(dfa_min_martix[m][n])==*it)
                            dfa_min_martix[m][n]= dfa_min_martix[m][n] >0 ? temp : -1*temp;
                    }
                }
            }
        }
    }
    // cout<<"min dfa:"<<all.size()<<endl;
    // for(int i=0; i< all.size(); ++i){
    //     set<int>::iterator it;
    //     for(it = all[i].begin(); it!=all[i].end(); ++it)
    //         cout<<*it<<" ";
    //     cout <<endl;
    // }
    return;
}

void printf_mar1()
{
    for(int i=0; i<Num_status; ++i){
        for (int j=0; j<Num_edge ; ++j){
            for(int k =0; k < nfa_matrix[i][j].size(); ++k)
                cout << nfa_matrix[i][j][k] << " ";
            cout << ";";
        }
        cout <<endl;
    }
    return;
}

void printf_mar(int dfa_matrix[][Num_edge])
{
    for(int i=0; i<Dfa_status; ++i){
        for (int j=0; j<Num_edge; ++j){
            cout<< dfa_martix[i][j] << " ";
        }
        cout <<endl;
    }
    return;
}


