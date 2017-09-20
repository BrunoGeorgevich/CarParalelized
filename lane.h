#ifndef LANE_H
#define LANE_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Lane
{
public:
    String name;
    Lane(Point lineP1, Point lineP2, Rect toTrack, String name = "");
    Point lineP1, lineP2;
    Rect toTrack;
    bool contacted;
    int objCounter;
    // Arrays for detect line contacts. The value indicate how "hot" is last conact
    // Contacts start to "cool" if not present anymore, until it reaches zero.
    vector<int> lineContactHeat;

    bool coolContacts();
    bool isContacted(const int xc, const int sensibility, const int hotness);
};

#endif // LANE_H
