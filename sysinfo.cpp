#include <iostream>
#include <sys/utsname.h>
#include <fstream>
#include <pwd.h>
#include <unistd.h>
#include "libs/colorizedPrint.hpp"
#include <utility>
#include <sys/utsname.h>
#include <vector>
#include <filesystem>
#include <cctype>
#include "libs/argsParser.hpp"

using namespace std;
namespace fs = std::filesystem;
using namespace argparse;

#include "helpers/programInteraction.hpp"

int main(int argc, char *argv[])
{

	bool anySectionSelected = false;
	bool userSectionSelected = false;
	bool cpuSectionSelected = false;
	bool ramSectionSelected = false;
	bool osSectionSelected = false;

	string lineFromFile;

	pair<string, string> keyAndValueFromInfo;
	string key;
	string value;

	ArgumentParser parser("SysInfo", "1.0");

	parser.add_argument("-c", "--cpu").default_value("-").help("Print the CPU section");
	parser.add_argument("-r", "--ram").default_value("-").help("Print the RAM section");
	parser.add_argument("-u", "--user").default_value("-").help("Print the USER section");
	parser.add_argument("-o", "--os").default_value("-").help("Print the OS section");

	try
	{
		parser.parse_args(argc, argv);
	}
	catch (runtime_error &err)
	{
		cout << parser;
		exit(1);
	}

	if (parser.is_used("-c") || parser.is_used("-r") || parser.is_used("-u") || parser.is_used("-o"))
	{

		anySectionSelected = true;
		if (parser.is_used("-c"))
		{
			cpuSectionSelected = true;
		}
		if (parser.is_used("-r"))
		{
			ramSectionSelected = true;
		}
		if (parser.is_used("-u"))
		{
			userSectionSelected = true;
		}
		if (parser.is_used("-o"))
		{
			osSectionSelected = true;
		}
	}

	if (!anySectionSelected || userSectionSelected)
	{
		colorizedPrint(foreColor::fGreen, backColor::bNone, "================= User =================\n");
		passwd *userDatabase = getpwuid(getuid());
		cout << "User name: ";
		colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {userDatabase->pw_name});
		cout << "User directory: ";
		colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {userDatabase->pw_dir});
		cout << "User default shell: ";
		colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {userDatabase->pw_shell});

		cout << endl;
	}

	if(ramSectionSelected || cpuSectionSelected || !anySectionSelected){
		colorizedPrint(foreColor::fGreen, backColor::bNone, "================ Hardware ================\n");	
	}

	if (ramSectionSelected || !anySectionSelected)
	{
		fstream memoryDatabase;
		memoryDatabase.open("/proc/meminfo", ios::in);

		if (memoryDatabase.is_open())
		{
			int valueToConvert = 0;
			while (getline(memoryDatabase, lineFromFile))
			{
				keyAndValueFromInfo = getKeyAndValue(lineFromFile);
				key = keyAndValueFromInfo.first;
				value = keyAndValueFromInfo.second;
				if (key == "MemTotal" || key == "MemAvailable" || key == "MemFree" || key == "Active" || key == "Percpu" || key == "SwapTotal" || key == "SwapFree" || key == "SwapCached" || key == "Buffers" || key == "Cached" || key == "KernelStack")
				{
					keyAndValueFromInfo.second = removeAllNotNumbers(removeAllSpaces(keyAndValueFromInfo.second));
					valueToConvert = convertStringToNumber(keyAndValueFromInfo.second);
					valueToConvert = convertKbToMb(valueToConvert);
					cout << keyAndValueFromInfo.first << ": ";
					colorizedPrint(foreColor::fNone, backColor::bNone, "%s MB\n", {to_string(valueToConvert)});
				}
			}
		}
		else
		{
			colorizedPrint(foreColor::fRed, backColor::bNone, "Error in open memory database. Skip\n \n");
		}

		memoryDatabase.close();

		cout << endl;
	}

	if (cpuSectionSelected || !anySectionSelected)
	{
		fstream cpuDatabase;
		cpuDatabase.open("/proc/cpuinfo", ios::in);

		if (cpuDatabase.is_open())
		{
			int numberOfProcessors = 0;
			vector<pair<int, string>> MHzOfEachProcessor;
			string cpuCores = "";
			string modelName = "";
			string cacheSize = "";

			while (getline(cpuDatabase, lineFromFile))
			{
				keyAndValueFromInfo = getKeyAndValue(lineFromFile);
				keyAndValueFromInfo.first = removeAllSpaces(keyAndValueFromInfo.first);
				key = keyAndValueFromInfo.first;
				value = keyAndValueFromInfo.second;
				if (key == "cpucores" || key == "processor" || key == "modelname" || key == "cachesize")
				{
					if (key == "processor")
					{
						numberOfProcessors += 1;
					}
					if (key == "cpucores")
					{
						cpuCores = value;
					}
					if (key == "modelname")
					{
						modelName = value;
					}
					if (key == "cachesize")
					{
						cacheSize = value;
					}
				}
			}

			string beginPartOfPath = "/sys/devices/system/cpu/cpu";
			string endPartOfPath = "/cpufreq/scaling_cur_freq";
			string completePath = "";
			string frequencyOfProcessor = "";
			for (int i = 0; i < numberOfProcessors; i++)
			{
				completePath = beginPartOfPath;
				completePath += to_string(i);
				completePath += endPartOfPath;

				fstream freqFile;
				freqFile.open(completePath, ios::in);

				if (freqFile.is_open())
				{
					getline(freqFile, frequencyOfProcessor);
					MHzOfEachProcessor.push_back(make_pair(i, frequencyOfProcessor));
				}
				else
				{
					cout << "cant open file";
				}
			}

			// Print all got informations
			cout << "Cpu cores: ";
			colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {cpuCores});
			cout << "Number of threads: ";
			colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {to_string(numberOfProcessors)});
			cout << "Cpu model name: ";
			colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {modelName});
			cout << "Cache size: ";
			colorizedPrint(foreColor::fNone, backColor::bNone, "%s\n", {cacheSize});
			cout << "Processors frequency: " << endl;
			for (auto cpuFrequency : MHzOfEachProcessor)
			{
				cout << "       " << cpuFrequency.first << ": ";
				colorizedPrint(foreColor::fNone, backColor::bNone, "%s GHz\n", {insertValueCommas(convertMhzToGhz(convertStringToNumber(cpuFrequency.second)))});
			}
		}
		else
		{
			colorizedPrint(foreColor::fRed, backColor::bNone, "Error in open cpu database. Skip\n \n");
		}
		cpuDatabase.close();

		cout << endl
			 << endl;
	}


	if (!anySectionSelected || osSectionSelected)
	{
		colorizedPrint(foreColor::fGreen, backColor::bNone, "================ System ================\n");

		utsname sysInfo;
		uname(&sysInfo);

		cout << "Distro: " << sysInfo.release << endl;
		cout << "OS kernel: " << sysInfo.sysname << endl;
		cout << "Archtecture: " << sysInfo.machine << endl;
	}

	return 0;
}
