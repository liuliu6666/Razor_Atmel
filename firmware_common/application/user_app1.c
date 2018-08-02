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


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static u16 LED_data[5][32]={

{0x01,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x7F,0xFC,0x48,0x24,0x44,0x44,0x4F,0xE4,0x41,0x04,0x41,0x04,0x5F,0xF4,0x41,0x04,0x41,0x04,0x41,0x14,0x40,0x08},/*"南",0*/

{0x02,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x01,0x00,0x11,0x10,0x11,0x08,0x21,0x04,0x45,0x04,0x02,0x00},/*"京",1*/

{0x00,0x00,0x00,0x00,0x7F,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00},/*"工",2*/

{0x08,0x00,0x1D,0xFC,0xF1,0x04,0x11,0x04,0x11,0x04,0xFD,0xFC,0x10,0x00,0x30,0x00,0x39,0xFE,0x54,0x20,0x54,0x20,0x91,0xFC,0x10,0x20,0x10,0x20,0x13,0xFE,0x10,0x00},/*"程",3*/

{0x22,0x08,0x11,0x08,0x11,0x10,0x00,0x20,0x7F,0xFE,0x40,0x02,0x80,0x04,0x1F,0xE0,0x00,0x40,0x01,0x80,0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00},/*"学",4*/



};
static u8 LED_data1[10];
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
 AT91C_BASE_PIOA->PIO_PER = PA_12_BLADE_UPOMI|PA_11_BLADE_UPIMO|PA_14_BLADE_MOSI|PA_15_BLADE_SCK;//A12 A11 A14 A15启用寄存器
 AT91C_BASE_PIOA->PIO_OER = PA_12_BLADE_UPOMI|PA_11_BLADE_UPIMO|PA_14_BLADE_MOSI|PA_15_BLADE_SCK;//A12=LE A11=OE A14=SDI A15=CLK输出使能
 AT91C_BASE_PIOA->PIO_PER = PA_03_HSMCI_MCCK|PA_04_HSMCI_MCCDA|PA_05_HSMCI_MCDA0|PA_06_HSMCI_MCDA1|PA_07_HSMCI_MCDA2|PA_08_SD_CS_MCDA3;//A3-A8启用寄存器
 AT91C_BASE_PIOA->PIO_OER = PA_03_HSMCI_MCCK|PA_04_HSMCI_MCCDA|PA_05_HSMCI_MCDA0|PA_06_HSMCI_MCDA1|PA_07_HSMCI_MCDA2|PA_08_SD_CS_MCDA3;//输出使能
 
 AT91C_BASE_PIOA->PIO_SODR = PA_03_HSMCI_MCCK;//CD_STB1=1
 
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
  
  for(u8 i=0;i<16;i++)
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_04_HSMCI_MCCDA;//INH1=0
    MBI_data(i);
    CD_DATA(i);
    AT91C_BASE_PIOA->PIO_SODR = PA_11_BLADE_UPIMO;//OE=1
    delay(5);
    AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI;//LE=1
    delay(5);
    AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI;//LE=0
    delay(5);
    AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;//OE=0
    AT91C_BASE_PIOA->PIO_SODR = PA_04_HSMCI_MCCDA;//INH1=1
    delay(5);
  }
} /* end UserApp1SM_Idle() */



void MBI_data(u8 k)
{
//  u8 m;
//  u8 x;
  static u8 data;
  u8 u8Counter=0;
  u8Counter++;

  for(u8 x=0;x<5;x++)
  {
    LED_data1[x*2]=LED_data[x][k*2];
    LED_data1[x*2+1]=LED_data[x][k*2+1];
  }
  
//  for(u8 m=5;m>0;m--)
//  {
//    for(x=k*2+2;x>(k*2);x--)
//    {
  if(u8Counter>=200)
  {
    for(u8 a=0;a<10;a++)
    {
    LED_data1[a]=LED_data1[a+1];
    }
    u8Counter=0;
  }
  for(u8 m=10;m>0;m--)
  {
//      data=LED_data[m-1][x-1];
    data=LED_data1[m-1];
      for(u8 j=0;j<8;j++)
      {
        AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;//CLK=0
        if((0x01&data)==1)AT91C_BASE_PIOA->PIO_SODR = PA_14_BLADE_MOSI;//SDI=1
        else AT91C_BASE_PIOA->PIO_CODR = PA_14_BLADE_MOSI;//SDI=0
        delay(5);
        AT91C_BASE_PIOA->PIO_SODR = PA_15_BLADE_SCK;//CLK=1
        delay(5);
        data=data>>1;
      }
    
  }
  
}
void CD_DATA(u8 n)
{
  switch (n)
  {
    case 0:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;break;//0000 C0
    case 1:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1;AT91C_BASE_PIOA->PIO_SODR = PA_05_HSMCI_MCDA0;break;//0001 C1
    case 2:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2|PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_06_HSMCI_MCDA1;break;//0010 C2
    case 3:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2;AT91C_BASE_PIOA->PIO_SODR = PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;break;//0011 C3
      
    case 4:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_07_HSMCI_MCDA2;break;//0100 C4
    case 5:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_06_HSMCI_MCDA1;AT91C_BASE_PIOA->PIO_SODR = PA_07_HSMCI_MCDA2|PA_05_HSMCI_MCDA0;break;//0101 C5
    case 6:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3|PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1;break;//0110 C6
    case 7:AT91C_BASE_PIOA->PIO_CODR = PA_08_SD_CS_MCDA3;AT91C_BASE_PIOA->PIO_SODR = PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;break;//0111 C7
    
    case 8: AT91C_BASE_PIOA->PIO_CODR = PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3;break;//1000 C8
    case 9: AT91C_BASE_PIOA->PIO_CODR = PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_05_HSMCI_MCDA0;break;//1001 C9
    case 10:AT91C_BASE_PIOA->PIO_CODR = PA_07_HSMCI_MCDA2|PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_06_HSMCI_MCDA1;break;//1010 C10
    case 11:AT91C_BASE_PIOA->PIO_CODR = PA_07_HSMCI_MCDA2;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;break;//1011 C11
    
    case 12:AT91C_BASE_PIOA->PIO_CODR = PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2;break;//1100 C12
    case 13:AT91C_BASE_PIOA->PIO_CODR = PA_06_HSMCI_MCDA1;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2|PA_05_HSMCI_MCDA0;break;//1101 C13
    case 14:AT91C_BASE_PIOA->PIO_CODR = PA_05_HSMCI_MCDA0;AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1;break;//1110 C14
    case 15:AT91C_BASE_PIOA->PIO_SODR = PA_08_SD_CS_MCDA3|PA_07_HSMCI_MCDA2|PA_06_HSMCI_MCDA1|PA_05_HSMCI_MCDA0;break;//1111 C15
  }
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

void delay(u16 l)
{
  for(u16 n=0;n<=l;n++)
  {
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
