#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QHash>

#include "framemanager.h"

using namespace cv;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(char *path, int numOfThreads, QObject *parent = nullptr);
private slots:
    void appendIntoProcessedFrameBuffer(ProcessedFrame *pf);
    void processAnotherFrame();
    void updateFrame();
private:
    QHash<int, ProcessedFrame *> m_buffer;
    VideoCapture *m_capture;
    FrameManager *m_fm;
    QTimer *m_timer;

    bool m_hasInit;
    int m_checkCount;
    int m_currentIndex;
};

#endif // CONTROLLER_H
