#include "controller.h"

Controller::Controller(char *path, int numOfThreads, QObject *parent) : QObject(parent)
{
    //Iniciar o detector de objetos
    ObjectDetector *objDetect = ObjectDetector::getInstance();
    //Definir os parâmetros de direção
    objDetect->setDetectionParams(20, 6, 5, 20);
    //Adicionar as pistas de interesse
    objDetect->addNewLane(Point(0, 84), Point(160, 84), Rect(0, 40, 180, 100), "Sul");
    objDetect->addNewLane(Point(120, 144), Point(274, 144), Rect(110, 100, 180, 100), "Norte");

    //Iniciar a captura do vídeo (arquivo)
    m_capture = new VideoCapture(path);
    //Iniciar o gestor das Threads
    m_fm = new FrameManager(1, this);
    //Iniciar o timer principal
    m_timer = new QTimer(this);

    //Criar um Janela chamada Live com 800x600
    namedWindow("Live", WINDOW_FREERATIO);
    resizeWindow("Live", 800, 600);

    //Iniciar o verificador de contagem dos frames
    m_checkCount = 0;
    //Inciar o contador dos frames
    m_currentIndex = 0;
    //Sempre que o timer acabar, atualize o frame corrent
    connect(m_timer, SIGNAL(timeout()),
            this, SLOT(updateFrame()));
    //Sempre que um novo frame for processado, adicione-o no buffer
    connect(m_fm, SIGNAL(newProcessedFrame(ProcessedFrame*)),
            this, SLOT(appendIntoProcessedFrameBuffer(ProcessedFrame*)));
    //Se uma thread estiver disponível, processe outro frame
    connect(m_fm, SIGNAL(newThreadAvailable()),
            this, SLOT(processAnotherFrame()));
    //Inicializar as threads do FrameManager
    m_fm->init();
    //Definir quantos frames por segundo serão exibidos
    int fps = 5;
    //Definir quanto tempo o frame permanecerá exibido
    m_timer->start(1000/fps);
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
        qDebug() << "PROCESSED FRAME IS NULL!" << "\n\tCURRENT KEY :: " << m_checkCount << "\n\tKEYS :: " << m_buffer.keys();
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
