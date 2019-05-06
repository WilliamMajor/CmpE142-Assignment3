#ifndef SWAP_H
#define SWAP_H

#include <iostream>
using namespace std;

class Swap
{
    public:
        Swap();
        ~Swap();
        string getPID() const;
        string getVM() const;
        
        void setPID(string argPID);
        void setVM(string argVM);

    private:
        string  PID;
        string  VM;
        
};
#endif