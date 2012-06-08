#include "myTuio.h"

namespace pho {

MyTuioListener::MyTuioListener(int port, Engine* tehEngine):engine(tehEngine) {

    TuioClient* tuioClient = new TuioClient(port);
    tuioClient->addTuioListener(this);
    tuioClient->connect();

    if (!tuioClient->isConnected()) {
        std::cout << "tuio client connection failed" << std::endl;
    }

    verbose = true;
}

void MyTuioListener::addTuioObject(TuioObject *tobj) {
    if (verbose)
        std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;

}

void MyTuioListener::updateTuioObject(TuioObject *tobj) {

    if (verbose)
        std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle()
        << " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;

}

void MyTuioListener::removeTuioObject(TuioObject *tobj) {

    if (verbose)
        std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void MyTuioListener::addTuioCursor(TuioCursor *tcur) {

    if (verbose)
        std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;

}

void MyTuioListener::updateTuioCursor(TuioCursor *tcur) {

    if (verbose)
        std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
                    << " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
}

void MyTuioListener::removeTuioCursor(TuioCursor *tcur) {

    if (verbose)
        std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void  MyTuioListener::refresh(TuioTime frameTime) {
    //std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

}
