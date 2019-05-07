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
#include "Swap.h"

using namespace std;

bool importProcesses(string argFileLocation, Processes * ProcessesArr, Pmem * PhysicalMem, Swap * SwapArr);
string replaceSpaceTab(string rawInput);
void parseline(string procInput, Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr);
int FIFO(Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr, int length);
int LRU(Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr, int length);
int Random(Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr, int length);

int instructionCount;
int processCount = 0;
int counter = 0;
int allocated = 0;
bool runFIFO = false;
bool runLRU = false;
bool runRandom = false;

int main() {

	string filelocation;
	int choice = 0;
	bool failed;
	bool swapped = true;
	
	bool badchoice;

	

	cout<<"Enter location of input file: ";
	cin>>filelocation;



		failed = true;
		runFIFO = false;
		runLRU = false;
		runRandom = true;
		
		//badchoice = false;
		filelocation = filelocation;
		Processes* ProcessArr = new Processes[100];
		Pmem* PhysicalMem = new Pmem[20]; // allocate 20 physical memory
		Swap* SwapArr = new Swap[100];

		while(failed)
		{
			(importProcesses(filelocation, ProcessArr, PhysicalMem, SwapArr)) ? failed = false : failed = true;
			if(failed)
			{
				cout << "Bad file location please try again"<< endl << endl;
				cout << "Enter location of input file: ";
				cin >> filelocation;
			}
		}

		cout <<"1. FIFO " << endl << "2. LRU " << endl << "3. Random" << endl << endl;
		cout << "What algorithm would you like to run?  ";
		cin >> choice;

		while(badchoice)
		{
			((choice >= 1) && (choice <= 3)) ? badchoice = false: badchoice = true;

			if(badchoice)
			{
				cout << choice << " is not in the specified range" << endl << endl;
				cout <<"1. FIFO " << endl << "2. LRU " << endl << "3. Random" << endl << endl;
				cout << "What algorithm would you like to run?  ";
				cin >> choice;
			}
			
		}

		switch(choice)
		{
			case 1: 
				runFIFO = true;
				runLRU = false;
				runRandom = false;
				break;
			case 2:
				runFIFO = false;
				runLRU = true;
				runRandom = false;
				break;
			case 3:
				runFIFO = false;
				runLRU = false;
				runRandom = true;
				break;
			case 4:
				exit(1);
			

		}

		for (int idx = 0; idx < processCount; idx++)
		{
			if(!ProcessArr[idx].getTerminated())
			{
				cout << "PROCESS " <<ProcessArr[idx].getPID() << endl;
				for(int i = 0; i < ProcessArr[idx].count; i++)
				{
					cout << "	Virtual " << ProcessArr[idx].getVM(i);
					for(int idc = 0; idc < 20; idc++)
					{
						if(PhysicalMem[idc].getPID() == ProcessArr[idx].getPID())
						{
							if(PhysicalMem[idc].getVM() == ProcessArr[idx].getVM(i))
							{
								cout << "	Physical " << PhysicalMem[idc].getVM() << endl;
								swapped = false;
							}
						}
					}
					if(swapped)
					{
						cout << "	Swap" << endl;
					}
					swapped = true;
				}
			}
		}
		cout << endl <<  "PHYSICAL" << endl << endl;
		for(int idx = 0; idx < 20; idx++)
		{
			cout << "	" << idx + 1 << "		";
			(PhysicalMem[idx].getFree()) ? cout << "FREE" << endl : cout << "Process	" <<  PhysicalMem[idx].getPID() << endl;
		}
	
}

bool importProcesses(string argFileLocation, Processes * ProcessesArr, Pmem * PhysicalMem, Swap * SwapArr)
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
		parseline(temp[i], ProcessesArr, PhysicalMem, SwapArr);
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

