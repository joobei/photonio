#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <deque>
#include "data.pb.h"
#include <iostream>
#include <boost/array.hpp>

using namespace keimote;

namespace pho {

enum InputTechnique {
    FPC,  //Free PlaneCasting
    PPC,  //Pivot PlaneCasting
    SN   //Space Navigator
};

enum App {
    Steering,
    Stacking,
    Docking
};

class EventQueue {
public:

    EventQueue();

    void push(keimote::PhoneEvent event);
    void push(boost::array<float, 7>);

    bool isEmpty();
    bool isSerialEmpty();

    keimote::PhoneEvent pop();
    boost::array<float, 7> serialPop();

    void setTechnique(InputTechnique newTechnique);
    void setApp(App newApp);
    InputTechnique getTechnique();

    App getApp();

private:
    std::deque<keimote::PhoneEvent> queue;
    std::deque<boost::array<float,7> > serialQueue;
    InputTechnique technique;
    App application;

};

}

#endif // EVENTQUEUE_H
