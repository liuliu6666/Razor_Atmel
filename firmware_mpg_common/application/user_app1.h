/**********************************************************************************************************************
File: user_app.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
1. Follow the instructions at the top of user_app.c
2. Use ctrl-h to find and replace all instances of "user_app" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for yournewtaskname.c

**********************************************************************************************************************/

#ifndef __USER_APP_H
#define __USER_APP_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Required constants for ANT channel configuration */

#define ANT_CHANNEL_TYPE_USERAPP_CHANNEL1        CHANNEL_TYPE_SLAVE    // Device is a MASTER
#define ANT_CHANNEL_USERAPP_CHANNEL1             ANT_CHANNEL_1         // Channel 0 - 7
#define ANT_DEVICEID_LO_USERAPP_CHANNEL1         (u8)0x01              // Low byte of two-byte Device #
#define ANT_DEVICEID_HI_USERAPP_CHANNEL1         (u8)0x00              // High byte of two-byte Device #
#define ANT_DEVICE_TYPE_USERAPP_CHANNEL1         (u8)120               // 1 - 255
#define ANT_TRANSMISSION_TYPE_USERAPP_CHANNEL1   (u8)1                 // 1-127 (MSB is pairing bit)
#define ANT_CHANNEL_PERIOD_LO_USERAPP_CHANNEL1   (u8)0x86              // Low byte of two-byte channel period 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP_CHANNEL1   (u8)0x1F              // High byte of two-byte channel period 
#define ANT_FREQUENCY_USERAPP_CHANNEL1           (u8)57                // 2400MHz + this number 0 - 99
#define ANT_TX_POWER_USERAPP_CHANNEL1            RADIO_TX_POWER_4DBM   // RADIO_TX_POWER_0DBM, RADIO_TX_POWER_MINUS5DBM, RADIO_TX_POWER_MINUS10DBM, RADIO_TX_POWER_MINUS20DBM


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_AntChannel1Assign(void);
static void UserApp1SM_ModeChoose(void);
static void UserApp1SM_Disconnect(void);
static void UserApp1SM_Close(void);
static void UserApp1SM_Function1(void);
static void UserApp1SM_Function2(void);
static void ShowRate(void);  

static void UserApp1SM_Error(void); 
#endif /* __USER_APP_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
