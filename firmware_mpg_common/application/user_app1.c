/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

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
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  static u8 au8String1[]="\n\rLED Programming Interface\n\rPress 1 to program LED command sequence\n\rPress 2 to show current USER program\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
  DebugPrintf(au8String1);
  
  for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
  {
    UserApp_au8UserInputBuffer[i] = 0;
  }
  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
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
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  static u8 u8CharCount;
  static u8 a = 0 ;/*to count bits in buffer*/
  static u8 u8Check = 0;
  static bool bInput = FALSE;
  static bool bLetterIsValid = FALSE;
  static u8 u8List = 0; /*To counter the number of item */
  static u8 u8Show = 0;/*for LedDisplayPrintListLine()*/
  static u8 u8Menu[] = "\n\rLED Programming Interface\n\rPress 1 to program LED command sequence\n\rPress 2 to show current USER program\n\r";
  
  static u8 u8PressError[] = "\n\rInvalid command\n\r";
  static u8 u8InputError[]= "\n\rInvalid command: incorrrect format. Please use L-ONTIME-OFFTIME\n\r";

  static u8 u8Press1[] = "\n\rEnter commands as LED-ONTIME-OFFTIME and press Enter\n\rTime is in milliseconds, max 100 commands\n\rLED colors: R(r),O(o),Y(y),G(g),C(c),B(b),P(p),W(w)\n\rExample: R-100-200 (RED on at 100ms and off at 200ms)\n\rPress Enter on blank line to end\n\r";
  static u8 u8ShowMessage5[] = "\n\rLED   ON TIME   OFF TIME\n\r-----------------------------------\n\r";
  static u8 u8ShowMessage6[] = "\n\rCommand Entry complete\n\rCommand entered:";
  static bool bSymbolIsValid = FALSE;
  static u8 u8State = 0;
  static LedCommandType Command1;
  static LedCommandType Command2;
  
  
  static u8 LetterIntoColor(u8 *au8Array);
  static bool LetterIsValid(u8 *auArray);
  static bool SymbolIsValid(u8 *au8Array);
  static u32 OnTime (u8 *auArray);
  static u32 OffTime (u8 *auArray);

  if(u8State==0)/*be in menu*/
  {
    u8CharCount=DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount]='\0';
    if(u8CharCount==1)/*when input sth.*/
    {
      if(UserApp_au8UserInputBuffer[0]=='1')/*press 1*/
      {
        u8List=0;
        LedDisplayStartList();
        DebugLineFeed();
        u8State=1;
      }
      else if(UserApp_au8UserInputBuffer[0]=='2')/*press 2*/
      {
        DebugLineFeed();
        u8State=4;
        DebugPrintf(u8ShowMessage6);
        DebugPrintNumber(u8List);
        DebugLineFeed();
        DebugPrintf(u8ShowMessage5);
      }
      else /*press others*/
      {
        DebugPrintf(u8PressError);
        DebugPrintf(u8Menu);
        u8State = 0;
      }
    }
  }
  if (u8State == 1)
  {
    DebugPrintf(u8Press1);/*when press 1,display instructions*/
    u8State = 2;
  }
  if(u8State==2)
  {
    if(G_au8DebugScanfBuffer[a]!=0x0D&&G_au8DebugScanfBuffer[a]!='\0')
    {
      a++;
    }/*count how long the input is*/
    if(G_au8DebugScanfBuffer[a]==0x0D&&a>0)
    {
      a=0;
      u8State=3;
      u8CharCount=DebugScanf(UserApp_au8UserInputBuffer);
      UserApp_au8UserInputBuffer[u8CharCount]='\0';
    }/*enter command and enter*/
    if (G_au8DebugScanfBuffer[0] == 0x0D)
    {
      u8CharCount  = DebugScanf(UserApp_au8UserInputBuffer);
      UserApp_au8UserInputBuffer[u8CharCount ] = '\0';
      u8State = 4;
      DebugPrintf(u8ShowMessage6);
      DebugPrintNumber(u8List);
      DebugLineFeed();
      DebugPrintf(u8ShowMessage5);
    }/*enter in blank line to end*/
  }
  if (u8State == 3)
  { 
    bInput = FALSE;
    bLetterIsValid = FALSE;
    bSymbolIsValid = FALSE;
    u8Check = 0;
    bLetterIsValid = LetterIsValid(UserApp_au8UserInputBuffer);
    bSymbolIsValid = SymbolIsValid(UserApp_au8UserInputBuffer);
    u8State = 2;
    if (bLetterIsValid)
    {
      u8Check++; 
    }
    if (UserApp_au8UserInputBuffer[1] == 0x2D)
    {
      u8Check++;
    }
    if (bSymbolIsValid)
    {
      u8Check++;
    }
    if (u8Check == 3)
    {
      bInput = TRUE;
    }
    if (bInput)
    {
      DebugLineFeed();
      u8State = 2;
      Command1.eLED = LetterIntoColor(UserApp_au8UserInputBuffer);
      Command1.u32Time = OnTime(UserApp_au8UserInputBuffer);
      Command1.bOn = TRUE;
      Command1.eCurrentRate = LED_PWM_100;
      LedDisplayAddCommand(USER_LIST , &Command1);
      
      Command2.eLED = LetterIntoColor(UserApp_au8UserInputBuffer);
      Command2.u32Time = OffTime(UserApp_au8UserInputBuffer);
      Command2.bOn = FALSE;
      Command2.eCurrentRate = LED_PWM_0;
      LedDisplayAddCommand(USER_LIST , &Command2);
      u8List++;
    }
    if (bInput == FALSE)
    {
      DebugPrintf(u8InputError);
      u8State = 2;
    }
  }
  if (u8State == 4)
  {
    if(LedDisplayPrintListLine(u8Show)){
       u8Show++; 
    }else{
      if(u8Show==0){
        DebugLineFeed();
        DebugPrintf("USERLIST IS EMPTY");
        DebugLineFeed();
      }
      u8Show=0;
      u8State = 5;
    }
    
#if 0
    for (u8Show=0;u8Show<u8List;u8Show++)
    {
      LedDisplayPrintListLine(u8Show);
    }
    if (u8Show == u8List)
    {
      u8State = 5;
    }
#endif
  }
  if (u8State ==5)
  {
    DebugPrintf(u8Menu);
    u8State = 0;
  }
  
  
  
} /* end UserApp1SM_Idle() */



