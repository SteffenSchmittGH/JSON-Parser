/*******************************************************************************
* File Name: MODULE_PWM.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "MODULE_PWM.h"

/* Error message for removed <resource> through optimization */
#ifdef MODULE_PWM_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* MODULE_PWM_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 MODULE_PWM_initVar = 0u;


/*******************************************************************************
* Function Name: MODULE_PWM_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  MODULE_PWM_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void MODULE_PWM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(MODULE_PWM_initVar == 0u)
    {
        MODULE_PWM_Init();
        MODULE_PWM_initVar = 1u;
    }
    MODULE_PWM_Enable();

}


/*******************************************************************************
* Function Name: MODULE_PWM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  MODULE_PWM_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MODULE_PWM_Init(void) 
{
    #if (MODULE_PWM_UsingFixedFunction || MODULE_PWM_UseControl)
        uint8 ctrl;
    #endif /* (MODULE_PWM_UsingFixedFunction || MODULE_PWM_UseControl) */

    #if(!MODULE_PWM_UsingFixedFunction)
        #if(MODULE_PWM_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 MODULE_PWM_interruptState;
        #endif /* (MODULE_PWM_UseStatus) */
    #endif /* (!MODULE_PWM_UsingFixedFunction) */

    #if (MODULE_PWM_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        MODULE_PWM_CONTROL |= MODULE_PWM_CFG0_MODE;
        #if (MODULE_PWM_DeadBand2_4)
            MODULE_PWM_CONTROL |= MODULE_PWM_CFG0_DB;
        #endif /* (MODULE_PWM_DeadBand2_4) */

        ctrl = MODULE_PWM_CONTROL3 & ((uint8 )(~MODULE_PWM_CTRL_CMPMODE1_MASK));
        MODULE_PWM_CONTROL3 = ctrl | MODULE_PWM_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        MODULE_PWM_RT1 &= ((uint8)(~MODULE_PWM_RT1_MASK));
        MODULE_PWM_RT1 |= MODULE_PWM_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        MODULE_PWM_RT1 &= ((uint8)(~MODULE_PWM_SYNCDSI_MASK));
        MODULE_PWM_RT1 |= MODULE_PWM_SYNCDSI_EN;

    #elif (MODULE_PWM_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = MODULE_PWM_CONTROL & ((uint8)(~MODULE_PWM_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~MODULE_PWM_CTRL_CMPMODE1_MASK));
        MODULE_PWM_CONTROL = ctrl | MODULE_PWM_DEFAULT_COMPARE2_MODE |
                                   MODULE_PWM_DEFAULT_COMPARE1_MODE;
    #endif /* (MODULE_PWM_UsingFixedFunction) */

    #if (!MODULE_PWM_UsingFixedFunction)
        #if (MODULE_PWM_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            MODULE_PWM_AUX_CONTROLDP0 |= (MODULE_PWM_AUX_CTRL_FIFO0_CLR);
        #else /* (MODULE_PWM_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            MODULE_PWM_AUX_CONTROLDP0 |= (MODULE_PWM_AUX_CTRL_FIFO0_CLR);
            MODULE_PWM_AUX_CONTROLDP1 |= (MODULE_PWM_AUX_CTRL_FIFO0_CLR);
        #endif /* (MODULE_PWM_Resolution == 8) */

        MODULE_PWM_WriteCounter(MODULE_PWM_INIT_PERIOD_VALUE);
    #endif /* (!MODULE_PWM_UsingFixedFunction) */

    MODULE_PWM_WritePeriod(MODULE_PWM_INIT_PERIOD_VALUE);

        #if (MODULE_PWM_UseOneCompareMode)
            MODULE_PWM_WriteCompare(MODULE_PWM_INIT_COMPARE_VALUE1);
        #else
            MODULE_PWM_WriteCompare1(MODULE_PWM_INIT_COMPARE_VALUE1);
            MODULE_PWM_WriteCompare2(MODULE_PWM_INIT_COMPARE_VALUE2);
        #endif /* (MODULE_PWM_UseOneCompareMode) */

        #if (MODULE_PWM_KillModeMinTime)
            MODULE_PWM_WriteKillTime(MODULE_PWM_MinimumKillTime);
        #endif /* (MODULE_PWM_KillModeMinTime) */

        #if (MODULE_PWM_DeadBandUsed)
            MODULE_PWM_WriteDeadTime(MODULE_PWM_INIT_DEAD_TIME);
        #endif /* (MODULE_PWM_DeadBandUsed) */

    #if (MODULE_PWM_UseStatus || MODULE_PWM_UsingFixedFunction)
        MODULE_PWM_SetInterruptMode(MODULE_PWM_INIT_INTERRUPTS_MODE);
    #endif /* (MODULE_PWM_UseStatus || MODULE_PWM_UsingFixedFunction) */

    #if (MODULE_PWM_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        MODULE_PWM_GLOBAL_ENABLE |= MODULE_PWM_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        MODULE_PWM_CONTROL2 |= MODULE_PWM_CTRL2_IRQ_SEL;
    #else
        #if(MODULE_PWM_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            MODULE_PWM_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            MODULE_PWM_STATUS_AUX_CTRL |= MODULE_PWM_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(MODULE_PWM_interruptState);

            /* Clear the FIFO to enable the MODULE_PWM_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            MODULE_PWM_ClearFIFO();
        #endif /* (MODULE_PWM_UseStatus) */
    #endif /* (MODULE_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MODULE_PWM_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void MODULE_PWM_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (MODULE_PWM_UsingFixedFunction)
        MODULE_PWM_GLOBAL_ENABLE |= MODULE_PWM_BLOCK_EN_MASK;
        MODULE_PWM_GLOBAL_STBY_ENABLE |= MODULE_PWM_BLOCK_STBY_EN_MASK;
    #endif /* (MODULE_PWM_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (MODULE_PWM_UseControl || MODULE_PWM_UsingFixedFunction)
        MODULE_PWM_CONTROL |= MODULE_PWM_CTRL_ENABLE;
    #endif /* (MODULE_PWM_UseControl || MODULE_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MODULE_PWM_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void MODULE_PWM_Stop(void) 
{
    #if (MODULE_PWM_UseControl || MODULE_PWM_UsingFixedFunction)
        MODULE_PWM_CONTROL &= ((uint8)(~MODULE_PWM_CTRL_ENABLE));
    #endif /* (MODULE_PWM_UseControl || MODULE_PWM_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (MODULE_PWM_UsingFixedFunction)
        MODULE_PWM_GLOBAL_ENABLE &= ((uint8)(~MODULE_PWM_BLOCK_EN_MASK));
        MODULE_PWM_GLOBAL_STBY_ENABLE &= ((uint8)(~MODULE_PWM_BLOCK_STBY_EN_MASK));
    #endif /* (MODULE_PWM_UsingFixedFunction) */
}

