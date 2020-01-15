#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gpioControl = new GpioControl(ui->gpioCntrl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

