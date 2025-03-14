/*******************************************************************************
* File Name: clk1.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_clk1_H)
#define CY_CLOCK_clk1_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void clk1_Start(void) ;
void clk1_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void clk1_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void clk1_StandbyPower(uint8 state) ;
void clk1_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 clk1_GetDividerRegister(void) ;
void clk1_SetModeRegister(uint8 modeBitMask) ;
void clk1_ClearModeRegister(uint8 modeBitMask) ;
uint8 clk1_GetModeRegister(void) ;
void clk1_SetSourceRegister(uint8 clkSource) ;
uint8 clk1_GetSourceRegister(void) ;
#if defined(clk1__CFG3)
void clk1_SetPhaseRegister(uint8 clkPhase) ;
uint8 clk1_GetPhaseRegister(void) ;
#endif /* defined(clk1__CFG3) */

#define clk1_Enable()                       clk1_Start()
#define clk1_Disable()                      clk1_Stop()
#define clk1_SetDivider(clkDivider)         clk1_SetDividerRegister(clkDivider, 1u)
#define clk1_SetDividerValue(clkDivider)    clk1_SetDividerRegister((clkDivider) - 1u, 1u)
#define clk1_SetMode(clkMode)               clk1_SetModeRegister(clkMode)
#define clk1_SetSource(clkSource)           clk1_SetSourceRegister(clkSource)
#if defined(clk1__CFG3)
#define clk1_SetPhase(clkPhase)             clk1_SetPhaseRegister(clkPhase)
#define clk1_SetPhaseValue(clkPhase)        clk1_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(clk1__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define clk1_CLKEN              (* (reg8 *) clk1__PM_ACT_CFG)
#define clk1_CLKEN_PTR          ((reg8 *) clk1__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define clk1_CLKSTBY            (* (reg8 *) clk1__PM_STBY_CFG)
#define clk1_CLKSTBY_PTR        ((reg8 *) clk1__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define clk1_DIV_LSB            (* (reg8 *) clk1__CFG0)
#define clk1_DIV_LSB_PTR        ((reg8 *) clk1__CFG0)
#define clk1_DIV_PTR            ((reg16 *) clk1__CFG0)

/* Clock MSB divider configuration register. */
#define clk1_DIV_MSB            (* (reg8 *) clk1__CFG1)
#define clk1_DIV_MSB_PTR        ((reg8 *) clk1__CFG1)

/* Mode and source configuration register */
#define clk1_MOD_SRC            (* (reg8 *) clk1__CFG2)
#define clk1_MOD_SRC_PTR        ((reg8 *) clk1__CFG2)

#if defined(clk1__CFG3)
/* Analog clock phase configuration register */
#define clk1_PHASE              (* (reg8 *) clk1__CFG3)
#define clk1_PHASE_PTR          ((reg8 *) clk1__CFG3)
#endif /* defined(clk1__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define clk1_CLKEN_MASK         clk1__PM_ACT_MSK
#define clk1_CLKSTBY_MASK       clk1__PM_STBY_MSK

/* CFG2 field masks */
#define clk1_SRC_SEL_MSK        clk1__CFG2_SRC_SEL_MASK
#define clk1_MODE_MASK          (~(clk1_SRC_SEL_MSK))

#if defined(clk1__CFG3)
/* CFG3 phase mask */
#define clk1_PHASE_MASK         clk1__CFG3_PHASE_DLY_MASK
#endif /* defined(clk1__CFG3) */

#endif /* CY_CLOCK_clk1_H */


/* [] END OF FILE */
