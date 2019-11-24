#include "Monitor.h"
#include <BAT.h>
#include<iostream>
#include <unistd.h>

Monitor *Monitor::instance=0;
pthread_mutex_t lock, crossing;
pthread_cond_t northQueue, southQueue, westQueue, eastQueue;
pthread_cond_t northFirst, southFirst, westFirst, eastFirst;
bool deadlock;
bool northWaiting, southWaiting, eastWaiting, westWaiting;

Monitor::Monitor()
{
    //ctor
    northCount = 0;
    eastCount = 0;
    southCount = 0;
    westCount = 0;

    deadlock = false;
    northWaiting = false;
    southWaiting = false;
    eastWaiting = false;
    westWaiting = false;
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&crossing, NULL);

    pthread_cond_init(&northFirst, NULL);
    pthread_cond_init(&southFirst, NULL);
    pthread_cond_init(&eastFirst, NULL);
    pthread_cond_init(&westFirst, NULL);

    pthread_cond_init(&northQueue, NULL);
    pthread_cond_init(&southQueue, NULL);
    pthread_cond_init(&eastQueue, NULL);
    pthread_cond_init(&westQueue, NULL);
}

Monitor::~Monitor()
{
    //dtor
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&crossing);

    pthread_cond_destroy(&northFirst);
    pthread_cond_destroy(&southFirst);
    pthread_cond_destroy(&eastFirst);
    pthread_cond_destroy(&westFirst);

    pthread_cond_destroy(&northQueue);
    pthread_cond_destroy(&southQueue);
    pthread_cond_destroy(&eastQueue);
    pthread_cond_destroy(&westQueue);
}

Monitor *Monitor::getInstance()
{
    if (instance == 0)
        instance = new Monitor();
    return instance;
}

void Monitor::arrive(BAT b)
{
    pthread_mutex_lock(&lock);
    cout << "BAT " << b.getId() << " from " << b.getDirection() << " arrived at crossing." << endl;
    switch(b.getDirectionID())
    {
    case 'n':
        northCount++;
        if (northCount > 1)
        {
            pthread_cond_wait(&northQueue, &lock);
        }
        break;
    case 's':
        southCount++;
        if (southCount > 1)
        {

            pthread_cond_wait(&southQueue, &lock);
        }
        break;
    case 'e':
        eastCount++;
        if (eastCount > 1)
        {
            pthread_cond_wait(&eastQueue, &lock);
        }
        break;
    case 'w':
        westCount++;
        if (westCount > 1)
        {
            pthread_cond_wait(&westQueue, &lock);
        }
        break;
    }
    pthread_mutex_unlock(&lock);
}

void Monitor::cross(BAT b)
{
    pthread_mutex_lock(&crossing);
    //cout << "BAT " << b.getId() << " from " << b.getDirection() << " ENTERED crossing." << endl;
    switch(b.getDirectionID())
    {
    case 'n':
        pthread_mutex_lock(&lock);
        if (westCount > 0)
        {
            northWaiting = true;
            pthread_mutex_unlock(&lock);
            pthread_cond_wait(&westFirst, &crossing);
            pthread_mutex_lock(&lock);
            northWaiting = false;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
        }
        break;
    case 's':
        pthread_mutex_lock(&lock);
        if (eastCount > 0)
        {
            southWaiting = true;
            pthread_mutex_unlock(&lock);
            pthread_cond_wait(&eastFirst, &crossing);
            pthread_mutex_lock(&lock);
            southWaiting = false;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
        }
        break;
    case 'e':
        pthread_mutex_lock(&lock);
        if (northCount > 0)
        {
            eastWaiting = true;
            pthread_mutex_unlock(&lock);
            pthread_cond_wait(&northFirst, &crossing);
            pthread_mutex_lock(&lock);
            eastWaiting = false;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
        }
        break;
    case 'w':
        pthread_mutex_lock(&lock);
        if (southCount > 0)
        {
            westWaiting = true;
            pthread_mutex_unlock(&lock);
            pthread_cond_wait(&southFirst, &crossing);
            pthread_mutex_lock(&lock);
            westWaiting = false;
            deadlock = false;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
        }
        break;
    }
    cout << "BAT " << b.getId() << " from " << b.getDirection() << " crossing.\n";
    sleep(1);
}

void Monitor::leave(BAT b)
{
    pthread_mutex_lock(&lock);
    cout << "BAT " << b.getId() << " from " << b.getDirection() << " leaving crossing.\n";
    switch(b.getDirectionID())
    {
    case 'n':
        northCount--;
        if (eastWaiting)
        {
            pthread_cond_signal(&northFirst);
        }
        if (northCount > 0)
        {
            pthread_cond_signal(&northQueue);
        }
        break;
    case 's':
        southCount--;
        if (westWaiting)
        {
            pthread_cond_signal(&southFirst);
        }
        if (southCount > 0)
        {
            pthread_cond_signal(&southQueue);
        }
        break;
    case 'e':
        eastCount--;
        if (southWaiting)
        {
            pthread_cond_signal(&eastFirst);
        }
        if (eastCount > 0)
        {
            pthread_cond_signal(&eastQueue);
        }
        break;
    case 'w':
        westCount--;
        if (northWaiting)
        {
            pthread_cond_signal(&westFirst);
        }
        if (westCount > 0)
        {
            pthread_cond_signal(&westQueue);
        }
        break;
    }
    pthread_mutex_unlock(&crossing);
    pthread_mutex_unlock(&lock);
}

void Monitor::check()
{
    pthread_mutex_lock(&lock);
    if (northWaiting && southWaiting && eastWaiting && westWaiting && !deadlock)
    {
        deadlock = true;
        cout << "DEADLOCK: BAT jam detected, signalling North to go" << endl;
        pthread_cond_signal(&westFirst);
    }
    pthread_mutex_unlock(&lock);
}
