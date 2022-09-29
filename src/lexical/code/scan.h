#include "utils.h"

bool is_letter(char now)
{
    if( (now >=65 && now <=90) || (now >=97 && now <=122))
        return true;
    return false;
}

bool is_number(char now)
{
    if( now >= 48 && now <=57)
        return true;
    return false;
}

bool is_legal(char now)
{
    if(edges.count(now) == 0)
        return false;
    return true;
}

bool is_space(char now)
{
    if(now == ' ')
        return true;
    return false;
}

int judge(string buffer, int state,char now)
{
    if((buffer == "GROUP "||buffer == "group " || buffer=="order " ||buffer =="ORDER ") &&state == 19 && (now == 'B' || now == 'b'))
        return 3;
    if(state == 31 && (now == 'Y' || now == 'y'))
        return 4;
    if((state == 16 || state == 29)&&now !='"' && now!='\n')
        return 0;
    if(is_letter(now))
        return 0;
    if(is_number(now))
        return 6;
    if(is_space(now))
        return 2;
    if(now == '\n')
        return 1;
    if(is_legal(now))
        return edges[now];
    else{
        cout<<"ERROR, ILLEGAL INPUT:"<<now<<endl;
        return -1;
    }
}

int is_keyword(string outcome)
{
    vector<string>:: iterator it = std::find(KeyWord.begin(), KeyWord.end(), outcome);
    if( it !=KeyWord.end())
        return (std::distance(KeyWord.begin(), it))+1;
    return 0;
}

int is_operator(string outcome)
{
    vector<string>::iterator it = std::find(OP.begin(), OP.end(), outcome);
    if(it != OP.end())
        return (std::distance(OP.begin(), it))+1;
    return 0;
}

int is_se(string outcome)
{
    vector<string>::iterator it = std::find(SE.begin(), SE.end(), outcome);
    if(it != SE.end())
        return (std::distance(SE.begin(), it))+1;
    return 0;
}

bool is_idn(string outcome)
{
    if(!is_letter(outcome[0]) && outcome[0] != '_')
        return 0;
    for(int i=0; i<outcome.length(); ++i){
        if((!is_letter(outcome[i]))&& (!is_number(outcome[i])) && (outcome[i]!='_')){
            return 0;
        }
    }
    return 1;
}

bool is_int(string outcome)
{
    if(!is_number(outcome[0]) && outcome[0]!='-')
        return 0;
    for(int i=1; i<outcome.length(); ++i){
        if(!is_number(outcome[i]))
            return 0;
    }
    return 1;
}

bool is_float(string outcome)
{
    if(!is_number(outcome[0]) && outcome[0]!='-')
        return 0;
    for(int i=1; i<outcome.length(); ++i){
        if((!is_number(outcome[i]))&& outcome[i]!='.')
            return 0;
    }
    return 1;
}

bool is_string(string outcome)
{
    if(outcome[0]=='"' && outcome[outcome.length()-1]=='"')
        return true;
    return false;
}

void judge_final(string outcome)
{
    if(outcome == "\n")
    return;
    int temp = is_keyword(outcome);
    if(temp){
        cout <<outcome<<"   <KW,"<< temp<<">"<<endl;
        return;
    }
    temp = is_operator(outcome);
    if(temp){
        cout<<outcome<<"    <OP,"<< temp<<">"<<endl;
        return;
    }
    temp = is_se(outcome);
    if(temp){
        cout<<outcome<<"    <SE,"<< temp<<">"<<endl;
        return;
    }
    if(is_idn(outcome)){
        cout<< outcome <<"  <IDN,"<<outcome<<">"<<endl;
        return;
    }
    if(is_int(outcome)){
        cout<<outcome<<"    <INT,"<<outcome<<">"<<endl;
        return;
    }
    if(is_float(outcome)){
        cout<<outcome<<"    <FLOAT,"<<outcome<<">"<<endl;
        return;
    }
    if(is_string(outcome)){
        cout<<outcome<<"    <STRING,"<<outcome<<">"<<endl;
        return;
    }
    cout<<"ERROR NOT DEFINED INPUT:"<<outcome<<endl;
    exit(-1);
}

int scan(std::ifstream& text)
{
    string line;
    while(std::getline(text, line)){
        // 读取一行内容
        if(line.empty())
            continue;
        // 去除首空格
        char pre=0;
        line.erase(0, line.find_first_not_of(" "));
        line= line+'\n';
        // cout<<line<<endl;
        int state = 1; // 起始状态
        string buffer; //buffer
        // cout << line<<endl;
        for(int i =0; i< line.length(); ++i){
            // 读每一个字符
            // cout<<line[i]<<endl;
            buffer = line.substr(0,i);
            int input = judge(buffer,state, line[i]);
            
            if(input == -1){
                return -1;
            }else{
                state = std::abs(dfa_martix[state-1][input]);
                // if(line[i]=='.') cout<<input<<" "<<state<<endl;
                // cout << state<<endl;
                if(state ==0){
                    // error 或 结束一次查找.
                    if(i >0)
                    {
                        buffer=line.substr(0, i);
                        if(buffer[i-1] ==' ') buffer.erase(buffer.find_last_not_of(" ") + 1);
                        line.erase(0, i);
                    }
                    else if(i == 0){
                        buffer = line.substr(0, i+1); 
                        line.erase(0, i+1);
                    }
                    // cout<<"test string:"<<buffer<<endl;
                    judge_final(buffer);
                    // return to initial state, and begin next lexical.
                    state =1 ;
                    i = -1;
                    if(line[0]==' ') line.erase(0, line.find_first_not_of(" "));
                    // cout<<line<<endl;
                }
            }
        }
        // break;
    }
    return 0;
}