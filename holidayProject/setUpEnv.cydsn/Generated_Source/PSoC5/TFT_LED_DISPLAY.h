/*******************************************************************************
* File Name: TFT_LED_DISPLAY.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_TFT_LED_DISPLAY_H)
#define CY_PWM_TFT_LED_DISPLAY_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 TFT_LED_DISPLAY_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define TFT_LED_DISPLAY_Resolution                     (8u)
#define TFT_LED_DISPLAY_UsingFixedFunction             (0u)
#define TFT_LED_DISPLAY_DeadBandMode                   (0u)
#define TFT_LED_DISPLAY_KillModeMinTime                (0u)
#define TFT_LED_DISPLAY_KillMode                       (0u)
#define TFT_LED_DISPLAY_PWMMode                        (0u)
#define TFT_LED_DISPLAY_PWMModeIsCenterAligned         (0u)
#define TFT_LED_DISPLAY_DeadBandUsed                   (0u)
#define TFT_LED_DISPLAY_DeadBand2_4                    (0u)

#if !defined(TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__REMOVED)
    #define TFT_LED_DISPLAY_UseStatus                  (1u)
#else
    #define TFT_LED_DISPLAY_UseStatus                  (0u)
#endif /* !defined(TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(TFT_LED_DISPLAY_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define TFT_LED_DISPLAY_UseControl                 (1u)
#else
    #define TFT_LED_DISPLAY_UseControl                 (0u)
#endif /* !defined(TFT_LED_DISPLAY_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define TFT_LED_DISPLAY_UseOneCompareMode              (1u)
#define TFT_LED_DISPLAY_MinimumKillTime                (1u)
#define TFT_LED_DISPLAY_EnableMode                     (0u)

#define TFT_LED_DISPLAY_CompareMode1SW                 (0u)
#define TFT_LED_DISPLAY_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define TFT_LED_DISPLAY__B_PWM__DISABLED 0
#define TFT_LED_DISPLAY__B_PWM__ASYNCHRONOUS 1
#define TFT_LED_DISPLAY__B_PWM__SINGLECYCLE 2
#define TFT_LED_DISPLAY__B_PWM__LATCHED 3
#define TFT_LED_DISPLAY__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define TFT_LED_DISPLAY__B_PWM__DBMDISABLED 0
#define TFT_LED_DISPLAY__B_PWM__DBM_2_4_CLOCKS 1
#define TFT_LED_DISPLAY__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define TFT_LED_DISPLAY__B_PWM__ONE_OUTPUT 0
#define TFT_LED_DISPLAY__B_PWM__TWO_OUTPUTS 1
#define TFT_LED_DISPLAY__B_PWM__DUAL_EDGE 2
#define TFT_LED_DISPLAY__B_PWM__CENTER_ALIGN 3
#define TFT_LED_DISPLAY__B_PWM__DITHER 5
#define TFT_LED_DISPLAY__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define TFT_LED_DISPLAY__B_PWM__LESS_THAN 1
#define TFT_LED_DISPLAY__B_PWM__LESS_THAN_OR_EQUAL 2
#define TFT_LED_DISPLAY__B_PWM__GREATER_THAN 3
#define TFT_LED_DISPLAY__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define TFT_LED_DISPLAY__B_PWM__EQUAL 0
#define TFT_LED_DISPLAY__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!TFT_LED_DISPLAY_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!TFT_LED_DISPLAY_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!TFT_LED_DISPLAY_PWMModeIsCenterAligned) */
        #if (TFT_LED_DISPLAY_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (TFT_LED_DISPLAY_UseStatus) */

        /* Backup for Deadband parameters */
        #if(TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_256_CLOCKS || \
            TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(TFT_LED_DISPLAY_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (TFT_LED_DISPLAY_KillModeMinTime) */

        /* Backup control register */
        #if(TFT_LED_DISPLAY_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (TFT_LED_DISPLAY_UseControl) */

    #endif /* (!TFT_LED_DISPLAY_UsingFixedFunction) */

}TFT_LED_DISPLAY_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    TFT_LED_DISPLAY_Start(void) ;
void    TFT_LED_DISPLAY_Stop(void) ;

#if (TFT_LED_DISPLAY_UseStatus || TFT_LED_DISPLAY_UsingFixedFunction)
    void  TFT_LED_DISPLAY_SetInterruptMode(uint8 interruptMode) ;
    uint8 TFT_LED_DISPLAY_ReadStatusRegister(void) ;
