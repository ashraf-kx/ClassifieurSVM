#include "mysvm.h"
#include "ui_mysvm.h"

mySVM::mySVM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mySVM)
{
    ui->setupUi(this);
    this->showMaximized();
    //this->setWindowIcon(QIcon(":/assets/svm_icon.png"));
    this->setWindowTitle("Simple test SVM");
    scene = new QGraphicsScene();
    svm = SVM::create();

//    logs.clear();
//    textEdit = new QTextEdit();
//    ui->dockWidget->setMinimumHeight(100);
//    ui->dockWidget->setWidget(textEdit);

    ui->actionRunTraining->setShortcut(Qt::Key_Enter);

    connect(ui->btnTrainSVM,SIGNAL(clicked()),this,SLOT(run()));   
}

mySVM::~mySVM()
{
    delete ui;
}

QImage mySVM::mat2QImage(cv::Mat const& src)
{
     Mat temp;
     cvtColor(src, temp,CV_BGR2RGB);
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

void mySVM::run()
{    
    // Set up training data.
    int trainingSampleCount = ui->vectorsCount->text().toInt();
    Mat trainingData(trainingSampleCount,2,CV_32FC1);

    srand((int)time(0));
    int min = 0;
    int max = 700;
    for(int i=0; i< trainingSampleCount; i++)
    {
        trainingData.at<float>(i,0) = min + rand() % (( max + 1 ) - min);
        trainingData.at<float>(i,1) = min + rand() % (( max + 1 ) - min);
    }

    Mat labels(trainingSampleCount, 1, CV_32SC1);

    // each training sample must have a label (labels in this example varie betwen 1 or -1).
    for(int i=0; i< trainingSampleCount; i++) {
        labels.at<int>(i) = (min + rand() % (( max + 1 ) - min)) > (max/2) ? 1 : -1;
        qDebug()<<"{"<<trainingData.at<float>(i,0)<<","<<trainingData.at<float>(i,1)<<"}"<<" labeled ["<<labels.at<int>(i)<<"]";
    }

    // Train the SVM.
    svm->setType(SVM::C_SVC);
    svm->setKernel(ui->svmKernalType->currentIndex());
    svm->setGamma(ui->svmGamma->text().toInt());
    svm->setC(ui->svmC->text().toInt());
    svm->setDegree(ui->svmDegree->text().toInt());
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    qDebug()<<"New SVM parameters is adjusted.";

    svm->train(trainingData, ROW_SAMPLE, labels);

    // Data for visual representation.
    int width = max, height = max;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    // Show the decision regions given by the SVM.
    Vec3b green(0,255,0), blue(255,0,0);
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
        {
            Mat sampleMat = (Mat_<float>(1,2) << j,i);
            float response = svm->predict(sampleMat);
            if (response == 1)
                image.at<Vec3b>(i,j)  = green;
            else if (response == -1)
                image.at<Vec3b>(i,j)  = blue;
        }

    // Show the training data
    int thickness = -1;
    for(int i=0; i< trainingSampleCount; i++)
        circle(image, Point(trainingData.at<float>(i,0), trainingData.at<float>(i,1)), 5, Scalar(255, 255, 255), thickness);

    // Show support vectors
    thickness = 2;
    Mat sv = svm->getUncompressedSupportVectors();
    for (int i = 0; i < sv.rows; i++)
    {
        const float* v = sv.ptr<float>(i);
        circle(image, Point((int) v[0], (int) v[1]), 6, Scalar(128, 128, 128), thickness);
    }

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(mat2QImage(image)));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();
//    const QString out;
//    qDebug()>>out;
//    textEdit->setText(out);
}
