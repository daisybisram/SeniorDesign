#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gpioControl = new GpioControl(ui->gpioCntrl);
    gpioControl->ToggleGpio(32);
    adcControl = new AdcControl();
    spiControl = new SpiControl();
    uint8_t val;
    spiControl->spi_Read_status(&val);
}

MainWindow::~MainWindow()
{
    delete ui;
}