u8 LetterIntoColor(u8 *au8Array)/*function for convert to color*/
{
  static u8 au8BigLetter[] =      { 'W'   ,'P'    ,'B'  ,'C'  ,'G'   ,'Y'    ,'O'    ,'R'};
  static u8 au8SmallLetter[] =    { 'w'   ,'p'    ,'b'  ,'c'  ,'g'   ,'y'    ,'o'    ,'r'};
  static LedNumberType au8Color[]={WHITE, PURPLE, BLUE, CYAN, GREEN, YELLOW, ORANGE, RED,};
  u8 q=0;
  u8 s=0;
  for(q=0;q<8;q++)
  {
    if(UserApp_au8UserInputBuffer[0]==au8BigLetter[q])
    {
      s=q;
    }
  }/*put bigletter into Color*/
  for(q=0;q<8;q++)
  {
    if(UserApp_au8UserInputBuffer[0]==au8SmallLetter[q])
    {
      s=q;
    }
  }/*put smallletter into Color*/
  return(au8Color[s]);
}


bool LetterIsValid(u8 *auArray)
{
  static u8 au8Color[] = {'W','P','B','C','G','Y','O','R','w','p','b','c','g','y','o','r'};
  bool bLetterIsValid = FALSE;
  for(u8 i=0;i<16;i++)
  {
    if(UserApp_au8UserInputBuffer[0] == au8Color[i])
    {
      bLetterIsValid = TRUE;
    }
  }
  return (bLetterIsValid);
}

bool SymbolIsValid(u8 *au8Array)
{
  static bool bSymbolIsValid = FALSE;
  
  for(u8 i=2;UserApp_au8UserInputBuffer[i]!='\0';i++)
  {
    if (UserApp_au8UserInputBuffer[i] == 0x2D)
    {
      bSymbolIsValid = TRUE;
    }
  }
  return (bSymbolIsValid);
}

u32 OnTime(u8 *auArray)/*function for ontime*/
{
  u32 u32OnTime = 0;
  static u8 z = 2;
  static u8 x = 0;
  
  for(z=2;UserApp_au8UserInputBuffer[z]!=0x2D;z++)
  {
    x = z+1;
  }/*confirm the ontime location and length*/
  for (z=2;z<x;z++)
  {
    u32OnTime =u32OnTime*10+(UserApp_au8UserInputBuffer[z]-'0');
  }/*calculate the ontime*/
  return (u32OnTime);
}

u32 OffTime(u8 *auArray)/*function for offtime*/
{
  u32 u32OffTime = 0;
  static u8 m=0;
  static u8 n=0;
  
  for(u8 i=0;i<20;i++)
  {
    if (UserApp_au8UserInputBuffer[i] == 0x0D)
    {
      m= i-1;
    }
  }/*find the '-' in which place=='m'*/
  for(u8 j=2;UserApp_au8UserInputBuffer[j]!=0x2D;j++)
  {
    n = j+1;
  }/*find the OffTime is which to start*/
  for (u8 k=n+1;k<=m;k++)
  {
    u32OffTime =u32OffTime*10+(UserApp_au8UserInputBuffer[k]-'0');
  }/*calculate*/
  return (u32OffTime);
}

#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
