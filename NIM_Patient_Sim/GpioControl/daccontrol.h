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

/*uint16_t EMG_data[400] = {0x837F,0x8384,0x83BA,0x8417,0x8488,0x84F3,0x854C,0x858F,0x85C3,0x85EF,0x8619,0x863D,0x8655,0x865B,0x864C,0x862E,
                          0x860A,0x85EC,0x85D9,0x85CE,0x85BC,0x8595,0x8552,0x84FD,0x84AD,0x8478,0x846F,0x8498,0x84F3,0x857F,0x8631,0x86ED,
                          0x877B,0x878D,0x86D8,0x8534,0x82AC,0x7F83,0x7C16,0x78CF,0x761D,0x747B,0x7475,0x769C,0x7B58,0x82BE,0x8C81,0x980E,
                          0xA4CB,0xB253,0xC07E,0xCF28,0xDDDB,0xEB8D,0xF6BC,0xFDDD,0xFFFF,0xFD46,0xF6E1,0xEE91,0xE5D8,0xDD51,0xD493,0xCA97,
                          0xBE78,0xB014,0xA034,0x9036,0x816E,0x74A4,0x69EB,0x60D8,0x58DA,0x5188,0x4AB6,0x4455,0x3E56,0x3894,0x32D9,0x2CF6,
                          0x26D0,0x206F,0x19F4,0x139D,0x0DB7,0x0896,0x0486,0x01B9,0x003D,0x0000,0x00CC,0x0261,0x047A,0x06E4,0x0989,0x0C75,
                          0x0FD6,0x13E3,0x18C7,0x1E8C,0x2525,0x2C79,0x348B,0x3D7C,0x4777,0x528A,0x5E7B,0x6AC5,0x76C0,0x81EA,0x8C26,0x95C7,
                          0x9F5E,0xA967,0xB3EF,0xBE7F,0xC849,0xD080,0xD6B5,0xDAFD,0xDDD3,0xDFD0,0xE15C,0xE289,0xE31E,0xE2CA,0xE15A,0xDEDE,
                          0xDBA0,0xD804,0xD45F,0xD0D6,0xCD5E,0xC9C9,0xC5EF,0xC1C4,0xBD60,0xB8F8,0xB4B8,0xB0BB,0xACF9,0xA95D,0xA5D3,0xA258,
                          0x9EFA,0x9BCF,0x98E4,0x9633,0x93A1,0x910F,0x8E64,0x8B9D,0x88D8,0x8645,0x8416,0x8262,0x811E,0x801A,0x7F20,0x7E0F,
                          0x7CEB,0x7BDA,0x7B09,0x7A8E,0x7A57,0x7A39,0x7A0C,0x79C1,0x796F,0x793D,0x7947,0x798A,0x79DF,0x7A1A,0x7A25,0x7A0F,
                          0x7A04,0x7A2A,0x7A89,0x7B03,0x7B69,0x7B9D,0x7BA9,0x7BB5,0x7BEC,0x7C60,0x7CFF,0x7DA2,0x7E2C,0x7E96,0x7EF2,0x7F53,
                          0x7FC2,0x8033,0x8094,0x80D9,0x8103,0x8122,0x814B,0x8190,0x81F6,0x8274,0x82F3,0x8353,0x837E,0x8375,0x8353,0x8342,
                          0x8361,0x83AF,0x840D,0x844E,0x845C,0x8440,0x841D,0x8415,0x842D,0x8450,0x845B,0x843E,0x8408,0x83E3,0x83F7,0x8450,
                          0x84D7,0x855B,0x85A9,0x85A7,0x855A,0x84E5,0x8474,0x842A,0x8415,0x842C,0x8458,0x8482,0x849A,0x849D,0x848D,0x8471,
                          0x844E,0x8429,0x8409,0x83F3,0x83E8,0x83E5,0x83DE,0x83C8,0x83A1,0x836F,0x8343,0x832F,0x833B,0x8367,0x83A5,0x83E3,
                          0x8414,0x842D,0x842F,0x841D,0x8401,0x83E1,0x83C7,0x83B8,0x83B9,0x83CD,0x83EF,0x8414,0x842A,0x8422,0x83F6,0x83B3,
                          0x8372,0x834E,0x8353,0x8379,0x83A5,0x83BF,0x83BA,0x839F,0x8385,0x837B,0x8386,0x8395,0x8396,0x8380,0x8359,0x8331,
                          0x8319,0x8312,0x8313,0x830E,0x8302,0x82FC,0x8313,0x8355,0x83B6,0x8416,0x844E,0x844A,0x8415,0x83D6,0x83B7,0x83C8,
                          0x83F7,0x841E,0x8418,0x83E3,0x839D,0x8372,0x8379,0x83A9,0x83DB,0x83E3,0x83B0,0x8357,0x8307,0x82F0,0x8327,0x839C,
                          0x8420,0x847F,0x8497,0x846B,0x841A,0x83D1,0x83AF,0x83B9,0x83DB,0x83F6,0x83F7,0x83E1,0x83C6,0x83BE,0x83D6,0x8406,
                          0x8439,0x8455,0x8446,0x840C,0x83B5,0x835E,0x8322,0x8316,0x8339,0x837A,0x83BC,0x83E7,0x83F2,0x83E5,0x83D3,0x83CB,
                          0x83D4,0x83E7,0x83F5,0x83F6,0x83E9,0x83D5,0x83C6,0x83C4,0x83D0,0x83E6,0x8402,0x8422,0x8440,0x8457,0x8460,0x8454,
                          0x8432,0x8406,0x83E4,0x83DA,0x83EE,0x8410,0x842B,0x842E,0x841B,0x8405,0x8403,0x841C,0x8442,0x8458,0x844A,0x8416,
                          0x83D2,0x839E,0x8390,0x83A9,0x83D5,0x83F8,0x8402,0x83F8,0x83ED,0x83F8,0x841D,0x844B,0x8461,0x8444,0x83F2,0x8388
                         };

    uint16_t CONSTANT_data[200] = {0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,
                                   0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF,0XFFFF};

    uint16_t CONSTANT_data1[200] = {0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,0X0000,0XFFFF,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                    0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                                   };

*/
/*
    //Asynchronous Mode: DAC active register updates and outputs immediately as CS goes HIGH
    spiControl->spi_Read_DAC(DAC_DEVID_ADDR, &data);
    spiControl->spi_Write_DAC(DAC_SYNC_ADDR, 0xF000);
    spiControl->spi_Write_DAC(DAC_CONFIG_ADDR, 0x0000);
    spiControl->spi_Write_DAC(DAC_GAIN_ADDR, 0x010F);   //second digit indicates VREF value (0=2.5, 1=1.25),
                                                        //last 4 indicate gain (0=1, 1=2)
    spiControl->spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0000);//third digit indicates synchronization (0=async, 1=sync)
    spiControl->spi_Read_DAC(DAC_GAIN_ADDR, &data);

    for (i = 0; i<43; i++)
     {
       spiControl->spi_Write_DAC(DAC_OUT0_ADDR, EMG_data[i]);  //Send EMG data
     }*/

#endif // DAC_H