#endif /* (TFT_LED_DISPLAY_UseStatus || TFT_LED_DISPLAY_UsingFixedFunction) */

#define TFT_LED_DISPLAY_GetInterruptSource() TFT_LED_DISPLAY_ReadStatusRegister()

#if (TFT_LED_DISPLAY_UseControl)
    uint8 TFT_LED_DISPLAY_ReadControlRegister(void) ;
    void  TFT_LED_DISPLAY_WriteControlRegister(uint8 control)
          ;
#endif /* (TFT_LED_DISPLAY_UseControl) */

#if (TFT_LED_DISPLAY_UseOneCompareMode)
   #if (TFT_LED_DISPLAY_CompareMode1SW)
       void    TFT_LED_DISPLAY_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (TFT_LED_DISPLAY_CompareMode1SW) */
#else
    #if (TFT_LED_DISPLAY_CompareMode1SW)
        void    TFT_LED_DISPLAY_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (TFT_LED_DISPLAY_CompareMode1SW) */
    #if (TFT_LED_DISPLAY_CompareMode2SW)
        void    TFT_LED_DISPLAY_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (TFT_LED_DISPLAY_CompareMode2SW) */
#endif /* (TFT_LED_DISPLAY_UseOneCompareMode) */

#if (!TFT_LED_DISPLAY_UsingFixedFunction)
    uint8   TFT_LED_DISPLAY_ReadCounter(void) ;
    uint8 TFT_LED_DISPLAY_ReadCapture(void) ;

    #if (TFT_LED_DISPLAY_UseStatus)
            void TFT_LED_DISPLAY_ClearFIFO(void) ;
    #endif /* (TFT_LED_DISPLAY_UseStatus) */

    void    TFT_LED_DISPLAY_WriteCounter(uint8 counter)
            ;
#endif /* (!TFT_LED_DISPLAY_UsingFixedFunction) */

void    TFT_LED_DISPLAY_WritePeriod(uint8 period)
        ;
uint8 TFT_LED_DISPLAY_ReadPeriod(void) ;

#if (TFT_LED_DISPLAY_UseOneCompareMode)
    void    TFT_LED_DISPLAY_WriteCompare(uint8 compare)
            ;
    uint8 TFT_LED_DISPLAY_ReadCompare(void) ;
#else
    void    TFT_LED_DISPLAY_WriteCompare1(uint8 compare)
            ;
    uint8 TFT_LED_DISPLAY_ReadCompare1(void) ;
    void    TFT_LED_DISPLAY_WriteCompare2(uint8 compare)
            ;
    uint8 TFT_LED_DISPLAY_ReadCompare2(void) ;
#endif /* (TFT_LED_DISPLAY_UseOneCompareMode) */


#if (TFT_LED_DISPLAY_DeadBandUsed)
    void    TFT_LED_DISPLAY_WriteDeadTime(uint8 deadtime) ;
    uint8   TFT_LED_DISPLAY_ReadDeadTime(void) ;
#endif /* (TFT_LED_DISPLAY_DeadBandUsed) */

#if ( TFT_LED_DISPLAY_KillModeMinTime)
    void TFT_LED_DISPLAY_WriteKillTime(uint8 killtime) ;
    uint8 TFT_LED_DISPLAY_ReadKillTime(void) ;
#endif /* ( TFT_LED_DISPLAY_KillModeMinTime) */

