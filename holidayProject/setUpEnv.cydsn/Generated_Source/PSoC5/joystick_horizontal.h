/*******************************************************************************
* File Name: joystick_horizontal.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_joystick_horizontal_H) /* Pins joystick_horizontal_H */
#define CY_PINS_joystick_horizontal_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "joystick_horizontal_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 joystick_horizontal__PORT == 15 && ((joystick_horizontal__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    joystick_horizontal_Write(uint8 value);
void    joystick_horizontal_SetDriveMode(uint8 mode);
uint8   joystick_horizontal_ReadDataReg(void);
uint8   joystick_horizontal_Read(void);
void    joystick_horizontal_SetInterruptMode(uint16 position, uint16 mode);
uint8   joystick_horizontal_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the joystick_horizontal_SetDriveMode() function.
     *  @{
     */
        #define joystick_horizontal_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define joystick_horizontal_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define joystick_horizontal_DM_RES_UP          PIN_DM_RES_UP
        #define joystick_horizontal_DM_RES_DWN         PIN_DM_RES_DWN
        #define joystick_horizontal_DM_OD_LO           PIN_DM_OD_LO
        #define joystick_horizontal_DM_OD_HI           PIN_DM_OD_HI
        #define joystick_horizontal_DM_STRONG          PIN_DM_STRONG
        #define joystick_horizontal_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define joystick_horizontal_MASK               joystick_horizontal__MASK
#define joystick_horizontal_SHIFT              joystick_horizontal__SHIFT
#define joystick_horizontal_WIDTH              1u

/* Interrupt constants */
#if defined(joystick_horizontal__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in joystick_horizontal_SetInterruptMode() function.
     *  @{
     */
        #define joystick_horizontal_INTR_NONE      (uint16)(0x0000u)
        #define joystick_horizontal_INTR_RISING    (uint16)(0x0001u)
        #define joystick_horizontal_INTR_FALLING   (uint16)(0x0002u)
        #define joystick_horizontal_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define joystick_horizontal_INTR_MASK      (0x01u) 
#endif /* (joystick_horizontal__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define joystick_horizontal_PS                     (* (reg8 *) joystick_horizontal__PS)
/* Data Register */
#define joystick_horizontal_DR                     (* (reg8 *) joystick_horizontal__DR)
/* Port Number */
#define joystick_horizontal_PRT_NUM                (* (reg8 *) joystick_horizontal__PRT) 
/* Connect to Analog Globals */                                                  
#define joystick_horizontal_AG                     (* (reg8 *) joystick_horizontal__AG)                       
/* Analog MUX bux enable */
#define joystick_horizontal_AMUX                   (* (reg8 *) joystick_horizontal__AMUX) 
/* Bidirectional Enable */                                                        
#define joystick_horizontal_BIE                    (* (reg8 *) joystick_horizontal__BIE)
/* Bit-mask for Aliased Register Access */
#define joystick_horizontal_BIT_MASK               (* (reg8 *) joystick_horizontal__BIT_MASK)
/* Bypass Enable */
#define joystick_horizontal_BYP                    (* (reg8 *) joystick_horizontal__BYP)
/* Port wide control signals */                                                   
#define joystick_horizontal_CTL                    (* (reg8 *) joystick_horizontal__CTL)
/* Drive Modes */
#define joystick_horizontal_DM0                    (* (reg8 *) joystick_horizontal__DM0) 
#define joystick_horizontal_DM1                    (* (reg8 *) joystick_horizontal__DM1)
#define joystick_horizontal_DM2                    (* (reg8 *) joystick_horizontal__DM2) 
/* Input Buffer Disable Override */
#define joystick_horizontal_INP_DIS                (* (reg8 *) joystick_horizontal__INP_DIS)
/* LCD Common or Segment Drive */
#define joystick_horizontal_LCD_COM_SEG            (* (reg8 *) joystick_horizontal__LCD_COM_SEG)
/* Enable Segment LCD */
#define joystick_horizontal_LCD_EN                 (* (reg8 *) joystick_horizontal__LCD_EN)
/* Slew Rate Control */
#define joystick_horizontal_SLW                    (* (reg8 *) joystick_horizontal__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define joystick_horizontal_PRTDSI__CAPS_SEL       (* (reg8 *) joystick_horizontal__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define joystick_horizontal_PRTDSI__DBL_SYNC_IN    (* (reg8 *) joystick_horizontal__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define joystick_horizontal_PRTDSI__OE_SEL0        (* (reg8 *) joystick_horizontal__PRTDSI__OE_SEL0) 
#define joystick_horizontal_PRTDSI__OE_SEL1        (* (reg8 *) joystick_horizontal__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define joystick_horizontal_PRTDSI__OUT_SEL0       (* (reg8 *) joystick_horizontal__PRTDSI__OUT_SEL0) 
#define joystick_horizontal_PRTDSI__OUT_SEL1       (* (reg8 *) joystick_horizontal__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define joystick_horizontal_PRTDSI__SYNC_OUT       (* (reg8 *) joystick_horizontal__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(joystick_horizontal__SIO_CFG)
    #define joystick_horizontal_SIO_HYST_EN        (* (reg8 *) joystick_horizontal__SIO_HYST_EN)
    #define joystick_horizontal_SIO_REG_HIFREQ     (* (reg8 *) joystick_horizontal__SIO_REG_HIFREQ)
    #define joystick_horizontal_SIO_CFG            (* (reg8 *) joystick_horizontal__SIO_CFG)
    #define joystick_horizontal_SIO_DIFF           (* (reg8 *) joystick_horizontal__SIO_DIFF)
#endif /* (joystick_horizontal__SIO_CFG) */

/* Interrupt Registers */
#if defined(joystick_horizontal__INTSTAT)
    #define joystick_horizontal_INTSTAT            (* (reg8 *) joystick_horizontal__INTSTAT)
    #define joystick_horizontal_SNAP               (* (reg8 *) joystick_horizontal__SNAP)
    
	#define joystick_horizontal_0_INTTYPE_REG 		(* (reg8 *) joystick_horizontal__0__INTTYPE)
#endif /* (joystick_horizontal__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_joystick_horizontal_H */


/* [] END OF FILE */
