#include <iostream>
#include <sys/utsname.h>
#include <fstream>
#include <pwd.h>
#include <unistd.h>
#include "libs/colorizedPrint.hpp"

using namespace std;

int main(int argc,char *argv[]){
	colorizedPrint(foreColor::fGreen, backColor::bNone,"================ User ================\n");
	passwd *userDatabase = getpwuid(getuid());
	cout << "User name: " << userDatabase->pw_name << endl;
	cout << "User directory" << userDatabase->pw_dir << endl;
	cout << "User default shell: " << userDatabase->pw_shell << endl;

	cout << endl;

	colorizedPrint(foreColor::fGreen, backColor::bNone,"================ Hardware ================\n");
	fstream memoryDatabase;
	memoryDatabase.open("/proc/meminfo", ios::in);

	string lineFromFile;

	if(memoryDatabase.is_open()){
		while(getline(cin,lineFromFile)){
			lineFromFile.
		}
	}
	else{
		colorizedPrint(foreColor::fGreen, backColor::bNone,"Error in open memory database. Skip\n \n");
	}

	return 0;
}
