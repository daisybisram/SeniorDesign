#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dac.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gpioControl = new GpioControl(ui->gpioCntrl);
    adcControl = new AdcControl();
    spiControl = new SpiControl();

    uint16_t data;
    //spiControl->spi_Read_DAC(DAC_CONFIG_ADDR, &data);
    //spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);
    //spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    //spiControl->spi_Read_DAC(DAC_CONFIG_ADDR, &data);

    uint16_t EMG_data[400] = {0XFFF9,0XFFF9,0XFFFB,0XFFFF,0X0003,0X0007,0X000A,0X000D,0X000F,0X0011,
                             0X0013,0X0014,0X0015,0X0015,0X0015,0X0013,0X0012,0X0011,0X0010,0X0010,
                             0X000F,0X000D,0X000B,0X0007,0X0004,0X0002,0X0002,0X0003,0X0007,0X000C,
                             0X0013,0X001B,0X0021,0X0021,0X001A,0X0009,0XFFF1,0XFFD0,0XFFAE,0XFF8C,
                             0XFF71,0XFF60,0XFF60,0XFF76,0XFFA6,0XFFF1,0X0054,0X00C9,0X014B,0X01D5,
                             0X0265,0X02FB,0X0390,0X041C,0X048E,0X04D6,0X04EC,0X04D0,0X048F,0X043B,
                             0X03E2,0X038B,0X0332,0X02CC,0X0251,0X01BE,0X011C,0X007A,0XFFE4,0XFF62,
                             0XFEF5,0XFE98,0XFE47,0XFDFC,0XFDB7,0XFD76,0XFD39,0XFCFE,0XFCC4,0XFC88,
                             0XFC49,0XFC08,0XFBC6,0XFB85,0XFB49,0XFB15,0XFAEC,0XFACF,0XFAC0,0XFABE,
                             0XFAC6,0XFAD6,0XFAEB,0XFB04,0XFB1F,0XFB3D,0XFB5F,0XFB88,0XFBBA,0XFBF5,
                             0XFC38,0XFC83,0XFCD5,0XFD30,0XFD96,0XFE06,0XFE80,0XFEFD,0XFF77,0XFFE9,
                             0X0050,0X00B2,0X0114,0X017A,0X01E5,0X0251,0X02B5,0X0308,0X0348,0X0373,
                             0X0390,0X03A4,0X03B4,0X03C0,0X03C6,0X03C3,0X03B4,0X039B,0X037A,0X0355,
                             0X0330,0X030C,0X02E8,0X02C4,0X029D,0X0272,0X0246,0X0219,0X01ED,0X01C5,
                             0X019F,0X017A,0X0156,0X0132,0X0110,0X00F0,0X00D2,0X00B7,0X009C,0X0082,
                             0X0067,0X004B,0X002F,0X0014,0XFFFF,0XFFEE,0XFFE1,0XFFD6,0XFFCD,0XFFC2,
                             0XFFB6,0XFFAB,0XFFA3,0XFF9E,0XFF9C,0XFF9B,0XFF99,0XFF96,0XFF93,0XFF91,
                             0XFF91,0XFF94,0XFF97,0XFF99,0XFF9A,0XFF99,0XFF98,0XFF9A,0XFF9E,0XFFA3,
                             0XFFA7,0XFFA9,0XFFA9,0XFFAA,0XFFAC,0XFFB1,0XFFB7,0XFFBD,0XFFC3,0XFFC7,
                             0XFFCB,0XFFCF,0XFFD3,0XFFD7,0XFFDB,0XFFDE,0XFFE0,0XFFE1,0XFFE3,0XFFE5,
                             0XFFE9,0XFFEE,0XFFF3,0XFFF7,0XFFF9,0XFFF9,0XFFF7,0XFFF7,0XFFF8,0XFFFB,
                             0XFFFF,0X0000,0X0001,0X0000,0XFFFF,0XFFFF,0X0000,0X0000,0X0001,0X0000,
                             0XFFFF,0XFFFD,0XFFFE,0X0000,0X0006,0X000B,0X000E,0X000E,0X000B,0X0006,
                             0X0002,0X0000,0XFFFF,0X0000,0X0001,0X0002,0X0003,0X0003,0X0003,0X0002,
                             0X0000,0X0000,0XFFFF,0XFFFE,0XFFFD,0XFFFD,0XFFFD,0XFFFC,0XFFFA,0XFFF8,
                             0XFFF7,0XFFF6,0XFFF6,0XFFF8,0XFFFB,0XFFFD,0XFFFF,0X0000,0X0000,0XFFFF,
                             0XFFFE,0XFFFD,0XFFFC,0XFFFB,0XFFFB,0XFFFC,0XFFFE,0XFFFF,0X0000,0X0000,
                             0XFFFE,0XFFFB,0XFFF9,0XFFF7,0XFFF7,0XFFF9,0XFFFB,0XFFFC,0XFFFB,0XFFFA,
                             0XFFF9,0XFFF9,0XFFF9,0XFFFA,0XFFFA,0XFFF9,0XFFF8,0XFFF6,0XFFF5,0XFFF5,
                             0XFFF5,0XFFF5,0XFFF4,0XFFF4,0XFFF5,0XFFF7,0XFFFB,0XFFFF,0X0000,0X0000,
                             0XFFFF,0XFFFD,0XFFFB,0XFFFC,0XFFFE,0XFFFF,0XFFFF,0XFFFD,0XFFFA,0XFFF9,
                             0XFFF9,0XFFFB,0XFFFD,0XFFFD,0XFFFB,0XFFF7,0XFFF4,0XFFF3,0XFFF6,0XFFFA,
                             0XFFFF,0X0002,0X0003,0X0001,0XFFFF,0XFFFC,0XFFFB,0XFFFB,0XFFFD,0XFFFE,
                             0XFFFE,0XFFFD,0XFFFC,0XFFFC,0XFFFD,0XFFFE,0X0000,0X0001,0X0000,0XFFFF,
                             0XFFFB,0XFFF8,0XFFF5,0XFFF5,0XFFF6,0XFFF9,0XFFFB,0XFFFD,0XFFFE,0XFFFD,
                             0XFFFC,0XFFFC,0XFFFC,0XFFFD,0XFFFE,0XFFFE,0XFFFD,0XFFFC,0XFFFC,0XFFFC,
                             0XFFFC,0XFFFD,0XFFFE,0X0000,0X0000,0X0001,0X0001,0X0001,0X0000,0XFFFE,
                             0XFFFD,0XFFFD,0XFFFD,0XFFFF,0X0000,0X0000,0XFFFF,0XFFFE,0XFFFE,0XFFFF,
                             0X0000,0X0001,0X0000,0XFFFF,0XFFFC,0XFFFA,0XFFFA,0XFFFB,0XFFFC,0XFFFE,
                             0XFFFE,0XFFFE,0XFFFD,0XFFFE,0XFFFF,0X0000,0X0001,0X0000,0XFFFE,0XFFF9};

    uint16_t CONSTANT_data[43] = {0X0000,0X0000,0X0000,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                  0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                  0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,
                                  0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,0XAAAA,
                                 };
    uint8_t i=0;

    //Asynchronous Mode: DAC active register updates and outputs immediately as CS goes HIGH
    spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    spiControl->spi_Write_DAC(DAC_SYNC_ADDR, 0xF000);
    spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);
    spiControl->spi_Write_DAC(DAC_GAIN_ADDR, 0x010F);   //second digit indicates VREF value (0=2.5, 1=1.25),
                                                        //last 4 indicate gain (0=1, 1=2)
    spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0000);//third digit indicates synchronization (0=async, 1=sync)
    spiControl->spi_Read_DAC(DAC_GAIN_ADDR, &data);

    for (i = 0; i<43; i++)
     {
       spiControl->spi_Write_DAC(DAC_OUT0_ADDR, EMG_data[i]);  //Send EMG data
     }


/*
    //Sychronous Mode: DAC active register updates on falling edge of LDAC and outputs as CS goes HIGH
    spiControl->spi_Write_DAC(DAC_SYNC_ADDR, 0xFF0F);
    spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);
    spiControl->spi_Write_DAC(DAC_GAIN_ADDR, 0x010F);
    spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0010);


    //Countinously send data
    for (i = 0; i<84; i++)
    {
       //spiControl->spi_Write_DAC(DAC_OUT0_ADDR, EMG_data[i]);     //Send EMG data
       spiControl->spi_Write_DAC(DAC_OUT0_ADDR, 0XFFFF);            //Send 2.5V
       spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0000);         //Pulls LDAC LOW to update register
       spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0010);         //Set LDAC HIGH to reset synchronous mode
    }
*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

