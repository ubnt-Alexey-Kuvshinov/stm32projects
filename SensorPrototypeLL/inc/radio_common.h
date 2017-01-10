//radio-related commands, parameters and registers

#ifndef RADIO_H
#define RADIO_H


// Radio wakeup time from SLEEP mode
#define RADIO_OSC_STARTUP                           1000 // [us]

//Radio PLL lock and Mode Ready delay which can vary with the temperature
#define RADIO_SLEEP_TO_RX                           2000 // [us]

//Radio complete Wake-up Time with margin for temperature compensation
#define RADIO_WAKEUP_TIME                           ( RADIO_OSC_STARTUP + RADIO_SLEEP_TO_RX )


//SX1276 definitions
#define XTAL_FREQ                                   32000000
#define FREQ_STEP                                   (XTAL_FREQ / 524288)	//61.03515625 Hz

#define RX_BUFFER_SIZE                              256

// Constant values need to compute the RSSI value
#define RSSI_OFFSET_LF                              -164.0
#define RSSI_OFFSET_HF                              -157.0



// ============================================================================
//  SX1276 Internal registers Address
// ============================================================================

#define REG_LR_FIFO                                 0x00 
// Common settings
#define REG_LR_OPMODE                               0x01 
#define REG_LR_FRFMSB                               0x06 
#define REG_LR_FRFMID                               0x07
#define REG_LR_FRFLSB                               0x08 
// Tx settings
#define REG_LR_PACONFIG                             0x09 
#define REG_LR_PARAMP                               0x0A 
#define REG_LR_OCP                                  0x0B 
// Rx settings
#define REG_LR_LNA                                  0x0C 
// LoRa registers
#define REG_LR_FIFOADDRPTR                          0x0D 
#define REG_LR_FIFOTXBASEADDR                       0x0E 
#define REG_LR_FIFORXBASEADDR                       0x0F 
#define REG_LR_FIFORXCURRENTADDR                    0x10 
#define REG_LR_IRQFLAGSMASK                         0x11 
#define REG_LR_IRQFLAGS                             0x12 
#define REG_LR_RXNBBYTES                            0x13 
#define REG_LR_RXHEADERCNTVALUEMSB                  0x14 
#define REG_LR_RXHEADERCNTVALUELSB                  0x15 
#define REG_LR_RXPACKETCNTVALUEMSB                  0x16 
#define REG_LR_RXPACKETCNTVALUELSB                  0x17 
#define REG_LR_MODEMSTAT                            0x18 
#define REG_LR_PKTSNRVALUE                          0x19 
#define REG_LR_PKTRSSIVALUE                         0x1A 
#define REG_LR_RSSIVALUE                            0x1B 
#define REG_LR_HOPCHANNEL                           0x1C 
#define REG_LR_MODEMCONFIG1                         0x1D 
#define REG_LR_MODEMCONFIG2                         0x1E 
#define REG_LR_SYMBTIMEOUTLSB                       0x1F 
#define REG_LR_PREAMBLEMSB                          0x20 
#define REG_LR_PREAMBLELSB                          0x21 
#define REG_LR_PAYLOADLENGTH                        0x22 
#define REG_LR_PAYLOADMAXLENGTH                     0x23 
#define REG_LR_HOPPERIOD                            0x24 
#define REG_LR_FIFORXBYTEADDR                       0x25
#define REG_LR_MODEMCONFIG3                         0x26
#define REG_LR_FEIMSB                               0x28
#define REG_LR_FEIMID                               0x29
#define REG_LR_FEILSB                               0x2A
#define REG_LR_RSSIWIDEBAND                         0x2C
#define REG_LR_TEST2F                               0x2F
#define REG_LR_TEST30                               0x30
#define REG_LR_DETECTOPTIMIZE                       0x31
#define REG_LR_INVERTIQ                             0x33
#define REG_LR_TEST36                               0x36
#define REG_LR_DETECTIONTHRESHOLD                   0x37
#define REG_LR_SYNCWORD                             0x39
#define REG_LR_TEST3A                               0x3A
#define REG_LR_INVERTIQ2                            0x3B

// end of documented register in datasheet
// I/O settings
#define REG_LR_DIOMAPPING1                          0x40
#define REG_LR_DIOMAPPING2                          0x41
// Version
#define REG_LR_VERSION                              0x42
// Additional settings
#define REG_LR_PLLHOP                               0x44
#define REG_LR_TCXO                                 0x4B
#define REG_LR_PADAC                                0x4D
#define REG_LR_FORMERTEMP                           0x5B
#define REG_LR_BITRATEFRAC                          0x5D
#define REG_LR_AGCREF                               0x61
#define REG_LR_AGCTHRESH1                           0x62
#define REG_LR_AGCTHRESH2                           0x63
#define REG_LR_AGCTHRESH3                           0x64
#define REG_LR_PLL                                  0x70

// ============================================================================
// SX1276 LoRa bits control definition
// ============================================================================


//RegFifo

//RegOpMode
#define RFLR_OPMODE_LONGRANGEMODE_MASK              0x7F 
#define RFLR_OPMODE_LONGRANGEMODE_OFF               0x00 // Default
#define RFLR_OPMODE_LONGRANGEMODE_ON                0x80 