void TFT_LED_DISPLAY_Init(void) ;
void TFT_LED_DISPLAY_Enable(void) ;
void TFT_LED_DISPLAY_Sleep(void) ;
void TFT_LED_DISPLAY_Wakeup(void) ;
void TFT_LED_DISPLAY_SaveConfig(void) ;
void TFT_LED_DISPLAY_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define TFT_LED_DISPLAY_INIT_PERIOD_VALUE          (255u)
#define TFT_LED_DISPLAY_INIT_COMPARE_VALUE1        (127u)
#define TFT_LED_DISPLAY_INIT_COMPARE_VALUE2        (63u)
#define TFT_LED_DISPLAY_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    TFT_LED_DISPLAY_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    TFT_LED_DISPLAY_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    TFT_LED_DISPLAY_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    TFT_LED_DISPLAY_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define TFT_LED_DISPLAY_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  TFT_LED_DISPLAY_CTRL_CMPMODE2_SHIFT)
#define TFT_LED_DISPLAY_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  TFT_LED_DISPLAY_CTRL_CMPMODE1_SHIFT)
#define TFT_LED_DISPLAY_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (TFT_LED_DISPLAY_UsingFixedFunction)
   #define TFT_LED_DISPLAY_PERIOD_LSB              (*(reg16 *) TFT_LED_DISPLAY_PWMHW__PER0)
   #define TFT_LED_DISPLAY_PERIOD_LSB_PTR          ( (reg16 *) TFT_LED_DISPLAY_PWMHW__PER0)
   #define TFT_LED_DISPLAY_COMPARE1_LSB            (*(reg16 *) TFT_LED_DISPLAY_PWMHW__CNT_CMP0)
   #define TFT_LED_DISPLAY_COMPARE1_LSB_PTR        ( (reg16 *) TFT_LED_DISPLAY_PWMHW__CNT_CMP0)
   #define TFT_LED_DISPLAY_COMPARE2_LSB            (0x00u)
   #define TFT_LED_DISPLAY_COMPARE2_LSB_PTR        (0x00u)
   #define TFT_LED_DISPLAY_COUNTER_LSB             (*(reg16 *) TFT_LED_DISPLAY_PWMHW__CNT_CMP0)
   #define TFT_LED_DISPLAY_COUNTER_LSB_PTR         ( (reg16 *) TFT_LED_DISPLAY_PWMHW__CNT_CMP0)
   #define TFT_LED_DISPLAY_CAPTURE_LSB             (*(reg16 *) TFT_LED_DISPLAY_PWMHW__CAP0)
   #define TFT_LED_DISPLAY_CAPTURE_LSB_PTR         ( (reg16 *) TFT_LED_DISPLAY_PWMHW__CAP0)
   #define TFT_LED_DISPLAY_RT1                     (*(reg8 *)  TFT_LED_DISPLAY_PWMHW__RT1)
   #define TFT_LED_DISPLAY_RT1_PTR                 ( (reg8 *)  TFT_LED_DISPLAY_PWMHW__RT1)

#else
   #if (TFT_LED_DISPLAY_Resolution == 8u) /* 8bit - PWM */

       #if(TFT_LED_DISPLAY_PWMModeIsCenterAligned)
           #define TFT_LED_DISPLAY_PERIOD_LSB      (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define TFT_LED_DISPLAY_PERIOD_LSB_PTR  ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define TFT_LED_DISPLAY_PERIOD_LSB      (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define TFT_LED_DISPLAY_PERIOD_LSB_PTR  ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (TFT_LED_DISPLAY_PWMModeIsCenterAligned) */

       #define TFT_LED_DISPLAY_COMPARE1_LSB        (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define TFT_LED_DISPLAY_COMPARE1_LSB_PTR    ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define TFT_LED_DISPLAY_COMPARE2_LSB        (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define TFT_LED_DISPLAY_COMPARE2_LSB_PTR    ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define TFT_LED_DISPLAY_COUNTERCAP_LSB      (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define TFT_LED_DISPLAY_COUNTERCAP_LSB_PTR  ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define TFT_LED_DISPLAY_COUNTER_LSB         (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define TFT_LED_DISPLAY_COUNTER_LSB_PTR     ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define TFT_LED_DISPLAY_CAPTURE_LSB         (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define TFT_LED_DISPLAY_CAPTURE_LSB_PTR     ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(TFT_LED_DISPLAY_PWMModeIsCenterAligned)
               #define TFT_LED_DISPLAY_PERIOD_LSB      (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define TFT_LED_DISPLAY_PERIOD_LSB_PTR  ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define TFT_LED_DISPLAY_PERIOD_LSB      (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define TFT_LED_DISPLAY_PERIOD_LSB_PTR  ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (TFT_LED_DISPLAY_PWMModeIsCenterAligned) */

            #define TFT_LED_DISPLAY_COMPARE1_LSB       (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define TFT_LED_DISPLAY_COMPARE1_LSB_PTR   ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define TFT_LED_DISPLAY_COMPARE2_LSB       (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define TFT_LED_DISPLAY_COMPARE2_LSB_PTR   ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define TFT_LED_DISPLAY_COUNTERCAP_LSB     (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define TFT_LED_DISPLAY_COUNTERCAP_LSB_PTR ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define TFT_LED_DISPLAY_COUNTER_LSB        (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define TFT_LED_DISPLAY_COUNTER_LSB_PTR    ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define TFT_LED_DISPLAY_CAPTURE_LSB        (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define TFT_LED_DISPLAY_CAPTURE_LSB_PTR    ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(TFT_LED_DISPLAY_PWMModeIsCenterAligned)
               #define TFT_LED_DISPLAY_PERIOD_LSB      (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define TFT_LED_DISPLAY_PERIOD_LSB_PTR  ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define TFT_LED_DISPLAY_PERIOD_LSB      (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define TFT_LED_DISPLAY_PERIOD_LSB_PTR  ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (TFT_LED_DISPLAY_PWMModeIsCenterAligned) */

            #define TFT_LED_DISPLAY_COMPARE1_LSB       (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define TFT_LED_DISPLAY_COMPARE1_LSB_PTR   ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define TFT_LED_DISPLAY_COMPARE2_LSB       (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define TFT_LED_DISPLAY_COMPARE2_LSB_PTR   ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define TFT_LED_DISPLAY_COUNTERCAP_LSB     (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define TFT_LED_DISPLAY_COUNTERCAP_LSB_PTR ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define TFT_LED_DISPLAY_COUNTER_LSB        (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define TFT_LED_DISPLAY_COUNTER_LSB_PTR    ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define TFT_LED_DISPLAY_CAPTURE_LSB        (*(reg16 *) TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define TFT_LED_DISPLAY_CAPTURE_LSB_PTR    ((reg16 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define TFT_LED_DISPLAY_AUX_CONTROLDP1          (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define TFT_LED_DISPLAY_AUX_CONTROLDP1_PTR      ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (TFT_LED_DISPLAY_Resolution == 8) */

   #define TFT_LED_DISPLAY_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define TFT_LED_DISPLAY_AUX_CONTROLDP0          (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define TFT_LED_DISPLAY_AUX_CONTROLDP0_PTR      ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (TFT_LED_DISPLAY_UsingFixedFunction) */