#if (MODULE_PWM_UseOneCompareMode)
    #if (MODULE_PWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: MODULE_PWM_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void MODULE_PWM_SetCompareMode(uint8 comparemode) 
        {
            #if(MODULE_PWM_UsingFixedFunction)

                #if(0 != MODULE_PWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << MODULE_PWM_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != MODULE_PWM_CTRL_CMPMODE1_SHIFT) */

                MODULE_PWM_CONTROL3 &= ((uint8)(~MODULE_PWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                MODULE_PWM_CONTROL3 |= comparemodemasked;

            #elif (MODULE_PWM_UseControl)

                #if(0 != MODULE_PWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << MODULE_PWM_CTRL_CMPMODE1_SHIFT)) &
                                                MODULE_PWM_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & MODULE_PWM_CTRL_CMPMODE1_MASK;
                #endif /* (0 != MODULE_PWM_CTRL_CMPMODE1_SHIFT) */

                #if(0 != MODULE_PWM_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << MODULE_PWM_CTRL_CMPMODE2_SHIFT)) &
                                               MODULE_PWM_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & MODULE_PWM_CTRL_CMPMODE2_MASK;
                #endif /* (0 != MODULE_PWM_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                MODULE_PWM_CONTROL &= ((uint8)(~(MODULE_PWM_CTRL_CMPMODE1_MASK |
                                            MODULE_PWM_CTRL_CMPMODE2_MASK)));
                MODULE_PWM_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (MODULE_PWM_UsingFixedFunction) */
        }
    #endif /* MODULE_PWM_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (MODULE_PWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: MODULE_PWM_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void MODULE_PWM_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != MODULE_PWM_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << MODULE_PWM_CTRL_CMPMODE1_SHIFT)) &
                                           MODULE_PWM_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & MODULE_PWM_CTRL_CMPMODE1_MASK;
            #endif /* (0 != MODULE_PWM_CTRL_CMPMODE1_SHIFT) */

            #if (MODULE_PWM_UseControl)
                MODULE_PWM_CONTROL &= ((uint8)(~MODULE_PWM_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                MODULE_PWM_CONTROL |= comparemodemasked;
            #endif /* (MODULE_PWM_UseControl) */
        }
    #endif /* MODULE_PWM_CompareMode1SW */

#if (MODULE_PWM_CompareMode2SW)


    /*******************************************************************************
    * Function Name: MODULE_PWM_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != MODULE_PWM_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << MODULE_PWM_CTRL_CMPMODE2_SHIFT)) &
                                                 MODULE_PWM_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & MODULE_PWM_CTRL_CMPMODE2_MASK;
        #endif /* (0 != MODULE_PWM_CTRL_CMPMODE2_SHIFT) */

        #if (MODULE_PWM_UseControl)
            MODULE_PWM_CONTROL &= ((uint8)(~MODULE_PWM_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            MODULE_PWM_CONTROL |= comparemodemasked;
        #endif /* (MODULE_PWM_UseControl) */
    }
    #endif /*MODULE_PWM_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!MODULE_PWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void MODULE_PWM_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(MODULE_PWM_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(MODULE_PWM_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(MODULE_PWM_CAPTURE_LSB_PTR));
    }

    #if (MODULE_PWM_UseStatus)


        /*******************************************************************************
        * Function Name: MODULE_PWM_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void MODULE_PWM_ClearFIFO(void) 
        {
            while(0u != (MODULE_PWM_ReadStatusRegister() & MODULE_PWM_STATUS_FIFONEMPTY))
            {
                (void)MODULE_PWM_ReadCapture();
            }
        }

    #endif /* MODULE_PWM_UseStatus */

#endif /* !MODULE_PWM_UsingFixedFunction */


/*******************************************************************************
* Function Name: MODULE_PWM_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void MODULE_PWM_WritePeriod(uint8 period) 
{
    #if(MODULE_PWM_UsingFixedFunction)
        CY_SET_REG16(MODULE_PWM_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(MODULE_PWM_PERIOD_LSB_PTR, period);
    #endif /* (MODULE_PWM_UsingFixedFunction) */
}

