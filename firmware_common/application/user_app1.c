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

extern u8 G_au8DebugScanfBuffer[];                     /*From debug.c*/
extern u8 G_au8DebugScanfCharCount;                     /*From debug.c*/

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8Message1[] ="B0:Name B1:Debug";
static u8 au8Message2[] ="B2:Menu";


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
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,au8Message1);
  LCDMessage(LINE2_START_ADDR,au8Message2);

  PWMAudioSetFrequency(BUZZER1,500);
  
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
  u8 u8CharCount;
  static u8 u8Number1=0; 
  static bool bInput=FALSE;
  
  static u8 au8Name[40]="LiuLiu                                  ";
  static u8 au8Name1[40]="LiuLiu                                  ";
  static u8 au8DisplayLine1[20];
  static u8 au8DisplayLine2[20];
  static u32 u32Counter=0;
  static u8 i=39;
  static u8 j;
  static u8 k;
  static u8 tmp;
  static u8 tmp1;
  static u8 u8LeftorRight=0;
  static u8 u8Judge=0;
  
  
  static u16 au16NotesRight[]    = {F5, F5, F5, F5, F5, E5, D5, E5, F5, G5, A5, A5, A5, A5, A5, G5, F5, G5, A5, A5S, C6, F5, F5, D6, C6, A5S, A5, G5, F5, NO, NO};
  static u16 au16DurationRight[] = {QN, QN, HN, EN, EN, EN, EN, EN, EN, QN, QN, QN, HN, EN, EN, EN, EN, EN, EN, QN,  HN, HN, EN, EN, EN, EN,  QN, QN, HN, HN, FN};
  static u16 au16NoteTypeRight[] = {RT, RT, HT, RT, RT, RT, RT, RT, RT, RT, RT, RT, HT, RT, RT, RT, RT, RT, RT, RT,  RT, HT, RT, RT, RT, RT,  RT, RT, RT, HT, HT};
  static u8 u8IndexRight = 0;
  static u32 u32RightTimer = 0;
  static u16 u16CurrentDurationRight = 0;
  static u16 u16NoteSilentDurationRight = 0;
  static bool bNoteActiveNextRight = TRUE;
  
  u8 u8CurrentIndex;

  if(u8Judge==0)/*If be in menu*/
  {
    if(WasButtonPressed(BUTTON0))/*choose name*/
    {
      ButtonAcknowledge(BUTTON0);
      for(u32 m=0;m<40;m++)
      {
        au8Name[m]=au8Name1[m];
      }
      u8LeftorRight=0;
      u8Judge=1;
    }
    if(WasButtonPressed(BUTTON1))/*choose debug*/
    {
      ButtonAcknowledge(BUTTON1);
      LCDCommand(LCD_CLEAR_CMD);
      u8Number1=0;
      u8Judge=2;
    }
    
  }
  
  if(u8Judge==1)
  {
    u32Counter++;
    if(u32Counter==500)/*500ms once*/
    {
      u32Counter=0;
      if(WasButtonPressed(BUTTON1))
      {
        ButtonAcknowledge(BUTTON1);
        u8LeftorRight=1;
        
      }
      if(WasButtonPressed(BUTTON0))
      {
        ButtonAcknowledge(BUTTON0);
        u8LeftorRight=2;
      }
      /*right*/
      if(u8LeftorRight==1)
      {
        if(au8DisplayLine2[19]!='u')
        {
          
          tmp=au8Name[39];
          for(i=39;i>0;i--)
          {
            au8Name[i]=au8Name[i-1];
          }
          au8Name[0]=tmp;
        }
        else
        {
          PWMAudioOn(BUZZER1);
        }
      }
      /*left*/
      if(u8LeftorRight==2)
      {
        if(au8DisplayLine1[0]!='L')
        {
          PWMAudioOff(BUZZER1);
          tmp1=au8Name[0];
          for(k=0;k<39;k++)
          {
            au8Name[k]=au8Name[k+1];
          }
          au8Name[39]=tmp1;
        }
        else
        {
          PWMAudioOn(BUZZER1);
        }
      }
      /*put au8Name in au8DisplayLine*/
      for(j=0;j<=19;j++)
      {
        au8DisplayLine1[j]=au8Name[j];
        au8DisplayLine2[j]=au8Name[j+20];
      }
      /*Display*/
      LCDMessage(LINE1_START_ADDR,au8DisplayLine1);
      LCDMessage(LINE2_START_ADDR,au8DisplayLine2);
    }
    
    
  }
  if(u8Judge==2)
  {
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    if(u8CharCount>0)/*when input */
    {
      if(UserApp_au8UserInputBuffer[0]==0x0D)/*enter*/
      {
        if(u8Number1<=19)
        {
          u8Number1=20;
          LCDClearChars(LINE2_START_ADDR,20 );
        }
        else if(u8Number1<=39)
        {
          u8Number1=0;
          LCDClearChars(LINE1_START_ADDR,20 );
        }
      }
      else
      {
        if(u8Number1<=19)/*display in Line1*/
        {
          bInput=TRUE;
          if(bInput==TRUE)
          {
            LCDMessage(LINE1_START_ADDR+u8Number1,UserApp_au8UserInputBuffer);
            u8Number1++;
            bInput=FALSE;
          }
        }
        else if(u8Number1<=39)/*display in Line2*/
        {
          bInput=TRUE;
          if(bInput==TRUE)
          {
            LCDMessage(LINE2_START_ADDR+u8Number1-20,UserApp_au8UserInputBuffer);
            u8Number1++;
            bInput=FALSE;
          }
        }
      }
    }
    
  }
  
  
  
  if(WasButtonPressed(BUTTON3))/*return menu*/
  {
    ButtonAcknowledge(BUTTON3);
    u8Judge=0;
    u8LeftorRight=0;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Message1);
    LCDMessage(LINE2_START_ADDR,au8Message2);
  }
  
  
  if(u8LeftorRight==1)
  {
    
    if(au8DisplayLine2[19]!='u')
    {
      
      if(IsTimeUp(&u32RightTimer, (u32)u16CurrentDurationRight))/*music*/
      {
        u32RightTimer = G_u32SystemTime1ms;
        u8CurrentIndex = u8IndexRight;
        /* Set up to play current note */
        if(bNoteActiveNextRight)
        {
          if(au16NoteTypeRight[u8CurrentIndex] == RT)
          {
            u16CurrentDurationRight = au16DurationRight[u8CurrentIndex] - REGULAR_NOTE_ADJUSTMENT;
            u16NoteSilentDurationRight = REGULAR_NOTE_ADJUSTMENT;
            bNoteActiveNextRight = FALSE;
          } /* end RT case */
          else if(au16NoteTypeRight[u8CurrentIndex] == ST)
          {
            u16CurrentDurationRight = STACCATO_NOTE_TIME;
            u16NoteSilentDurationRight = au16DurationRight[u8CurrentIndex] - STACCATO_NOTE_TIME;
            bNoteActiveNextRight = FALSE;
          } /* end ST case */
          else if(au16NoteTypeRight[u8CurrentIndex] == HT)
          {
            u16CurrentDurationRight = au16DurationRight[u8CurrentIndex];
            u16NoteSilentDurationRight = 0;
            bNoteActiveNextRight = TRUE;
            u8IndexRight++;
            if(u8IndexRight == sizeof(au16NotesRight) / sizeof(u16) )
            {
              u8IndexRight = 0;
            }
          } /* end HT case */
          /* Set the buzzer frequency for the note (handle NO special case) */
          if(au16NotesRight[u8CurrentIndex] != NO)
          {
            PWMAudioSetFrequency(BUZZER1, au16NotesRight[u8CurrentIndex]);
            PWMAudioOn(BUZZER1);
          }
          else
          {                
            PWMAudioOff(BUZZER1);
          }
        } /* end if(bNoteActiveNextRight) */
        else
        {      
          /* No active note */
          PWMAudioOff(BUZZER1);
          u32RightTimer = G_u32SystemTime1ms;
          u16CurrentDurationRight = u16NoteSilentDurationRight;
          bNoteActiveNextRight = TRUE;
          u8IndexRight++;
          if(u8IndexRight == sizeof(au16NotesRight) / sizeof(u16) )
          {
            u8IndexRight = 0;
          }
        } /* end else if(bNoteActiveNextRight) */
      } /* end if(IsTimeUp(&u32RightTimer, (u32)u16CurrentDurationRight)) */
      
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
