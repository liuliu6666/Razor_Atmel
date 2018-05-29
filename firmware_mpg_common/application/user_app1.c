/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

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
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentMessageTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static AntAssignChannelInfoType UserApp1_CHANNEL1_sChannelInfo; /* ANT setup parameters */


static u8 UserApp1_au8MessageFail1[] = "\n\r***ANT channel 1 setup failed***\n\n\r";

static  u8 au8Line1[] = "HR    MaxMin    Back";
static  u8 au8Line2[] = "B0    B1         B3";



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
  u8 au8ANTPlusNetworkKey[] = {0xB9,0xA5,0x21,0xFB,0xBD,0x72,0xC3,0x45};
  

  
  LCDCommand(LCD_CLEAR_CMD);
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);


 /* Configure Channel 1 for this application */
  UserApp1_CHANNEL1_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP_CHANNEL1;
 
  UserApp1_CHANNEL1_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP_CHANNEL1;

  UserApp1_CHANNEL1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_CHANNEL1_sChannelInfo.AntNetworkKey[i] = au8ANTPlusNetworkKey[i];
  }
  
  /* Attempt to queue the ANT channel 1 setup */
  if( AntAssignChannel(&UserApp1_CHANNEL1_sChannelInfo) )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntChannel1Assign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    DebugPrintf(UserApp1_au8MessageFail1);
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

/*----------------------------------------------------------------------------------------------------------------------*/
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
/* Wait for the ANT channel assignment */
static void UserApp1SM_AntChannel1Assign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL1) == ANT_CONFIGURED)
  {
    DebugPrintf("\n\rChannel1 assign OK\n\n\r");
    
    UserApp1_StateMachine = UserApp1SM_ModeChoose;
  }
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
  

} /* end UserApp1SM_WaitChannel1Assign() */





  /*-------------------------------------------------------------------------------------------------------------------*/

static void UserApp1SM_ModeChoose(void)
{
  static bool bDisplayed = FALSE;

  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Line1);
    LCDMessage(LINE2_START_ADDR,au8Line2);
    bDisplayed = TRUE;
  } 
  if (WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL1);
    LCDCommand(LCD_CLEAR_CMD);
    UserApp1_StateMachine = UserApp1SM_Function1;
  }  
  if (WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL1);
    LCDCommand(LCD_CLEAR_CMD);
    UserApp1_StateMachine = UserApp1SM_Function2;
  }
} /* end UserApp1SM_Idle() */
     
/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 1 */
static void UserApp1SM_Function1(void)
{
  static u8 au8LastData[1] = {0xFF};

  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(au8LastData[0] != G_au8AntApiCurrentMessageBytes[7])
      {
        au8LastData[0] = G_au8AntApiCurrentMessageBytes[7];
        ShowRate();
      }
    }
  } 
  /*press B0 return to mode choose*/
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    au8LastData[0] = 0xFF;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Line1);
    LCDMessage(LINE2_START_ADDR,au8Line2);
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL1);
    UserApp1_StateMachine = UserApp1SM_ModeChoose;
  }
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
  }

} /* end UserApp1SM_Function1() */

static void ShowRate(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  u8 au8Display1[] = "  Ave. rate:   bpm";
  u8 u8AveRate = 0;
  //u16 u16Timer = 0;
  u8 au8PrintContant[100];
  
  u8AveRate = G_au8AntApiCurrentMessageBytes[7];

  au8Display1[12] = u8AveRate / 100 + '0';
  au8Display1[13] = (u8AveRate % 100) / 10 + '0';
  au8Display1[14] = u8AveRate % 10 + '0';  
   
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8Display1);

 // u16Timer++;
  //if(u16Timer == 1000)
  //{
   // u16Timer = 0;
    for(u8 i = 0;i < 100; i++)
    {
      au8PrintContant[i] = '\0';
    }

    for(u8 i = 0;i <  u8AveRate/3; i++)
    {
      au8PrintContant[i] = '*';
    }
    
    DebugPrintf(au8PrintContant);
    DebugPrintNumber(u8AveRate);
    DebugPrintf("\r\n");
    
    //u8AveRate = 0;
 // }
  return;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 2 */
static void UserApp1SM_Function2(void)
{
  static bool bDisplayed = FALSE;
  static u16 u16Timer = 0;
  static u8 au8HRData[] = {0,200,0,0,0};
  static u8 au8Display1[] = "Max:    |Min:    bpm";
  static u8 au8Display2[] = "Time: 0h 00min 00sec";
  
  static u8 au8PrintContant[100];
  static u8 u8AveRate = 0;
  u16Timer++;
  
  if(AntReadAppMessageBuffer())
  {
    /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      u8AveRate = G_au8AntApiCurrentMessageBytes[7];
      if(G_au8AntApiCurrentMessageBytes[7] > au8HRData[0])
      {
        au8HRData[0] = G_au8AntApiCurrentMessageBytes[7];
      }
      if(G_au8AntApiCurrentMessageBytes[7] < au8HRData[1])
      {
        au8HRData[1] = G_au8AntApiCurrentMessageBytes[7];
      }  
    }
  } 
  if(u16Timer == 1000)
  {
    u16Timer = 0;
    au8HRData[4]++;
    
    if(au8HRData[4] == 60)
    {
      au8HRData[3]++;
      au8HRData[4] = 0;
      if(au8HRData[3] == 60)
      {
        au8HRData[2]++;
        au8HRData[3] = 0;
      }
    }

    for(u8 i = 0;i < 100; i++)
    {
      au8PrintContant[i] = '\0';
    }

    for(u8 i = 0;i <  u8AveRate/3; i++)
    {
      au8PrintContant[i] = '*';
    }
    
    DebugPrintf(au8PrintContant);
    DebugPrintNumber(u8AveRate);
    DebugPrintf("\r\n");
    
    au8Display1[5] = au8HRData[0] / 100 + '0';
    au8Display1[6] = (au8HRData[0] % 100) / 10 + '0';
    au8Display1[7] = au8HRData[0] % 10 + '0';
    au8Display1[13] = au8HRData[1] / 100 + '0';
    au8Display1[14] = (au8HRData[1] % 100) / 10 + '0';
    au8Display1[15] = au8HRData[1] % 10 + '0';
    au8Display2[6] = au8HRData[2] + '0';
    au8Display2[9] = au8HRData[3] / 10 + '0';
    au8Display2[10] = au8HRData[3] % 10 + '0';
    au8Display2[15] = au8HRData[4] / 10 + '0';
    au8Display2[16] = au8HRData[4] %10 + '0';
    
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Display1);
    LCDMessage(LINE2_START_ADDR,au8Display2);  
  }
  
  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Max:     Min:    bpm");
    LCDMessage(LINE2_START_ADDR," Time: 0h00min00sec");
    bDisplayed = TRUE;
  }
  if (WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Line1);
    LCDMessage(LINE2_START_ADDR,au8Line2);
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL1);
    UserApp1_StateMachine = UserApp1SM_ModeChoose;
  }
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
  }

} /* end UserApp1SM_Function2() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