#define RFLR_OPMODE_ACCESSSHAREDREG_MASK            0xBF 
#define RFLR_OPMODE_ACCESSSHAREDREG_ENABLE          0x40 
#define RFLR_OPMODE_ACCESSSHAREDREG_DISABLE         0x00 // Default

#define RFLR_OPMODE_FREQMODE_ACCESS_MASK            0xF7
#define RFLR_OPMODE_FREQMODE_ACCESS_LF              0x08 // Default
#define RFLR_OPMODE_FREQMODE_ACCESS_HF              0x00 

#define RFLR_OPMODE_MASK                            0xF8 
#define RFLR_OPMODE_SLEEP                           0x00 
#define RFLR_OPMODE_STANDBY                         0x01 // Default
#define RFLR_OPMODE_SYNTHESIZER_TX                  0x02 
#define RFLR_OPMODE_TRANSMITTER                     0x03 
#define RFLR_OPMODE_SYNTHESIZER_RX                  0x04 
#define RFLR_OPMODE_RECEIVER                        0x05 
// LoRa specific modes
#define RFLR_OPMODE_RECEIVER_SINGLE                 0x06 
#define RFLR_OPMODE_CAD                             0x07 

//RegFrf (MHz)
#define RFLR_FRFMSB_434_MHZ                         0x6C // Default
#define RFLR_FRFMID_434_MHZ                         0x80 // Default
#define RFLR_FRFLSB_434_MHZ                         0x00 // Default

// RegPaConfig
#define RFLR_PACONFIG_PASELECT_MASK                 0x7F 
#define RFLR_PACONFIG_PASELECT_PABOOST              0x80 
#define RFLR_PACONFIG_PASELECT_RFO                  0x00 // Default

#define RFLR_PACONFIG_MAX_POWER_MASK                0x8F

#define RFLR_PACONFIG_OUTPUTPOWER_MASK              0xF0 
 
//RegPaRamp
#define RFLR_PARAMP_TXBANDFORCE_MASK                0xEF 
#define RFLR_PARAMP_TXBANDFORCE_BAND_SEL            0x10 
#define RFLR_PARAMP_TXBANDFORCE_AUTO                0x00 // Default

#define RFLR_PARAMP_MASK                            0xF0 
#define RFLR_PARAMP_3400_US                         0x00 
#define RFLR_PARAMP_2000_US                         0x01 
#define RFLR_PARAMP_1000_US                         0x02
#define RFLR_PARAMP_0500_US                         0x03 
#define RFLR_PARAMP_0250_US                         0x04 
#define RFLR_PARAMP_0125_US                         0x05 
#define RFLR_PARAMP_0100_US                         0x06 
#define RFLR_PARAMP_0062_US                         0x07 
#define RFLR_PARAMP_0050_US                         0x08 
#define RFLR_PARAMP_0040_US                         0x09 // Default
#define RFLR_PARAMP_0031_US                         0x0A 
#define RFLR_PARAMP_0025_US                         0x0B 
#define RFLR_PARAMP_0020_US                         0x0C 
#define RFLR_PARAMP_0015_US                         0x0D 
#define RFLR_PARAMP_0012_US                         0x0E 
#define RFLR_PARAMP_0010_US                         0x0F 

//RegOcp
#define RFLR_OCP_MASK                               0xDF 
#define RFLR_OCP_ON                                 0x20 // Default
#define RFLR_OCP_OFF                                0x00   

#define RFLR_OCP_TRIM_MASK                          0xE0
#define RFLR_OCP_TRIM_045_MA                        0x00
#define RFLR_OCP_TRIM_050_MA                        0x01   
#define RFLR_OCP_TRIM_055_MA                        0x02 
#define RFLR_OCP_TRIM_060_MA                        0x03 
#define RFLR_OCP_TRIM_065_MA                        0x04 
#define RFLR_OCP_TRIM_070_MA                        0x05 
#define RFLR_OCP_TRIM_075_MA                        0x06 
#define RFLR_OCP_TRIM_080_MA                        0x07  
#define RFLR_OCP_TRIM_085_MA                        0x08
#define RFLR_OCP_TRIM_090_MA                        0x09 
#define RFLR_OCP_TRIM_095_MA                        0x0A 
#define RFLR_OCP_TRIM_100_MA                        0x0B  // Default
#define RFLR_OCP_TRIM_105_MA                        0x0C 
#define RFLR_OCP_TRIM_110_MA                        0x0D 
#define RFLR_OCP_TRIM_115_MA                        0x0E 
#define RFLR_OCP_TRIM_120_MA                        0x0F 
#define RFLR_OCP_TRIM_130_MA                        0x10
#define RFLR_OCP_TRIM_140_MA                        0x11   
#define RFLR_OCP_TRIM_150_MA                        0x12 
#define RFLR_OCP_TRIM_160_MA                        0x13 
#define RFLR_OCP_TRIM_170_MA                        0x14 
#define RFLR_OCP_TRIM_180_MA                        0x15 
#define RFLR_OCP_TRIM_190_MA                        0x16 
#define RFLR_OCP_TRIM_200_MA                        0x17  
#define RFLR_OCP_TRIM_210_MA                        0x18
#define RFLR_OCP_TRIM_220_MA                        0x19 
#define RFLR_OCP_TRIM_230_MA                        0x1A 
#define RFLR_OCP_TRIM_240_MA                        0x1B

