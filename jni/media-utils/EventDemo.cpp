#include "EventManager.h"

//========================================================
// Simple demo stuff from here down
//========================================================

// DEMO Notifier is an event broadcaster
class Notifier : public EventBroadcaster
{
public:
    int get() const {return i;};
    void set(int _i) {
        i=_i;
        notify();
    };

private:
    int i ; //C++11 nicety
};

// DEMO Responder is an EventListener
class Responder : public EventListener
{
public:
    Responder(std::string _name = std::string()) : name(_name){}

    ~Responder()
    {
        std::cout << "Responder "<< name <<" deleted" << std::endl;
    }

    void eventCallback(EventBroadcaster* b_ptr)
    {
        auto Notifier_ptr = static_cast<Notifier*>(b_ptr);
        std::cout << name << " notified that value is " << Notifier_ptr->get() << std::endl;
    }

private:
    std::string name;
};

int main_event_demo() {


    Notifier n;
    Responder r1("r1");

    // Test #1 connect and signal
    r1.connect(n);
    n.set(5);

    // Test #2 disconnect at the responder end
    r1.disconnect();
    n.set(7);

    // Test #3 reconnect multiple times
    r1.connect(n);
    r1.connect(n);
    r1.connect(n);
    n.set(9999999);

    // Test #4 disconnect at the broadcaster
    n.removeEventListener(&r1);
    n.removeEventListener(&r1); //Check it can be done more than once
    n.set(7);

    // Test #5 multiple listeners
    Responder  r2("r2"), r3("r3");
    r3.connect(n);
    r2.connect(n);
//    r1.connect(n.addEventListener(&r1)); //Can still use this ugly syntax if you want
    n.set(321);


    n.removeAllEventListeners();
    n.set(2);

    // Test #6 multiple listeners again after a remove all
    r3.connect(n);
    r2.connect(n);
    r1.connect(n);
    n.set(4444);

    // Test #7 this is the cool one showing automatic scoped connection management
    {
        std::unique_ptr<Responder> r(  new Responder("r_heap")  );
        r->connect(n);
        n.set(11);
        n.set(55);
    }

    n.set(88888888); //No heap listener (lambda magic at work)
    return 0;
}
