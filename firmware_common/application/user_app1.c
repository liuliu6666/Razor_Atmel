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

//static u16 LED_data[5][32]={
//
//{0x01,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x7F,0xFC,0x48,0x24,0x44,0x44,0x4F,0xE4,0x41,0x04,0x41,0x04,0x5F,0xF4,0x41,0x04,0x41,0x04,0x41,0x14,0x40,0x08},/*"南",0*/
//
//{0x02,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x01,0x00,0x11,0x10,0x11,0x08,0x21,0x04,0x45,0x04,0x02,0x00},/*"京",1*/
//
//{0x00,0x00,0x00,0x00,0x7F,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00},/*"工",2*/
//
//{0x08,0x00,0x1D,0xFC,0xF1,0x04,0x11,0x04,0x11,0x04,0xFD,0xFC,0x10,0x00,0x30,0x00,0x39,0xFE,0x54,0x20,0x54,0x20,0x91,0xFC,0x10,0x20,0x10,0x20,0x13,0xFE,0x10,0x00},/*"程",3*/
//
//{0x22,0x08,0x11,0x08,0x11,0x10,0x00,0x20,0x7F,0xFE,0x40,0x02,0x80,0x04,0x1F,0xE0,0x00,0x40,0x01,0x80,0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00},/*"学",4*/
//
//};
//
//static u8 LED_data1[10];

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
  /* Enable PIO and output */
  AT91C_BASE_PIOA->PIO_PER = PA_00_GND;
  AT91C_BASE_PIOA->PIO_PER = PA_05_A;
  AT91C_BASE_PIOA->PIO_PER = PA_08_D;
  AT91C_BASE_PIOA->PIO_PER = PA_03_CD_STB;
  AT91C_BASE_PIOA->PIO_PER = PA_07_C;
  AT91C_BASE_PIOA->PIO_PER = PA_06_B;
  AT91C_BASE_PIOA->PIO_PER = PA_04_INH;
  AT91C_BASE_PIOA->PIO_PER = PA_10_I2C_M_SCL;
  AT91C_BASE_PIOA->PIO_PER = PA_09_I2C_M_SDA;
  AT91C_BASE_PIOA->PIO_PER = PA_12_M_LE;
  AT91C_BASE_PIOA->PIO_PER = PA_11_M_OE;
  AT91C_BASE_PIOA->PIO_PER = PA_15_M_CLK;
  AT91C_BASE_PIOA->PIO_PER = PA_14_M_SDI;
  AT91C_BASE_PIOA->PIO_PER = PA_19_U0_PIMO;
  AT91C_BASE_PIOA->PIO_PER = PA_18_U0_POMI;
  AT91C_BASE_PIOA->PIO_PER = PB_06_F_SDO;
  AT91C_BASE_PIOA->PIO_PER = PB_05_F_CS;
  AT91C_BASE_PIOA->PIO_PER = PB_08_SI;
  AT91C_BASE_PIOA->PIO_PER = PB_07_SCLK;

  AT91C_BASE_PIOA->PIO_OER = PA_00_GND;
  AT91C_BASE_PIOA->PIO_OER = PA_05_A;
  AT91C_BASE_PIOA->PIO_OER = PA_08_D;
  AT91C_BASE_PIOA->PIO_OER = PA_03_CD_STB;
  AT91C_BASE_PIOA->PIO_OER = PA_07_C;
  AT91C_BASE_PIOA->PIO_OER = PA_06_B;
  AT91C_BASE_PIOA->PIO_OER = PA_04_INH;
  AT91C_BASE_PIOA->PIO_OER = PA_10_I2C_M_SCL;
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_M_SDA;
  AT91C_BASE_PIOA->PIO_OER = PA_12_M_LE;
  AT91C_BASE_PIOA->PIO_OER = PA_11_M_OE;
  AT91C_BASE_PIOA->PIO_OER = PA_15_M_CLK;
  AT91C_BASE_PIOA->PIO_OER = PA_14_M_SDI;
  AT91C_BASE_PIOA->PIO_OER = PA_19_U0_PIMO;
  AT91C_BASE_PIOA->PIO_OER = PA_18_U0_POMI;
  AT91C_BASE_PIOA->PIO_OER = PB_06_F_SDO;
  AT91C_BASE_PIOA->PIO_OER = PB_05_F_CS;
  AT91C_BASE_PIOA->PIO_OER = PB_08_SI;
  AT91C_BASE_PIOA->PIO_OER = PB_07_SCLK;
  
 /* Initialize the GPIO ports */
  AT91C_BASE_PIOA->PIO_CODR = PA_00_GND;
  AT91C_BASE_PIOA->PIO_SODR = PA_03_CD_STB;
  AT91C_BASE_PIOA->PIO_CODR = PA_04_INH;
  AT91C_BASE_PIOA->PIO_CODR = PA_05_A;
  AT91C_BASE_PIOA->PIO_CODR = PA_06_B;
  AT91C_BASE_PIOA->PIO_CODR = PA_07_C;
  AT91C_BASE_PIOA->PIO_CODR = PA_08_D;
  AT91C_BASE_PIOA->PIO_CODR = PA_11_M_OE;
  AT91C_BASE_PIOA->PIO_SODR = PA_12_M_LE;
  AT91C_BASE_PIOA->PIO_CODR = PA_14_M_SDI;
  AT91C_BASE_PIOA->PIO_CODR = PA_15_M_CLK;
  AT91C_BASE_PIOB->PIO_SODR = PB_05_F_CS;
  AT91C_BASE_PIOB->PIO_CODR = PB_06_F_SDO;
  AT91C_BASE_PIOB->PIO_CODR = PB_07_SCLK;
  AT91C_BASE_PIOB->PIO_CODR = PB_08_SI;
   
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
  bool bRollTemp = FALSE;
  static u8 u8CD4515Data = 0;
  static u16 u16RollCounter = 0;
  static u8 u8MBI5026GFData[7][16][2] = 
  {
/*    {{0x00,0x80},
{0x20,0x40},
{0x10,0x40},
{0x17,0xFC},
{0x00,0x10},
{0x02,0x10},
{0xF1,0x20},
{0x10,0xA0},
{0x10,0x40},
{0x10,0xA0},
{0x11,0x10},
{0x12,0x08},
{0x14,0x08},
{0x28,0x00},
{0x47,0xFE},
{0x00,0x00},
    //"这",0
  },
  
    {{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x00,0x00},
{0xFF,0xFE},
{0x01,0x00},
{0x11,0x00},
{0x11,0xF8},
{0x11,0x00},
{0x29,0x00},
{0x45,0x00},
{0x83,0xFE},
    //"是",1
  },*/
  
    {{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0xFF,0xFE},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
    //"一",2
  },
  
    {{0x01,0x00},
{0x01,0x00},
{0x02,0x80},
{0x04,0x40},
{0x08,0x20},
{0x10,0x10},
{0x21,0x08},
{0xC1,0x06},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
    //"个",3
  },
  
    {{0x00,0x80},
{0x20,0x40},
{0x17,0xFC},
{0x11,0x10},
{0x82,0x48},
{0x44,0x84},
{0x41,0x20},
{0x13,0xF0},
{0x10,0x10},
{0x20,0x88},
{0xE1,0x50},
{0x23,0x20},
{0x25,0x10},
{0x29,0x48},
{0x21,0x86},
{0x01,0x00},
    //"滚",4
  },

    {{0x00,0x40},
{0x00,0x40},
{0x7C,0x40},
{0x00,0x40},
{0x01,0xFC},
{0x00,0x44},
{0xFE,0x44},
{0x20,0x44},
{0x20,0x44},
{0x20,0x84},
{0x48,0x84},
{0x44,0x84},
{0xFD,0x04},
{0x45,0x04},
{0x02,0x28},
{0x04,0x10},
//"动",5
  },

    {{0x00,0x00},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x04,0x40},
{0x44,0x44},
{0x24,0x44},
{0x14,0x48},
{0x14,0x50},
{0x04,0x40},
{0xFF,0xFE},
{0x00,0x00},
//"显",6
  },

    {{0x00,0x00},
{0x3F,0xF8},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0xFF,0xFE},
{0x01,0x00},
{0x01,0x00},
{0x11,0x10},
{0x11,0x08},
{0x21,0x04},
{0x41,0x02},
{0x81,0x02},
{0x05,0x00},
{0x02,0x00},
//"示",7
  },

    {{0x00,0x00},
{0x3F,0xF8},
{0x20,0x08},
{0x20,0x08},
{0x3F,0xF8},
{0x24,0x10},
{0x22,0x20},
{0x2F,0xF8},
{0x22,0x20},
{0x22,0x20},
{0x3F,0xFC},
{0x22,0x20},
{0x42,0x20},
{0x44,0x20},
{0x84,0x20},
{0x08,0x20},
//"屏",8
  },

