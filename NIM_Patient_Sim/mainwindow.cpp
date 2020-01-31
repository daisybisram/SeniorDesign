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
    spiControl->spi_Read_DAC(DAC_CONFIG_ADDR, &data);
    spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0001);
    spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    spiControl->spi_Read_DAC(DAC_CONFIG_ADDR, &data);

}

MainWindow::~MainWindow()
{
    delete ui;
}