//RegLna
#define RFLR_LNA_GAIN_MASK                          0x1F 
#define RFLR_LNA_GAIN_G1                            0x20 // Default
#define RFLR_LNA_GAIN_G2                            0x40 
#define RFLR_LNA_GAIN_G3                            0x60 
#define RFLR_LNA_GAIN_G4                            0x80 
#define RFLR_LNA_GAIN_G5                            0xA0 
#define RFLR_LNA_GAIN_G6                            0xC0 

#define RFLR_LNA_BOOST_LF_MASK                      0xE7 
#define RFLR_LNA_BOOST_LF_DEFAULT                   0x00 // Default

#define RFLR_LNA_BOOST_HF_MASK                      0xFC 
#define RFLR_LNA_BOOST_HF_OFF                       0x00 // Default
#define RFLR_LNA_BOOST_HF_ON                        0x03 

//RegFifoAddrPtr
#define RFLR_FIFOADDRPTR                            0x00 // Default

//RegFifoTxBaseAddr
#define RFLR_FIFOTXBASEADDR                         0x80 // Default

//RegFifoTxBaseAddr
#define RFLR_FIFORXBASEADDR                         0x00 // Default

//RegFifoRxCurrentAddr (Read Only)

//RegIrqFlagsMask
#define RFLR_IRQFLAGS_RXTIMEOUT_MASK                0x80 
#define RFLR_IRQFLAGS_RXDONE_MASK                   0x40 
#define RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK          0x20 
#define RFLR_IRQFLAGS_VALIDHEADER_MASK              0x10 
#define RFLR_IRQFLAGS_TXDONE_MASK                   0x08 
#define RFLR_IRQFLAGS_CADDONE_MASK                  0x04 
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK       0x02 
#define RFLR_IRQFLAGS_CADDETECTED_MASK              0x01 

//RegIrqFlags
#define RFLR_IRQFLAGS_RXTIMEOUT                     0x80 
#define RFLR_IRQFLAGS_RXDONE                        0x40 
#define RFLR_IRQFLAGS_PAYLOADCRCERROR               0x20 
#define RFLR_IRQFLAGS_VALIDHEADER                   0x10 
#define RFLR_IRQFLAGS_TXDONE                        0x08 
#define RFLR_IRQFLAGS_CADDONE                       0x04 
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL            0x02 
#define RFLR_IRQFLAGS_CADDETECTED                   0x01 

//RegFifoRxNbBytes (Read Only)

//RegRxHeaderCntValueMsb (Read Only)

//RegRxHeaderCntValueLsb (Read Only)

//RegRxPacketCntValueMsb (Read Only)

//RegRxPacketCntValueLsb (Read Only)

//RegModemStat (Read Only)
#define RFLR_MODEMSTAT_RX_CR_MASK                   0x1F 
#define RFLR_MODEMSTAT_MODEM_STATUS_MASK            0xE0 
 
//RegPktSnrValue (Read Only)

//RegPktRssiValue (Read Only)

//RegRssiValue (Read Only)

//RegHopChannel (Read Only)
#define RFLR_HOPCHANNEL_PLL_LOCK_TIMEOUT_MASK       0x7F 
#define RFLR_HOPCHANNEL_PLL_LOCK_FAIL               0x80 
#define RFLR_HOPCHANNEL_PLL_LOCK_SUCCEED            0x00 // Default
                                                    
#define RFLR_HOPCHANNEL_CRCONPAYLOAD_MASK           0xBF
#define RFLR_HOPCHANNEL_CRCONPAYLOAD_ON             0x40
#define RFLR_HOPCHANNEL_CRCONPAYLOAD_OFF            0x00 // Default

#define RFLR_HOPCHANNEL_CHANNEL_MASK                0x3F 

//RegModemConfig1
#define RFLR_MODEMCONFIG1_BW_MASK                   0x0F 
#define RFLR_MODEMCONFIG1_BW_7_81_KHZ               0x00 
#define RFLR_MODEMCONFIG1_BW_10_41_KHZ              0x10 
#define RFLR_MODEMCONFIG1_BW_15_62_KHZ              0x20 
#define RFLR_MODEMCONFIG1_BW_20_83_KHZ              0x30 
#define RFLR_MODEMCONFIG1_BW_31_25_KHZ              0x40 
#define RFLR_MODEMCONFIG1_BW_41_66_KHZ              0x50 
#define RFLR_MODEMCONFIG1_BW_62_50_KHZ              0x60 
#define RFLR_MODEMCONFIG1_BW_125_KHZ                0x70 // Default
#define RFLR_MODEMCONFIG1_BW_250_KHZ                0x80 
#define RFLR_MODEMCONFIG1_BW_500_KHZ                0x90 
                                                    
#define RFLR_MODEMCONFIG1_CODINGRATE_MASK           0xF1 
#define RFLR_MODEMCONFIG1_CODINGRATE_4_5            0x02
#define RFLR_MODEMCONFIG1_CODINGRATE_4_6            0x04 // Default
#define RFLR_MODEMCONFIG1_CODINGRATE_4_7            0x06 
#define RFLR_MODEMCONFIG1_CODINGRATE_4_8            0x08 
                                                    
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK       0xFE 
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_ON         0x01 
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF        0x00 // Default

