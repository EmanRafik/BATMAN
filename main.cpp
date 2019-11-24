#include <iostream>
#include <BAT.h>
#include <Monitor.h>
#include <unistd.h>

using namespace std;

int idCount = 0;

struct parameters
{
    int id;
    char direction;
};

void *checker_func(void *args)
{
    while (1)
    {
        Monitor *m = Monitor::getInstance();
        m->check();
    }
}

void *create_BAT(void *args)
{
    struct parameters *p = (struct parameters*)args;
    BAT *b = new BAT((*p).id);
    b->setDirection((*p).direction);
    b->start();
}

int main()
{
    pthread_t checker;
    pthread_create(&checker,NULL, &checker_func, NULL);
    string input;
    cin >> input;
    pthread_t threads[input.length()];
    for (unsigned int i = 0; i < input.length(); i++)
    {
        if (input.at(i) != 'n' && input.at(i) != 's' && input.at(i) != 'e' && input.at(i) != 'w')
        {
            cout << "Invalid input";
            return 0;
        }
        struct parameters *p = (struct parameters *) malloc(sizeof(struct parameters));
        (*p).id= ++idCount;
        (*p).direction = input.at(i);
        pthread_create(&threads[i], NULL, &create_BAT, (void *)p);
    }

    for (unsigned int i = 0; i < input.length(); i++)
    {
        pthread_join(threads[i],NULL);
    }
    return 0;
}
