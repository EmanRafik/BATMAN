#ifndef MONITOR_H
#define MONITOR_H

#include <BAT.h>


class Monitor
{
    public:

        virtual ~Monitor();
        static Monitor *getInstance();
        void arrive(BAT b);
        void cross(BAT b);
        void leave(BAT b);
        void check();

    protected:

    private:
        Monitor();
        static Monitor *instance;
        int northCount, eastCount, southCount, westCount;
};

#endif // MONITOR_H
