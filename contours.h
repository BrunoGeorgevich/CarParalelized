#ifndef CONTOURS_H
#define CONTOURS_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

#include <QObject>

#include <bits/stdc++.h>

using namespace cv;
using namespace std;

class Contours : public QObject
{
    Q_OBJECT
public:
    explicit Contours(vector<vector<Point>> data = vector<vector<Point>>(), QObject *parent = nullptr);
    vector<vector<Point> > data();
    void setData(vector<vector<Point> > data);
    void append(vector<vector<Point> > data);
    void clear();
private:
    vector<vector<Point>> m_data;
};

#endif // CONTOURS_H