void parseline(string Input, Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr)
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
	if(newPID && (jobdata[0] != ""))
	{
		newProcess[processCount].setPID(jobdata[0]);
		newProcess[processCount].firstTouched = clock();
		newProcess[processCount].lastTouched = clock();
		processCount++;
	}
	else
	{
		if(!newProcess[PID].getKilled())
		{
			

			if(instruction == "A")
			{
				if(newProcess[PID].getTerminated())
				{
					newProcess[PID].setKilled(true);
					cout << "killing3 " << newProcess[PID].getPID() << endl;
					for(int idx = 0; idx < 20; idx++)
					{
						if(PhysicalMem[idx].getPID() == jobdata[0])
						{
							PhysicalMem[idx].setFree(true);
							PhysicalMem[idx].setPID("");
							PhysicalMem[idx].setVM("");
						}
					}
				}
				else
				{
					
					for(int idx = 0; idx < 20; idx++)
					{
						if(PhysicalMem[idx].getFree())
						{
							allocated = idx;
							idx = 21;
						}
					}
					if(allocated == 20)
					{

						if(runFIFO)
						{
							int toreplace = 0;
							toreplace = FIFO(newProcess, PhysicalMem, SwapArr, processCount);
							newProcess[PID].setVM(jobdata[1], newProcess[PID].count);
							PhysicalMem[toreplace].setVM(newProcess[PID].getVM(newProcess[PID].count));
							PhysicalMem[toreplace].setPID(jobdata[0]);
							PhysicalMem[toreplace].setFree(false);
							newProcess[PID].count++;
						}
						if(runLRU)
						{
							int toreplace = 0;
							toreplace = LRU(newProcess, PhysicalMem, SwapArr, processCount);
							newProcess[PID].setVM(jobdata[1], newProcess[PID].count);
							PhysicalMem[toreplace].setVM(newProcess[PID].getVM(newProcess[PID].count));
							PhysicalMem[toreplace].setPID(jobdata[0]);
							PhysicalMem[toreplace].setFree(false);
							newProcess[PID].count++;
							newProcess[PID].lastTouched = clock();
						}
						if(runRandom)
						{
							int toreplace = 0;
							toreplace = Random(newProcess, PhysicalMem, SwapArr, processCount);
							newProcess[PID].setVM(jobdata[1], newProcess[PID].count);
							PhysicalMem[toreplace].setVM(newProcess[PID].getVM(newProcess[PID].count));
							PhysicalMem[toreplace].setPID(jobdata[0]);
							PhysicalMem[toreplace].setFree(false);
							newProcess[PID].count++;
							newProcess[PID].lastTouched = clock();
						}
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
					newProcess[PID].lastTouched = clock();
				}
				
				
				
				
			}
			else if(instruction == "W")
			{
				int vmcounter = 0;
				bool exist = false;
				newProcess[PID].lastTouched = clock();
				for(int i = 0; i < 200; i++)
				{
					if(jobdata[1] == newProcess[PID].getVM(vmcounter))
					{
						int dirtycount = 0;
						exist = true;
						//cout << "got here" << endl;
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
					// int swaparr = 0;
					

					newProcess[PID].setKilled(true);
					cout << "killing1 " << newProcess[PID].getPID() << endl;
					for(int idx = 0; idx < 20; idx++)
					{
						if(PhysicalMem[idx].getPID() == jobdata[0])
						{
							PhysicalMem[idx].setFree(true);
						}
					}
				}
			}
			else if(instruction == "R")
			{
				int vmcounter = 0;
				bool exist = false;
				bool dirty = false;
				newProcess[PID].lastTouched = clock();
				//while(newProcess[PID].getVM(vmcounter) !=  "")
				for(int i = 0; i < 200; i++)
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
							newProcess[PID].setKilled(true);
							cout << "killing2 " << newProcess[PID].getPID() << endl;
							for(int idx = 0; idx < 20; idx++)
							{
								if(PhysicalMem[idx].getPID() == jobdata[0])
								{
									PhysicalMem[idx].setFree(true);
								}
							}
						}
					}
					vmcounter++;
				}
				if(!exist)
				{
					newProcess[PID].setKilled(true);
					cout << "killing3 " << newProcess[PID].getPID() << endl;
					for(int idx = 0; idx < 20; idx++)
					{
						if(PhysicalMem[idx].getPID() == jobdata[0])
						{
							PhysicalMem[idx].setFree(true);
						}
					}
				}
			}
			else if(instruction == "F")
			{
				int vmcounter = 0;
				bool exist = false;
				newProcess[PID].lastTouched = clock();
				for(int i = 0; i < 200; i++)
				{
					if(jobdata[1] == newProcess[PID].getVM(vmcounter))
					{
						exist = true;
						for(int idx = 0; idx < 20; idx++)
						{
							if((PhysicalMem[idx].getVM() == jobdata[1]) && (PhysicalMem[idx].getPID() == newProcess[PID].getPID()))
							{
								PhysicalMem[idx].setFree(true);
							}
						}
						newProcess[PID].setVM("", vmcounter);
					}
					vmcounter++;
				}
				if(!exist)
				{
					
					newProcess[PID].setKilled(true);
					cout << "killing4 " << newProcess[PID].getPID() << endl;
					for(int idx = 0; idx < 20; idx++)
					{
						if(PhysicalMem[idx].getPID() == jobdata[0])
						{
							PhysicalMem[idx].setFree(true);
						}
					}
				}
			}
			else if(instruction == "T")
			{
				for(int i = 0; i < 200; i++)
				{
					newProcess[PID].setVM("", i);
				}
				newProcess[PID].setTerminated(true);

				for(int idx = 0; idx < 20; idx++)
				{
					if(PhysicalMem[idx].getPID() == jobdata[0])
					{
						PhysicalMem[idx].setFree(true);
					}
				}
			}
			else if(instruction == "C")
			{
				newProcess[PID].setTerminated(false);
			}
		}	
	}
}

