#include "adccontrol.h"
#include "ui_adccontrol.h"
#include "stdint.h"

struct adc_sampling_cb_data {
    adc_t *adc;
    int number_of_samples;
};

static adc_t *adc;
struct adc_sampling_cb_data cb_data;
AdcControl *adcControl;

AdcControl::AdcControl(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::AdcControl)
{
    uint8_t chip = 0;
    uint8_t channel = 1;
    adc = ldx_adc_request(chip, channel);
    adcControl = this;
}

AdcControl::~AdcControl()
{
    /* Free adc */
    ldx_adc_stop_sampling(adc);
    ldx_adc_free(adc);
}

int adcCallback(void *arg)
{
    // When positive edge is detected
    uint8_t mv = 90;
    emit adcControl->postiveEdgeDetected(mv);
}

void AdcControl::Start(uint8_t interval)
{
    cb_data.adc = adc;
    ldx_adc_start_sampling(adc, &adcCallback, interval, &cb_data))
}

void AdcControl::Stop(void)
{
    ldx_adc_stop_sampling(adc);
}
