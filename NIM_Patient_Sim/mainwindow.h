#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GpioControl/gpiocontrol.h"
#include "adccontrol.h"
#include "spicontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    GpioControl *gpioControl;
    AdcControl *adcControl;
    SpiControl *spiControl;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H