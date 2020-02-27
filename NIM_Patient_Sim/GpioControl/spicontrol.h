#ifndef SPICONTROL_H
#define SPICONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/spi.h"
#include <QElapsedTimer>

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
    int spi_Check_ADC_Reg(uint8_t reg, uint32_t *data);
    int spi_Write_ADC(uint8_t reg, uint8_t data);
    int spi_Read_ADC_Conversion(uint32_t *data);
    int DAC_output(uint8_t output, float_t gain, uint16_t delay_time_ms, uint32_t amplitude_uV);
    void timer_interrupt(int gpionum);
//    void TimerFlag(int flag);
    QElapsedTimer *us_timer = new QElapsedTimer;

};

#endif // SPICONTROL_H
