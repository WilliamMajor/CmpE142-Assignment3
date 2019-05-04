#include "Processes.h"
#include <iostream>
#include <ctime>
#include <chrono>


using namespace std;

//Constructors
Processes::Processes()
{
    PID         = 0;
    A           = "";
    Page        = 0;
    lastTouched = clock();
    
}

Processes::~Processes()
{
}

Processes::Processes(int argPID, int argA, int argPage)
{
    PID     = argPID;
    A       = argA;
    Page    = argPage;
}

//Getter functions
int Processes::getPID() const
{
    return PID;
}

string Processes::getA() const
{
    return A;
}

int Processes::getPage() const
{
    return Page;
}


//Setter Functions
void Processes::setPID(int argPID)
{
    PID = argPID;
}

void Processes::setA(string argA)
{
    A = argA;
}

void Processes::setPage(int argPage)
{
    Page = argPage;
}

