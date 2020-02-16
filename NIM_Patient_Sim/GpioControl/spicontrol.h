#ifndef SPICONTROL_H
#define SPICONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/spi.h"

//namespace Ui {
//class SpiControl;
//}

class SpiControl : public QScrollArea
{
    Q_OBJECT
public:
    explicit SpiControl(QWidget *parent = nullptr);
    ~SpiControl();
    int spi_Read_DAC(uint8_t reg, uint16_t *data);
    int spi_Write_DAC(uint8_t reg, uint16_t write_data);
    int spi_Read_ADC(uint8_t reg, uint8_t *data);
    int spi_Write_ADC(uint8_t reg, uint8_t data);
    int DAC_output(uint8_t output, uint16_t data[], uint16_t delay_time_ms, uint16_t amplitude);


public:
    //Ui::SpiControl *ui;

signals:

public slots:

};

#endif // SPICONTROL_H
