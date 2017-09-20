#ifndef FRAMETHREAD_H
#define FRAMETHREAD_H

#include "lanethread.h"

#include <QThread>
#include <QDebug>

class FrameThread : public QThread
{
    Q_OBJECT
public:
    FrameThread(Mat frame, int index, QObject *parent = nullptr);
    ProcessedFrame *current() const;
    void process(Mat frame, int index);
protected:
    void run();
private slots:
    void laneThreadHasFinished(Contours *contours);
private:
    QList<LaneThread *> m_threads;
    ProcessedFrame *m_current;
    int m_checkCount;

    bool isFinished();
};

#endif // FRAMETHREAD_H
