#include "data.pb.h"
#include "eventQueue.h"

namespace pho {

EventQueue::EventQueue() {
    queue.clear();
}

void EventQueue::push(keimote::PhoneEvent event) {
    queue.push_front(event);
}

bool EventQueue::isEmpty() {
    return queue.empty();
}


keimote::PhoneEvent EventQueue::pop() {
    keimote::PhoneEvent tempMessage;
    tempMessage = queue.front();
    queue.pop_front();
    return tempMessage;
}


}
