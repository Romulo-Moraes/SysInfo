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

	string key;
	string value;

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

	cout << endl;

	fstream cpuDatabase;

	cpuDatabase.open("/proc/cpuinfo", ios::in);

	if(cpuDatabase.is_open()){
		int numberOfProcessors = 0;
		vector<pair<int,string>> MHzOfEachProcessor;
		string cpuCores = "";
		string modelName = "";
		string cacheSize = "";

		while(getline(cpuDatabase,lineFromFile)){
			keyAndValueFromInfo = getKeyAndValue(lineFromFile);
			keyAndValueFromInfo.first = removeAllSpaces(keyAndValueFromInfo.first);
			key = keyAndValueFromInfo.first;
			value = keyAndValueFromInfo.second;
			if(key == "cpucores" || key == "cpuMHz" || key == "processor" || key == "modelname" || key == "cachesize"){
				if(key == "cpuMHz"){
					MHzOfEachProcessor.push_back(make_pair(numberOfProcessors,value));
				}
				if(key == "processor"){
					numberOfProcessors += 1;
				}
				if(key == "cpucores"){
					cpuCores = value;
				}
				if(key == "modelname"){
					modelName = value;
				}
				if(key == "cachesize"){
					cacheSize = value;
				}
			}

		}

		// Print all got informations
		cout << "Cpu cores: " << cpuCores << endl;
		cout << "Number of threads: " << numberOfProcessors << endl;
		cout << "Cpu model name: " << modelName << endl;
		cout << "Cache size: " << cacheSize << endl;
		cout << "MHz average: " << getMHzAverage(MHzOfEachProcessor);
	}
	else{
		colorizedPrint(foreColor::fGreen, backColor::bNone,"Error in open cpu database. Skip\n \n");
	}

	cpuDatabase.close();

	return 0;
}
