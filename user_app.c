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
    static u16 u16TimerCounter = 0;             //static variable to count total ms that the device has been running for
    static u8Direction = 0;                     // Used to keep track of the direction the LEDs are moving (converging = 0/diverging = 1 from center)
    
    u8 u8LedState[] = {0x0C,0x12,0x21,0x00};    //My list of LED states to create a pattern
    u16 u16LataState = 0x80&LATA;               // created a variable with 6LSB cleared in order to preserve any other important data on LATA
    
    u16TimerCounter++;                          //increment counter each time through userapp/ ~1ms
    
    
    /*
     * Each LED state change occurs after 100ms as seen in the highest level If statement.
     * first state change occurs at 100ms, last one at 300
     * u8Direction determines the next LED state based on if the LED pattern converges or diverges from center.
     * this is seen with the second level if/else statement.
     * One period of the animation lasts 300ms in each direction, or 600ms total.  
    */
    if(u16TimerCounter == 100)                  
    {
        if(u8Direction == 0)                    //if LEDs converge 
        {
            LATA = u16LataState|u8LedState[0]; 
        } else                                  //if LEDs diverge from center
        {
            LATA = u16LataState|u8LedState[1];  
        }
    } else if(u16TimerCounter == 200)
    {
        if(u8Direction == 0)                    //if LEDs converge to center 
        {
            LATA = u16LataState|u8LedState[1];
        }else
        {
            LATA = u16LataState|u8LedState[0];  //if LEDs converge to center
        }
    }else if(u16TimerCounter == 300)
    {
        if(u8Direction == 0)                    //if LEDs diverge from center
        {
            LATA = u16LataState|u8LedState[2];
            u8Direction = 1;                    //reset direction back to 0 for the next timer cycle to converge
            u16TimerCounter = 0;                //reset timer back to 0
        } else                                  //if LEDs converge to center
        {
            LATA = u16LataState|u8LedState[3];
            u8Direction = 0;                    // reset direction back to 0 for the next timer cycle to diverge
            u16TimerCounter =0;                 //reset timer back to zero
        }
    }
    
} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

void TimeXus(u16 u16Microseconds){
    T0CON0 &= 0x7F;
    
    u16 u16OverFlowCheck = 0xFFFF - u16Microseconds;
    
    u8 u8LowInput = u16OverFlowCheck & 0xFF;
    u8 u8HighInput = (u16OverFlowCheck>>8)& 0xFF;
    
    TMR0L = u8LowInput;
    TMR0H = u8HighInput;
    
    PIR3 = PIR3&0x7F;
    
    T0CON0 = T0CON0 | 0X80;
}



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
