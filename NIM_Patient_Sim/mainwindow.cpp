#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include "daccontrol.h"
#include "adccontrol.h"


#define STIM_DETECT 101  //Pin 13 EXP_GPIO_7
#define RELAY_GPIO  205  //Pin 12 EXP_GPIO_6
#define USER_LED0   34

int gpioArray[36];
int gpio_EMG_trigger_interrupt(void *arg);
int Gain_Control_Gpio(float_t gain, int gpioNum);
void ToggleGpio(int gpioNum);
float_t gain;
uint16_t delay_time_ms=0;
uint32_t amplitude_uV=4000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    /* Creates a process to ensure the SPI device is active */
    QProcess process;
    process.startDetached("modprobe spidev");

    adcControl = new AdcControl();
    spiControl = new SpiControl();


//    uint16_t data;
//    spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);


/* ADS8910b */
//   uint8_t adc_reg_data;
//    uint32_t data_check;
//    spiControl->spi_Write_ADC(ADS_OFST_CAL_ADDR, 0x00);             //Write
//    spiControl->spi_Check_ADC_Reg(ADS_OFST_CAL_ADDR, &data_check);  //Read

    uint8_t adc_reg_data;
    uint8_t adc_reg_data1;
    uint8_t adc_reg_data2;
    spiControl->spi_Write_ADC(ADS_PATN_LSB_ADDR, 0x25);              //Write
    spiControl->spi_Write_ADC(ADS_PATN_MID_ADDR, 0x43);              //Write
    spiControl->spi_Write_ADC(ADS_PATN_MSB_ADDR, 0x05);              //Write
    spiControl->spi_Read_ADC(ADS_PATN_LSB_ADDR, &adc_reg_data);      //Read
    spiControl->spi_Read_ADC(ADS_PATN_MID_ADDR, &adc_reg_data1);     //Read
    spiControl->spi_Read_ADC(ADS_PATN_MSB_ADDR, &adc_reg_data2);     //Read

    uint32_t adc_conversion_data;
//    spiControl->spi_Write_ADC(ADS_DATA_CNTL_ADDR, 0x00);           //Set DATA_VAL=0 in ADC for 18-bit output word
    spiControl->spi_Write_ADC(ADS_DATA_CNTL_ADDR, 0x01);           //Set DATA_VAL=1 (FIXED PATTERN REGISTERS)
    spiControl->spi_Read_ADC_Conversion(&adc_conversion_data);
    //rx_data8[2] >> 4 is NOT returning the correct 4 LSB


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

}

//If the falling edge on GPIO STIM_DETECT occurs this function is called for the DAC_output
int gpio_EMG_trigger_interrupt(void *arg)
{
    SpiControl *spi;

    ToggleGpio(USER_LED0);

    if(amplitude_uV<6251)                   //Gain set in SW
    {
         gain =0.01;
    }
    else gain =0.1;

    Gain_Control_Gpio(gain, RELAY_GPIO);    //Gain set in HW

    spi->DAC_output(DAC_OUT0_ADDR, gain, delay_time_ms, amplitude_uV);   //Latenc y and Ammplitude set from UI

}



int Gain_Control_Gpio(float_t gain, int gpioNum)
{
    gpio_value_t output_value;
    gpio_t *gpio_output;

    gpio_output = ldx_gpio_request((unsigned int)gpioNum, GPIO_OUTPUT_LOW, REQUEST_SHARED);

    if (gain > 0.01)
    {   output_value = GPIO_HIGH;
        ToggleGpio(35);
    }

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

