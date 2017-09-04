#include "framemanager.h"

FrameManager::FrameManager(int numOfThreads, QObject *parent) : QObject(parent)
{
    m_currentIndex = 0;

    for(int i = 0; i < numOfThreads; i++)
        m_threads.append(new FrameThread(Mat(),0,this));
}

void FrameManager::processFrame(Mat frame, int index)
{
    while(m_threads.size() <= 0) {
        qDebug() << "THREAD ARRAY IS 0!";
    }

    FrameThread *ft = m_threads.first();
    m_threads.removeFirst();
    connect(ft, SIGNAL(finished()),
            this, SLOT(processFrameHasFinished()));
    ft->process(frame, index);
}

void FrameManager::init()
{
    foreach (FrameThread *ft, m_threads) {
        emit newThreadAvailable();
    }
}

void FrameManager::processFrameHasFinished()
{
    FrameThread *ft = (FrameThread *)sender();
    disconnect(ft, SIGNAL(finished()),
            this, SLOT(processFrameHasFinished()));
    ProcessedFrame *pf = ft->current();
    emit newProcessedFrame(pf);
    m_threads.append(ft);
    emit newThreadAvailable();
}
