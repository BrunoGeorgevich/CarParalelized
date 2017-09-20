#include "processedframe.h"

ProcessedFrame::ProcessedFrame(ProcessedFrame *pf, QObject *parent)
{
    ProcessedFrame(pf->frame(), pf->index(), pf->contours(), parent);
}

ProcessedFrame::ProcessedFrame(Mat frame, int index, Contours *contours, QObject *parent) :
    QObject(parent)
{
    m_frame = frame.clone();
    m_index = index;
    m_contours = contours;
}

Mat ProcessedFrame::frame(){
    return m_frame;
}

int ProcessedFrame::index() const
{
    return m_index;
}

void ProcessedFrame::setFrame(const Mat &frame)
{
    m_frame = frame;
}

void ProcessedFrame::setIndex(int index)
{
    m_index = index;
}

void ProcessedFrame::clear()
{
    m_contours->clear();
}

Contours *ProcessedFrame::contours() const
{
    return m_contours;
}

void ProcessedFrame::setContours(Contours *contours)
{
    m_contours = contours;
}
