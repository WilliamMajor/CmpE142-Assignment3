#ifndef PMEM_H
#define PMEM_H

#include <iostream>
using namespace std;

class Pmem
{
    public:
        Pmem();
        Pmem(string argPID);
        ~Pmem();
        string getPID() const;
        string getVM() const;
        bool getFree()  const;
        
        void setPID(string argPID);
        void setVM(string argVM);
        void setFree(bool argFree);

    private:
        string  PID;
        string  VM;
        bool    Free;
        
};
#endif