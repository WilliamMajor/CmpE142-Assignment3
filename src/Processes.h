#ifndef PROCESSES_H
#define PROCESSES_H

#include <iostream>
using namespace std;

class Processes
{
    public:
        //Constructors
        Processes();
        ~Processes();

        //Getters
        string getPID() const;
        string getVM(int i) const;
        string getDirty(int i) const;
        bool getKilled() const;
        bool getTerminated() const;
        
        //Setters
        void setPID(string argPID);
        void setVM(string argVM, int i);
        void setDirty(string argDirty, int i);
        void setKilled(bool argKilled);
        void setTerminated(bool argKilled); 
        clock_t lastTouched;
        clock_t firstTouched;

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