int FIFO(Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr, int length)
{
	int first = 0;
	string vmswap[200];
	int vmswapCount = 0;
	int swapchoice = 0;
	int swaparrayidx = 0;
	int pmswapped = 0;

	for(int i = 0; i < 200; i++)
	{
		vmswap[i] = "";
	}

	for(int idx = 0; idx < length; idx++)
	{
		if((int)newProcess[idx].firstTouched < (int)newProcess[first].firstTouched)
		{

			first = idx;
			
		}
	}
	for(int i = 0; i < 200; i++)
	{
		if(newProcess[first].getVM(i) != "")
		{
			vmswap[vmswapCount] = newProcess[first].getVM(i);
			vmswapCount++;
		}
	}

	srand(time(NULL));
	swapchoice = rand() % vmswapCount + 0;

	while(SwapArr[swaparrayidx].getVM() != "")
	{
		swaparrayidx++;
	}
	//add to the list of swapped vms
	SwapArr[swaparrayidx].setVM(newProcess[first].getVM(swapchoice));
	//free up the space in the physical memory
	for (int idx = 0; idx < 20; idx++)
	{
		if(PhysicalMem[idx].getPID() == newProcess[first].getPID())
		{
			if(PhysicalMem[idx].getVM() == newProcess[first].getVM(swapchoice))
			{
				PhysicalMem[idx].setFree(true);
				PhysicalMem[idx].setVM("");
				pmswapped = idx;

			}
		}

	}
	return pmswapped;
}

int LRU(Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr, int length)
{
	int lasttouched = 0;
	string vmswap[200];
	int vmswapCount = 0;
	int swapchoice = 0;
	int swaparrayidx = 0;
	int pmswapped = 0;
	
	for(int i = 0; i < 200; i++)
	{
		vmswap[i] = "";
	}

	//cout << length << endl;
	for(int i = 0; i < length; i++)
	{
		//cout << i << endl;
		//cout << lasttouched << endl << endl;
		if((int)newProcess[i].lastTouched > (int)newProcess[lasttouched].lastTouched)
		{

			lasttouched = i;
			
		}

	}
	
	//cout << newProcess[lasttouched].getPID() << endl;
	for(int i = 0; i < 200; i++)
	{
		
		if(newProcess[lasttouched].getVM(i) != "")
		{
			vmswap[vmswapCount] = newProcess[lasttouched].getVM(i);
			vmswapCount++;
		}
	}

	cout << vmswapCount << endl;
	srand(time(NULL));
	swapchoice = rand() % vmswapCount + 0;
	cout << "sc " << swapchoice << endl << endl;

	while(SwapArr[swaparrayidx].getVM() != "")
	{
		swaparrayidx++;
	}
	//add to the list of swapped vms
	SwapArr[swaparrayidx].setVM(newProcess[lasttouched].getVM(swapchoice));
	//free up the space in the physical memory
	
	for (int idx = 0; idx < 20; idx++)
	{
		if(PhysicalMem[idx].getPID() == newProcess[lasttouched].getPID())
		{
			if(PhysicalMem[idx].getVM() == newProcess[lasttouched].getVM(swapchoice))
			{
				cout <<"to swap:"<< endl;
				cout << PhysicalMem[idx].getPID() << " ";
				cout << PhysicalMem[idx].getVM() << endl;
				cout << "idx " << idx << endl<< endl;
				PhysicalMem[idx].setFree(true);
				PhysicalMem[idx].setVM("");
				pmswapped = idx;

			}
		}

	}
	return pmswapped;
}

int Random(Processes * newProcess, Pmem * PhysicalMem, Swap * SwapArr, int length)
{
	int random = 0;
	string vmswap[200];
	int vmswapCount = 0;
	int swapchoice = 0;
	int swaparrayidx = 0;
	int pmswapped = 0;

	for(int i = 0; i < 200; i++)
	{
		vmswap[i] = "";
	}

	srand(rand());
	random = rand() % length + 0;

	for(int i = 0; i < 200; i++)
	{
		if(newProcess[random].getVM(i) != "")
		{
			vmswap[vmswapCount] = newProcess[random].getVM(i);
			vmswapCount++;
		}
	}

	srand(time(NULL));
	swapchoice = rand() % vmswapCount + 0;

	while(SwapArr[swaparrayidx].getVM() != "")
	{
		swaparrayidx++;
	}
	//add to the list of swapped vms
	SwapArr[swaparrayidx].setVM(newProcess[random].getVM(swapchoice));
	//free up the space in the physical memory
	for (int idx = 0; idx < 20; idx++)
	{
		if(PhysicalMem[idx].getPID() == newProcess[random].getPID())
		{
			if(PhysicalMem[idx].getVM() == newProcess[random].getVM(swapchoice))
			{
				PhysicalMem[idx].setFree(true);
				PhysicalMem[idx].setVM("");
				pmswapped = idx;

			}
		}

	}
	return pmswapped;
}