//RegModemConfig2
#define RFLR_MODEMCONFIG2_SF_MASK                   0x0F 
#define RFLR_MODEMCONFIG2_SF_6                      0x60 
#define RFLR_MODEMCONFIG2_SF_7                      0x70 // Default
#define RFLR_MODEMCONFIG2_SF_8                      0x80 
#define RFLR_MODEMCONFIG2_SF_9                      0x90 
#define RFLR_MODEMCONFIG2_SF_10                     0xA0 
#define RFLR_MODEMCONFIG2_SF_11                     0xB0 
#define RFLR_MODEMCONFIG2_SF_12                     0xC0 

#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_MASK     0xF7 
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_ON       0x08 
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF      0x00 

#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK         0xFB 
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON           0x04 
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_OFF          0x00 // Default
 
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK       0xFC 
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB            0x00 // Default

//RegSymbTimeoutLsb
#define RFLR_SYMBTIMEOUTLSB_SYMBTIMEOUT             0x64 // Default

//RegPreambleLengthMsb
#define RFLR_PREAMBLELENGTHMSB                      0x00 // Default

//RegPreambleLengthLsb
#define RFLR_PREAMBLELENGTHLSB                      0x08 // Default

// RegPayloadLength
#define RFLR_PAYLOADLENGTH                          0x0E // Default

//RegPayloadMaxLength
#define RFLR_PAYLOADMAXLENGTH                       0xFF // Default

//RegHopPeriod
#define RFLR_HOPPERIOD_FREQFOPPINGPERIOD            0x00 // Default

//RegFifoRxByteAddr (Read Only)

//RegModemConfig3
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK  0xF7 
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON    0x08 
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF   0x00 // Default

#define RFLR_MODEMCONFIG3_AGCAUTO_MASK              0xFB 
#define RFLR_MODEMCONFIG3_AGCAUTO_ON                0x04 // Default 
#define RFLR_MODEMCONFIG3_AGCAUTO_OFF               0x00 

//RegFeiMsb (Read Only)

//RegFeiMid (Read Only)

//RegFeiLsb (Read Only)

//RegRssiWideband (Read Only)

//RegDetectOptimize
#define RFLR_DETECTIONOPTIMIZE_MASK                 0xF8
#define RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12          0x03 // Default
#define RFLR_DETECTIONOPTIMIZE_SF6                  0x05

//RegInvertIQ
#define RFLR_INVERTIQ_RX_MASK                       0xBF
#define RFLR_INVERTIQ_RX_OFF                        0x00
#define RFLR_INVERTIQ_RX_ON                         0x40
#define RFLR_INVERTIQ_TX_MASK                       0xFE
#define RFLR_INVERTIQ_TX_OFF                        0x01
#define RFLR_INVERTIQ_TX_ON                         0x00

//RegDetectionThreshold
#define RFLR_DETECTIONTHRESH_SF7_TO_SF12            0x0A // Default
#define RFLR_DETECTIONTHRESH_SF6                    0x0C

// RegInvertIQ2
#define RFLR_INVERTIQ2_ON                           0x19
#define RFLR_INVERTIQ2_OFF                          0x1D

//RegDioMapping1
#define RFLR_DIOMAPPING1_DIO0_MASK                  0x3F
#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40
#define RFLR_DIOMAPPING1_DIO0_10                    0x80
#define RFLR_DIOMAPPING1_DIO0_11                    0xC0

#define RFLR_DIOMAPPING1_DIO1_MASK                  0xCF
#define RFLR_DIOMAPPING1_DIO1_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO1_01                    0x10
#define RFLR_DIOMAPPING1_DIO1_10                    0x20
#define RFLR_DIOMAPPING1_DIO1_11                    0x30

#define RFLR_DIOMAPPING1_DIO2_MASK                  0xF3
#define RFLR_DIOMAPPING1_DIO2_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO2_01                    0x04
#define RFLR_DIOMAPPING1_DIO2_10                    0x08
#define RFLR_DIOMAPPING1_DIO2_11                    0x0C

#define RFLR_DIOMAPPING1_DIO3_MASK                  0xFC
#define RFLR_DIOMAPPING1_DIO3_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO3_01                    0x01
#define RFLR_DIOMAPPING1_DIO3_10                    0x02
#define RFLR_DIOMAPPING1_DIO3_11                    0x03

//RegDioMapping2
#define RFLR_DIOMAPPING2_DIO4_MASK                  0x3F
#define RFLR_DIOMAPPING2_DIO4_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO4_01                    0x40
#define RFLR_DIOMAPPING2_DIO4_10                    0x80
#define RFLR_DIOMAPPING2_DIO4_11                    0xC0

#define RFLR_DIOMAPPING2_DIO5_MASK                  0xCF
#define RFLR_DIOMAPPING2_DIO5_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO5_01                    0x10
#define RFLR_DIOMAPPING2_DIO5_10                    0x20
#define RFLR_DIOMAPPING2_DIO5_11                    0x30

#define RFLR_DIOMAPPING2_MAP_MASK                   0xFE
#define RFLR_DIOMAPPING2_MAP_PREAMBLEDETECT         0x01
#define RFLR_DIOMAPPING2_MAP_RSSI                   0x00  // Default

//RegVersion (Read Only)

