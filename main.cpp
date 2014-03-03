#include "mysvm.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mySVM w;
    w.show();
    return a.exec();
}
