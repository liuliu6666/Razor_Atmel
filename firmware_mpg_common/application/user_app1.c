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
static AntAssignChannelInfoType UserApp1_CHANNEL2_sChannelInfo; /* ANT setup parameters */

static u8 UserApp1_au8MessageFail1[] = "\n\r***ANT channel 1 setup failed***\n\n\r";
static u8 UserApp1_au8MessageFail2[] = "\n\r***ANT channel 2 setup failed***\n\n\r";
static u8 UserApp1_au8Logs[5][5];


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
  
  for(u8 i = 0; i < 5; i++)
  {
    for(u8 j = 0; j < 5; j++)
    {
      UserApp1_au8Logs[i][j] = 0;
    }
  }
  
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
  
 /* Configure Channel 2 for this application */
  UserApp1_CHANNEL2_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP_CHANNEL2;
 
  UserApp1_CHANNEL2_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP_CHANNEL2;

  UserApp1_CHANNEL2_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_CHANNEL2_sChannelInfo.AntNetworkKey[i] = au8ANTPlusNetworkKey[i];
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
/* Wait for the ANT channel assignment */
static void UserApp1SM_AntChannel1Assign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL1) == ANT_CONFIGURED)
  {
    /* Attempt to queue the ANT channel 2 setup */
    if( AntAssignChannel(&UserApp1_CHANNEL2_sChannelInfo) )
    {
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_AntChannel2Assign;
    }
    else
    {
      /* The task isn't properly initialized, so shut it down and don't run */
      DebugPrintf(UserApp1_au8MessageFail2);
      UserApp1_StateMachine = UserApp1SM_Error;
    }
  }
  else
  {
    /* Watch for time out */
    if(IsTimeUp(&UserApp1_u32Timeout, 3000))
    {
      DebugPrintf(UserApp1_au8MessageFail1);
      UserApp1_StateMachine = UserApp1SM_Error;    
    }
  }

} /* end UserApp1SM_WaitChannel1Assign() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT master channel assignment */
static void UserApp1SM_AntChannel2Assign(void)
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL2) == ANT_CONFIGURED )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL1);
    UserApp1_StateMachine = UserApp1SM_AntChannel1Open;
  }
  else
  {
    /* Watch for time out */
    if(IsTimeUp(&UserApp1_u32Timeout, 3000))
    {
      DebugPrintf(UserApp1_au8MessageFail2);
      UserApp1_StateMachine = UserApp1SM_Error;    
    }
  }

} /* end UserApp1SM_AntChannel2Assign */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT  channel open */
static void UserApp1SM_AntChannel1Open(void)
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL1) == ANT_OPEN )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL2);
    UserApp1_StateMachine = UserApp1SM_AntChannel2Open;
  }
  else
  {
    /* Watch for time out */
    if(IsTimeUp(&UserApp1_u32Timeout, 3000))
    {
      DebugPrintf(UserApp1_au8MessageFail2);
      UserApp1_StateMachine = UserApp1SM_Error;    
    }
  }

} /* end UserApp1SM_AntChannel1Open */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT  channel open */
static void UserApp1SM_AntChannel2Open(void)
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL2) == ANT_OPEN )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitForPairing;
  }
  else
  {
    /* Watch for time out */
    if(IsTimeUp(&UserApp1_u32Timeout, 3000))
    {
      DebugPrintf(UserApp1_au8MessageFail2);
      UserApp1_StateMachine = UserApp1SM_Error;    
    }
  }

} /* end UserApp1SM_AntChannel2Open */



