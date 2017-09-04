#ifndef LANETHREAD_H
#define LANETHREAD_H

#include <QThread>

#include "processedframe.h"
#include "objectdetector.h"

class LaneThread : public QThread
{
public:
    LaneThread(Mat frame, int laneIndex, QObject *parent);
    void setProcessedFrame(ProcessedFrame *pf);
    ProcessedFrame *processedFrame() const;
    int laneIndex() const;

protected:
    ProcessedFrame *m_pf;
    int m_laneIndex;
    void run();
};

#endif // LANETHREAD_H
