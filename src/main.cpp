#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <unistd.h>
#include <ctime>

#include "Processes.h"

using namespace std;

bool importProcesses(string argFileLocation, Processes * ProcessesArr);
string replaceSpaceTab(string rawInput);
void parseline(string procInput, Processes& newProcess);

int processCount;

int main(int argc, char *argv[]) {

	string filelocation;
	int choice = 0;
	bool failed;
	bool badchoice;


	cout<<"Enter location of input file: ";
	//cin>>filelocation;

		failed = true;
		badchoice = false;
		filelocation = "memory.dat";
		Processes* ProcessArr = new Processes[100];
		while(failed)
		{
			(importProcesses(filelocation, ProcessArr)) ? failed = false : failed = true;
			if(failed)
			{
				cout << "Bad file location please try again"<< endl << endl;
				cout << "Enter location of input file: ";
				cin >> filelocation;
			}
		}
		for (int idx = 0; idx < processCount; idx++)
		{
			cout << ProcessArr[idx].getPID() << " " << ProcessArr[idx].getA() << " "; 
			(ProcessArr[idx].getPage() == 0) ? cout << "" : cout << ProcessArr[idx].getPage();
			cout << " "  << ProcessArr[idx].lastTouched << endl;
		}
	
}

bool importProcesses(string argFileLocation, Processes * ProcessesArr)
{
	ifstream Processesfile(argFileLocation);
	int lineCount = 0;
	string temp[1000];
	int idx = 0;
	//Processesfile.open(argFileLocation);
	if (Processesfile.fail())
	{
		cout <<"error opening file";
		return false;
	}

	cout << endl <<"importing Processes..." << endl << endl;

	//start by counting the number of Processes we will be creating/populating
	while (!Processesfile.eof())
	{
		getline(Processesfile, temp[idx]);
		lineCount++;
		idx++;
		
	}
	 processCount = lineCount -1;

	//parse and fill Processes with data
	for (int i = 0; i <= processCount; i++)
	{
		parseline(temp[i], ProcessesArr[i]);
	}

	
	return true;	
}

string replaceSpaceTab(string rawInput)
{
	bool fstf = false;
	string commastring = rawInput;
	//replace the fist 
	for(char& c : commastring)
	{
		if((c == ' ' && !fstf) || (c == '	' && !fstf))
		{
			c = '\n';
			fstf = true;
		}
		else if((c == ' ') || (c == '	'))
		{
			//do nothing we will handle these next
		}
		else
		{
			fstf = false;
		}
		
	}
	commastring.erase(remove(commastring.begin(), commastring.end(), ' '), commastring.end());
	commastring.erase(remove(commastring.begin(), commastring.end(), '	'), commastring.end());


	return commastring;
}

void parseline(string Input, Processes& newProcess)
{
	string csvInput = replaceSpaceTab(Input);
	int idx = 0;
	string jobdata[3];
	stringstream input(csvInput);
	string data[3];
	while(getline(input , data[idx]))
	{
		jobdata[idx] = data[idx];
		idx++;
	}
	if(jobdata[0] != "") newProcess.setPID(stoi(jobdata[0]));
	newProcess.setA(jobdata[1]);
	if(jobdata[2] != "")
	{
		newProcess.setPage(stoi(jobdata[2]));
	}
	newProcess.lastTouched = clock();
}