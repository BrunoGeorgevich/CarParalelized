#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include "lane.h"
#include "processedframe.h"

#include <QList>
#include <QDebug>

using namespace cv;
using namespace std;

class ObjectDetector
{
public:
    void setDetectionParams(int minContourSize, int hotness, int sensX, int sensY);
    void addNewLane(Point lineP1, Point lineP2, Rect toTrack, String name="");

    void drawLanes(ProcessedFrame *pf);
    void genContours(const int laneIndex, Mat &frame, ProcessedFrame *pf);
    void genContours(const int laneIndex, ProcessedFrame *pf);
    void analyzeContours(const int laneIndex, ProcessedFrame *pf);
    int numOfLanes();

    static ObjectDetector *getInstance();
private:
    static ObjectDetector *m_instance;
    ObjectDetector(int history, int threshold, bool shadow);
    Mat frame, diff;

    // BGS variables
    Ptr< BackgroundSubtractor> pMOG2;

    // Contour variables
    int minContourSize;

    // Contact line variables
    QList<Lane> lanes;
    int hotness, sensX, sensY;

    // Morphology variables
    Point kernelOrigin;
    Mat kernelClose;
    Mat kernelOpen;
    Mat kernelDilate;


};

#endif // OBJECTDETECTOR_H
