#include "data.pb.h"
#include "eventQueue.h"

namespace pho {

EventQueue::EventQueue() {
    queue.clear();
}

void EventQueue::push(keimote::PhoneEvent event) {
    queue.push_front(event);
}

void EventQueue::push(boost::array<float,28> tEvent ) {
    serialQueue.push_front(tEvent);
}

bool EventQueue::isEmpty() {
    return queue.empty();
}

bool EventQueue::isSerialEmpty() {
    return serialQueue.empty();
}

keimote::PhoneEvent EventQueue::pop() {
    keimote::PhoneEvent tempMessage;
    tempMessage = queue.front();
    queue.pop_front();
    return tempMessage;
}

boost::array<float, 28> EventQueue::serialPop() {
    boost::array<float,28> temp;
    temp = serialQueue.front();
    serialQueue.pop_front();
    return temp;
}


}