//RegPllHop
#define RFLR_PLLHOP_FASTHOP_MASK                    0x7F
#define RFLR_PLLHOP_FASTHOP_ON                      0x80
#define RFLR_PLLHOP_FASTHOP_OFF                     0x00 // Default

//RegTcxo
#define RFLR_TCXO_TCXOINPUT_MASK                    0xEF
#define RFLR_TCXO_TCXOINPUT_ON                      0x10
#define RFLR_TCXO_TCXOINPUT_OFF                     0x00  // Default

//RegPaDac
#define RFLR_PADAC_20DBM_MASK                       0xF8
#define RFLR_PADAC_20DBM_ON                         0x07
#define RFLR_PADAC_20DBM_OFF                        0x04  // Default

//RegFormerTemp

//RegBitrateFrac
#define RF_BITRATEFRAC_MASK                         0xF0

//RegAgcRef

//RegAgcThresh1

//RegAgcThresh2

//RegAgcThresh3

//RegPll
#define RF_PLL_BANDWIDTH_MASK                       0x3F
#define RF_PLL_BANDWIDTH_75                         0x00
#define RF_PLL_BANDWIDTH_150                        0x40
#define RF_PLL_BANDWIDTH_225                        0x80
#define RF_PLL_BANDWIDTH_300                        0xC0  // Default




typedef enum
{
 	RM_SLEEP						= 0x80,
	RM_STANDBY,
	RM_FREQUENCY_SYNTHESIS_TX,
	RM_TX,
	RM_FREQUENCY_SYNTHESIS_RX,
	RM_RX_CONTINUOUS,
	RM_RX_SINGLE,
	RM_CHANEL_ACTIVITY_DETECTION
} RadioModes;

typedef enum
{
 	RBW_0							= 0b0000,					//  7.8 kHz
	RBW_1							= 0b0001,					// 10.4 kHz
	RBW_2							= 0b0010,					// 15.6 kHz
	RBW_3							= 0b0011,					// 20.8 kHz
	RBW_4							= 0b0100,					// 31.2 kHz
	RBW_5							= 0b0101,					// 41.4 kHz
	RBW_6							= 0b0110,					// 62.5 kHz
	RBW_7							= 0b0111,					//125.0 kHz
	RBW_8							= 0b1000,					//250.0 kHz
	RBW_9							= 0b1001,					//500.0 kHz
} RadioBW1276;

typedef enum
{
	RBW72_7							= 0b00,						//125.0 kHz
	RBW72_8							= 0b01,						//250.0 kHz
	RBW72_9							= 0b10,						//500.0 kHz
} RadioBW1272;


typedef enum
{
 	RCR_4_of_5						= 0b001,					// 4/5
 	RCR_4_of_6						= 0b010,					// 4/6
 	RCR_4_of_7						= 0b011,					// 4/7
 	RCR_4_of_8						= 0b100,					// 4/8
} RadioCR;

typedef enum
{
 	RSF_6							= 6,						//  64 chips / symbol
	RSF_7							= 7,						//  128 chips / symbol
 	RSF_8							= 8,						//  256 chips / symbol
	RSF_9							= 9,						//  512 chips / symbol
 	RSF_10							= 10,						// 1024 chips / symbol
	RSF_11							= 11,						// 2048 chips / symbol
 	RSF_12							= 12,						// 4096 chips / symbol
} RadioSF;

typedef enum
{
 	REH_EXPLICIT					= 0,						//Explicit Header mode
	REH_IMPLICIT					= 1,						//Implicit Header mode
} RadioEH;

