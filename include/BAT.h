#ifndef BAT_H
#define BAT_H
#include<iostream>
#include<string>
#include<pthread.h>

using namespace std;

class BAT
{
    public:
        BAT(int id);
        virtual ~BAT();
        void setDirection(char direction);
        int getId();
        char getDirectionID();
        string getDirection();
        void start();

    protected:

    private:
        int id;
        char directionID;
        string direction;
};

#endif // BAT_H
