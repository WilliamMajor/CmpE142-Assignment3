#include "Processes.h"
#include <iostream>
#include <ctime>


using namespace std;

//Constructors
Processes::Processes()
{
    PID         = "";

    Terminated  = false;
    killed      = false;
    lastTouched = clock();
    
}

Processes::~Processes()
{
}

//Getter functions
string Processes::getPID() const
{
    return PID;
}

int Processes::getVM(int idx) const
{
    return VM[idx];
}

int Processes::getPM() const
{
    return PM;
}

bool Processes::getKilled() const
{
    return killed;
}

bool Processes::getTerminated() const
{
    return Terminated;
}



//Setter Functions
void Processes::setPID(string argPID)
{
    PID = argPID;
}

void Processes::setVM(int argVM, int idx)
{
    VM[idx] = argVM;
}

void Processes::setPM(int argPM)
{
    PM = argPM;
}

void Processes::setKilled(bool argKilled)
{
    killed  = argKilled;
}

void Processes::setTerminated(bool argTerminated)
{
    Terminated = argTerminated;
}