/*    {{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x18,0x00},
{0x24,0x00},
{0x24,0x00},
{0x18,0x00},
{0x00,0x00},
{0x00,0x00},
//"。",9
  },*/
  };

    //AT91C_BASE_PIOA->PIO_CODR = PA_04_INH;//INH1=0
    //AT91C_BASE_PIOA->PIO_SODR = PA_11_M_OE;//OE=1
    delay(5);
    
    u16RollCounter++;

    if(u16RollCounter == 100)
    {
      u16RollCounter = 0;
      for(int j=0;j<16;j++)//循环用来显示16行
      {
        bRollTemp = (bool)(u8MBI5026GFData[0][j][0] & 0x80);//第一个字的最左边最高位 保护作用
        for(int i=0;i<7;i++)//每个字
        {
          u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] << 1;//一个字的某行左移
          if(u8MBI5026GFData[i][j][1] & 0x80)//如果第二个八位 的最高位为高电平
          {//则
            u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] | 0x01;//令第一个八位的最低位为高电平
          }
          else//否则
          {
            u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] & 0xFE;//令第一个八位的最低位为低电平
          }
          u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] << 1;//一个字的第二个八位左移
          if(i == 6)//如果到第七个字
          {
            if(bRollTemp)//如果保护值为高电平
            {
              u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] | 0x01;//令第二个八位的最低位为高电平
            }
            else//否则
            {
              u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] & 0xFE;//令第二个八位的最低位为低电平
            }
          }
          else//如果没到
          {
            if(u8MBI5026GFData[i+1][j][0] & 0x80)//如果下一个字的第一个八位的为高位为高
            {
              u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] | 0x01;//则令这个字的第二个八位的最低位为高
            }
            else//否则
            {
              u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] & 0xFE;//令这个字的第二个八位的最低位为低
            }
          }
        }
      }
    }
    for(u8 x=0;x<16;x++)
    {
      AT91C_BASE_PIOA->PIO_CODR = PA_04_INH;//INH1=0
      CD_Data(x);
      for(int m=5;m>0;m--)
      {
        MBI5025BF_SendData(u8MBI5026GFData[m-1][x][1]);
        MBI5025BF_SendData(u8MBI5026GFData[m-1][x][0]);
      }
      AT91C_BASE_PIOA->PIO_SODR = PA_11_M_OE;//OE=1
      delay(5);
      AT91C_BASE_PIOA->PIO_SODR = PA_12_M_LE;//LE=1
      delay(5);
      AT91C_BASE_PIOA->PIO_CODR = PA_12_M_LE;//LE=0
      delay(5);
      AT91C_BASE_PIOA->PIO_CODR = PA_11_M_OE;//OE=0
      AT91C_BASE_PIOA->PIO_SODR = PA_04_INH;//INH1=1
      delay(5);
    }
  
} /* end UserApp1SM_Idle() */

