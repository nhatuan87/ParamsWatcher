#include "ParamsWatcher.h"

list<ParamsWatcher *> ParamsWatcher::watchers;

void ParamsWatcher::update()
{
    for (list<ParamsWatcher *>::iterator it = watchers.begin(); it != watchers.end(); ++it)
    {
        (*it)->_update();
    }
}