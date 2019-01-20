/*******************************************************************************
* File Name: Nine_Axis_Orientation.h  
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

#if !defined(CY_PINS_Nine_Axis_Orientation_H) /* Pins Nine_Axis_Orientation_H */
#define CY_PINS_Nine_Axis_Orientation_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Nine_Axis_Orientation_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Nine_Axis_Orientation__PORT == 15 && ((Nine_Axis_Orientation__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Nine_Axis_Orientation_Write(uint8 value);
void    Nine_Axis_Orientation_SetDriveMode(uint8 mode);
uint8   Nine_Axis_Orientation_ReadDataReg(void);
uint8   Nine_Axis_Orientation_Read(void);
void    Nine_Axis_Orientation_SetInterruptMode(uint16 position, uint16 mode);
uint8   Nine_Axis_Orientation_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Nine_Axis_Orientation_SetDriveMode() function.
     *  @{
     */
        #define Nine_Axis_Orientation_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Nine_Axis_Orientation_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Nine_Axis_Orientation_DM_RES_UP          PIN_DM_RES_UP
        #define Nine_Axis_Orientation_DM_RES_DWN         PIN_DM_RES_DWN
        #define Nine_Axis_Orientation_DM_OD_LO           PIN_DM_OD_LO
        #define Nine_Axis_Orientation_DM_OD_HI           PIN_DM_OD_HI
        #define Nine_Axis_Orientation_DM_STRONG          PIN_DM_STRONG
        #define Nine_Axis_Orientation_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Nine_Axis_Orientation_MASK               Nine_Axis_Orientation__MASK
#define Nine_Axis_Orientation_SHIFT              Nine_Axis_Orientation__SHIFT
#define Nine_Axis_Orientation_WIDTH              2u

/* Interrupt constants */
#if defined(Nine_Axis_Orientation__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Nine_Axis_Orientation_SetInterruptMode() function.
     *  @{
     */
        #define Nine_Axis_Orientation_INTR_NONE      (uint16)(0x0000u)
        #define Nine_Axis_Orientation_INTR_RISING    (uint16)(0x0001u)
        #define Nine_Axis_Orientation_INTR_FALLING   (uint16)(0x0002u)
        #define Nine_Axis_Orientation_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Nine_Axis_Orientation_INTR_MASK      (0x01u) 
#endif /* (Nine_Axis_Orientation__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Nine_Axis_Orientation_PS                     (* (reg8 *) Nine_Axis_Orientation__PS)
/* Data Register */
#define Nine_Axis_Orientation_DR                     (* (reg8 *) Nine_Axis_Orientation__DR)
/* Port Number */
#define Nine_Axis_Orientation_PRT_NUM                (* (reg8 *) Nine_Axis_Orientation__PRT) 
/* Connect to Analog Globals */                                                  
#define Nine_Axis_Orientation_AG                     (* (reg8 *) Nine_Axis_Orientation__AG)                       
/* Analog MUX bux enable */
#define Nine_Axis_Orientation_AMUX                   (* (reg8 *) Nine_Axis_Orientation__AMUX) 
/* Bidirectional Enable */                                                        
#define Nine_Axis_Orientation_BIE                    (* (reg8 *) Nine_Axis_Orientation__BIE)
/* Bit-mask for Aliased Register Access */
#define Nine_Axis_Orientation_BIT_MASK               (* (reg8 *) Nine_Axis_Orientation__BIT_MASK)
/* Bypass Enable */
#define Nine_Axis_Orientation_BYP                    (* (reg8 *) Nine_Axis_Orientation__BYP)
/* Port wide control signals */                                                   
#define Nine_Axis_Orientation_CTL                    (* (reg8 *) Nine_Axis_Orientation__CTL)
/* Drive Modes */
#define Nine_Axis_Orientation_DM0                    (* (reg8 *) Nine_Axis_Orientation__DM0) 
#define Nine_Axis_Orientation_DM1                    (* (reg8 *) Nine_Axis_Orientation__DM1)
#define Nine_Axis_Orientation_DM2                    (* (reg8 *) Nine_Axis_Orientation__DM2) 
/* Input Buffer Disable Override */
#define Nine_Axis_Orientation_INP_DIS                (* (reg8 *) Nine_Axis_Orientation__INP_DIS)
/* LCD Common or Segment Drive */
#define Nine_Axis_Orientation_LCD_COM_SEG            (* (reg8 *) Nine_Axis_Orientation__LCD_COM_SEG)
/* Enable Segment LCD */
#define Nine_Axis_Orientation_LCD_EN                 (* (reg8 *) Nine_Axis_Orientation__LCD_EN)
/* Slew Rate Control */
#define Nine_Axis_Orientation_SLW                    (* (reg8 *) Nine_Axis_Orientation__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Nine_Axis_Orientation_PRTDSI__CAPS_SEL       (* (reg8 *) Nine_Axis_Orientation__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Nine_Axis_Orientation_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Nine_Axis_Orientation__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Nine_Axis_Orientation_PRTDSI__OE_SEL0        (* (reg8 *) Nine_Axis_Orientation__PRTDSI__OE_SEL0) 
#define Nine_Axis_Orientation_PRTDSI__OE_SEL1        (* (reg8 *) Nine_Axis_Orientation__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Nine_Axis_Orientation_PRTDSI__OUT_SEL0       (* (reg8 *) Nine_Axis_Orientation__PRTDSI__OUT_SEL0) 
#define Nine_Axis_Orientation_PRTDSI__OUT_SEL1       (* (reg8 *) Nine_Axis_Orientation__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Nine_Axis_Orientation_PRTDSI__SYNC_OUT       (* (reg8 *) Nine_Axis_Orientation__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Nine_Axis_Orientation__SIO_CFG)
    #define Nine_Axis_Orientation_SIO_HYST_EN        (* (reg8 *) Nine_Axis_Orientation__SIO_HYST_EN)
    #define Nine_Axis_Orientation_SIO_REG_HIFREQ     (* (reg8 *) Nine_Axis_Orientation__SIO_REG_HIFREQ)
    #define Nine_Axis_Orientation_SIO_CFG            (* (reg8 *) Nine_Axis_Orientation__SIO_CFG)
    #define Nine_Axis_Orientation_SIO_DIFF           (* (reg8 *) Nine_Axis_Orientation__SIO_DIFF)
#endif /* (Nine_Axis_Orientation__SIO_CFG) */

/* Interrupt Registers */
#if defined(Nine_Axis_Orientation__INTSTAT)
    #define Nine_Axis_Orientation_INTSTAT            (* (reg8 *) Nine_Axis_Orientation__INTSTAT)
    #define Nine_Axis_Orientation_SNAP               (* (reg8 *) Nine_Axis_Orientation__SNAP)
    
	#define Nine_Axis_Orientation_0_INTTYPE_REG 		(* (reg8 *) Nine_Axis_Orientation__0__INTTYPE)
	#define Nine_Axis_Orientation_1_INTTYPE_REG 		(* (reg8 *) Nine_Axis_Orientation__1__INTTYPE)
#endif /* (Nine_Axis_Orientation__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Nine_Axis_Orientation_H */


/* [] END OF FILE */
