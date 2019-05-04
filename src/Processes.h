#ifndef PROCESSES_H
#define PROCESSES_H

#include <iostream>
using namespace std;

class Processes
{
    public:
        Processes();
        Processes(int PID, int A, int Page);
        ~Processes();
        int getPID() const;
        string getA() const;
        int getPage() const;
        void setPID(int argPID);
        void setA(string argA);
        void setPage(int argPage);
        clock_t lastTouched;

    private:
        int     PID;
        string  A;
        int     Page;
        
};
#endif