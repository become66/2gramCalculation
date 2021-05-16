#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;


int main() {
    unordered_map<string,unordered_map<string,unsigned long long>> ngram;
    bool existNonASCII = false;
    bool reset = true;
    string first, second;

    std::ifstream ifs("ocw.50000.txt", std::ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } else {
        int c;
        while (!ifs.eof()) {
            c = ifs.get();
            if(c == 10){ //new line
                if(existNonASCII){
                    reset = true;
                    first = second = "";
                }
                else{
                    if(!reset){ 
                        ngram[first][second]++;
                        // cout<<"("<<first<<":"<<second<<")";
                        // cout<<"\n-------------------------------------------------------\n";
                        first = second = "";
                        reset = true;
                    }
                }
                existNonASCII = false;
            }
            else if(c == 32){ //space
                 if(existNonASCII){
                    reset = true;
                    first = second = "";
                }
                else{
                    if(reset){ 
                        reset = false; //switch to cumulate second
                    }
                    else{
                        ngram[first][second]++;
                        // cout<<"("<<first<<":"<<second<<")";
                        first = second;
                        second = "";
                    }                
                }
                existNonASCII = false;
            }
            else if(c <= 127 && c > 0){
                if(reset){
                    first.push_back((char)c);
                }
                else{
                    second.push_back((char)c);
                }
            }
            else{ //non ascii code
                existNonASCII = true;
            }
        }
    }
    ifs.close();

    
    map<unsigned long long,vector<pair<string,string>>> rank;
    for(const auto& i: ngram){ // build the sorted data structure
        for(const auto& j:i.second){
            rank[j.second].push_back(make_pair(i.first, j.first));
        }
    }

    for(const auto&i : rank){ // output the frequency in ascending order
        if(i.first > 200){
            cout<<i.first<<":\n";
            for(const auto&j: i.second){
                cout<<"("<<j.first<<" "<<j.second<<")  ";
            }
            cout<<"\n------------------------------------------\n";
        }
    }

    unordered_map<string,map<unsigned long long,string>> result;
    for(const auto& i: ngram){ // build the predicting next word data structure
        for(const auto& j:i.second){
            result[i.first][j.second] = j.first;
        }
    }


    string firstWord;
    cout<<"pls input the first word:\n";
    while(cin>>firstWord){

        cout<<"--------------------------\nsecond word frequency:\n";
        auto wordMap = result[firstWord];
        for(const auto& i:wordMap){
            cout<<i.second<<":"<<i.first<<"\n";
        }
        cout<<"--------------------------\n";
        cout<<"pls input the first word:\n";
    }


    return 0;
}




