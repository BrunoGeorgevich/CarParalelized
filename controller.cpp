#include "controller.h"

Controller::Controller(char *path, int numOfThreads, QObject *parent) : QObject(parent)
{
    m_capture = new VideoCapture(path);
    m_fm = new FrameManager(numOfThreads/2, this);
    m_timer = new QTimer(this);

    namedWindow("Live", WINDOW_FREERATIO);
    resizeWindow("Live", 800, 600);

    m_checkCount = 0;
    m_currentIndex = 0;
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(updateFrame()));
    connect(m_fm, SIGNAL(newProcessedFrame(ProcessedFrame*)),
            this, SLOT(appendIntoProcessedFrameBuffer(ProcessedFrame*)));
    connect(m_fm, SIGNAL(newThreadAvailable()),
            this, SLOT(processAnotherFrame()));
    m_fm->init();
//    int fps = 60;
//    m_timer->start(1000/fps);
}

void Controller::appendIntoProcessedFrameBuffer(ProcessedFrame *pf)
{
    m_buffer.insert(pf->index(), pf);
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
        return;
    }
    m_fm->processFrame(frame, m_currentIndex++);
}

void Controller::updateFrame()
{
    ProcessedFrame *pf = m_buffer.take(m_checkCount);
    if(pf == NULL && m_checkCount == m_currentIndex && m_hasInit) {
        m_timer->stop();
        qDebug() << "STOPPING TIMER ::" << !m_timer->isActive();
        return;
    }
    if(pf == NULL) {
//        qDebug() << "PRECESSED FRAME IS NULL!" << m_buffer.size();
        return;
    }

    ObjectDetector *detector = ObjectDetector::getInstance();

    for(int i = 0; i < detector->numOfLanes(); i++)
        detector->analyzeContours(i, pf);

    detector->drawLanes(pf);

    imshow("Live", pf->frame());

    m_buffer.remove(m_checkCount);
    ++m_checkCount;
}