typedef enum
{
	//Direct reading of LoRa registers, command ID corresponds to register address and the following byte specifies the number of bytes to read (minimum 1 byte).
	//For multibyte reads, register address increments automatically for each next byte, except FIFO. Each command and reply are ended with \r\n.
	RC_GET_FIFO					= REG_LR_FIFO, 				//read FIFO, number of bytes to read is specified in the byte following RC_GET_FIFO command byte
	RC_GET_MODEM_MODE			= REG_LR_OPMODE, 			//LoRa/FSK, current radio mode and other parameters of RegOpMode, 1 byte
	RC_GET_FREQUENCY			= REG_LR_FRFMSB, 			//Tx/Rx frequency in Hz, MSB first, 3 bytes
	RC_GET_PA_CONFIG			= REG_LR_PACONFIG, 			//Max Power, Output Power and PA select of RegPaConfig, 1 byte
	RC_GET_CURRENT_PROTECTION	= REG_LR_OCP, 				//Overload current enabling and limiting value of RegOcp, 1 byte
	RC_GET_LNA_GAIN				= REG_LR_LNA, 				//Low-Noise Amplifier gain and current boost settings in RegLna, 1 byte
	RC_GET_FIFO_POINTER			= REG_LR_FIFOADDRPTR, 		//SPI interface address pointer in FIFO data buffer from RegFifoAddrPtr, 1 byte
	RC_GET_FIFO_TX_BASE_ADDR	= REG_LR_FIFOTXBASEADDR, 	//base address in FIFO data buffer for TX modulator, RegFifoTxBaseAddr, 1 byte
	RC_GET_FIFO_RX_BASE_ADDR	= REG_LR_FIFORXBASEADDR, 	//base address in FIFO data buffer for RX demodulator, RegFifoRxBaseAddr, 1 byte
	RC_GET_FIFO_RX_LAST_PKT_ADDR= REG_LR_FIFORXCURRENTADDR, //Start address (in data buffer) of last packet received, RegFifoRxCurrentAddr, 1 byte
	RC_GET_IRQ_MASK				= REG_LR_IRQFLAGSMASK, 		//RegIrqFlagsMask state, setting bits of this register masks the corresponding IRQ in RegIrqFlags, 1 byte
	RC_GET_IRQ_FLAGS			= REG_LR_IRQFLAGS,			//gets triggered interrupts from RegIrqFlags, writing 1 to the register bits clears corresponding IRQs, 1 byte
	RC_GET_RX_BYTES_COUNT		= REG_LR_RXNBBYTES,			//reads number of payload bytes of latest packet received from RegRxNbBytes, 1 byte
	RC_GET_RX_HEADERS_COUNT		= REG_LR_RXHEADERCNTVALUEMSB,	//Number of valid headers received since last transition into Rx mode, from RegRxHeaderCnt, 2 bytes
	RC_GET_RX_PACKETS_COUNT		= REG_LR_RXPACKETCNTVALUEMSB,	//Number of valid packets received since last transition into Rx mode, from RegRxPacketCntValue, 2 bytes
	RC_GET_MODEM_STATUS			= REG_LR_MODEMSTAT, 		//Coding rate of last header received and other current RX statuses from RegModemStat, 1 byte
	RC_GET_PACKET_SNR			= REG_LR_PKTSNRVALUE, 		//Estimation of SNR on last packet received.In two’s compliment format multiplied by 4 from RegPktSnrValue, 1 byte
	RC_GET_PACKET_RSSI			= REG_LR_PKTRSSIVALUE, 		//RSSI of the latest packet received (dBm) from RegPktRssiValue, 1 byte
	RC_GET_CURRENT_RSSI			= REG_LR_RSSIVALUE, 		//Current RSSI value (dBm) from RegRssiValue, 1 byte
	RC_GET_MODEM_CONFIG12_		= REG_LR_MODEMCONFIG1, 		//Bandwidth, Coding rate, Explicit header selection, from RegModemConfig1
															//Spreading factor, TxContinuous, RxPayloadCrcOn, and RX Time-Out MSB, from RegModemConfig2
															//and RX Time-Out LSB, from RegSymbTimeoutLsb, 3 bytes in total
	RC_GET_PREAMBLE_LENGTH		= REG_LR_PREAMBLEMSB, 		//Preamble length from RegPreambleMsb and Lsb, 2 bytes
	RC_GET_PAYLOAD_LENGTH		= REG_LR_PAYLOADLENGTH, 	//expected packet length in implicit header mode in bytes from RegPayloadLength, 1 byte
	RC_GET_MAX_PAYLOAD_LENGTH	= REG_LR_PAYLOADMAXLENGTH, 	//if header payload length exceeds value from RegMaxPayloadLength, a header CRC error is generated, 1 byte
	RC_GET_FIFO_RX_CUR_ADDR		= REG_LR_FIFORXBYTEADDR,	//Current value of RX data buffer pointer (address of last byte written by Lora receiver), from RegFifoRxByteAddr, 1 byte
	RC_GET_MODEM_CONFIG3		= REG_LR_MODEMCONFIG3, 		//LowDataRateOptimize and AgcAutoOn, from RegModemConfig3, 1 byte
	RC_GET_FREQUENCY_ERROR		= REG_LR_FEIMSB, 			//Estimated frequency error from modem from RegFeiMsb/Mid/Lsb, 3 bytes
	RC_GET_DETECTION_OPTIMIZE	= REG_LR_DETECTOPTIMIZE, 	//LoRa Detection Optimize: 0x03 for SF7 to SF12 and	0x05 for SF6, from RegDetectOptimize, 1 byte
	RC_GET_DETECTION_THRESHHOLD	= REG_LR_DETECTIONTHRESHOLD,//LoRa detection threshold: 0x0A for SF7 to SF12 and 0x0C for SF6, from RegDetectOptimize, 1 byte
	RC_GET_DIO_MAPPINGS			= REG_LR_DIOMAPPING1,		//LorA DIO 0-5 mappings from RegDioMapping1/2, 2 bytes

	//Direct writing to LoRa registers, command ID corresponds to register address and the following bytes represent values to be written (minimum 1 byte).
	//For multibyte writes, register address increments automatically for each next byte, except FIFO. Each command and reply are ended with \r\n.
	RC_SET_FIFO					= 0x80 |  REG_LR_FIFO, 				//next byte specifies the number of bytes to write
	RC_SET_MODEM_MODE			= 0x80 |  REG_LR_OPMODE, 			//Format: bit 7 can be modified only in Sleep mode: 0 - FSK/OOK Mode, 1 - LoRa,
																	//bit 6: AccessSharedReg,
																	//bit 3: LowFrequencyModeOn,
																	//bits 2-0: device mode as per RadioModes enum
	RC_SET_FREQUENCY			= 0x80 |  REG_LR_FRFMSB, 			//3-byte value which is Desired Frequency / FREQ_STEP, must be modified only in SLEEP or STAND-BY mode
	RC_SET_PA_CONFIG			= 0x80 |  REG_LR_PACONFIG, 			//bit 7: 0 - output on RFO pin / 1 - PA,
																	//bits 6-4: set output Power,
																	//bits 3-0: PaRamp up time whatever it is
	RC_SET_CURRENT_PROTECTION	= 0x80 |  REG_LR_OCP, 				//bit 5: enables output current limiting when set to 1,
																	//bits 4-0: limiting value
	RC_SET_LNA_GAIN	= 0x80 |  REG_LR_LNA, 				//bits 7-5: gain selection from Max to Min,
																	//bits 1-0: LnaBoostHf enabling
	RC_SET_FIFO_POINTER			= 0x80 |  REG_LR_FIFOADDRPTR, 		//sets SPI interface address pointer in FIFO data buffer
	RC_SET_FIFO_TX_BASE_ADDR	= 0x80 |  REG_LR_FIFOTXBASEADDR, 	//sets base address in FIFO data buffer for TX modulator, RegFifoTxBaseAddr, 1 byte
	RC_SET_FIFO_RX_BASE_ADDR	= 0x80 |  REG_LR_FIFORXBASEADDR, 	//sets base address in FIFO data buffer for RX demodulator, RegFifoRxBaseAddr, 1 byte
	RC_SET_IRQ_MASK				= 0x80 |  REG_LR_IRQFLAGSMASK, 		//bit 7: RxTimeoutMask, 6: RxDoneMask, 5: PayloadCrcErrorMask, 4: ValidHeaderMask, 3: TxDoneMask, 2: CadDoneMask, 1: FhssChangeChannelMask, 0: CadDetectedMask
	RC_SET_IRQ_FLAGS			= 0x80 |  REG_LR_IRQFLAGS,			//writing a 1 clears the IRQ, bit 7: RxTimeout, 6: RxDone, 5: PayloadCrcError, 4: ValidHeader, 3: TxDone, 2: CadDone, 1: FhssChangeChannel, 0: CadDetected
	RC_SET_RX_PACKETS_COUNT		= 0x80 |  REG_LR_RXPACKETCNTVALUEMSB,	//Msb/Lsb bytes, Header and packet counters are reseted in Sleep mode, and, I guess, by writing 0s here? Msb is marked as rc, but Lsb as r only
	RC_SET_MODEM_CONFIG12_		= 0x80 |  REG_LR_MODEMCONFIG1, 		//byte 1 bits 7-4: Bandwidth as per RadioBandwidhes enum,
																	//byte 1 bits 3-1: Coding rate as per RadioCodingRate enum,
																	//byte 1 bit 0: 0 - Explicit Header mode, 1 - Implicit Header mode
																	//byte 2 bits 7-4: Spreading factor as per RadioSpreadingFactor enum,
																	//byte 2 bit 3: TxContinuous 0 - normal mode, a single packet is sent, 1 - continuous mode, send multiple packets, across the FIFO used for spectral analysis),
																	//byte 2 bit 2: RxPayloadCrcOn, 0 - CRC disabled, 1 - CRC enabled, in implicit header mode this determines if receiver should expect a payload CRC
																	//byte 2 bits 1-0: RX Time-Out MSB in symbols
																	//byte 3 bits 7-0: RX Time-Out LSB
	RC_SET_PREAMBLE_LENGTH		= 0x80 |  REG_LR_PREAMBLEMSB, 		//Preamble length Msb and Lsb in symbols, total lenght = PreambleLength + 4.25 Symbols, 2 bytes
	RC_SET_PAYLOAD_LENGTH		= 0x80 |  REG_LR_PAYLOADLENGTH, 	//Payload length in bytes. The register needs to be set in implicit	header mode for the expected packet length. A 0 value is not permitted
	RC_SET_MAX_PAYLOAD_LENGTH	= 0x80 |  REG_LR_PAYLOADMAXLENGTH, 	//Maximum payload length; if header payload length exceeds this value a header CRC error is generated. Allows filtering of packets with a bad size.
	RC_SET_MODEM_CONFIG3		= 0x80 |  REG_LR_MODEMCONFIG3, 		//bit 3: LowDataRateOptimize, 0 - Disabled, 1 - Enabled; mandated for when the symbol length exceeds 16ms
																	//bit 2: AgcAutoOn, 0 - LNA gain set by register LnaGain, 1 - LNA gain set by the internal AGC loop
	RC_SET_DETECTION_OPTIMIZE	= 0x80 |  REG_LR_DETECTOPTIMIZE, 	//LoRa Detection Optimize, set to 0x03 for SF7 to SF12 and to 0x05 for SF6
	RC_SET_DETECTION_THRESHHOLD	= 0x80 |  REG_LR_DETECTIONTHRESHOLD,//LoRa detection threshold, set to 0x0A for SF7 to SF12 and to 0x0C for SF6
	RC_SET_DIO_MAPPINGS			= 0x80 |  REG_LR_DIOMAPPING1,		//byte 1 bits 7-6: Dio 0 Mapping, check the data sheet for mapping in LoRa mode
																	//byte 1 bits 5-4: Dio 1 Mapping, check the data sheet for mapping in LoRa mode
																	//byte 1 bits 3-2: Dio 2 Mapping, check the data sheet for mapping in LoRa mode
																	//byte 1 bits 1-0: Dio 3 Mapping, check the data sheet for mapping in LoRa mode
																	//byte 2 bits 7-6: Dio 4 Mapping, check the data sheet for mapping in LoRa mode
																	//byte 2 bits 5-4: Dio 5 Mapping, check the data sheet for mapping in LoRa mode
	RC_GET_DATE					= 0x02,								//get the current day of week, day, month and year
	RC_SET_DATE					= 0x03,								//set of week, day, month and year
	RC_GET_TIME					= 0x04,								//get the current hour, minute and second
	RC_SET_TIME					= 0x05,								//set the time in BCD format
	RC_SET_RF_SWITCH			= 0x2D,								//byte 1 == 0 connects antenna to RFI, byte 1 == non 0, - to RFO
	RC_SET_GREEN_LED			= REG_LR_PLL + 1,					//turn green LED on, "q"
	RC_SET_RED_LED,													//turn green LED off, "r"
	RC_TMP_GREEN_TOGGLE,											//toggle green LED, "s"
	RC_TMP_RED_TOGGLE,												//toggle red LED, "v"

	//	RC_SET_MODE,					//sets one of the RadioModes values specified in the byte following the command
//	RC_SET_CHANNEL,					//sets Tx/Rx frequency, frequency value to be in 3 bytes following the command
//	RD_SET_BANDWIDTH,				//sets one of the RadioBandwidthes already expressed in register values, three following bytes
//	RC_SET_CODING_RATE,				//sets one of RadioCodingRates, specified in the byte following the command
//	RC_SET_HEADER_TYPE,					//sets Tx/Rx frequency
	RC_GET_GPS				= 0xF8,								//temporary command to check a data string from gps
	RC_GET_PICTURE				= 0xF9,								//command to camera to take a picture
	RC_PICTURE_DATA				= 0xFA,								//picture data message or get next data command, if the following byte is 0, or resend data if 1
	RC_END_PICTURE				= 0xFB,								//message to external device that all picture data was transfered
	RC_RX_MESSAGE				= 0xFC,								//tells external device that following data is received LoRa message
	RC_TX_MESSAGE				= 0xFD,								//tells radio device that following data is a LoRa message to transmit
	RC_SET_WAKEUP				= 0xFE,								//set wake up time and date
	RC_TEXT						= 0xFF								//tells external device that following data is plain text
} RadioCommands;


