#pragma once

#include <Arduino.h>
#include <list>

using namespace std;

class ParamsWatcher
{
public:
    ParamsWatcher()
    {
        watchers.push_back(this);
    };
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
        : ParamsWatcher(), _obj(obj) {}

    void setCallback(void (*callback)(T))
    {
        for (int i = 0; i < sizeof(T); i++)
        {
            ((char *)&_obj_saved)[i] = ((char *)&_obj)[i];
        }
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
            for (int i = 0; i < sizeof(T); i++)
            {
                ((char *)&_obj_saved)[i] = ((char *)&_obj)[i];
            }
            _callback(_obj);
        }
    }
};

class TimeWatcher : public ParamsWatcher
{
public:
    TimeWatcher() : ParamsWatcher() {}

    void setCallback(void (*callback)(), unsigned int period = 1)
    {
        _callback = callback;
        _period = period;
    }

private:
    unsigned int _period;
    unsigned long _time_saved;
    void (*_callback)();
    bool _changed()
    {
        if (_period == 0) {
            return false;
        }
        return _time_saved != (millis() / _period);
    }
    void _update()
    {
        if (_callback and _changed())
        {
            _time_saved = millis() / _period;
            _callback();
        }
    }
};