#include "scan.h"

int main(int argc, char* argv[])
{
    string test_file;
    if(argc == 1){
        cout<<"Please input the relative path of testfile\n";
        return -1;
    }
    if(argc> 2){
        cout<<"input parameter error!\n";
        return -1;
    }
    else {
        test_file = argv[1];
        // cout<<test_file<<endl;
    }
    std::ifstream nfa;
    nfa.open("nfa.txt");
    build_nfa(nfa);
    // printf_mar1();
    nfa2dfa();
    // cout<<"printmar"<<endl;
    // printf_mar(dfa_martix);
    // cout<<"dfa_min"<<endl;
    dfa_min();
    // cout<<"print_mar"<<endl;
    // printf_mar(dfa_martix);
    std::ifstream code;
    code.open(test_file);
    scan(code);
}

