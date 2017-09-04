#include "lanethread.h"

LaneThread::LaneThread(Mat frame, int laneIndex, QObject *parent) : QThread(parent)
{
    m_pf = new ProcessedFrame(frame);
    m_pf->setParent(this);

    m_laneIndex = laneIndex;
}

void LaneThread::setProcessedFrame(ProcessedFrame *pf)
{
    m_pf = pf;
}

ProcessedFrame *LaneThread::processedFrame() const
{
    return m_pf;
}

int LaneThread::laneIndex() const
{
    return m_laneIndex;
}

void LaneThread::run()
{
    ObjectDetector *detector = ObjectDetector::getInstance();
    detector->genContours(m_laneIndex, m_pf);
}
