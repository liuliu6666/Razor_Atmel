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

extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

/* Char buffer: */
static u8 UserApp_au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE  ];  

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
  for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
  {
    UserApp_au8UserInputBuffer[i] = 0;
  }

  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(RED);
  LedOff(ORANGE);
  LCDCommand(LCD_CLEAR_CMD);

  PWMAudioSetFrequency(BUZZER1, 200);

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
  u8 u8DebugOut1[] = "Entering state 1";
  u8 u8DebugOut2[] = "Entering state 2";
  u8 u8LCD1[] = "STATE 1";
  u8 u8LCD2[] = "STATE 2";
  u8 u8State = 0;
  static u32 u32BlinkCount = 0;
  static u8 u8Counter = 0;
  static bool bBuzzer1 = FALSE;
  u8 u8CharCount;

  if(G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1] == 0x0D)
  {
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    DebugLineFeed();
  }

  if(WasButtonPressed(BUTTON1)||UserApp_au8UserInputBuffer[0] == 0x31)
  {
    ButtonAcknowledge(BUTTON1);
    UserApp_au8UserInputBuffer[0] = 0x00;
    LCDCommand(LCD_CLEAR_CMD);
    bBuzzer1 = FALSE;
    u8State = 1;
  }
  if(WasButtonPressed(BUTTON2)||UserApp_au8UserInputBuffer[0] == 0x32)
  {
    ButtonAcknowledge(BUTTON2);
    UserApp_au8UserInputBuffer[0] = 0x00;
    LCDCommand(LCD_CLEAR_CMD);
    bBuzzer1 = TRUE;
    u8State = 2;
  }
  if(u8State == 1)
  {
    DebugPrintf(u8DebugOut1);
    DebugLineFeed();
    
    LCDMessage(LINE1_START_ADDR, u8LCD1);
    
    LedOn(WHITE);
    LedOn(PURPLE);
    LedOn(BLUE);
    LedOn(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(RED);
    LedOff(ORANGE);
    
    LedOn(LCD_RED);
    LedOff(LCD_GREEN);
    LedOn(LCD_BLUE);

    PWMAudioOff(BUZZER1);
    u8State = 0;
  }
  if(u8State == 2)
  {
    DebugPrintf(u8DebugOut2);
    DebugLineFeed();

    LCDMessage(LINE2_START_ADDR, u8LCD2);
    
    LedBlink(GREEN,LED_1HZ);
    LedBlink(YELLOW,LED_2HZ);
    LedBlink(ORANGE,LED_4HZ);
    LedBlink(RED,LED_8HZ);
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    
    LedOn(LCD_RED);
    LedPWM(LCD_GREEN, LED_PWM_35);
    LedOff(LCD_BLUE);

    u8State = 0;
  }
  if(bBuzzer1)
  {
    u32BlinkCount++;

    if(u32BlinkCount == 100)
    {
      u32BlinkCount = 0;
      PWMAudioOff(BUZZER1);
      u8Counter++;
      if(u8Counter == 9)
      {
        PWMAudioOn(BUZZER1);
      }
      if(u8Counter == 10)
      {
        PWMAudioOff(BUZZER1);
        u8Counter = 0;
      }
    }
  }
 


  


} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