#if(TFT_LED_DISPLAY_KillModeMinTime )
    #define TFT_LED_DISPLAY_KILLMODEMINTIME        (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define TFT_LED_DISPLAY_KILLMODEMINTIME_PTR    ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (TFT_LED_DISPLAY_KillModeMinTime ) */

#if(TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_256_CLOCKS)
    #define TFT_LED_DISPLAY_DEADBAND_COUNT         (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define TFT_LED_DISPLAY_DEADBAND_COUNT_PTR     ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define TFT_LED_DISPLAY_DEADBAND_LSB_PTR       ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define TFT_LED_DISPLAY_DEADBAND_LSB           (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (TFT_LED_DISPLAY_UsingFixedFunction)
        #define TFT_LED_DISPLAY_DEADBAND_COUNT         (*(reg8 *)  TFT_LED_DISPLAY_PWMHW__CFG0)
        #define TFT_LED_DISPLAY_DEADBAND_COUNT_PTR     ((reg8 *)   TFT_LED_DISPLAY_PWMHW__CFG0)
        #define TFT_LED_DISPLAY_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << TFT_LED_DISPLAY_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define TFT_LED_DISPLAY_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define TFT_LED_DISPLAY_DEADBAND_COUNT         (*(reg8 *)  TFT_LED_DISPLAY_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define TFT_LED_DISPLAY_DEADBAND_COUNT_PTR     ((reg8 *)   TFT_LED_DISPLAY_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define TFT_LED_DISPLAY_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << TFT_LED_DISPLAY_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define TFT_LED_DISPLAY_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (TFT_LED_DISPLAY_UsingFixedFunction) */
#endif /* (TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_256_CLOCKS) */



