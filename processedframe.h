#ifndef PROCESSEDFRAME_H
#define PROCESSEDFRAME_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

#include <QObject>

#include <bits/stdc++.h>

using namespace cv;
using namespace std;

class ProcessedFrame : public QObject
{
    Q_OBJECT
public:
    explicit ProcessedFrame(ProcessedFrame *pf, QObject *parent = nullptr);
    ProcessedFrame(Mat frame, int index = 0, vector<vector<Point>> contours = vector<vector<Point>>(), QObject *parent = nullptr);
    Mat frame();
    int index() const;
    vector<vector<Point> > contours();
    void setContours(const vector<vector<Point> > &contours);
    void setFrame(const Mat &frame);
    void setIndex(int index);
    void clear();
private:
    Mat m_frame;
    int m_index;
    vector<vector<Point>> m_contours;
};

#endif // PROCESSEDFRAME_H