void MBI5025BF_SendData(u8 u8data)
{
  for(u8 j=0;j<8;j++)
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_15_M_CLK;//CLK=0
    if((0x01&u8data)==1)AT91C_BASE_PIOA->PIO_SODR = PA_14_M_SDI;//SDI=1
    else AT91C_BASE_PIOA->PIO_CODR = PA_14_M_SDI;//SDI=0
    delay(5);
    AT91C_BASE_PIOA->PIO_SODR = PA_15_M_CLK;//CLK=1
    delay(5);
    u8data=u8data>>1;
  }
}
void CD_Data(u8 u8data)
{
  switch (u8data)
  {
    case 0:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_07_C|PA_06_B|PA_05_A;break;//0000 C0
    case 1:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_07_C|PA_06_B;                 AT91C_BASE_PIOA->PIO_SODR = PA_05_A;break;//0001 C1
    case 2:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_07_C|PA_05_A;                 AT91C_BASE_PIOA->PIO_SODR = PA_06_B;break;//0010 C2
    case 3:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_07_C;                         AT91C_BASE_PIOA->PIO_SODR = PA_06_B|PA_05_A;break;//0011 C3
      
    case 4:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_06_B|PA_05_A;                 AT91C_BASE_PIOA->PIO_SODR = PA_07_C;break;//0100 C4
    case 5:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_06_B;                         AT91C_BASE_PIOA->PIO_SODR = PA_07_C|PA_05_A;break;//0101 C5
    case 6:AT91C_BASE_PIOA->PIO_CODR = PA_08_D|PA_05_A;                         AT91C_BASE_PIOA->PIO_SODR = PA_07_C|PA_06_B;break;//0110 C6
    case 7:AT91C_BASE_PIOA->PIO_CODR = PA_08_D;                                 AT91C_BASE_PIOA->PIO_SODR = PA_07_C|PA_06_B|PA_05_A;break;//0111 C7
    
    case 8: AT91C_BASE_PIOA->PIO_CODR = PA_07_C|PA_06_B|PA_05_A;                AT91C_BASE_PIOA->PIO_SODR = PA_08_D;break;//1000 C8
    case 9: AT91C_BASE_PIOA->PIO_CODR = PA_07_C|PA_06_B;                        AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_05_A;break;//1001 C9
    case 10:AT91C_BASE_PIOA->PIO_CODR = PA_07_C|PA_05_A;                        AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_06_B;break;//1010 C10
    case 11:AT91C_BASE_PIOA->PIO_CODR = PA_07_C;                                AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_06_B|PA_05_A;break;//1011 C11
    
    case 12:AT91C_BASE_PIOA->PIO_CODR = PA_06_B|PA_05_A;                        AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_07_C;break;//1100 C12
    case 13:AT91C_BASE_PIOA->PIO_CODR = PA_06_B;                                AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_07_C|PA_05_A;break;//1101 C13
    case 14:AT91C_BASE_PIOA->PIO_CODR = PA_05_A;                                AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_07_C|PA_06_B;break;//1110 C14
    case 15:                                                                    AT91C_BASE_PIOA->PIO_SODR = PA_08_D|PA_07_C|PA_06_B|PA_05_A;break;//1111 C15
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
