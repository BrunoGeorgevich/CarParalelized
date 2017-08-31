#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <QObject>
#include <QDebug>

#include "framethread.h"

class FrameManager : public QObject
{
    Q_OBJECT
public:
    explicit FrameManager(int numOfThreads, QObject *parent = nullptr);
    void processFrame(Mat frame, int index);
    void init();
signals:
    void newProcessedFrame(ProcessedFrame *pf);
    void newThreadAvailable();
private slots:
    void processFrameHasFinished();
private:
    QList<FrameThread *> m_threads;
    int m_currentIndex;
};

#endif // FRAMEMANAGER_H
