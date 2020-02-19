#include "gpiocontrol.h"
#include "ui_gpiocontrol.h"
#include "libdigiapix/spi.h"
#include "stdint.h"

/* GPIO INITIALIZATION */
gpio_t *gpio_output0;
gpio_t *gpio_output1;
gpio_t *gpio_output2;
gpio_t *gpio_input0;

/*  GPIO TESTING  */
gpio_t *gpio_output3;
#define GPIO_Test1   33

#define USER_LED0   34
#define USER_LED1   35
#define USER_LED2   36
#define STIM_DETECT 37


         /* Digital Potentiometer Addresses */
#define RDAC_1 0000                                        
#define RDAC_2 0001
         /* Digital Potentiometer Commands */
#define COMMAND_1 0000   //Do Nothing
//#define COMMAND_4 0100   //Decrement RDAC(1 or 2) by 6dB
//#define COMMAND_5 0101   //Decrement all by 6dB
#define COMMAND_6 0110   //Decrement RDAC(1 or 2) by 1
//#define COMMAND_7 0111   //Decrement all by 1
//#define COMMAND_12 1100  //Increment RDAC(1 or 2) by 6dB
//#define COMMAND_13 1101  //Increment all by 6dB
#define COMMAND_14 1110  //Increment RDAC(1 or 2) by 1
//#define COMMAND_15 1111  //Increment all by 1


int gpioValArray[36];
int gpioCallback(void *arg);

GpioControl::GpioControl(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::GpioControl)

{//
    ui->setupUi(this);

// GPIO Input interrupt
    gpio_input0 = ldx_gpio_request((unsigned int)STIM_DETECT, GPIO_IRQ_EDGE_FALLING, REQUEST_SHARED);
    ldx_gpio_set_active_mode(gpio_input0, GPIO_ACTIVE_HIGH);
    //ldx_gpio_start_wait_interrupt(gpio_input0, &gpioCallback, this);

//  GPIO TESTING
    gpio_output3 = ldx_gpio_request((unsigned int)GPIO_Test1, GPIO_OUTPUT_HIGH, REQUEST_SHARED);

// GPIO Outputs(LED) w/ PushButtons
    gpio_output0 = ldx_gpio_request((unsigned int)USER_LED0, GPIO_OUTPUT_LOW, REQUEST_SHARED);
    gpio_output1 = ldx_gpio_request((unsigned int)USER_LED1, GPIO_OUTPUT_LOW, REQUEST_SHARED);
    gpio_output2 = ldx_gpio_request((unsigned int)USER_LED2, GPIO_OUTPUT_LOW, REQUEST_SHARED);
    if (!gpio_output0 || !gpio_output1 || !gpio_output2) {
        printf("Failed to initialize output GPIO\n");
    }
    connect(ui->pushButton, &QPushButton::released, [this]() { ToggleGpio(USER_LED2); });
    connect(ui->pushButton_2, &QPushButton::released, [this]() { ToggleGpio(USER_LED1); });
    connect(ui->pushButton_3, &QPushButton::released, [this]() { ToggleGpio(USER_LED0); });
}

GpioControl::~GpioControl()
{
    delete ui;
    ldx_gpio_free(gpio_output0);
    ldx_gpio_free(gpio_output1);
    ldx_gpio_free(gpio_output2);
    ldx_gpio_free(gpio_input0);
}

/*int gpioCallback(void *arg)
{
    GpioControl *gpioCntrl = (GpioControl*)(arg);
    gpioCntrl->ToggleGpio(USER_LED0);
    gpioCntrl->ToggleGpio(USER_LED1);
    gpioCntrl->ToggleGpio(USER_LED2);
    printf("Pulse Recieved\n");
}*/


void GpioControl::ToggleGpio(int gpioNum)
{
    gpio_value_t output_value;
    gpio_t *gpio_output;

    gpio_output = ldx_gpio_request((unsigned int)gpioNum, GPIO_OUTPUT_LOW, REQUEST_SHARED);

    if (gpioValArray[gpioNum] == GPIO_LOW)
        output_value = GPIO_HIGH;
    else
        output_value = GPIO_LOW;

    gpioValArray[gpioNum] = output_value;

    ldx_gpio_set_value(gpio_output, output_value);
    ldx_gpio_free(gpio_output);
}



