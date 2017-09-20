#include "lanethread.h"

LaneThread::LaneThread(Mat frame, int laneIndex, QObject *parent) : QThread(parent)
{
    m_frame = frame;
    m_laneIndex = laneIndex;
}

int LaneThread::laneIndex() const
{
    return m_laneIndex;
}

void LaneThread::run()
{
    qDebug() << "A" << m_laneIndex;
    ObjectDetector *detector = ObjectDetector::getInstance();
    qDebug() << "B" << m_laneIndex;
    Contours *c = new Contours(detector->genContours(m_laneIndex, m_frame));
    qDebug() << "C" << m_laneIndex;
    contoursCalculated(c);
    qDebug() << "D" << m_laneIndex;
}

Mat LaneThread::frame() const
{
    return m_frame;
}

void LaneThread::setFrame(Mat frame)
{
    m_frame = frame;
}
