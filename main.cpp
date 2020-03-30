#include "sqlpractice.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQLPractice *w = new SQLPractice();
    w->show();
    return a.exec();


    // Line to be added in master
    // Line test B
    // Added in testC

}
