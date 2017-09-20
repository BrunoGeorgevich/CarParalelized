#ifndef PROCESSEDFRAME_H
#define PROCESSEDFRAME_H

#include "contours.h"

class ProcessedFrame : public QObject
{
    Q_OBJECT
public:
    explicit ProcessedFrame(ProcessedFrame *pf, QObject *parent = nullptr);
    ProcessedFrame(Mat frame, int index = 0, Contours *contours = nullptr, QObject *parent = nullptr);
    Mat frame();
    int index() const;
    void setFrame(const Mat &frame);
    void setIndex(int index);
    void clear();
    Contours *contours() const;
    void setContours(Contours *contours);

private:
    Mat m_frame;
    int m_index;
    Contours *m_contours;
};

#endif // PROCESSEDFRAME_H
