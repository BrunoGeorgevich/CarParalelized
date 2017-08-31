#include "framethread.h"

FrameThread::FrameThread(Mat frame, int index, int test, QObject *parent) : QThread(parent)
{
    m_current = new ProcessedFrame(frame, index, vector<vector<Point>>(), this);
    m_test = test;
    m_pMOG2 = createBackgroundSubtractorMOG2(150,32,true);
}

void FrameThread::run()
{
    m_current->clear();
//    qDebug() << "THE THREAD IS RUNNING :: " << m_test;
    // Morphology kernel. Possible shapes are: MORP_CORSS, MORPH_ELLIPSE and MORPH_RECT
    Mat kernel = getStructuringElement(MORPH_CROSS, Size(7, 7));
    //Process Frame
    Mat frame = m_current->frame();
    Mat diff;
    diff = frame.clone();
    // Blur for less noise
    blur(diff, diff, Size(4, 4) );
    // Background subtraction
    m_pMOG2->apply(diff, diff);
    // Morphology Process
    morphologyEx(diff, diff, CV_MOP_CLOSE, kernel);    // Fill any small holes
    morphologyEx(diff, diff, CV_MOP_OPEN, kernel);     // Remove noise
    morphologyEx(diff, diff, CV_MOP_DILATE, kernel);   // Dilate to merge adjacent blobs...
    // Binarize to delete shadows
    threshold(diff, diff, 120, 255, CV_THRESH_BINARY);
    // Find countours
    vector<vector<Point> > contours;
    findContours(diff, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    m_current->setFrame(frame);
    m_current->setContours(contours);
}

ProcessedFrame *FrameThread::current() const
{
    return m_current;
}

void FrameThread::process(Mat frame, int index)
{
    m_current->setFrame(frame.clone());
    m_current->setIndex(index);
    start();
}
