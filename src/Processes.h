#ifndef PROCESSES_H
#define PROCESSES_H

#include <iostream>
using namespace std;

class Processes
{
    public:
        Processes();
        ~Processes();
        string getPID() const;
        //int getPM() const;
        string getVM(int i) const;
        bool getKilled() const;
        bool getTerminated() const;
        
        void setPID(string argPID);
        void setVM(string argVM, int i);
        //void setPM(int argPM);
        void setKilled(bool argKilled);
        void setTerminated(bool argKilled); 
        clock_t lastTouched;

        int count;

    private:
        string  PID;
        string  VM[200];
        string  dirty[200];
        //int     PM;
        bool    killed;
        bool    Terminated;
        
};
#endif