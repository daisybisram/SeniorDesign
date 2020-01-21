#ifndef ADCCONTROL_H
#define ADCCONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/adc.h"

namespace Ui {
class AdcControl;
}

class AdcControl : public QScrollArea
{
    Q_OBJECT
public:
    explicit AdcControl(QWidget *parent = nullptr);
    ~AdcControl();
    void Start();

public:
    Ui::AdcControl *ui;

signals:
    void postiveEdgeDetected(uint8_t mv);

public slots:
};

#endif // ADCCONTROL_H
