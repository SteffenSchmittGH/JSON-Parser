/*******************************************************************************
* File Name: SEVEN_CONTROL_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "SEVEN_CONTROL.h"

/* Check for removal by optimization */
#if !defined(SEVEN_CONTROL_Sync_ctrl_reg__REMOVED)

static SEVEN_CONTROL_BACKUP_STRUCT  SEVEN_CONTROL_backup = {0u};

    
/*******************************************************************************
* Function Name: SEVEN_CONTROL_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SEVEN_CONTROL_SaveConfig(void) 
{
    SEVEN_CONTROL_backup.controlState = SEVEN_CONTROL_Control;
}


/*******************************************************************************
* Function Name: SEVEN_CONTROL_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void SEVEN_CONTROL_RestoreConfig(void) 
{
     SEVEN_CONTROL_Control = SEVEN_CONTROL_backup.controlState;
}


/*******************************************************************************
* Function Name: SEVEN_CONTROL_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SEVEN_CONTROL_Sleep(void) 
{
    SEVEN_CONTROL_SaveConfig();
}


/*******************************************************************************
* Function Name: SEVEN_CONTROL_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SEVEN_CONTROL_Wakeup(void)  
{
    SEVEN_CONTROL_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
