/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

static u8 u8MenuChoose = 2;
static u8 u8ShowLevel[1] = "0";
u8 *u8VolumeLevel = u8ShowLevel;

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  AT91C_BASE_PIOA->PIO_PER |=  PA_16_BLADE_CS|PA_15_BLADE_SCK|PA_13_BLADE_MISO|PA_14_BLADE_MOSI|PA_12_BLADE_UPOMI|PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOA->PIO_PDR &=~(PA_16_BLADE_CS|PA_15_BLADE_SCK|PA_13_BLADE_MISO|PA_14_BLADE_MOSI|PA_12_BLADE_UPOMI|PA_11_BLADE_UPIMO);
  AT91C_BASE_PIOA->PIO_OER |=  PA_16_BLADE_CS|PA_15_BLADE_SCK|PA_13_BLADE_MISO|PA_14_BLADE_MOSI|PA_12_BLADE_UPOMI|PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOA->PIO_ODR &=~(PA_16_BLADE_CS|PA_15_BLADE_SCK|PA_13_BLADE_MISO|PA_14_BLADE_MOSI|PA_12_BLADE_UPOMI|PA_11_BLADE_UPIMO);
  
  AT91C_BASE_PIOB->PIO_PER |=  PB_04_BLADE_AN1|PB_03_BLADE_AN0;
  AT91C_BASE_PIOB->PIO_PDR &=~(PB_04_BLADE_AN1|PB_03_BLADE_AN0);
  AT91C_BASE_PIOB->PIO_OER |=  PB_04_BLADE_AN1;
  AT91C_BASE_PIOB->PIO_ODR &=~ PB_04_BLADE_AN1;

  Aud2();
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    RedBlink();
    VolumeUp();//Volume+
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    RedBlink();
    VolumeDown();//Volume-
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    RedBlink();
    UserApp1_StateMachine = Adc12;//find X9C103 position
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    RedBlink();
    ChannelChange();//change channel
    u8MenuChoose = u8MenuChoose+1;
  }


} /* end UserApp1SM_Idle() */

void ChannelChange(void)
{
  switch(u8MenuChoose){
  case 1:Aud2();break;
  case 2:Aud1();break;
  case 3:SilentMode();u8MenuChoose = 0;break;
  }
}
void RedBlink(void)
{
  LedOn(RED);
  for(u16 i=0;i<5000;i++)
  {
    
  }
  LedOff(RED);
}
void Aud2(void)//mobile output
{
  LedOn(GREEN);
  LedOff(BLUE);
  LedOff(PURPLE);//Green on
  LCDClearChars(LINE1_START_ADDR, 20);
  LCDMessage(LINE1_START_ADDR, "Phone");//Display Phone
  AT91C_BASE_PIOA->PIO_CODR |= PA_16_BLADE_CS;//A=0
  AT91C_BASE_PIOA->PIO_CODR |= PA_15_BLADE_SCK;//B=0
  AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;//C=0
}
void Aud1(void)//mic output
{
  LedOff(GREEN);
  LedOn(BLUE);
  LedOff(PURPLE);//Blue on
  LCDClearChars(LINE1_START_ADDR, 20);
  LCDMessage(LINE1_START_ADDR, "MIC");//Display Mic
  AT91C_BASE_PIOA->PIO_SODR |= PA_16_BLADE_CS;//A=1
  AT91C_BASE_PIOA->PIO_CODR |= PA_15_BLADE_SCK;//B=0
  AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;//C=0
}
void SilentMode(void)
{
  LedOff(GREEN);
  LedOff(BLUE);
  LedOn(PURPLE);//Purple on
  LCDClearChars(LINE1_START_ADDR, 20);
  LCDMessage(LINE1_START_ADDR, "Silent");//Display Silence
  AT91C_BASE_PIOA->PIO_SODR |= PA_15_BLADE_SCK;//B=1
  AT91C_BASE_PIOA->PIO_CODR |= PA_13_BLADE_MISO;//C=0
}
void VolumeUp(void)
{
  AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO;//CS=0 片选低电平有效
  AT91C_BASE_PIOA->PIO_SODR |= PA_12_BLADE_UPOMI;//U/D=1
  if(u8MenuChoose!=1)
  {
    for(u8 m = 0;m<9;m++)
    {//下降沿触发
      AT91C_BASE_PIOA->PIO_SODR |= PA_14_BLADE_MOSI;//INC=1
      LedOn(YELLOW);
      for(u8 n = 0;n<100;n++)
      {
        
      }
      AT91C_BASE_PIOA->PIO_CODR |= PA_14_BLADE_MOSI;//INC=0
      LedOff(YELLOW);
    }
    AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO;//CS=1 
    if (u8ShowLevel[0] < '9')
    {
      u8ShowLevel[0] = *u8VolumeLevel + 1;
    }
    LCDClearChars(LINE2_START_ADDR, 20);
    LCDMessage(LINE2_START_ADDR,u8ShowLevel);//display volume change

  }
}
void VolumeDown(void)
{
  AT91C_BASE_PIOA->PIO_CODR |= PA_11_BLADE_UPIMO;//CS=0
  AT91C_BASE_PIOA->PIO_CODR |= PA_12_BLADE_UPOMI;//U/D=0
  if(u8MenuChoose!=1)
  {
    for(u8 p = 0;p<9;p++)
    {
      AT91C_BASE_PIOA->PIO_SODR |= PA_14_BLADE_MOSI;//INC=1
      LedOn(ORANGE);
      for(u8 q =0;q<100;q++)
      {
        
      }
      AT91C_BASE_PIOA->PIO_CODR |= PA_14_BLADE_MOSI;//INC=0
      LedOff(ORANGE);
    }
    AT91C_BASE_PIOA->PIO_SODR |= PA_11_BLADE_UPIMO;//CS=1
    if (u8ShowLevel[0] > '0')
    {
      u8ShowLevel[0] = *u8VolumeLevel - 1;
    }
    LCDClearChars(LINE2_START_ADDR, 20);
    LCDMessage(LINE2_START_ADDR,u8ShowLevel);
  }
}
void Adc12(void)
{
  static u32 u32TimeCounter = 0;
  static u32 u32AdcResult = 0;
  static u8  u8AdcResultLevel = 0;
  static bool bAdcStart = TRUE;
  
  if (bAdcStart)
  {
    bAdcStart = FALSE;
    LedOn(WHITE);
    AT91C_BASE_PIOB->PIO_SODR |=PB_04_BLADE_AN1;//RE=1
    AT91C_BASE_PIOA->PIO_SODR |=PA_13_BLADE_MISO;//C=1
    Adc12StartConversion(ADC12_CH2);
  }
  if (Adc12StartConversion(ADC12_CH2) == TRUE)
  {
    u32AdcResult = AT91C_BASE_ADC12B->ADC12B_CDR[ADC12_CH2];
    u32AdcResult = u32AdcResult/41; 
    u8AdcResultLevel = u32AdcResult/10; 
    u8ShowLevel[0] = u8AdcResultLevel+ '0' ;
    
    u32TimeCounter++;
    
    if (u32TimeCounter > 300)
    {
      u32TimeCounter = 0;
      LedOff(WHITE);
      LCDClearChars(LINE2_START_ADDR, 20);
      LCDMessage(LINE2_START_ADDR,u8ShowLevel); 
      AT91C_BASE_PIOB->PIO_CODR |=PB_04_BLADE_AN1;//RE=0
      AT91C_BASE_PIOA->PIO_CODR |=PA_13_BLADE_MISO;//C=0
      bAdcStart = TRUE;
      UserApp1_StateMachine = UserApp1SM_Idle;
    }
  }    
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
