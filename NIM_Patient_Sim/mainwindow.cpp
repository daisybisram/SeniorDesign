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
//DAC_DEVID_ADDR
    uint16_t data;
    //spiControl->spi_Read_DAC(DAC_CONFIG_ADDR, &data);
    //spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);
    //spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    //spiControl->spi_Read_DAC(DAC_CONFIG_ADDR, &data);

    uint16_t EMG_data[21] = {0X0198,0X01E0,0X0223,0X025A,0X0281,0X0299,0X02A3,0X02A1,0X0296,0X0283,
                             0X0269,0X0247,0X021C,0X01E9,0X01AF,0X0170,0X012D,0X00E8,0X00A0,0X0057,0X0009};
     uint8_t i;
    //Synchronous Mode: DAC active register updates immediately and outputs as CS goes HIGH
    for (i = 0; i<20; i++)
    {
       spiControl->spi_Write_DAC(DAC_OUT0_ADDR, 0XAAAA);
    }

    //Asychronous Mode: DAC active register updates on falling edge of LDAC and outputs as CS goes HIGH
    spiControl->spi_Write_DAC(DAC_SYNC_ADDR, 0xFF0F);
    spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x001F);

    for (i = 0; i<20; i++)
    {
       spiControl->spi_Write_DAC(DAC_OUT0_ADDR, 0XAAAA);
       spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x000F);
       spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x001F);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

