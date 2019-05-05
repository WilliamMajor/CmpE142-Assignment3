#ifndef PROCESSES_H
#define PROCESSES_H

#include <iostream>
using namespace std;

class Processes
{
    public:
        Processes();
        //Processes(string PID);
        ~Processes();
        string getPID() const;
        int getPM() const;
        int getVM(int i) const;
        bool getKilled() const;
        bool getTerminated() const;
        
        void setPID(string argPID);
        void setVM(int argVM, int i);
        void setPM(int argPM);
        void setKilled(bool argKilled);
        void setTerminated(bool argKilled); 
        clock_t lastTouched;

    private:
        string  PID;
        int  VM[200];
        int     PM;
        bool    killed;
        bool    Terminated;
        
};
#endif