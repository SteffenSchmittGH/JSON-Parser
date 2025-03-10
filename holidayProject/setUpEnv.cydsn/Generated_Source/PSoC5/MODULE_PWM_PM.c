/*******************************************************************************
* File Name: MODULE_PWM_PM.c
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

#include "MODULE_PWM.h"

static MODULE_PWM_backupStruct MODULE_PWM_backup;


/*******************************************************************************
* Function Name: MODULE_PWM_SaveConfig
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
*  MODULE_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void MODULE_PWM_SaveConfig(void) 
{

    #if(!MODULE_PWM_UsingFixedFunction)
        #if(!MODULE_PWM_PWMModeIsCenterAligned)
            MODULE_PWM_backup.PWMPeriod = MODULE_PWM_ReadPeriod();
        #endif /* (!MODULE_PWM_PWMModeIsCenterAligned) */
        MODULE_PWM_backup.PWMUdb = MODULE_PWM_ReadCounter();
        #if (MODULE_PWM_UseStatus)
            MODULE_PWM_backup.InterruptMaskValue = MODULE_PWM_STATUS_MASK;
        #endif /* (MODULE_PWM_UseStatus) */

        #if(MODULE_PWM_DeadBandMode == MODULE_PWM__B_PWM__DBM_256_CLOCKS || \
            MODULE_PWM_DeadBandMode == MODULE_PWM__B_PWM__DBM_2_4_CLOCKS)
            MODULE_PWM_backup.PWMdeadBandValue = MODULE_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(MODULE_PWM_KillModeMinTime)
             MODULE_PWM_backup.PWMKillCounterPeriod = MODULE_PWM_ReadKillTime();
        #endif /* (MODULE_PWM_KillModeMinTime) */

        #if(MODULE_PWM_UseControl)
            MODULE_PWM_backup.PWMControlRegister = MODULE_PWM_ReadControlRegister();
        #endif /* (MODULE_PWM_UseControl) */
    #endif  /* (!MODULE_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MODULE_PWM_RestoreConfig
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
*  MODULE_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MODULE_PWM_RestoreConfig(void) 
{
        #if(!MODULE_PWM_UsingFixedFunction)
            #if(!MODULE_PWM_PWMModeIsCenterAligned)
                MODULE_PWM_WritePeriod(MODULE_PWM_backup.PWMPeriod);
            #endif /* (!MODULE_PWM_PWMModeIsCenterAligned) */

            MODULE_PWM_WriteCounter(MODULE_PWM_backup.PWMUdb);

            #if (MODULE_PWM_UseStatus)
                MODULE_PWM_STATUS_MASK = MODULE_PWM_backup.InterruptMaskValue;
            #endif /* (MODULE_PWM_UseStatus) */

            #if(MODULE_PWM_DeadBandMode == MODULE_PWM__B_PWM__DBM_256_CLOCKS || \
                MODULE_PWM_DeadBandMode == MODULE_PWM__B_PWM__DBM_2_4_CLOCKS)
                MODULE_PWM_WriteDeadTime(MODULE_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(MODULE_PWM_KillModeMinTime)
                MODULE_PWM_WriteKillTime(MODULE_PWM_backup.PWMKillCounterPeriod);
            #endif /* (MODULE_PWM_KillModeMinTime) */

            #if(MODULE_PWM_UseControl)
                MODULE_PWM_WriteControlRegister(MODULE_PWM_backup.PWMControlRegister);
            #endif /* (MODULE_PWM_UseControl) */
        #endif  /* (!MODULE_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: MODULE_PWM_Sleep
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
*  MODULE_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void MODULE_PWM_Sleep(void) 
{
    #if(MODULE_PWM_UseControl)
        if(MODULE_PWM_CTRL_ENABLE == (MODULE_PWM_CONTROL & MODULE_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            MODULE_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            MODULE_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (MODULE_PWM_UseControl) */

    /* Stop component */
    MODULE_PWM_Stop();

    /* Save registers configuration */
    MODULE_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: MODULE_PWM_Wakeup
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
*  MODULE_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MODULE_PWM_Wakeup(void) 
{
     /* Restore registers values */
    MODULE_PWM_RestoreConfig();

    if(MODULE_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        MODULE_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
