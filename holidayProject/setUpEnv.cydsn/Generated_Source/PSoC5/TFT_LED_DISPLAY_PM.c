/*******************************************************************************
* File Name: TFT_LED_DISPLAY_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TFT_LED_DISPLAY.h"

static TFT_LED_DISPLAY_backupStruct TFT_LED_DISPLAY_backup;


/*******************************************************************************
* Function Name: TFT_LED_DISPLAY_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_LED_DISPLAY_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TFT_LED_DISPLAY_SaveConfig(void) 
{

    #if(!TFT_LED_DISPLAY_UsingFixedFunction)
        #if(!TFT_LED_DISPLAY_PWMModeIsCenterAligned)
            TFT_LED_DISPLAY_backup.PWMPeriod = TFT_LED_DISPLAY_ReadPeriod();
        #endif /* (!TFT_LED_DISPLAY_PWMModeIsCenterAligned) */
        TFT_LED_DISPLAY_backup.PWMUdb = TFT_LED_DISPLAY_ReadCounter();
        #if (TFT_LED_DISPLAY_UseStatus)
            TFT_LED_DISPLAY_backup.InterruptMaskValue = TFT_LED_DISPLAY_STATUS_MASK;
        #endif /* (TFT_LED_DISPLAY_UseStatus) */

        #if(TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_256_CLOCKS || \
            TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_2_4_CLOCKS)
            TFT_LED_DISPLAY_backup.PWMdeadBandValue = TFT_LED_DISPLAY_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(TFT_LED_DISPLAY_KillModeMinTime)
             TFT_LED_DISPLAY_backup.PWMKillCounterPeriod = TFT_LED_DISPLAY_ReadKillTime();
        #endif /* (TFT_LED_DISPLAY_KillModeMinTime) */

        #if(TFT_LED_DISPLAY_UseControl)
            TFT_LED_DISPLAY_backup.PWMControlRegister = TFT_LED_DISPLAY_ReadControlRegister();
        #endif /* (TFT_LED_DISPLAY_UseControl) */
    #endif  /* (!TFT_LED_DISPLAY_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: TFT_LED_DISPLAY_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_LED_DISPLAY_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TFT_LED_DISPLAY_RestoreConfig(void) 
{
        #if(!TFT_LED_DISPLAY_UsingFixedFunction)
            #if(!TFT_LED_DISPLAY_PWMModeIsCenterAligned)
                TFT_LED_DISPLAY_WritePeriod(TFT_LED_DISPLAY_backup.PWMPeriod);
            #endif /* (!TFT_LED_DISPLAY_PWMModeIsCenterAligned) */

            TFT_LED_DISPLAY_WriteCounter(TFT_LED_DISPLAY_backup.PWMUdb);

            #if (TFT_LED_DISPLAY_UseStatus)
                TFT_LED_DISPLAY_STATUS_MASK = TFT_LED_DISPLAY_backup.InterruptMaskValue;
            #endif /* (TFT_LED_DISPLAY_UseStatus) */

            #if(TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_256_CLOCKS || \
                TFT_LED_DISPLAY_DeadBandMode == TFT_LED_DISPLAY__B_PWM__DBM_2_4_CLOCKS)
                TFT_LED_DISPLAY_WriteDeadTime(TFT_LED_DISPLAY_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(TFT_LED_DISPLAY_KillModeMinTime)
                TFT_LED_DISPLAY_WriteKillTime(TFT_LED_DISPLAY_backup.PWMKillCounterPeriod);
            #endif /* (TFT_LED_DISPLAY_KillModeMinTime) */

            #if(TFT_LED_DISPLAY_UseControl)
                TFT_LED_DISPLAY_WriteControlRegister(TFT_LED_DISPLAY_backup.PWMControlRegister);
            #endif /* (TFT_LED_DISPLAY_UseControl) */
        #endif  /* (!TFT_LED_DISPLAY_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: TFT_LED_DISPLAY_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_LED_DISPLAY_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void TFT_LED_DISPLAY_Sleep(void) 
{
    #if(TFT_LED_DISPLAY_UseControl)
        if(TFT_LED_DISPLAY_CTRL_ENABLE == (TFT_LED_DISPLAY_CONTROL & TFT_LED_DISPLAY_CTRL_ENABLE))
        {
            /*Component is enabled */
            TFT_LED_DISPLAY_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            TFT_LED_DISPLAY_backup.PWMEnableState = 0u;
        }
    #endif /* (TFT_LED_DISPLAY_UseControl) */

    /* Stop component */
    TFT_LED_DISPLAY_Stop();

    /* Save registers configuration */
    TFT_LED_DISPLAY_SaveConfig();
}


/*******************************************************************************
* Function Name: TFT_LED_DISPLAY_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_LED_DISPLAY_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TFT_LED_DISPLAY_Wakeup(void) 
{
     /* Restore registers values */
    TFT_LED_DISPLAY_RestoreConfig();

    if(TFT_LED_DISPLAY_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        TFT_LED_DISPLAY_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
