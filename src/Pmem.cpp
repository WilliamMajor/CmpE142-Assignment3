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

//Setters
void Pmem::setFree(bool argFree)
{
    Free = argFree;
}

void Pmem::setPID(string argPID)
{
    PID = argPID;
}