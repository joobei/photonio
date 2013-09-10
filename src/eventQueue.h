#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <deque>
#include "data.pb.h"
#include <iostream>
#include <boost/array.hpp>

using namespace keimote;

namespace pho {

class EventQueue {
public:

    EventQueue();

    void push(keimote::PhoneEvent event);
    void push(boost::array<float, 14>);

    bool isEmpty();
    bool isSerialEmpty();

    keimote::PhoneEvent pop();
    boost::array<float, 14> serialPop();

private:
    std::deque<keimote::PhoneEvent> queue;
    std::deque<boost::array<float,14> > serialQueue;
};

}

#endif // EVENTQUEUE_H
