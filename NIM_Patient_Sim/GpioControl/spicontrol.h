#ifndef SPICONTROL_H
#define SPICONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/spi.h"

namespace Ui {
class SpiControl;
}

class SpiControl : public QScrollArea
{
    Q_OBJECT
public:
    explicit SpiControl(QWidget *parent = nullptr);
    ~SpiControl();
    static int spi_Write_enable();
    static int spi_Read_status(uint8_t *status);
    static int spi_Write(int page_index, uint8_t* data);
    static int spi_Read(int page_index, uint8_t* data);

public:
    Ui::SpiControl *ui;

signals:

public slots:

};

#endif // SPICONTROL_H
