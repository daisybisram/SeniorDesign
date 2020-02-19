#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/gpio.h"

namespace Ui {
class GpioControl;
}

class GpioControl : public QScrollArea
{
    Q_OBJECT
public:
    explicit GpioControl(QWidget *parent = nullptr);
    ~GpioControl();
    void Gain_Control_Gpio(float_t gain, int gpioNum);

public:
    Ui::GpioControl *ui;

signals:

public slots:
    void ToggleGpio(int gpioNum);
};

#endif // ADCCONTROL_H
