#include "Pmem.h"

using namespace std;

//Constructors
Pmem::Pmem()
{
    Free = true;
}
Pmem::Pmem(string argPID)
{
    PID = argPID;
}

Pmem::~Pmem()
{
}

//Getters
bool Pmem::getFree() const
{
    return Free;
}

string Pmem::getPID() const
{
    return PID;
}

string Pmem::getVM() const
{
    return VM;
}

//Setters
void Pmem::setFree(bool argFree)
{
    Free = argFree;
}

void Pmem::setPID(string argPID)
{
    PID = argPID;
}

void Pmem::setVM(string argVM)
{
    VM = argVM;
}