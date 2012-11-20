#ifndef MYTUIO_H
#define MYTUIO_H

#include "TUIO/TuioClient.h"
#include "TUIO/TuioListener.h"
#include "TUIO/TuioObject.h"
#include "TUIO/TuioCursor.h"
#include "TUIO/TuioPoint.h"
#include "glm/glm.hpp"

using namespace TUIO;

namespace pho {

class Engine;

class MyTuioListener: public TuioListener {

public:
    MyTuioListener(int port,pho::Engine* tehEngine);
    ~MyTuioListener() {
            tuioClient->disconnect();
            delete tuioClient;
    }

    void addTuioObject(TuioObject *tobj);
    void updateTuioObject(TuioObject *tobj);
    void removeTuioObject(TuioObject *tobj);

    void addTuioCursor(TuioCursor *tcur);
    void updateTuioCursor(TuioCursor *tcur);
    void removeTuioCursor(TuioCursor *tcur);

    void refresh(TuioTime frameTime);
    TuioClient* tuioClient;
private:
    bool verbose;
    pho::Engine* engine;

};

}

#endif
