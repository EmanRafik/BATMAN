#include "BAT.h"
#include <iostream>
#include <pthread.h>
#include <Monitor.h>
#include <unistd.h>

using namespace std;

BAT::BAT(int id)
{
    //ctor
    this->id = id;
}

BAT::~BAT()
{
    //dtor
}

void BAT::setDirection(char direction)
{
    this->directionID = direction;
    switch(direction)
    {
    case 'n':
        this->direction = "north";
        break;
    case 'e':
        this->direction = "east";
        break;
    case 's':
        this->direction = "south";
        break;
    case 'w':
        this->direction = "west";
        break;
    }
}

int BAT::getId()
{
    return this->id;
}

char BAT::getDirectionID()
{
    return this->directionID;
}

string BAT::getDirection()
{
    return this->direction;
}

void BAT::start()
{
    Monitor *m = Monitor::getInstance();
    m->arrive(*this);
    m->cross(*this);
    m->leave(*this);
}