#if (TFT_LED_DISPLAY_UsingFixedFunction)
    #define TFT_LED_DISPLAY_STATUS                 (*(reg8 *) TFT_LED_DISPLAY_PWMHW__SR0)
    #define TFT_LED_DISPLAY_STATUS_PTR             ((reg8 *) TFT_LED_DISPLAY_PWMHW__SR0)
    #define TFT_LED_DISPLAY_STATUS_MASK            (*(reg8 *) TFT_LED_DISPLAY_PWMHW__SR0)
    #define TFT_LED_DISPLAY_STATUS_MASK_PTR        ((reg8 *) TFT_LED_DISPLAY_PWMHW__SR0)
    #define TFT_LED_DISPLAY_CONTROL                (*(reg8 *) TFT_LED_DISPLAY_PWMHW__CFG0)
    #define TFT_LED_DISPLAY_CONTROL_PTR            ((reg8 *) TFT_LED_DISPLAY_PWMHW__CFG0)
    #define TFT_LED_DISPLAY_CONTROL2               (*(reg8 *) TFT_LED_DISPLAY_PWMHW__CFG1)
    #define TFT_LED_DISPLAY_CONTROL3               (*(reg8 *) TFT_LED_DISPLAY_PWMHW__CFG2)
    #define TFT_LED_DISPLAY_GLOBAL_ENABLE          (*(reg8 *) TFT_LED_DISPLAY_PWMHW__PM_ACT_CFG)
    #define TFT_LED_DISPLAY_GLOBAL_ENABLE_PTR      ( (reg8 *) TFT_LED_DISPLAY_PWMHW__PM_ACT_CFG)
    #define TFT_LED_DISPLAY_GLOBAL_STBY_ENABLE     (*(reg8 *) TFT_LED_DISPLAY_PWMHW__PM_STBY_CFG)
    #define TFT_LED_DISPLAY_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) TFT_LED_DISPLAY_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define TFT_LED_DISPLAY_BLOCK_EN_MASK          (TFT_LED_DISPLAY_PWMHW__PM_ACT_MSK)
    #define TFT_LED_DISPLAY_BLOCK_STBY_EN_MASK     (TFT_LED_DISPLAY_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define TFT_LED_DISPLAY_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define TFT_LED_DISPLAY_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define TFT_LED_DISPLAY_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define TFT_LED_DISPLAY_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define TFT_LED_DISPLAY_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define TFT_LED_DISPLAY_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define TFT_LED_DISPLAY_CTRL_ENABLE            (uint8)((uint8)0x01u << TFT_LED_DISPLAY_CTRL_ENABLE_SHIFT)
    #define TFT_LED_DISPLAY_CTRL_RESET             (uint8)((uint8)0x01u << TFT_LED_DISPLAY_CTRL_RESET_SHIFT)
    #define TFT_LED_DISPLAY_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << TFT_LED_DISPLAY_CTRL_CMPMODE2_SHIFT)
    #define TFT_LED_DISPLAY_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << TFT_LED_DISPLAY_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TFT_LED_DISPLAY_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define TFT_LED_DISPLAY_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << TFT_LED_DISPLAY_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define TFT_LED_DISPLAY_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define TFT_LED_DISPLAY_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define TFT_LED_DISPLAY_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define TFT_LED_DISPLAY_STATUS_TC_INT_EN_MASK_SHIFT            (TFT_LED_DISPLAY_STATUS_TC_SHIFT - 4u)
    #define TFT_LED_DISPLAY_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define TFT_LED_DISPLAY_STATUS_CMP1_INT_EN_MASK_SHIFT          (TFT_LED_DISPLAY_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define TFT_LED_DISPLAY_STATUS_TC              (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_TC_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_CMP1            (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define TFT_LED_DISPLAY_STATUS_TC_INT_EN_MASK              (uint8)((uint8)TFT_LED_DISPLAY_STATUS_TC >> 4u)
    #define TFT_LED_DISPLAY_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)TFT_LED_DISPLAY_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define TFT_LED_DISPLAY_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define TFT_LED_DISPLAY_RT1_MASK              (uint8)((uint8)0x03u << TFT_LED_DISPLAY_RT1_SHIFT)
    #define TFT_LED_DISPLAY_SYNC                  (uint8)((uint8)0x03u << TFT_LED_DISPLAY_RT1_SHIFT)
    #define TFT_LED_DISPLAY_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define TFT_LED_DISPLAY_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << TFT_LED_DISPLAY_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define TFT_LED_DISPLAY_SYNCDSI_EN            (uint8)((uint8)0x0Fu << TFT_LED_DISPLAY_SYNCDSI_SHIFT)


