#include "mysvm.h"
#include "ui_mysvm.h"

mySVM::mySVM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mySVM)
{
    ui->setupUi(this);
    this->showMaximized();
    //this->setWindowIcon(QIcon(":/Icons/IconAOCR.png"));
    this->setWindowTitle("Simple Test SVM ");
    scene = new QGraphicsScene();
    ui->actionRun_Training->setShortcut(Qt::Key_Enter);
    connect(ui->actionRun_Training,SIGNAL(triggered()),this,SLOT(Run()));
    connect(ui->Bt_trainSVM,SIGNAL(clicked()),this,SLOT(Run()));

}

mySVM::~mySVM()
{
    delete ui;
}

QImage mySVM::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

Mat mySVM::Run()
{
    // Portion Of Code that we will start with for the instance  "WE 'RE LATE" [07_04_2015]. FEELING SHITTY.

    // Data for visual representation
    const int WIDTH = 700, HEIGHT = 700;
    Mat I = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);

    // Simplicity [ Train DATA ]
    int nbrTrainSample = ui->LE_vector_nbr->text().toInt();
    Mat trainData(nbrTrainSample,2,CV_32FC1);   // [ Xpos, Ypos ].
    srand((int)time(0));
    for(int i=0; i< nbrTrainSample; i++)
    {
        trainData.at<float>(i,0) = ((rand() % 1000) + 1)/2;
        trainData.at<float>(i,1)  =  ((rand() % 1000) + 1)/2;
    }

    Mat labels(nbrTrainSample,1,CV_32FC1);
    int nbrClass = ui->LE_class_nbr->text().toInt();
    for(int i=0; i< labels.rows;)
    {
        for (int j = 0; j < nbrClass; j++)
        {
            labels.at<float>(i,0) = j;
            i++;
            if(i >= labels.rows ) break;
        }
    }

    // --------------------- 2. Set Up Parametres SVM -------------------------------------------
    CvSVMParams params;
    params.svm_type    = SVM::C_SVC;
    params.kernel_type = ui->CB_type->currentIndex();
    params.C           = ui->LE_c_value->text().toInt();
    params.degree      = ui->LE_degree_value->text().toInt();
    params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);

    //------------------------ 3. Train The SVM --------------------------------------------------
    cout << "Starting training process" << endl;
    CvSVM svm;
    std::cout<<trainData<<endl;
    std::cout<<labels<<endl;
    svm.train(trainData, labels, Mat(), Mat(), params);
    cout << "Finished training process" << endl;

    //------------------------ 4. Show The Decision Regions ----------------------------------------
    QList<Vec3b> colors;
    colors.clear();
    colors<<Vec3b(255,255,255);
    colors<<Vec3b(255,255,49);
    colors<<Vec3b(79,238,255);
    colors<<Vec3b(255,155,212);
    colors<<Vec3b(152,255,124);
    colors<<Vec3b(255,255,50);


    for (int i = 0; i < ui->LE_class_nbr->text().toInt(); ++i) {
        srand((int)time(0));
        unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
        unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
        unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));
        Vec3b col(r,g,b);
        colors<<col;
    }

    for (int i = 0; i < I.rows; i++)
        for (int j = 0; j < I.cols; j++)
        {
            Mat sampleMat = (Mat_<float>(1,2) << i, j);
            float response = svm.predict(sampleMat);
            for (int k = 0; k < (int)response; k++) {
                I.at<Vec3b>(j,i)  = colors.at(k);
            }
        }

    //----------------------- 5. Show the training data --------------------------------------------
    int thick = -3;
    int lineType = 5;
    float px, py;

    QList<Scalar> Colors2;
    Colors2.clear();
    Colors2<<Scalar(0,255,0);
    Colors2<<Scalar(0,0,255);
    Colors2<<Scalar(255,0,0);
    Colors2<<Scalar(50,50,50);
    Colors2<<Scalar(100,150,100);
    Colors2<<Scalar(255,50,255);
    /*
    for (int i = 0; i < ui->LE_class_nbr->text().toInt(); ++i) {
        srand((int)time(0));
        unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
        unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
        unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));
        Colors2<<Scalar(b,r,g);
    }*/
    for (int i = 0; i < nbrTrainSample; )
    {
        for (int k = 0; k < ui->LE_class_nbr->text().toInt(); k++)
        {
            px = trainData.at<float>(i,0);
            py = trainData.at<float>(i,1);
            circle(I, Point( (int) px,  (int) py ), 3, Colors2.at(k), thick, lineType);
            i++;
            if(i >= nbrTrainSample ) break;
        }
    }

    //------------------------- 6. Show support vectors --------------------------------------------
    thick = 1;
    lineType  = 8;
    int x     = svm.get_support_vector_count();
    std::cout<<"number of support vectors : "<<x<<endl;
    for (int i = 0; i < x; ++i)
    {
        const float* v = svm.get_support_vector(i);
        std::cout<<"x : "<<(int) v[0]<<".     Y : "<<(int) v[1]<<endl;
        circle( I,  Point( (int) v[0], (int) v[1]), 3, Scalar(48, 193, 255), thick, lineType);
    }
    imwrite("Last_Example.png",I);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(Mat2QImage(I)));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->update();

// END First Sample.
    return I;
}
