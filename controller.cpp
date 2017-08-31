#include "controller.h"

Controller::Controller(char *path, QObject *parent) : QObject(parent)
{
    m_capture = new VideoCapture(path);
    m_fm = new FrameManager(4, this);

    namedWindow("Live", WINDOW_FREERATIO);
    resizeWindow("Live", 800, 600);

    Mat frame;
    *m_capture >> frame;
    p1 = Point(frame.cols*(0.0), frame.rows*(0.35));
    p2 = Point(frame.cols*(0.50), frame.rows*(0.35));
    p3 = Point(frame.cols*(0.4), frame.rows*(0.6));
    p4 = Point(frame.cols*(0.85), frame.rows*(0.6));

    m_checkCount = 0;
    m_currentIndex = 0;

    count1 = 0;
    count2 = 0;

    t0 = getTickCount();

    connect(m_fm, SIGNAL(newProcessedFrame(ProcessedFrame*)),
            this, SLOT(appendIntoProcessedFrameBuffer(ProcessedFrame*)));
    connect(m_fm, SIGNAL(newThreadAvailable()),
            this, SLOT(processAnotherFrame()));
    m_fm->init();
}


//Verify the intersection between the point and the line
bool Controller::checkIntersection(Rect r, Mat frame, double percent) {
    if((r.y + r.height/2) > frame.rows*percent*0.97 && (r.y + r.height/2) < frame.rows*percent*1.03)
        return true;
    return false;
}

// Check if XC contacted line between p1 and p2 recently
bool Controller::is_contacted(const int xc, const Point p1, const Point p2, const int sensibility, vector<int> &contact_heat, const int hotness ) {
    bool contact = false;
    for(int i = -sensibility; i < sensibility; ++i) {
        // If there is still a hot contact, return true
        if ( contact_heat[ xc - p1.x - i ] != 0 )
            contact = true;

        // Else, make it hot
        contact_heat[ xc - p1.x - i ] = hotness;
    }
    return contact;
}

// Cool contacts by one point
void Controller::cool_contacts( vector <int> &contact_heat ) {
    for (int i = 0; i < contact_heat.size(); ++i)
        if( contact_heat[i] > 0) --contact_heat[i];
}

void Controller::appendIntoProcessedFrameBuffer(ProcessedFrame *pf)
{
    m_buffer.insert(pf->index(), pf);
    if(m_buffer.value(m_checkCount, NULL) != NULL){
        ProcessedFrame *pf = m_buffer.value(m_checkCount);
        vector<vector<Point>> contours = pf->contours();
        Mat frame = pf->frame();

        //        qDebug() << "CC ::" << pf->contours().size();

        //         Counters for each lane

        // Min Contourn size
        int min_size = 20;

        // Arrays for detect line contacts. The value indicate how "hot" is last conact
        // Contacts start to "cool" if not present anymore, until it reaches zero.
        vector <int> line1_contact(p2.x - p1.x) , line2_contact(p4.x - p3.x) ;
        fill(line2_contact.begin(), line2_contact.end(), 0);
        int hotness = 10, sensibility = 5;;

        // Bool to check if there was any contact
        bool contact1 = false;
        bool contact2 = false;

        // Check intersections
        for( size_t i = 0; i< contours.size(); i++ )
        {
            // Extract rect from iterator
            Rect rect = boundingRect(contours.at(i));
            // Check if rect has min size
            if(rect.height > min_size && rect.width > min_size) {
                // Define X and Y centroids
                int xc = rect.x + rect.width/2;
                int yc = rect.y + rect.height/2;

                // Draw blue rectangle
                rectangle(frame, rect, CV_RGB(0,0,255));
                // Draw green circle
                circle(frame, Point(xc, yc), 2, CV_RGB(0, 255, 0), 2);

                // Check intersections
                if ( abs(yc - p1.y) <= 5 && xc >= p1.x && xc <= p2.x && !is_contacted(xc, p1, p2, sensibility, line1_contact, hotness) ) { // Check if xc is near the line 1 and if contact was not computed befor
                    qDebug() << "Contagem sentido sul (linha 1): " << ++count1;
                    contact1 = true;
                }
                else if ( abs(yc - p3.y) < 5 && xc >= p3.x && xc <= p4.x && ! is_contacted(xc, p3, p4, sensibility, line2_contact, hotness)) {    // Check if xc is near the line 2 and if contact was not computed before
                    qDebug() << "Contagem sentido norte (linha 2): " << ++count2;
                    contact2 = true;
                }
            }
        }


        // Cool the last contacts by one point
        cool_contacts( line1_contact );
        cool_contacts( line2_contact );

        // Draw lines
        //line(frame, Point(frame.cols*(0.50), frame.rows*(0)), Point(frame.cols*(0.5), frame.rows*(1)), Scalar(0,255,0), 1);
        //line(frame, Point(frame.cols*(0), frame.rows*(0.5)), Point(frame.cols*(1), frame.rows*(0.5)), Scalar(0,255,0), 1);
        line(frame, p1, p2, contact1 ? Scalar(255,0,0) : Scalar(0,0,255), 2);
        line(frame, p3, p4, contact2 ? Scalar(255,0,0) : Scalar(0,0,255), 2);

        //        imshow("diff", diff);
        imshow("Live", frame);
        m_buffer.remove(m_checkCount);
        ++m_checkCount;
    }
}

void Controller::processAnotherFrame()
{
    if(!m_capture->isOpened()) {
        qDebug() << "CAPTURE IS CLOSED!";
        return;
    }
    Mat frame;
    *m_capture >> frame;
    if(frame.empty()) {
        qDebug() << "FRAME IS EMPTY!";
        t1 = getTickCount();
        double secs = (t1-t0)/getTickFrequency();
        qDebug() << "TIME ELAPSED ::" << secs;
        return;
    }
    m_fm->processFrame(frame, m_currentIndex);
    ++m_currentIndex;
}
