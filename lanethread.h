#ifndef LANETHREAD_H
#define LANETHREAD_H

#include <QThread>

#include "processedframe.h"
#include "objectdetector.h"

class LaneThread : public QThread
{
    Q_OBJECT
public:
    LaneThread(Mat frame, int laneIndex, QObject *parent);
    int laneIndex() const;
    Mat frame() const;
    void setFrame(Mat frame);
signals:
    void contoursCalculated(Contours *);
protected:
    void run();
private:
    Mat m_frame;
    int m_laneIndex;
};

#endif // LANETHREAD_H
