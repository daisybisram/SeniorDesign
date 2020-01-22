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
    spiControl->spi_Read()
}

MainWindow::~MainWindow()
{
    delete ui;
}

