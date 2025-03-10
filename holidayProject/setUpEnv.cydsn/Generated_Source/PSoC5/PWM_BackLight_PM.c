/*******************************************************************************
* File Name: PWM_BackLight_PM.c
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

#include "PWM_BackLight.h"

static PWM_BackLight_backupStruct PWM_BackLight_backup;


/*******************************************************************************
* Function Name: PWM_BackLight_SaveConfig
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
*  PWM_BackLight_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_BackLight_SaveConfig(void) 
{

    #if(!PWM_BackLight_UsingFixedFunction)
        #if(!PWM_BackLight_PWMModeIsCenterAligned)
            PWM_BackLight_backup.PWMPeriod = PWM_BackLight_ReadPeriod();
        #endif /* (!PWM_BackLight_PWMModeIsCenterAligned) */
        PWM_BackLight_backup.PWMUdb = PWM_BackLight_ReadCounter();
        #if (PWM_BackLight_UseStatus)
            PWM_BackLight_backup.InterruptMaskValue = PWM_BackLight_STATUS_MASK;
        #endif /* (PWM_BackLight_UseStatus) */

        #if(PWM_BackLight_DeadBandMode == PWM_BackLight__B_PWM__DBM_256_CLOCKS || \
            PWM_BackLight_DeadBandMode == PWM_BackLight__B_PWM__DBM_2_4_CLOCKS)
            PWM_BackLight_backup.PWMdeadBandValue = PWM_BackLight_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_BackLight_KillModeMinTime)
             PWM_BackLight_backup.PWMKillCounterPeriod = PWM_BackLight_ReadKillTime();
        #endif /* (PWM_BackLight_KillModeMinTime) */

        #if(PWM_BackLight_UseControl)
            PWM_BackLight_backup.PWMControlRegister = PWM_BackLight_ReadControlRegister();
        #endif /* (PWM_BackLight_UseControl) */
    #endif  /* (!PWM_BackLight_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_BackLight_RestoreConfig
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
*  PWM_BackLight_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_BackLight_RestoreConfig(void) 
{
        #if(!PWM_BackLight_UsingFixedFunction)
            #if(!PWM_BackLight_PWMModeIsCenterAligned)
                PWM_BackLight_WritePeriod(PWM_BackLight_backup.PWMPeriod);
            #endif /* (!PWM_BackLight_PWMModeIsCenterAligned) */

            PWM_BackLight_WriteCounter(PWM_BackLight_backup.PWMUdb);

            #if (PWM_BackLight_UseStatus)
                PWM_BackLight_STATUS_MASK = PWM_BackLight_backup.InterruptMaskValue;
            #endif /* (PWM_BackLight_UseStatus) */

            #if(PWM_BackLight_DeadBandMode == PWM_BackLight__B_PWM__DBM_256_CLOCKS || \
                PWM_BackLight_DeadBandMode == PWM_BackLight__B_PWM__DBM_2_4_CLOCKS)
                PWM_BackLight_WriteDeadTime(PWM_BackLight_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_BackLight_KillModeMinTime)
                PWM_BackLight_WriteKillTime(PWM_BackLight_backup.PWMKillCounterPeriod);
            #endif /* (PWM_BackLight_KillModeMinTime) */

            #if(PWM_BackLight_UseControl)
                PWM_BackLight_WriteControlRegister(PWM_BackLight_backup.PWMControlRegister);
            #endif /* (PWM_BackLight_UseControl) */
        #endif  /* (!PWM_BackLight_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_BackLight_Sleep
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
*  PWM_BackLight_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_BackLight_Sleep(void) 
{
    #if(PWM_BackLight_UseControl)
        if(PWM_BackLight_CTRL_ENABLE == (PWM_BackLight_CONTROL & PWM_BackLight_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_BackLight_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_BackLight_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_BackLight_UseControl) */

    /* Stop component */
    PWM_BackLight_Stop();

    /* Save registers configuration */
    PWM_BackLight_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_BackLight_Wakeup
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
*  PWM_BackLight_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_BackLight_Wakeup(void) 
{
     /* Restore registers values */
    PWM_BackLight_RestoreConfig();

    if(PWM_BackLight_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_BackLight_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
