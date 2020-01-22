#include "spicontrol.h"
//#include "ui_spicontrol.h"
#include "stdint.h"
#include <unistd.h>

#define WREN				0x06
#define WRDI				0x04
#define WRITE				0x02
#define READ				0x03
#define RDSR				0x05

#define CLK_MODE			SPI_CLK_MODE_0
#define CHIP_SELECT			SPI_CS_ACTIVE_LOW
#define BIT_ORDER			SPI_BO_MSB_FIRST
#define MAX_BUS_SPEED		1000000 /* 1MHz */
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

    /* Set the write-enable bit. */
    //spi_Write_enable();

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

/*
 * spi_Write_enable() - Sets the SPI write enable bit
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */

static int spi_Write_enable(void)
{
    uint8_t write_data[1] = {0};

    printf("[INFO] Setting write enable bit...\n");
    write_data[0] = WREN;

    return ldx_spi_write(spi_dev, write_data, sizeof(write_data));
}

/*
 * spi_Read_status() - Reads the SPI status register
 *
 * @status:	Variable to store the read status.
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */

static int spi_Read_status(uint8_t *status)
{
    uint8_t write_data[2] = {0};
    uint8_t read_data[2] = {0};

    printf("[INFO] Reading status register...\n");
    write_data[0] = RDSR;
    if (ldx_spi_transfer(spi_dev, write_data, read_data, 2) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    printf("[INFO] SPI Status Register is 0x%02x\n", read_data[1]);
    *status = read_data[1];

    return EXIT_SUCCESS;
}

/*
 * read_status_register() - Reads the SPI status register
 *
 * @status:	Variable to store the read status.
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */
static int read_status_register(uint8_t *status)
{
    uint8_t write_data[2] = {0};
    uint8_t read_data[2] = {0};

    printf("[INFO] Reading status register...\n");
    write_data[0] = RDSR;
    if (ldx_spi_transfer(spi_dev, write_data, read_data, 2) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("[INFO] SPI Status Register is 0x%02x\n", read_data[1]);
    *status = read_data[1];

    return EXIT_SUCCESS;
}

/*
 * spi_Write() - Writes an EEPROM page with the given data
 *
 * @page_index:	index of the EEPROM page to write.
 * @data:	the data to write.
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */

static int spi_Write(int page_index, uint8_t* data)
{
    uint8_t *write_data;
    uint16_t page_address = page_size * page_index;
    uint8_t status = 0;
    int i, ret = 0;

    /* Create the page write buffer */
    write_data = (uint8_t *)calloc(page_size + OPERATION_BYTES + address_bytes,sizeof(uint8_t));
    if (write_data == NULL)
    {
        printf("Unable to allocate memory to write the page.");
        return EXIT_FAILURE;
    }

    printf("[INFO] Writing %d bytes to page %d at address 0x%x...\n", page_size,page_index, page_address);
    write_data[0] = WRITE; // Operation.
    for (i = 0; i < address_bytes; i++)
    {
        write_data[i + OPERATION_BYTES] = (page_address >> (8 * (address_bytes - i - 1)));
    }

    /* Fill the data array. */
    for (i = 0; i < page_size; i++)
    {
        write_data[(i + OPERATION_BYTES + address_bytes)] = data[i];
    }

    /* Perform the write operation. */
    if (ldx_spi_write(spi_dev, write_data, page_size + OPERATION_BYTES +
              address_bytes) != EXIT_SUCCESS)
    {
        free(write_data);
        return EXIT_FAILURE;
    }

    /* Wait for the operation to complete. */
    do {
        /* Read the status register to check the WIP (write-in-progress) bit. */
        ret = read_status_register(&status);
        if (ret != EXIT_SUCCESS)
        {
            free(write_data);
            return EXIT_FAILURE;
        }
        /* Check the WIP (write-in-progress) status bit. */
        if (status & 0x01) {
            printf("[INFO] Write in progress...\n");
            usleep(1);
        } else
        {
            printf("[INFO] Write finished!\n");
        }
    } while (status & 0x1);

    free(write_data);

    return EXIT_SUCCESS;
}

/*
 * spi_Read() - Reads an EEPROM page
 *
 * @page_index:	index of the EEPROM page to read.
 * @data:	buffer to store the read data in.
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE otherwise.
 */

static int spi_Read(int page_index, uint8_t* data)
{
    uint8_t *write_data;
    uint8_t *read_data;
    uint16_t page_address;
    int i = 0;

    page_address = page_size * page_index;

    /* Create the buffers */
    write_data = (uint8_t *)calloc(page_size + OPERATION_BYTES + address_bytes, sizeof(uint8_t));
    read_data = (uint8_t *)calloc(page_size + OPERATION_BYTES + address_bytes, sizeof(uint8_t));
    if (write_data == NULL || read_data == NULL)
    {
        printf("Unable to allocate memory to read the page.");
        return EXIT_FAILURE;
    }

    printf("[INFO] Reading page %d at address 0x%x...\n", page_index, page_address);
    write_data[0] = READ; // Operation.
    for (i = 0; i < address_bytes; i++)
    {
        write_data[i + OPERATION_BYTES] = (page_address >> (8 * (address_bytes - i - 1)));
    }

    /* Perform the read operation with a transfer */
    if (ldx_spi_transfer(spi_dev, write_data, read_data, page_size + OPERATION_BYTES + address_bytes) != EXIT_SUCCESS)
    {
        free(write_data);
        free(read_data);
        return EXIT_FAILURE;
    }
    for (i = 0; i < page_size; i++)
    {
        data[i] = read_data[(i + OPERATION_BYTES + address_bytes)];
    }

    free(write_data);
    free(read_data);

    return EXIT_SUCCESS;
}
