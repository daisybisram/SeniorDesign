#include "stdint.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dac.h"
#include "spicontrol.h"
#include <libdigiapix/spi.h>

DacControl::DacwSAControl(QWidget *parent) :
    QScrollArea(parent)
    //ui(new Ui::SpiControl)
{

}

DacControl::~DacControl()
{
    /* Free spi */
    ldx_spi_free(spi_dev);

    /* Free buffers */
    free(tx_buffer);
    free(rx_buffer);
}
uint16_t EMG_data[21] = {0X0198,0X01E0,0X0223,0X025A,0X0281,0X0299,0X02A3,0X02A1,0X0296,0X0283,
                         0X0269,0X0247,0X021C,0X01E9,0X01AF,0X0170,0X012D,0X00E8,0X00A0,0X0057,0X0009};
uint8_t i;
for (i = 0; i<20; i++)
{

}
