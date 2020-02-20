#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
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

    void onUpLatencyReleased(void);
    void onDownLatencyReleased(void);
    void onUpAmplitudeReleased(void);
    void onDownAmplitudeReleased(void);

public:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