#else
    #define TFT_LED_DISPLAY_STATUS                (*(reg8 *)   TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define TFT_LED_DISPLAY_STATUS_PTR            ((reg8 *)    TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define TFT_LED_DISPLAY_STATUS_MASK           (*(reg8 *)   TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__MASK_REG)
    #define TFT_LED_DISPLAY_STATUS_MASK_PTR       ((reg8 *)    TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__MASK_REG)
    #define TFT_LED_DISPLAY_STATUS_AUX_CTRL       (*(reg8 *)   TFT_LED_DISPLAY_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define TFT_LED_DISPLAY_CONTROL               (*(reg8 *)   TFT_LED_DISPLAY_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define TFT_LED_DISPLAY_CONTROL_PTR           ((reg8 *)    TFT_LED_DISPLAY_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define TFT_LED_DISPLAY_CTRL_ENABLE_SHIFT      (0x07u)
    #define TFT_LED_DISPLAY_CTRL_RESET_SHIFT       (0x06u)
    #define TFT_LED_DISPLAY_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define TFT_LED_DISPLAY_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define TFT_LED_DISPLAY_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define TFT_LED_DISPLAY_CTRL_ENABLE            (uint8)((uint8)0x01u << TFT_LED_DISPLAY_CTRL_ENABLE_SHIFT)
    #define TFT_LED_DISPLAY_CTRL_RESET             (uint8)((uint8)0x01u << TFT_LED_DISPLAY_CTRL_RESET_SHIFT)
    #define TFT_LED_DISPLAY_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << TFT_LED_DISPLAY_CTRL_CMPMODE2_SHIFT)
    #define TFT_LED_DISPLAY_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << TFT_LED_DISPLAY_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define TFT_LED_DISPLAY_STATUS_KILL_SHIFT          (0x05u)
    #define TFT_LED_DISPLAY_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define TFT_LED_DISPLAY_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define TFT_LED_DISPLAY_STATUS_TC_SHIFT            (0x02u)
    #define TFT_LED_DISPLAY_STATUS_CMP2_SHIFT          (0x01u)
    #define TFT_LED_DISPLAY_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define TFT_LED_DISPLAY_STATUS_KILL_INT_EN_MASK_SHIFT          (TFT_LED_DISPLAY_STATUS_KILL_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (TFT_LED_DISPLAY_STATUS_FIFONEMPTY_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (TFT_LED_DISPLAY_STATUS_FIFOFULL_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_TC_INT_EN_MASK_SHIFT            (TFT_LED_DISPLAY_STATUS_TC_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_CMP2_INT_EN_MASK_SHIFT          (TFT_LED_DISPLAY_STATUS_CMP2_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_CMP1_INT_EN_MASK_SHIFT          (TFT_LED_DISPLAY_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define TFT_LED_DISPLAY_STATUS_KILL            (uint8)((uint8)0x00u << TFT_LED_DISPLAY_STATUS_KILL_SHIFT )
    #define TFT_LED_DISPLAY_STATUS_FIFOFULL        (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_FIFOFULL_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_FIFONEMPTY_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_TC              (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_TC_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_CMP2            (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_CMP2_SHIFT)
    #define TFT_LED_DISPLAY_STATUS_CMP1            (uint8)((uint8)0x01u << TFT_LED_DISPLAY_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define TFT_LED_DISPLAY_STATUS_KILL_INT_EN_MASK            (TFT_LED_DISPLAY_STATUS_KILL)
    #define TFT_LED_DISPLAY_STATUS_FIFOFULL_INT_EN_MASK        (TFT_LED_DISPLAY_STATUS_FIFOFULL)
    #define TFT_LED_DISPLAY_STATUS_FIFONEMPTY_INT_EN_MASK      (TFT_LED_DISPLAY_STATUS_FIFONEMPTY)
    #define TFT_LED_DISPLAY_STATUS_TC_INT_EN_MASK              (TFT_LED_DISPLAY_STATUS_TC)
    #define TFT_LED_DISPLAY_STATUS_CMP2_INT_EN_MASK            (TFT_LED_DISPLAY_STATUS_CMP2)
    #define TFT_LED_DISPLAY_STATUS_CMP1_INT_EN_MASK            (TFT_LED_DISPLAY_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define TFT_LED_DISPLAY_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define TFT_LED_DISPLAY_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define TFT_LED_DISPLAY_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define TFT_LED_DISPLAY_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define TFT_LED_DISPLAY_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* TFT_LED_DISPLAY_UsingFixedFunction */

#endif  /* CY_PWM_TFT_LED_DISPLAY_H */


/* [] END OF FILE */
