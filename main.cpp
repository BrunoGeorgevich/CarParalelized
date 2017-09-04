#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller *controller = new Controller("rtmp://midia.smtt.infonet.com.br:1944/live/smttcamera008 ");
//    Controller *controller = new Controller("../assets/video.avi");

    return a.exec();
}
