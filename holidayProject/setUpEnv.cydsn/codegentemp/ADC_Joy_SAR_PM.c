/*******************************************************************************
* File Name: ADC_Joy_SAR_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_Joy_SAR.h"


/***************************************
* Local data allocation
***************************************/

static ADC_Joy_SAR_BACKUP_STRUCT  ADC_Joy_SAR_backup =
{
    ADC_Joy_SAR_DISABLED
};


/*******************************************************************************
* Function Name: ADC_Joy_SAR_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_Joy_SAR_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_Joy_SAR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_Joy_SAR_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_Joy_SAR_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_Joy_SAR_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_Joy_SAR_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_Joy_SAR_Sleep(void)
{
    if((ADC_Joy_SAR_PWRMGR_SAR_REG  & ADC_Joy_SAR_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_Joy_SAR_SAR_CSR0_REG & ADC_Joy_SAR_SAR_SOF_START_CONV) != 0u)
        {
            ADC_Joy_SAR_backup.enableState = ADC_Joy_SAR_ENABLED | ADC_Joy_SAR_STARTED;
        }
        else
        {
            ADC_Joy_SAR_backup.enableState = ADC_Joy_SAR_ENABLED;
        }
        ADC_Joy_SAR_Stop();
    }
    else
    {
        ADC_Joy_SAR_backup.enableState = ADC_Joy_SAR_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_Joy_SAR_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_Joy_SAR_Sleep() was called. If the component was enabled before the
*  ADC_Joy_SAR_Sleep() function was called, the
*  ADC_Joy_SAR_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_Joy_SAR_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_Joy_SAR_Wakeup(void)
{
    if(ADC_Joy_SAR_backup.enableState != ADC_Joy_SAR_DISABLED)
    {
        ADC_Joy_SAR_Enable();
        #if(ADC_Joy_SAR_DEFAULT_CONV_MODE != ADC_Joy_SAR__HARDWARE_TRIGGER)
            if((ADC_Joy_SAR_backup.enableState & ADC_Joy_SAR_STARTED) != 0u)
            {
                ADC_Joy_SAR_StartConvert();
            }
        #endif /* End ADC_Joy_SAR_DEFAULT_CONV_MODE != ADC_Joy_SAR__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
