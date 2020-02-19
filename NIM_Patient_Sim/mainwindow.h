#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gpiocontrol.h"
#include "adccontrol.h"
#include "spicontrol.h"
#include "daccontrol.h"

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

signals:

public slots:
    void on_up_Latency_released();

    void on_down_Latency_released();

    void on_up_Amplitude_released();

    void on_down_Amplitude_released();

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
