#ifndef ADCCONTROL_H
#define ADCCONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/adc.h"

//ADS8910b Addresses (9-bits w/ first bit = 0)
#define ADS_PD_CNTL_ADDR	0x04		//controls low power modes
#define ADS_SDI_CNTL_ADDR	0x08		//selects SPI protocol for writing data
#define ADS_SDO_CNTL_ADDR	0x0C		//configures protocol for reading data
#define ADS_DATA_CNTL_ADDR	0x10		//configures 22-bit output data
#define ADS_PATN_LSB_ADDR	0x14		//controls 8 LSB of output pattern when DATA_VAL = 1b
#define ADS_PATN_MID_ADDR	0x15		//controls middle 8 bits of output pattern when DATA_VAL = 1b
#define ADS_PATN_MSB_ADDR	0x16		//controls 4 MSB of output pattern when DATA_VAL = 1b
#define ADS_OFST_CAL_ADDR	0x20		//selcts external V reference for offset calibration
#define ADS_REF_MRG_ADDR	0x30		//sescts margin for reference buffer output

//ADS8910b Commands
#define ADS_NOP        0x00      //NO OPERATION 1
#define ADS_CLR_BITS   0x10      //Clears 8-bits from address
#define ADS_READ_REG   0x11      //Reads Contents from address
#define ADS_WRITE_REG  0x12      //Writes 8-bits to address
#define ADS_SET_BITS   0x13      //Sets bits in address without affecting other bits (only the bits denoted by a 1)
#define ADS_NOP_2      0x1F      //NO OPERATION 2

//namespace Ui {
//class AdcControl;
//}

class AdcControl : public QScrollArea
{
    Q_OBJECT
public:
    explicit AdcControl(QWidget *parent = nullptr);
    ~AdcControl();
    void Start();

public:
    //Ui::AdcControl *ui;
    void Stop(void);

signals:
    void EdgeDetected(uint8_t mv);

public slots:
     void Start(uint8_t interval);
};

#endif // ADCCONTROL_H
