#include "objectdetector.h"

ObjectDetector * ObjectDetector::m_instance = NULL;

ObjectDetector::ObjectDetector(int history, int threshold, bool shadow)
{
    // Init line contact dependant variables
    pMOG2 = createBackgroundSubtractorMOG2(history,threshold,shadow);
    lanes = QList<Lane>();
    kernelOrigin = Point(-1, -1);
    kernelClose = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    kernelOpen = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    kernelDilate = getStructuringElement(MORPH_ELLIPSE, Size(5, 2));
}

void ObjectDetector::addNewLane(Point lineP1, Point lineP2, Rect toTrack, String name)
{
    Lane newLane(lineP1, lineP2, toTrack, name);
    this->lanes.append(newLane);
}

void ObjectDetector::drawLanes(ProcessedFrame *pf)
{
    Mat frame  = pf->frame();

    double alpha = 0.9;
    Mat overlay = Mat::zeros(frame.size(), frame.type());
    foreach (Lane lane, lanes) {
        // Draw rectangle
        rectangle(overlay, lane.toTrack, CV_RGB(0, 255, 0), -1);
        addWeighted( frame, alpha, overlay, 1 - alpha, 0.0, frame);
        // Draw line
        line(frame, lane.lineP1, lane.lineP2, lane.contacted ? Scalar(255,0,0) : Scalar(0,0,255), 2);
    }
}

void ObjectDetector::genContours(const int laneIndex, Mat &frame, ProcessedFrame *pf)
{
    Mat fg, diff;
    subtract(frame, pf->frame(), diff);
    // Transform ROI to grayscale
    cvtColor(diff(lanes[laneIndex].toTrack), fg, CV_BGR2GRAY);
    // Apply background subtraction
    //pMOG2->apply(fg, fg, .75);
    // Morphology Process
    blur(fg, fg, Size(3,3));
//  morphologyEx(fg, fg, MORPH_OPEN, kernelOpen, kernelOrigin );  // Remove noise
    morphologyEx(fg, fg, MORPH_CLOSE, kernelClose, kernelOrigin); // Close small gaps
    morphologyEx(fg, fg, MORPH_DILATE, kernelDilate, kernelOrigin);   // Dilate to merge adjacent blobs...
    // Binarize to delete shadows
    blur(fg, fg, Size(3,3));
    threshold(fg, fg, 10, 255, CV_THRESH_BINARY);
    // Generate the contours vector
    vector<vector<Point>> contours;
    findContours(fg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    pf->setContours(contours);
}

void ObjectDetector::genContours(const int laneIndex, ProcessedFrame *pf)
{
    Mat fg;
    // Transform ROI to grayscale
    fg = pf->frame()(lanes[laneIndex].toTrack).clone();
    // Apply background subtraction
    pMOG2->apply(fg, fg, .75);
    // Morphology Process
    blur(fg, fg, Size(3,3));
//  morphologyEx(fg, fg, MORPH_OPEN, kernelOpen, kernelOrigin );  // Remove noise
    morphologyEx(fg, fg, MORPH_CLOSE, kernelClose, kernelOrigin); // Close small gaps
    morphologyEx(fg, fg, MORPH_DILATE, kernelDilate, kernelOrigin);   // Dilate to merge adjacent blobs...
    // Binarize to delete shadows
    blur(fg, fg, Size(3,3));
    threshold(fg, fg, 10, 255, CV_THRESH_BINARY);
    // Generate the contours vector
    vector<vector<Point>> contours;
    findContours(fg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    pf->setContours(contours);
}

void ObjectDetector::analyzeContours(const int laneIndex, ProcessedFrame *pf)
{
    // Bool to check if there was any contact
    Lane * lane = &(lanes[laneIndex]);
    lane->contacted = false;
    // Region of interest from image
    Mat to_track = pf->frame()(lane->toTrack);

    vector<vector<Point>> contours = pf->contours();
    // Check intersections
    for( size_t i = 0; i < contours.size(); ++i)
    {
        // Extract rect from iterator
        Rect rect = boundingRect(contours.at(i));
        // Check if rect has min size
        if(rect.height > minContourSize && rect.width > minContourSize) {
            // Define X and Y centroids
            int xc = rect.x + rect.width/2 + lane->toTrack.x;
            int yc = rect.y + rect.height/2 + lane->toTrack.y;
            // Draw blue rectangle
            rectangle(to_track, rect, CV_RGB(0,0,255));
            // Draw green circle
            circle(pf->frame(), Point(xc, yc), 2, CV_RGB(0, 255, 0), 2);

            Point *p1 = &(lane->lineP1);
            Point *p2 = &(lane->lineP2);
            // Check intersections
            if ( abs(yc - p1->y) <= sensY && xc >= p1->x && xc <= p2->x && !lane->isContacted(xc, sensX, hotness) ) { // Check if xc is near the line 1 and if contact was not computed befor
                ++(lane->objCounter);
                // Only print if named lane
                if (lane->name != "") qDebug() << "Contagem da linha " << QString::fromStdString(lane->name) << ": " << lane->objCounter;
                lane->contacted = true;
            }
        }
    }
}

int ObjectDetector::numOfLanes()
{
    return lanes.size();
}

ObjectDetector *ObjectDetector::getInstance()
{
    if(m_instance == NULL) m_instance = new ObjectDetector(150, 32, true);
    return m_instance;
}

void ObjectDetector::setDetectionParams(int minContourSize, int hotness, int sensX, int sensY) {
    this->minContourSize = minContourSize;
    this->hotness = hotness;
    this->sensX = sensX;
    this->sensY = sensY;
}
