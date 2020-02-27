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

int gpioValArray[36];
int gpioCallback1(void *arg);

GpioControl::GpioControl(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::GpioControl)

{
    ui->setupUi(this);

// GPIO Input interrupt
    gpio_input0 = ldx_gpio_request((unsigned int)STIM_DETECT, GPIO_IRQ_EDGE_FALLING, REQUEST_SHARED);

    ldx_gpio_set_active_mode(gpio_input0, GPIO_ACTIVE_LOW);
    ldx_gpio_start_wait_interrupt(gpio_input0, &gpioCallback1, this);

// GPIO Outputs(LED) w/ PushButtons
    gpio_output0 = ldx_gpio_request((unsigned int)USER_LED0, GPIO_OUTPUT_LOW, REQUEST_SHARED);
    gpio_output1 = ldx_gpio_request((unsigned int)USER_LED1, GPIO_OUTPUT_LOW, REQUEST_SHARED);
    gpio_output2 = ldx_gpio_request((unsigned int)USER_LED2, GPIO_OUTPUT_LOW, REQUEST_SHARED);
    if (!gpio_output0 || !gpio_output1 || !gpio_output2) {
        printf("Failed to initialize output GPIO\n");
    }
   /* connect(ui->pushButton, &QPushButton::released, [this]() { ToggleGpio(USER_LED2); });
    connect(ui->pushButton_2, &QPushButton::released, [this]() { ToggleGpio(USER_LED1); });
    connect(ui->pushButton_3, &QPushButton::released, [this]() { ToggleGpio(USER_LED0); });*/
}

GpioControl::~GpioControl()
{
    delete ui;
    ldx_gpio_free(gpio_output0);
    ldx_gpio_free(gpio_output1);
    ldx_gpio_free(gpio_output2);
    ldx_gpio_free(gpio_input0);
}

int gpioCallback1(void *arg)
{
    GpioControl *gpioCntrl = (GpioControl*)(arg);
    gpioCntrl->ToggleGpio(USER_LED0);
    gpioCntrl->ToggleGpio(USER_LED1);
    gpioCntrl->ToggleGpio(USER_LED2);
    printf("Pulse Recieved\n");
}


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



