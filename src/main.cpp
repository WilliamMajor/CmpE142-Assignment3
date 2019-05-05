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
#include "Pmem.h"

using namespace std;

bool importProcesses(string argFileLocation, Processes * ProcessesArr, Pmem * PhysicalMem);
string replaceSpaceTab(string rawInput);
void parseline(string procInput, Processes * newProcess, Pmem * PhysicalMem);

int instructionCount;
int processCount = 0;
int allocated = 0;

int main() {

	string filelocation;
	//int choice = 0;
	bool failed;
	//bool badchoice;


	cout<<"Enter location of input file: ";
	//cin>>filelocation;

		failed = true;
		//badchoice = false;
		filelocation = "memory.dat";
		Processes* ProcessArr = new Processes[100];
		Pmem* PhysicalMem = new Pmem[20]; // allocate 20 physical memory

		while(failed)
		{
			(importProcesses(filelocation, ProcessArr, PhysicalMem)) ? failed = false : failed = true;
			if(failed)
			{
				cout << "Bad file location please try again"<< endl << endl;
				cout << "Enter location of input file: ";
				cin >> filelocation;
			}
		}
		// for (int idx = 0; idx < processCount-1; idx++)
		// {
		// 	cout << ProcessArr[idx].getPID() << " " << ProcessArr[idx].lastTouched << " " << endl;
		// 	for(int i = 0; i < ProcessArr[idx].count; i++)
		// 	{
		// 		cout << ProcessArr[idx].getVM(i) << endl;
		// 	}
		// }
		// cout << endl <<  "physical memory" << endl << endl;
		// for(int idx = 0; idx < 20; idx++)
		// {
		// 	cout << PhysicalMem[idx].getPID() << " " << PhysicalMem[idx].getVM() << " ";
		// 	(PhysicalMem[idx].getFree()) ? cout << "FREE" << endl : cout << "TAKEN" << endl;
		// }
	
}

bool importProcesses(string argFileLocation, Processes * ProcessesArr, Pmem * PhysicalMem)
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
	 instructionCount = lineCount -1;

	//parse and fill Processes with data
	for (int i = 0; i <= instructionCount; i++)
	{
		parseline(temp[i], ProcessesArr, PhysicalMem);
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

void parseline(string Input, Processes * newProcess, Pmem * PhysicalMem)
{
	string csvInput = replaceSpaceTab(Input);
	int idx = 0;
	int PID;
	bool newPID = true;
	string jobdata[2];
	string instruction;
	stringstream input(csvInput);
	string data[3];

	while(getline(input , data[idx])) idx++;

	jobdata[0] = data[0];	//process ID
	instruction = data[1]; // instruction 
	jobdata[1] = data[2]; //Virtual memory

	for (int i = 0; i < processCount; i++)
	{
		if(jobdata[0] == newProcess[i].getPID())
		{
			newPID = false;
			PID = i;
			i = processCount;
		}
	}
	if(newPID)
	{
		newProcess[processCount].setPID(jobdata[0]);
		processCount++;
	}
	else
	{
		newProcess[PID].lastTouched = clock();

		if(instruction == "A")
		{
			if(allocated >= 20)
			{
				cout << "switching to be done" << endl;
			}
			else
			{
				newProcess[PID].setVM(jobdata[1], newProcess[PID].count);
				PhysicalMem[allocated].setVM(newProcess[PID].getVM(newProcess[PID].count));
				PhysicalMem[allocated].setPID(jobdata[0]);
				PhysicalMem[allocated].setFree(false);
				allocated++;
				newProcess[PID].count++;
			}
			
			
		}
		else if(instruction == "W")
		{
			int vmcounter = 0;
			bool exist = false;
			while(newProcess[PID].getVM(vmcounter) !=  "")
			{
				if(jobdata[1] == newProcess[PID].getVM(vmcounter))
				{
					int dirtycount = 0;
					exist = true;
					while(newProcess[PID].getDirty(dirtycount) != "")
					{
						dirtycount++;
					}
					newProcess[PID].setDirty(newProcess[PID].getVM(vmcounter), dirtycount);
				}
				vmcounter++;
			}
			if(!exist)
			{
				cout << "Need to kill process" <<endl;
			}
		}
		else if(instruction == "R")
		{
			int vmcounter = 0;
			bool exist = false;
			bool dirty = false;
			while(newProcess[PID].getVM(vmcounter) !=  "")
			{
				if(jobdata[1] == newProcess[PID].getVM(vmcounter))
				{
					int dirtycount = 0;
					exist = true;
					for(int idx = 0; idx < 200; idx++)
					{
						if(newProcess[PID].getDirty(idx) == newProcess[PID].getVM(vmcounter))
						{
							dirty = true;
						}
					}
					if(!dirty)
					{
						cout << "need to kill process" << endl;
					}
				}
				vmcounter++;
			}
			if(!exist)
			{
				cout << "Need to kill process" <<endl;
			}
		}
		else if(instruction == "F")
		{
			cout << "working F" << endl;
		}
		else if(instruction == "T")
		{
			cout << "working T" << endl;
		}
		else if(instruction == "C")
		{
			cout << "an error should be thrown here" << endl;
		}
	}
}