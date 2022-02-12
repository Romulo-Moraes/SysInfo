#include <iostream>
#include <sys/utsname.h>
#include <fstream>
#include <pwd.h>
#include <unistd.h>
#include "helpers/programInteraction.hpp"
#include "libs/colorizedPrint.hpp"
#include <utility>

using namespace std;

int main(int argc,char *argv[]){
	colorizedPrint(foreColor::fGreen, backColor::bNone,"================ User ================\n");
	passwd *userDatabase = getpwuid(getuid());
	cout << "User name: " << userDatabase->pw_name << endl;
	cout << "User directory: " << userDatabase->pw_dir << endl;
	cout << "User default shell: " << userDatabase->pw_shell << endl;


	colorizedPrint(foreColor::fGreen, backColor::bNone,"================ Hardware ================\n");
	fstream memoryDatabase;
	memoryDatabase.open("/proc/meminfo", ios::in);

	string lineFromFile;

	pair<string,string> keyAndValueFromInfo;

	if(memoryDatabase.is_open()){
		int valueToConvert = 0;
		while(getline(memoryDatabase,lineFromFile)){
			keyAndValueFromInfo = getKeyAndValue(lineFromFile);
			if(keyAndValueFromInfo.first == "MemTotal" || keyAndValueFromInfo.first == "MemAvailable" || keyAndValueFromInfo.first == "MemFree"){
				keyAndValueFromInfo.second = removeAllNotNumbers(removeAllSpaces(keyAndValueFromInfo.second));
				valueToConvert = convertStringToNumber(keyAndValueFromInfo.second);
				valueToConvert = convertKbToGb(valueToConvert);
				cout << keyAndValueFromInfo.first << ": " << valueToConvert << " GB" << endl;
			}
		}	
	}
	else{
		colorizedPrint(foreColor::fGreen, backColor::bNone,"Error in open memory database. Skip\n \n");
	}

	memoryDatabase.close();

	return 0;
}
