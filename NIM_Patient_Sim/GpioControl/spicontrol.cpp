#include "spicontrol.h"
//#include "ui_spicontrol.h"
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

#include <libdigiapix/spi.h>

#define DEFAULT_SPI_ALIAS		"DEFAULT_SPI"
#define DEFAULT_SPI_ADDRESS_SIZE	1
#define DEFAULT_SPI_PAGE_SIZE		16
#define DEFAULT_SPI_PAGE_INDEX		0

#define ARG_SPI_DEVICE		0
#define ARG_SPI_SLAVE		0

#define BUFF_SIZE			256

#define CLK_MODE			SPI_CLK_MODE_3
#define CHIP_SELECT			SPI_CS_ACTIVE_LOW
#define BIT_ORDER			SPI_BO_MSB_FIRST
#define MAX_BUS_SPEED		50000000
#define BITS_PER_WORD		SPI_BPW_8

#define OPERATION_BYTES		1

static spi_t *spi_dev;
static unsigned int page_size, address_bytes = 0;
static uint8_t *tx_buffer;
static uint8_t *rx_buffer;

SpiControl::SpiControl(QWidget *parent) :
    QScrollArea(parent)
    //ui(new Ui::SpiControl)
{
    int spi_device = 0, spi_slave = 0;
    spi_transfer_cfg_t transfer_mode;

    spi_device = ldx_spi_get_device(DEFAULT_SPI_ALIAS);
    spi_slave = ldx_spi_get_slave(DEFAULT_SPI_ALIAS);

    /* Request SPI */
    spi_dev = ldx_spi_request((unsigned int)spi_device, (unsigned int)spi_slave);

    /* Configure the transfer mode */
    transfer_mode.clk_mode = CLK_MODE;
    transfer_mode.chip_select = CHIP_SELECT;
    transfer_mode.bit_order = BIT_ORDER;

    ldx_spi_set_transfer_mode(spi_dev, &transfer_mode);

    /* Configure the bits-per-word */
    ldx_spi_set_bits_per_word(spi_dev, BITS_PER_WORD);

    /* Configure the max bus speed */
    ldx_spi_set_speed(spi_dev, MAX_BUS_SPEED);

    /* Initialize the write and read buffers */
    tx_buffer = (uint8_t *)calloc(page_size, sizeof(uint8_t));
    rx_buffer = (uint8_t *)calloc(page_size, sizeof(uint8_t));
}

SpiControl::~SpiControl()
{
    /* Free spi */
    ldx_spi_free(spi_dev);

    /* Free buffers */
    free(tx_buffer);
    free(rx_buffer);
}


int SpiControl::spi_Read_DAC(uint8_t reg, uint16_t *data)
{
    uint8_t read_reg = DAC_READ_BIT | reg;

    uint8_t tx_data8[3] = {read_reg, 0x00, 0x00};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};

//READ ACCESS CYCLE
  if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }


//READ CYCLE
//After the READ ACCESS cycle, we send any command on MOSI, but the data we'll get back will be from the previous READ ACCESS command
//On MISO, we will see the first byte echoing our command from the READ ACCESS cycle
//so we know on MOSI we won't be trying to read DEVID again.

    if (ldx_spi_read(spi_dev, rx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

   *data = rx_data8[1] << 8 | rx_data8[2];
        return EXIT_SUCCESS;

}

int SpiControl::spi_Write_DAC(uint8_t reg, uint16_t data)
{
    uint8_t write_reg = DAC_WRITE_BIT | reg;
    uint8_t write_data_8MSB = data>>8;
    uint8_t write_data_8LSB = data;

    uint8_t tx_data8[3] = {write_reg, write_data_8MSB, write_data_8LSB};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};


    if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;

}


int SpiControl::spi_Read_ADC(uint8_t reg, uint8_t *data)
{

uint8_t read_reg = 0;

uint8_t tx_data8[3] = {read_reg, 0x00, 0x00};
uint8_t rx_data8[3] = {0x00, 0x00, 0x00};

//READ ACCESS CYCLE
if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }


if (ldx_spi_read(spi_dev, rx_data8, 3) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

*data = rx_data8[1] << 8 | rx_data8[2];
    return EXIT_SUCCESS;

}

int SpiControl::spi_Write_ADC(uint8_t reg, uint8_t data)
{
    uint8_t write_reg = 0;
    uint8_t write_data_8MSB = data>>8;
    uint8_t write_data_8LSB = data;

    uint8_t tx_data8[3] = {write_reg, write_data_8MSB, write_data_8LSB};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};


    if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}
