#pragma once

#include <list>

using namespace std;

class ParamsWatcher
{
public:
    ParamsWatcher(){};
    static void update();
    static list<ParamsWatcher *> watchers;

private:
    virtual void _update() = 0;
};

template <typename T>
class Watcher : public ParamsWatcher
{
public:
    Watcher(T &obj)
        : _obj(obj)
    {
        watchers.push_back(this);
    }

    void setCallback(void (*callback)(T))
    {
        _callback = callback;
    }

private:
    T _obj_saved;
    T &_obj;
    void (*_callback)(T);
    bool _changed()
    {
        for (int i = 0; i < sizeof(T); i++)
        {
            if (((char *)&_obj_saved)[i] != ((char *)&_obj)[i])
            {
                return true;
            }
        }
        return false;
    }
    void _update()
    {
        if (_callback and _changed())
        {
            _callback(_obj);
            for (int i = 0; i < sizeof(T); i++)
            {
                ((char *)&_obj_saved)[i] = ((char *)&_obj)[i];
            }
        }
    }
};