typedef enum
{
	//Radio communication protocol defines
	RMT_COMMUNICATION_REQUEST 			= 0xAA,						//to CC - request for concentrator channel to send data to, format: AA (msgID), XX(delay in seconds)
	RMT_COMMUNICATION_SCHEDULING 		= 0xAB,						//from CC - time and channel parameters for sending data, format: AB, XX (freq LSB), XX (freq MidSB), XX (freq MSB), XX(delay in seconds), XX(bandwidth), XX(spreading factor), XX(coding rate), XX(explicit header)
	RMT_COMMUNICATION_DATA				= 0xAC,						//to CD - data dump, format:
	RMT_COMMUNICATION_DATA_CONFIRMATION	= 0xAD,						//from CD - data dump confirmation, may have individual confirmations for data fields sent
} RadioMessageTypes;

typedef enum
{
	//Radio message may have dynamic content depending on situation, e.g data availability. To allow this, each data field would have type (and length ?) headers
	RMDT_DEVICE_ID,													//identification number of target/source device
	RMDT_GPS,														//gps coordinates
	RMDT_ALTIMETER,													//altimeter information: temperature
	RMDT_TIME,														//internal time of a device
	RMDT_BATTERY,													//battery level
	RMDT_PICTURE,													//image data
} RadioMessageDataTypes;

