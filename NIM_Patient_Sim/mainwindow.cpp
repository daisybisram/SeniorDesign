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
    spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    printf("%d\n", data);

}

MainWindow::~MainWindow()
{
    delete ui;
}

