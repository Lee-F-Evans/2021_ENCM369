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
#define _XTAL_FREQ 64000000

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
   /**************** Everything below in Init. is just a boot up sequence and NOT part of the lab ***********************/ 
    for(int i =0; i<12;i++)
    {
        LATA ^= 0x3F;
        __delay_ms(250);
    }
    /**************** Everything above is just a boot up sequence and NOT part of the lab ***********************/    
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
    static u8 u8PreviousState = 0;      // Global variable to keep track of the previous state to allow for logically only one button press to one increment
    
    if(LATA == 0xBF){                   // this is a check to see if the current value displayed by the LEDs is max, if true, it will reset to the ground state with only LED7 on
        LATA = 0x80;
    }
    
    if(((PORTB&0x20) == 0x20) && (u8PreviousState == 0)){   // utilizing bit masking to determine if the button has been pressed and the previous button state was 0 do the things
        LATA++;                         // increment LATA once, simulating binary counting from 128 to 159, or 1000 0000 to 1011 1111.
        __delay_ms(50);                 // debounce delay
        u8PreviousState = 1;            // set previous state high so we do not increment next time through if the button was not released
    }
    if((PORTB&0x20) == 0x00){           // using bit masking to determine once the button is released, reseting the counter so the device allows an increment on the next press
        u8PreviousState = 0;        
    }
} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
