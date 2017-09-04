#include "processedframe.h"

ProcessedFrame::ProcessedFrame(ProcessedFrame *pf, QObject *parent)
{
    ProcessedFrame(pf->frame(), pf->index(), pf->contours(), parent);
}

ProcessedFrame::ProcessedFrame(Mat frame, int index, vector<vector<Point> > contours, QObject *parent) :
    QObject(parent)
{
    m_frame = frame.clone();
    m_index = index;
    foreach (vector<Point> vp, contours) {
        m_contours.push_back(vp);
    }
}

Mat ProcessedFrame::frame(){
    return m_frame;
}

int ProcessedFrame::index() const
{
    return m_index;
}

vector<vector<Point> > ProcessedFrame::contours()
{
    return m_contours;
}

void ProcessedFrame::setContours(const vector<vector<Point> > &contours)
{
    foreach (vector<Point> vp, contours) {
        m_contours.push_back(vp);
    }
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
    m_contours.clear();
}
