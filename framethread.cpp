#include "framethread.h"

FrameThread::FrameThread(Mat frame, int index, QObject *parent) : QThread(parent)
{
    m_current = new ProcessedFrame(frame.clone(), index, new Contours(), this);
    int numOfLanes = ObjectDetector::getInstance()->numOfLanes();

    for(int i = 0; i < numOfLanes; i++)
        m_threads.append(new LaneThread(frame,i, this));

    foreach (LaneThread *thread, m_threads)
        connect(thread, SIGNAL(contoursCalculated(Contours*)),
                this, SLOT(laneThreadHasFinished(Contours*)));
}

void FrameThread::laneThreadHasFinished(Contours *contours)
{
    LaneThread *thread = (LaneThread *)sender();

    qDebug() << m_current->index() <<
                thread->laneIndex() <<
                "T ::" << m_checkCount <<
                "MAX ::" << ObjectDetector::getInstance()->numOfLanes() <<
                "FT ::" << this;

    m_current->contours()->append(contours->data());

    qDebug() << "CONTOURS ::" << m_current->contours()->data().size();

    ++m_checkCount;

    if(m_checkCount ==  m_threads.size()){
    }
}

bool FrameThread::isFinished()
{
    bool aux = true;
    foreach (LaneThread *thread, m_threads){
        aux = aux && thread->isFinished();
    }
    return aux;
}

ProcessedFrame *FrameThread::current() const
{
    return m_current;
}

void FrameThread::process(Mat frame, int index)
{
    m_current = new ProcessedFrame(frame.clone(),index,new Contours(), this);

    foreach (LaneThread *thread, m_threads){
        thread->setFrame(m_current->frame());
        thread->start();
    }

    start();
}

void FrameThread::run()
{
    m_current->clear();
    m_checkCount = 0;
    while(!isFinished()) { }
}
