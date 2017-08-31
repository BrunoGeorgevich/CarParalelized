#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller *controller = new Controller("../assets/video.avi");

    return a.exec();
}
