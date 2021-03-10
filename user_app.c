/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief 
 * This program simply implements a 6 bit binary counter on LAT 0:5, with a 
 * single test LED constantly high on LAT7. Everything in UserAppInitialize is just a boot up animation, 
 * is not part of the lab and can be disregarded for for grading

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- _XTAL_FREQ : states processor speed for the use of function __delay_ms()  

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/
#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                             /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserAppInitialize(void)
{
   
    T0CON0 = 0x90;       // enable timer0, 16 bit timer, no post scaler
    T0CON1 = 0x54;       // clock source = Fosc/4, Asynch, prescaler set to 1:16
      
} /* end UserAppInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
- 

Promises:
- 

*/
void UserAppRun(void)
{
    static u8 u8Counter = 0;               // Counter to track current position within DAC. total count = 1 byte
    static u8 UserApp_au8sinTable[] =      // 8 bit sine signal with 256 samples between 0 and 255
    {
    0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9b,0x9e,0xa2,0xa5,0xa7,0xaa,0xad,
    0xb0,0xb3,0xb6,0xb9,0xbc,0xbe,0xc1,0xc4,0xc6,0xc9,0xcb,0xce,0xd0,0xd3,0xd5,0xd7,
    0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xeb,0xed,0xee,0xf0,0xf1,0xf3,0xf4,
    0xf5,0xf6,0xf8,0xf9,0xfa,0xfa,0xfb,0xfc,0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfd,0xfd,0xfc,0xfb,0xfa,0xfa,0xf9,0xf8,0xf6,
    0xf5,0xf4,0xf3,0xf1,0xf0,0xee,0xed,0xeb,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
    0xda,0xd7,0xd5,0xd3,0xd0,0xce,0xcb,0xc9,0xc6,0xc4,0xc1,0xbe,0xbc,0xb9,0xb6,0xb3,
    0xb0,0xad,0xaa,0xa7,0xa5,0xa2,0x9e,0x9b,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
    0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x64,0x61,0x5d,0x5a,0x58,0x55,0x52,
    0x4f,0x4c,0x49,0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x34,0x31,0x2f,0x2c,0x2a,0x28,
    0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x14,0x12,0x11,0x0f,0x0e,0x0c,0x0b,
    0x0a,0x09,0x07,0x06,0x05,0x05,0x04,0x03,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x03,0x04,0x05,0x05,0x06,0x07,0x09,
    0x0a,0x0b,0x0c,0x0e,0x0f,0x11,0x12,0x14,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
    0x25,0x28,0x2a,0x2c,0x2f,0x31,0x34,0x36,0x39,0x3b,0x3e,0x41,0x43,0x46,0x49,0x4c,
    0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c, 
    };

    if(u8Counter == 0xff)       // if counter has reach max DAC value, reset to 0 to start sine cycle over
    {
        DAC1DATL = UserApp_au8sinTable[u8Counter]; // update DAC with value at the top of 0->255 to preserve a value before roll over
        u8Counter = 0;
    }
    DAC1DATL = UserApp_au8sinTable[u8Counter]; // update DAC with new sine value
    u8Counter+=4;                              // increment DAC position in 8 bit sine signal by 4
} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

void TimeXus(u16 u16Microseconds){
    T0CON0 &= 0x7F;                 // turns off timer
    
    u16 u16OverFlow = 0xFFFF - u16Microseconds;     //subtract time we want to run for from max time possible
    
    u8 u8LowInput = u16OverFlow & 0xFF;             // create temporary variable and store lower 8 bits of our start time
    u8 u8HighInput = (u16OverFlow>>8)& 0xFF;        // create temporary variable and store lower 8 bits of our start time
    
    TMR0L = u8LowInput;                 // start the lower 8 bits at predetermined time 
    TMR0H = u8HighInput;                // start the upper 8 bits at predetermined time
    
    PIR3 = PIR3&0x7F;                   // clear roll over flag
    
    T0CON0 = T0CON0 | 0X80;             // enable timer
}



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
