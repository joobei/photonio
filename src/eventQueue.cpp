#include "data.pb.h"
#include "eventQueue.h"

namespace pho {

EventQueue::EventQueue() :
    technique(FPC),application(Docking) {
    queue.clear();
}

void EventQueue::push(keimote::PhoneEvent event) {
    queue.push_front(event);
}

void EventQueue::push(boost::array<float,7> tEvent ) {
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

boost::array<float, 7> EventQueue::serialPop() {
    boost::array<float,7> temp;
    temp = serialQueue.front();
    serialQueue.pop_front();
    return temp;
}

void EventQueue::setTechnique(InputTechnique newTechnique) {
    technique = newTechnique;
}

void EventQueue::setApp(App newApp) {
    application = newApp;
}


pho::InputTechnique EventQueue::getTechnique() {
    return technique;
}

pho::App EventQueue::getApp(){
    return application;
}
}
