string convertMhzToGhz(int number){
    return to_string((number / 1024));
}
double convertKbToMb(int number){
    return (number / 1024);
}

string insertValueCommas(string text){
    int textSize = text.length() - 3;
    while(textSize > 0){
        text.insert(textSize,",");
        textSize -= 3;
        break;
    }

    return text;
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

double getMHzAverage(vector<pair<int,string>> values){
    double storeMHzAsNumber = 0;
    size_t sizeOfValues = values.size();
    for(auto processorMHz : values){
        storeMHzAsNumber += convertStringToNumber(removeAllNotNumbers(removeAllSpaces(processorMHz.second)));
    }

    return (storeMHzAsNumber / sizeOfValues);
}