#if (MODULE_PWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void MODULE_PWM_WriteCompare(uint8 compare) \
                                       
    {
        #if(MODULE_PWM_UsingFixedFunction)
            CY_SET_REG16(MODULE_PWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(MODULE_PWM_COMPARE1_LSB_PTR, compare);
        #endif /* (MODULE_PWM_UsingFixedFunction) */

        #if (MODULE_PWM_PWMMode == MODULE_PWM__B_PWM__DITHER)
            #if(MODULE_PWM_UsingFixedFunction)
                CY_SET_REG16(MODULE_PWM_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(MODULE_PWM_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (MODULE_PWM_UsingFixedFunction) */
        #endif /* (MODULE_PWM_PWMMode == MODULE_PWM__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_WriteCompare1(uint8 compare) \
                                        
    {
        #if(MODULE_PWM_UsingFixedFunction)
            CY_SET_REG16(MODULE_PWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(MODULE_PWM_COMPARE1_LSB_PTR, compare);
        #endif /* (MODULE_PWM_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_WriteCompare2(uint8 compare) \
                                        
    {
        #if(MODULE_PWM_UsingFixedFunction)
            CY_SET_REG16(MODULE_PWM_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(MODULE_PWM_COMPARE2_LSB_PTR, compare);
        #endif /* (MODULE_PWM_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (MODULE_PWM_DeadBandUsed)


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!MODULE_PWM_DeadBand2_4)
            CY_SET_REG8(MODULE_PWM_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            MODULE_PWM_DEADBAND_COUNT &= ((uint8)(~MODULE_PWM_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(MODULE_PWM_DEADBAND_COUNT_SHIFT)
                MODULE_PWM_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << MODULE_PWM_DEADBAND_COUNT_SHIFT)) &
                                                    MODULE_PWM_DEADBAND_COUNT_MASK;
            #else
                MODULE_PWM_DEADBAND_COUNT |= deadtime & MODULE_PWM_DEADBAND_COUNT_MASK;
            #endif /* (MODULE_PWM_DEADBAND_COUNT_SHIFT) */

        #endif /* (!MODULE_PWM_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!MODULE_PWM_DeadBand2_4)
            return (CY_GET_REG8(MODULE_PWM_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(MODULE_PWM_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(MODULE_PWM_DEADBAND_COUNT & MODULE_PWM_DEADBAND_COUNT_MASK)) >>
                                                                           MODULE_PWM_DEADBAND_COUNT_SHIFT));
            #else
                return (MODULE_PWM_DEADBAND_COUNT & MODULE_PWM_DEADBAND_COUNT_MASK);
            #endif /* (MODULE_PWM_DEADBAND_COUNT_SHIFT) */
        #endif /* (!MODULE_PWM_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (MODULE_PWM_UseStatus || MODULE_PWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: MODULE_PWM_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(MODULE_PWM_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(MODULE_PWM_STATUS_PTR));
    }

#endif /* (MODULE_PWM_UseStatus || MODULE_PWM_UsingFixedFunction) */


#if (MODULE_PWM_UseControl)


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(MODULE_PWM_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(MODULE_PWM_CONTROL_PTR, control);
    }

#endif /* (MODULE_PWM_UseControl) */


#if (!MODULE_PWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadCapture(void) 
    {
        return (CY_GET_REG8(MODULE_PWM_CAPTURE_LSB_PTR));
    }

#endif /* (!MODULE_PWM_UsingFixedFunction) */


#if (MODULE_PWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadCompare(void) 
    {
        #if(MODULE_PWM_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(MODULE_PWM_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(MODULE_PWM_COMPARE1_LSB_PTR));
        #endif /* (MODULE_PWM_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadCompare1(void) 
    {
        return (CY_GET_REG8(MODULE_PWM_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadCompare2(void) 
    {
        return (CY_GET_REG8(MODULE_PWM_COMPARE2_LSB_PTR));
    }

#endif /* (MODULE_PWM_UseOneCompareMode) */


/*******************************************************************************
* Function Name: MODULE_PWM_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 MODULE_PWM_ReadPeriod(void) 
{
    #if(MODULE_PWM_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(MODULE_PWM_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(MODULE_PWM_PERIOD_LSB_PTR));
    #endif /* (MODULE_PWM_UsingFixedFunction) */
}

#if ( MODULE_PWM_KillModeMinTime)


    /*******************************************************************************
    * Function Name: MODULE_PWM_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void MODULE_PWM_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(MODULE_PWM_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: MODULE_PWM_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 MODULE_PWM_ReadKillTime(void) 
    {
        return (CY_GET_REG8(MODULE_PWM_KILLMODEMINTIME_PTR));
    }

#endif /* ( MODULE_PWM_KillModeMinTime) */

/* [] END OF FILE */
