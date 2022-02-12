#include <iostream>
#include <vector>
#include <utility>
#include <cctype>

using namespace std;


double convertKbToGb(int number){
    return (number / 1024);
}

double convertStringToNumber(string text){
    return atof(text.c_str());
};

string removeAllNotNumbers(string text){
    string newText = "";
    for(int i = 0;i < text.length();i++){
        if(isdigit(text[i]) || text[i] == '.'){
            newText += text[i];
        }
    }

    return newText;
}

string removeAllSpaces(string text){
    string newText = "";
    for(int i = 0;i < text.length();i++){
        if(!isspace(text[i])){
            newText += text[i];
        }
    }

    return newText;
}

pair<string,string> getKeyAndValue(string text){
    size_t divisor = text.find(":");
    string key = text.substr(0,divisor);
    string value = text.substr(divisor + 1);

    return make_pair(key,value);
}