/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for pairing */
static void UserApp1SM_WaitForPairing(void)
{
  static bool bDisplayed = FALSE;
  static bool bHRMPaired = FALSE;
  static bool bControlPaired = FALSE;
  static u8 au8TestMessage[] = {0,0,0,0,0,0,0,0};
  
  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Waiting for Pairing");
    LCDMessage(LINE2_START_ADDR,".  .  .  .  .  .");
    bDisplayed = TRUE;
  }
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 1)
      {
        bHRMPaired = TRUE;
      }
      
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0)
        {
          bControlPaired = TRUE;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  }
  
  if(bHRMPaired && bControlPaired)
  {
    DebugPrintf("HRM and remote control paired\n\r");
    UserApp1_StateMachine = UserApp1SM_Idle;
  }

  
} /* end UserApp1SM_WaitForParing */

  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp1SM_Idle(void)
{
  static bool bDisplayed = FALSE;
  static u8 au8TestMessage[] = {0xFF, 0, 0, 0, 0, 0, 0, 0};
  
  if(!bDisplayed)
  {
    DebugPrintf("\r\nIdle is running.\r\n");
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Channel paired !");
    LCDMessage(LINE2_START_ADDR,"Waiting for commands");
    bDisplayed = TRUE;
  }
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        switch(G_au8AntApiCurrentMessageBytes[0])
        {
        case 1: UserApp1_StateMachine = UserApp1SM_Function1;
                DebugPrintf("\r\nFunction 1 is running.\r\n");
                bDisplayed = FALSE;
                break;
        case 2: UserApp1_StateMachine = UserApp1SM_Function2;
                DebugPrintf("\r\nFunction 2 is running.\r\n");
                bDisplayed = FALSE;
                break;
        case 3: UserApp1_StateMachine = UserApp1SM_Function3;
                DebugPrintf("\r\nFunction 3 is running.\r\n");
                bDisplayed = FALSE;
                break;
        case 4: UserApp1_StateMachine = UserApp1SM_Function4;
                DebugPrintf("\r\nFunction 4 is running.\r\n");
                bDisplayed = FALSE;
                break;
        default: ;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */
} /* end UserApp1SM_Idle() */
     


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 1 */
static void UserApp1SM_Function1(void)
{
  static bool bDisplayed = FALSE;
  static u8 au8TestMessage[] = {1, 0, 0, 0, 0, 0, 0, 0};
  static u16 u16Timer = 0;
  static u8 u8AveRate = 0;
  static u8 au8HRData[] = {0,0,0};
  static u8 au8Display1[] = "  Ave. rate:   bpm";
  static u8 au8Display2[] = "Time: 0h 00min 00sec";
  static u8 au8PrintContant[100];
  
  u16Timer++;
  
  if(u16Timer == 1000)
  {
    for(u8 i = 0;i < 100; i++)
    {
      au8PrintContant[i] = '\0';
    }
    
    u16Timer = 0;
    au8HRData[2]++;
    
    if(au8HRData[2] == 60)
    {
      au8HRData[1]++;
      au8HRData[2] = 0;
      if(au8HRData[1] == 60)
      {
        au8HRData[0]++;
        au8HRData[1] = 0;
      }
    }
    
    for(u8 i = 0;i <  u8AveRate/3; i++)
    {
      au8PrintContant[i] = '*';
    }
    DebugPrintf(au8PrintContant);
    DebugPrintNumber(u8AveRate);
    DebugPrintf("\r\n");
    au8Display1[12] = u8AveRate / 100 + '0';
    au8Display1[13] = (u8AveRate % 100) / 10 + '0';
    au8Display1[14] = u8AveRate % 10 + '0';
    au8Display2[6] = au8HRData[0] + '0';
    au8Display2[9] = au8HRData[1] / 10 + '0';
    au8Display2[10] = au8HRData[1] % 10 + '0';
    au8Display2[15] = au8HRData[2] / 10 + '0';
    au8Display2[16] = au8HRData[2] %10 + '0';
    
    
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Display1);
    LCDMessage(LINE2_START_ADDR,au8Display2);
    
    u8AveRate = 0;
  }
  
  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Display1);
    LCDMessage(LINE2_START_ADDR,au8Display2);
    bDisplayed = TRUE;
  }
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 1)
      {
        u8AveRate = G_au8AntApiCurrentMessageBytes[7];
      }
      
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0xFF)
        {
          bDisplayed = FALSE;
          u16Timer = 0;
          u8AveRate = 0;
          au8HRData[0] = 0;
          au8HRData[1] = 0;
          au8HRData[2] = 0;
          au8Display1[12] = ' ';
          au8Display1[13] = ' ';
          au8Display1[14] = ' ';
          au8Display2[6] = '0';
          au8Display2[9] = '0';
          au8Display2[10] = '0';
          au8Display2[15] = '0';
          au8Display2[16] = '0';
          UserApp1_StateMachine = UserApp1SM_Idle;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */

} /* end UserApp1SM_Function1() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 2 */
static void UserApp1SM_Function2(void)
{
  static bool bDisplayed = FALSE;
  static u8 au8TestMessage[] = {2, 0, 0, 0, 0, 0, 0, 0};
  static u16 u16Timer = 0;
  static u8 au8HRData[] = {0,200,0,0,0};
  static u8 au8Display1[] = "Max:    |Min:    bpm";
  static u8 au8Display2[] = "Time: 0h 00min 00sec";
  
  u16Timer++;
  
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
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 1)
      {
        if(G_au8AntApiCurrentMessageBytes[7] > au8HRData[0])
        {
          au8HRData[0] = G_au8AntApiCurrentMessageBytes[7];
        }
        
        if(G_au8AntApiCurrentMessageBytes[7] < au8HRData[1])
        {
          au8HRData[1] = G_au8AntApiCurrentMessageBytes[7];
        }
      }
      
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0xFF)
        {
          bDisplayed = FALSE;
          u16Timer = 0;
          au8Display1[5] = ' ';
          au8Display1[6] = ' ';
          au8Display1[7] = ' ';
          au8Display1[13] = ' ';
          au8Display1[14] = ' ';
          au8Display1[15] = ' ';
          au8Display2[6] = '0';
          au8Display2[9] = '0';
          au8Display2[10] = '0';
          au8Display2[15] = '0';
          au8Display2[16] = '0';
          
          for(u8 i = 4; i > 0; i--)
          {
            for (u8 j = 0; j < 5; j++)
            {
              UserApp1_au8Logs[i][j] = UserApp1_au8Logs[i-1][j];
            }
          }
          
          for(u8 i = 0; i < 5;i++)
          {
            UserApp1_au8Logs[0][i] = au8HRData[i];
            au8HRData[i] = 0;
          }
          
          au8HRData[1] = 200;
          UserApp1_StateMachine = UserApp1SM_Idle;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */
} /* end UserApp1SM_Function2() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 3 */
static void UserApp1SM_Function3(void)
{
  static u8 au8TestMessage[] = {3,0,0,0,0,0,0,0};
  static u8 au8MaxandMinHeartRate[]=" .Max:    |Min:     ";
  static u8 au8TimeDispaly[]="Time: 0h00min00sec  ";
  static u8 u8RowArrayIndex=0;
  static u8 u8ColumnArrayIndex=0;
  static u8 u8RateInforIndex=6;
  static u8 u8TimeInforIndex=6;
  static u8 u8InforCounter=0;
  static bool bDisplayed = TRUE;
  static bool bB1Pressed = FALSE;
  static bool bB2Pressed = FALSE;
  
  if(bDisplayed)
  {
    bDisplayed = FALSE;
    u8InforCounter=0;
    au8MaxandMinHeartRate[0] = u8InforCounter + 1 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex] / 100 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+1]=(UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex] % 100) / 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+2]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex] % 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+9]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+1] / 100 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+10]=(UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+1] % 100) / 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+11]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+1] % 10 + '0';
    au8TimeDispaly[u8TimeInforIndex]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+2] + '0';
    au8TimeDispaly[u8TimeInforIndex+2]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+3] / 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+3]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+3] % 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+7]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+4] / 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+8]=UserApp1_au8Logs[u8RowArrayIndex][u8ColumnArrayIndex+4] % 10 + '0';
 
    au8TimeDispaly[19] = 'V';
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8MaxandMinHeartRate);
    LCDMessage(LINE2_START_ADDR,au8TimeDispaly);
  }
  
  if(bB1Pressed)
  {
    bB1Pressed = FALSE;
    
    if(u8InforCounter!=4)
    {
      u8InforCounter++;
    }
    au8MaxandMinHeartRate[0] = u8InforCounter + 1 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex] / 100 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+1]=(UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex] % 100) / 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+2]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex] % 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+9]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+1] / 100 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+10]=(UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+1] % 100) / 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+11]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+1] % 10 + '0';
    au8TimeDispaly[u8TimeInforIndex]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+2] + '0';
    au8TimeDispaly[u8TimeInforIndex+2]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+3] / 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+3]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+3] % 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+7]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+4] / 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+8]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+4] % 10 + '0';
    
    if(u8InforCounter == 4)
    {
      au8MaxandMinHeartRate[19] = 23;
      au8TimeDispaly[19] = ' ';
    }
    else
    {
      au8MaxandMinHeartRate[19] = 23;
      au8TimeDispaly[19] = 'V';
    }
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8MaxandMinHeartRate);
    LCDMessage(LINE2_START_ADDR,au8TimeDispaly);
  }
  
  if(bB2Pressed)
  {
    bB2Pressed = FALSE;
    
    if(u8InforCounter!=0)
    {
      u8InforCounter--;
    }
    au8MaxandMinHeartRate[0] = u8InforCounter + 1 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex] / 100 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+1]=(UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex] % 100) / 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+2]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex] % 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+9]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+1] / 100 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+10]=(UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+1] % 100) / 10 + '0';
    au8MaxandMinHeartRate[u8RateInforIndex+11]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+1] % 10 + '0';
    au8TimeDispaly[u8TimeInforIndex]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+2] + '0';
    au8TimeDispaly[u8TimeInforIndex+2]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+3] / 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+3]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+3] % 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+7]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+4] / 10 + '0';
    au8TimeDispaly[u8TimeInforIndex+8]=UserApp1_au8Logs[u8RowArrayIndex+u8InforCounter][u8ColumnArrayIndex+4] % 10 + '0';
    
    if(u8InforCounter == 0)
    {
      au8MaxandMinHeartRate[19] = ' ';
      au8TimeDispaly[19] = 'V';
    }
    else
    {
      au8MaxandMinHeartRate[19] = 23;
      au8TimeDispaly[19] = 'V';
    }
    LCDMessage(LINE1_START_ADDR,au8MaxandMinHeartRate);
    LCDMessage(LINE2_START_ADDR,au8TimeDispaly); 
  }

  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0xFF)
        {
          bDisplayed = TRUE;
          UserApp1_StateMachine = UserApp1SM_Idle;
        }
        
        if(G_au8AntApiCurrentMessageBytes[0] == 0x10)
        {
          bB1Pressed = TRUE;
        }
        
        if(G_au8AntApiCurrentMessageBytes[0] == 0x20)
        {
          bB2Pressed = TRUE;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */
  
} /* end UserApp1SM_Function3() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 4 */
static void UserApp1SM_Function4(void)
{
  static u8 au8TestMessage[] = {4,0,0,0,0,0,0,0};
  static u16 u16DisplayInterval=0;
  static u8 au8ScrollMessage[]="Sheng andHeandWang  ";
   
  u16DisplayInterval++;
   
  if(u16DisplayInterval==1000)
  {
    u16DisplayInterval=0;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8ScrollMessage); 
    
    for(u8 u8index=19;u8index>0;u8index--)
    {
      au8ScrollMessage[u8index]=au8ScrollMessage[u8index-1];
    }
    au8ScrollMessage[0]=au8ScrollMessage[19];
  }
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0xFF)
        {
          UserApp1_StateMachine = UserApp1SM_Idle;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */
} /* end UserApp1SM_Function4() */



/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
