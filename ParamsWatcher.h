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
    void _update()
    {
        if (_obj != _obj_saved and _callback)
        {
            _callback(_obj);
            _obj_saved = _obj;
        }
    }
};