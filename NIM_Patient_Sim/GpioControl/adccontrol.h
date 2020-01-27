#ifndef ADCCONTROL_H
#define ADCCONTROL_H

#include <QScrollArea>
#include <QWidget>
#include <QtWidgets>
#include "libdigiapix/adc.h"

#define ADS_PD_CNTL_ADDR	0x004h		//controls low power modes
#define ADS_SDI_CNTL_ADDR	0x008h		//selects SPI protocol for writing data
#define ADS_SDO_CNTL_ADDR	0x00Ch		//configures protocol for reading data
#define ADS_DATA_CNTL_ADDR	0x010h		//configures 22-bit output data
#define ADS_PATN_LSB_ADDR	0x014h		//controls 8 LSB of output pattern when DATA_VAL = 1b
#define ADS_PATN_CMID_ADDR	0x015h		//controls middle 8 bits of output pattern when DATA_VAL = 1b
#define ADS_PATN_MSB_ADDR	0x016h		//controls 4 MSB of output pattern when DATA_VAL = 1b
#define ADS_OFST_CAL_ADDR	0x020h		//selcts external V reference for offset calibration
#define ADS_REF_MRG_ADDR	0x030h		//sescts margin for reference buffer output

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
