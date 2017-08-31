#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QHash>

#include "framemanager.h"

using namespace cv;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(char *path, QObject *parent = nullptr);
private slots:
    void appendIntoProcessedFrameBuffer(ProcessedFrame *pf);
    void processAnotherFrame();
private:
    QHash<int, ProcessedFrame *> m_buffer;
    VideoCapture *m_capture;
    FrameManager *m_fm;

    int m_checkCount;
    int m_currentIndex;
    int count1, count2;

    int64 t0, t1;
    Point p1, p2, p3, p4;
    bool checkIntersection(Rect r, Mat frame, double percent);
    bool is_contacted(const int xc, const Point p1, const Point p2, const int sensibility, vector<int> &contact_heat, const int hotness);
    void cool_contacts(vector<int> &contact_heat);
};

#endif // CONTROLLER_H