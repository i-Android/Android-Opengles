#ifndef __EventManager_H__
#define __EventManager_H__


#include <iostream>
#include <list>
#include <memory>
#include <algorithm>
#include <functional>

class EventBroadcaster;
class EventListener;

//Connection class
class EventConnection
{
public:
    EventConnection(std::function<void ()> _disconnect_handler) :disconnect_handler(_disconnect_handler) {}

    //Handle the disconnect housekeeping when the connection is deleted
    ~EventConnection(){disconnect_handler();}

private:
    std::function<void ()> disconnect_handler;
};

// Pure virtual listener base class declartion
class EventListener
{
public:
    EventListener() :c(nullptr){};
    virtual void eventCallback(EventBroadcaster*) =0;
    void connect(EventBroadcaster& b); // Some nicer syntax
    void disconnect();
    bool isConnected();

private:
    std::unique_ptr<EventConnection> c ; //C++11 allows these to be set without initializer list
    void connect(EventConnection* _c); // This connection syntax is just to ugly and is set to private because of it!!
};


// Broadcaster base class
class EventBroadcaster
{
public:
    ~EventBroadcaster()
    {
        removeAllEventListeners(); //sever all connections on delete
    }

    EventConnection* addEventListener(EventListener* l_ptr)
    {
        // Check to see if listener already added
        if( !binary_search (listenerList.begin(), listenerList.end(), l_ptr) )
        {
            listenerList.push_back(l_ptr);
            //Need to capture by making a copy [=] - wacky things happened with [&] - l_ptr gets munglerated
            return new EventConnection([=](){this->removeEventListener(l_ptr);});
        } else {
            return nullptr;
        }

    }

    void removeEventListener(EventListener* l_ptr)
    {
        // This fololwing command informs the connection object of the disconnect
        // We don't get into an infinite loops because the connection is already null after the first call
        l_ptr->disconnect();
        listenerList.remove(l_ptr);
    }

    void removeAllEventListeners()
    {
        // Can't use clear because we need to tell each listener it has been disconnected
        // Can't use a range-based for on a container with a changing size!
        while(listenerList.size()>0){
            removeEventListener(listenerList.back());
        }
    }

    void notify()
    {
        for(auto l_ptr : listenerList) {
            if(l_ptr){l_ptr->eventCallback(this);}
        }
    }

private:
    std::list<EventListener*> listenerList;
};


//Event listener defninitions
void EventListener::connect(EventConnection* _c)
{
    if(_c) {// Need this if statement or we get a nullptr if this connection already registered
        disconnect(); // Sever any previous connection (these listeners can only observe one object)
        c.reset(_c);
    }
}

void EventListener::connect(EventBroadcaster& b) // Some nicer syntax
{
    connect(b.addEventListener(this));
}

void EventListener::disconnect()
{
    if(c) {c.reset(nullptr);} // No need to null a null
}

bool EventListener::isConnected()
{
    return (c!=nullptr);
}

#endif // __EventManager_H__

