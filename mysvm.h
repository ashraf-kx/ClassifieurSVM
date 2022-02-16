#ifndef MYSVM_H
#define MYSVM_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsWidget>

#include <QTime>
#include <ctime>
#include <iostream>

#include <QTextEdit>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace ml;

#define	NTRAINING_SAMPLES	4			// Number of training samples per class
#define FRAC_LINEAR_SEP		0.9f	    // Fraction of samples which compose the linear separable part

namespace Ui {
class mySVM;
}

class mySVM : public QMainWindow
{
    Q_OBJECT

public:
    explicit mySVM(QWidget *parent = 0);
    ~mySVM();
    QImage mat2QImage(cv::Mat const& src);
public slots:
    void run();
private:
    Ui::mySVM *ui;
    QGraphicsScene *scene;
    Ptr<SVM> svm;
//    Ptr<TrainData> trainingData;
//    QTextEdit *textEdit;
//    QStringList logs;
};

#endif // MYSVM_H
