#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <deque>
#include "data.pb.h"
#include <iostream>
#include <boost/array.hpp>

using namespace keimote;

namespace pho {

enum App {
    Steering,
    Stacking,
    Docking
};

class EventQueue {
public:
    EventQueue();
    void push(keimote::PhoneEvent event);
    bool isEmpty();
    keimote::PhoneEvent pop(); 
private:
    std::deque<keimote::PhoneEvent> queue;
};

}

#endif // EVENTQUEUE_H
