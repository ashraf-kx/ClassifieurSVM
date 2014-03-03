#ifndef MYSVM_H
#define MYSVM_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsWidget>

#include <QTime>
#include <ctime>
#include <iostream>

//////////////////////// INCLUDE OPEN Computer Vision /////////////////////
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml//ml.hpp>
////////////////////////            END               /////////////////////

using namespace cv;
using namespace std;

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
    QImage Mat2QImage(cv::Mat const& src);
public slots:
    Mat Run();
private:
    Ui::mySVM *ui;
    QGraphicsScene *scene;
    //QList<Vec3b> colors;

};

#endif // MYSVM_H
