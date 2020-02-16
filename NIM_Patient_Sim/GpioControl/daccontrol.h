#ifndef DAC_H
#define DAC_H

#define DAC_WRITE_BIT       0x0
#define DAC_READ_BIT        0x80
#define DAC_NOP_ADDR		0x00		//no operation commands
#define DAC_DEVID_ADDR		0x01		//sets device ID, resolution, # of channels
#define DAC_SYNC_ADDR		0x2		//sets synchronization parameters
#define DAC_CONFIG_ADDR		0x3		//sets CRC, ALARM, Disable SDO...
#define DAC_GAIN_ADDR		0x04		//sets GAIN parameter
#define DAC_TRIGGER_ADDR	0x05		//synchronously load DACs set to synchronous mode by SYNC REG
#define DAC_BRDCAST_ADDR	0x6		//forces DAC channels to update active data register DAC{[15:0]}
#define DAC_STATUS_ADDR		0x7		//set reference alarm bit
#define DAC_OUT0_ADDR       0x08		//store 16 or 14 bit data to be loaded to DAC{[15:0]}


#endif // DAC_H
