#include "Swap.h"
#include <iostream>
#include <ctime>


using namespace std;

//Constructors
Swap::Swap()
{
    PID         = ""; 
}

Swap::~Swap()
{
}

//Getter functions
string Swap::getPID() const
{
    return PID;
}

string Swap::getVM() const
{
    return VM;
}



//Setter Functions
void Swap::setPID(string argPID)
{
    PID = argPID;
}

void Swap::setVM(string argVM)
{
    VM = argVM;
}
