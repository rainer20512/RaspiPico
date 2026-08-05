
/**
  ******************************************************************************
  * @file    i2c_specs.h
  * @author  Rainer
  * @brief   Definitions of i2c message types, message flags, message contents
  *          and buffer sizes
  *          all that stuff that has to be identical on Pi Zero and Raspi part
  ******************************************************************************
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _I2C_SPECS_H_
#define _I2C_SPECS_H_

#include <assert.h>

/* Recognized data types, both in datapoints, XML and manual edit */
typedef enum {
  GUI_UINT8   = 0,
  GUI_UINT16  = 1,
  GUI_RGB888  = 2,
  GUI_UINT32  = 3,
  GUI_INT8    = 4,
  GUI_INT16   = 5,
  GUI_INT32   = 6,
  GUI_STRING  = 7,
  GUI_STYLE   = 8,
  GUI_RAWIMG  = 9,
  GUI_FONT    = 10,
  GUI_IMAGE   = 11,
  GUI_LABEL   = 12,
  GUI_MAXELEM = 13,
} XML_Datatype_Enum;


/****************************************************************************** 
 * Specify max length of a property or element name in XML
 * has a major impact on RAM useage
 *****************************************************************************/ 
#define ID_MAXNAMELEN           16
#define XML_PARSER_VER_MAJOR    1
#define XML_PARSER_VER_MINOR    0
#define XML_PARSER_VER_BUILD    2
#define XML_PARSER_VERSION_STR  STR(XML_PARSER_VER_MAJOR) "." STR(XML_PARSER_VER_MINOR) "." STR(XML_PARSER_VER_BUILD)
#define XML_PARSER_VERSION_BIN  ( ( XML_PARSER_VER_MAJOR << 10 ) | ( XML_PARSER_VER_MINOR <<5 ) | XML_PARSER_VER_BUILD )

/****************************************************************************** 
 * Specify buffer sizes, I2C-commands and flags
 *****************************************************************************/ 
#define MASTER_TXSIZE           64
#define SLAVE_TXSIZE            128   

#define I2C_CMD_PARSER_RESET	0x10  	/* Tell I2C slave to reset its parser */
#define I2C_CMD_QRY_STATUS    	0x11  	/* Tell i2C slave to send its status in next i2c slave read */
#define I2C_CMD_QRY_DPINFO       0x20    /* Tell i2c slave to send info about number of datapoints   */
#define I2C_CMD_RAW_DATA      	0x55  	/* Tell i2c slave, that package contains raw data */
#define WRAPPER_EXCESS_BYTES	2       /* number of bytes added by length and flag Bytes */

/* Answer flag bits for "query status"  */
#define I2C_SLAVEFLAG_BUSY      (1<<0)	/* i2c slave is busy                  */
#define I2C_SLAVEFLAG_HASDATA   (1<<1)	/* i2c slave has additional raw data  */	


/****************************************************************************** 
 * Answer to command "I2C_CMD_QRY_DPINFO"
 *****************************************************************************/ 
typedef struct {
  uint16_t    num_dp_total;             /* total number of datapoints defined */
  uint16_t    num_dp_per_transfer;      /* max number of datapoints in one transfer ( delimited by SLAVE_TXSIZE ) */
} I2C_DPinfo_Answer_T;

/****************************************************************************** 
 * Information about one Datapoint
 *****************************************************************************/ 
typedef struct {
  uint8_t             ordinal_num;              /* ordinal number                    */
  uint8_t             DP_Id;                    /* unique ID of Datapoint            */
  char                DP_name[ID_MAXNAMELEN];   /* Null-terminated Name of Datapoint */
  XML_Datatype_Enum   DP_datatype;              /* Datatype of Datapoint             */  
} I2C_DP_Transfer_T;

/* make sure, the I2C transfer buf is capable of holding at least on datapoint ifo element */
static_assert(sizeof(I2C_DP_Transfer_T) <= SLAVE_TXSIZE, "Slave send buffer too small to take at least one datapoint!");


#define DP_PER_XML_XFER     (SLAVE_TXSIZE/sizeof(I2C_DP_Transfer_T))


/****************************************************************************** 
 * List of datapoints, with max cardinality to fit in slave tx buffer
 *****************************************************************************/ 
typedef struct {
  I2C_DP_Transfer_T dp[DP_PER_XML_XFER];
} I2C_DP_List_T;
#endif /* _I2C_SPECS_H_  */

