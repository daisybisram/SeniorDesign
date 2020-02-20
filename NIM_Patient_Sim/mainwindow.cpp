#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "daccontrol.h"

#define STIM_DETECT 101  //Pin 13 EXP_GPIO_7
#define RELAY_GPIO  205  //Pin 12 EXP_GPIO_6
#define USER_LED0   34
#define USER_LED1   35
#define USER_LED2   36

int gpioArray[36];
int gpio_EMG_trigger_interrupt(void *arg);
int gpioCallback(void *arg);
int Gain_Control_Gpio(float_t gain, int gpioNum);
void ToggleGpio(int gpioNum);
int EMG_triggered_flag = 0;

float_t gain;
uint16_t delay_time_ms=0;
uint32_t amplitude_uV=4000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    adcControl = new AdcControl();
    spiControl = new SpiControl();

    uint16_t data;
    spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    //spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);

    //Initialize GPIOs
    gpio_t *gpio_input0;

    //This section includes the call of an interrupt to trigger the EMG response output
    gpio_input0 = ldx_gpio_request((unsigned int)STIM_DETECT, GPIO_IRQ_EDGE_FALLING, REQUEST_SHARED);
    ldx_gpio_set_active_mode(gpio_input0, GPIO_ACTIVE_LOW);
    ldx_gpio_start_wait_interrupt(gpio_input0, &gpio_EMG_trigger_interrupt, this);    //Callback function sets trigger flag to 1

    //Initialize DAC to Sychronous Mode: DAC active register updates on falling edge of LDAC and outputs as CS goes HIGH
    spiControl->spi_Write_DAC(DAC_SYNC_ADDR, 0xFF0F);
    spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);
    spiControl->spi_Write_DAC(DAC_GAIN_ADDR, 0x0100);
    spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0010);


    //Set amplitude and latency on UI
    connect(ui->latency_UP_button, &QPushButton::released, [this]() { onUpLatencyReleased(); });
    connect(ui->latency_DOWN_button, &QPushButton::released, [this]() { onDownLatencyReleased(); });
    connect(ui->amplitude_UP_button, &QPushButton::released, [this]() { onUpAmplitudeReleased(); });
    connect(ui->amplitude_DOWN_button, &QPushButton::released, [this]() { onDownAmplitudeReleased(); });


    if(amplitude_uV<6251)
    {
        gain =0.01;
    }
    else gain =0.1;

    spiControl->DAC_output(DAC_OUT0_ADDR, gain, delay_time_ms, amplitude_uV);

    //Constant checking of the trigger flag (if the falling edge on GPIO STIM_DETECT occurs proceed to DAC_output function)

    while(1)
    {
        if(EMG_triggered_flag == 1)
        {
            EMG_triggered_flag = 0;                 //Clear the flag

            if(amplitude_uV<6251)                   //Gain set in SW
            {
                gain =0.01;
            }
            else gain =0.1;

            Gain_Control_Gpio(gain, RELAY_GPIO);    //Gain set in HW

            spiControl->DAC_output(DAC_OUT0_ADDR, gain, delay_time_ms, amplitude_uV);   //Latency and Ammplitude set from UI
        }
    }

}

int gpio_EMG_trigger_interrupt(void *arg)
{
    GpioControl *gpioCntrl = (GpioControl*)(arg);
    ToggleGpio(USER_LED0);
    EMG_triggered_flag = 1;
}

int Gain_Control_Gpio(float_t gain, int gpioNum)
{
    gpio_value_t output_value;
    gpio_t *gpio_output;

    gpio_output = ldx_gpio_request((unsigned int)gpioNum, GPIO_OUTPUT_LOW, REQUEST_SHARED);

    if (gain > 0.01)
        output_value = GPIO_HIGH;

    gpioArray[gpioNum] = output_value;

    ldx_gpio_set_value(gpio_output, output_value);
    ldx_gpio_free(gpio_output);
}

void MainWindow::onUpLatencyReleased(void)
{
    if(delay_time_ms<40)
    {
       delay_time_ms++;
    }
    ui->latencyDisplay->display((int) delay_time_ms);
}

void MainWindow::onDownLatencyReleased(void)
{
    if(delay_time_ms>0)
    {
        delay_time_ms--;
    }
    ui->latencyDisplay->display((int) delay_time_ms);
}

void MainWindow::onUpAmplitudeReleased(void)
{

    if(amplitude_uV<100000)
    {
        amplitude_uV= amplitude_uV + 100;
    }
    ui->amplitudeDisplay->display((int) amplitude_uV);
}

void MainWindow::onDownAmplitudeReleased(void)
{
    if(amplitude_uV>100)
    {
        amplitude_uV= amplitude_uV - 100;
    }
    ui->amplitudeDisplay->display((int) amplitude_uV);
}

void ToggleGpio(int gpioNum)
{
    gpio_value_t output_value;
    gpio_t *gpio_output;

    gpio_output = ldx_gpio_request((unsigned int)gpioNum, GPIO_OUTPUT_LOW, REQUEST_SHARED);

    if (gpioArray[gpioNum] == GPIO_LOW)
        output_value = GPIO_HIGH;
    else
        output_value = GPIO_LOW;

    gpioArray[gpioNum] = output_value;

    ldx_gpio_set_value(gpio_output, output_value);
    ldx_gpio_free(gpio_output);
}

MainWindow::~MainWindow()
{
    delete ui;
}

