#include "framethread.h"

FrameThread::FrameThread(Mat frame, int index, QObject *parent) : QThread(parent)
{
    m_current = new ProcessedFrame(frame, index, vector<vector<Point>>(), this);
    int numOfLanes = ObjectDetector::getInstance()->numOfLanes();

    for(int i = 0; i < numOfLanes; i++)
        m_threads.append(new LaneThread(frame,i, this));

    foreach (LaneThread *thread, m_threads)
        connect(thread, SIGNAL(finished()),
                this, SLOT(laneThreadHasFinished()));
}

void FrameThread::run()
{
    m_current->clear();
    m_checkCount = 0;
    foreach (LaneThread *thread, m_threads)
        thread->start();
}

void FrameThread::laneThreadHasFinished()
{
    LaneThread *thread = (LaneThread *)sender();

    qDebug() << m_current->index() << thread->laneIndex() << "T ::" << m_checkCount << "MAX ::" << ObjectDetector::getInstance()->numOfLanes() << "FT ::" << this;

    ProcessedFrame *pf = thread->processedFrame();
    foreach (vector<Point> v, pf->contours()) {
        m_current->contours().push_back(v);
    }

    qDebug() << "CONTOURS ::" << m_current->contours().size();

    if(m_checkCount ==  m_threads.size() - 1)
        emit frameThreadHasFinished();

    ++m_checkCount;
}

ProcessedFrame *FrameThread::current() const
{
    return m_current;
}

void FrameThread::process(Mat frame, int index)
{
    m_current = new ProcessedFrame(frame,index,vector<vector<Point>>(), this);
    foreach (LaneThread *thread, m_threads)
        thread->setProcessedFrame(m_current);
    start();
}
