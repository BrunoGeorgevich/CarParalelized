#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ObjectDetector *objDetect = ObjectDetector::getInstance();
    objDetect->setDetectionParams(20, 6, 5, 20);
    objDetect->addNewLane(Point(0, 84), Point(160, 84), Rect(0, 40, 180, 100), "Sul");
    objDetect->addNewLane(Point(120, 144), Point(274, 144), Rect(110, 100, 180, 100), "Norte");

//    Controller *controller = new Controller("rtmp://midia.smtt.infonet.com.br:1944/live/smttcamera008 ");

    Controller *controller = new Controller("../assets/video.avi", 4);

    return a.exec();
}
