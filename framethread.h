#ifndef FRAMETHREAD_H
#define FRAMETHREAD_H

#include "processedframe.h"

#include <QThread>
#include <QDebug>

class FrameThread : public QThread
{
    Q_OBJECT
public:
    FrameThread(Mat frame, int index, int test, QObject *parent = nullptr);
    ProcessedFrame *current() const;
    void process(Mat frame, int index);
protected:
    void run();
private:
    ProcessedFrame *m_current;
    int m_test;
    Ptr< BackgroundSubtractor> m_pMOG2;
};

#endif // FRAMETHREAD_H
