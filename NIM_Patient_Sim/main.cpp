#include "mainwindow.h"

#include <QMainWindow>
#include "gpiocontrol.h"
#include "adccontrol.h"
#include "spicontrol.h"
#include "daccontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}