typedef enum
{
	//Position of information fields inside Radio messages
	RMF_DEVICE_ID			= 0,									//Device ID
	RMF_MSG_TYPE			= 4,									//message type code position
	RMF_TX_SETTINGS			= 5,									//beginning of communication parameters
} RadioMessageFields;

typedef enum
{
	//Length of fixed-size RF messages
	RMS_MINIMUM				= 5,									//size of a shortest message possible: 4 bytes Device ID, 1 - command
	RMS_CC_REQUEST			= 6,									//request to Control Channel for communication: one byte of message type, 1 byte of comm delay and 4 bytes of Device ID
	RMS_CC_REPLY			= 13,									//CC reply: 4 bytes of Device ID, 1 byte message type, 8 bytes of communication parameters
} RadioMessagesSize;

//Time considerations for sensor devices and concentrators:
// 1) time is that of UTC (Patrick)
// 2) time is counted from Jan. 01, 2000 as per STM32s, to save bandwidth
// 3) below is time representation in YY-MM-DD-HH-MM-SS-mmmm (milliseconds) in bits and bytes, suggesting 8 bytes for now
// 2016	10	31	13:	39	:24	:1000
//	7	4	5	5	6	6	10		= 43, 5 bytes, 3 bits (accuracy can be cut down to 4 ms to fit 5 bytes)
//	1	1	1	1	1	1	2		= 8 bytes
// 4) on control channel, communication request from the device and reply from concentrator indicate the number of seconds (to the maximum of 255 seconds) from
//	transmission start when the data session is requested / granted. Correspondingly on the receiving end that delay time is reduced by the packet transmission/
//  reception time and added to current time on receiver to get the absolute time. This way both communication ends do not have their time synchronized to achieve
//  synchronization of communication.
// 5) separate message or protocol would be used for absolute calendar synchronization


#endif // RADIO_H
