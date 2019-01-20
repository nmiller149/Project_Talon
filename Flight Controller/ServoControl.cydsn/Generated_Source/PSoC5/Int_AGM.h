/*******************************************************************************
* File Name: Int_AGM.h  
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

#if !defined(CY_PINS_Int_AGM_H) /* Pins Int_AGM_H */
#define CY_PINS_Int_AGM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Int_AGM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Int_AGM__PORT == 15 && ((Int_AGM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Int_AGM_Write(uint8 value);
void    Int_AGM_SetDriveMode(uint8 mode);
uint8   Int_AGM_ReadDataReg(void);
uint8   Int_AGM_Read(void);
void    Int_AGM_SetInterruptMode(uint16 position, uint16 mode);
uint8   Int_AGM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Int_AGM_SetDriveMode() function.
     *  @{
     */
        #define Int_AGM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Int_AGM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Int_AGM_DM_RES_UP          PIN_DM_RES_UP
        #define Int_AGM_DM_RES_DWN         PIN_DM_RES_DWN
        #define Int_AGM_DM_OD_LO           PIN_DM_OD_LO
        #define Int_AGM_DM_OD_HI           PIN_DM_OD_HI
        #define Int_AGM_DM_STRONG          PIN_DM_STRONG
        #define Int_AGM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Int_AGM_MASK               Int_AGM__MASK
#define Int_AGM_SHIFT              Int_AGM__SHIFT
#define Int_AGM_WIDTH              1u

/* Interrupt constants */
#if defined(Int_AGM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Int_AGM_SetInterruptMode() function.
     *  @{
     */
        #define Int_AGM_INTR_NONE      (uint16)(0x0000u)
        #define Int_AGM_INTR_RISING    (uint16)(0x0001u)
        #define Int_AGM_INTR_FALLING   (uint16)(0x0002u)
        #define Int_AGM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Int_AGM_INTR_MASK      (0x01u) 
#endif /* (Int_AGM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Int_AGM_PS                     (* (reg8 *) Int_AGM__PS)
/* Data Register */
#define Int_AGM_DR                     (* (reg8 *) Int_AGM__DR)
/* Port Number */
#define Int_AGM_PRT_NUM                (* (reg8 *) Int_AGM__PRT) 
/* Connect to Analog Globals */                                                  
#define Int_AGM_AG                     (* (reg8 *) Int_AGM__AG)                       
/* Analog MUX bux enable */
#define Int_AGM_AMUX                   (* (reg8 *) Int_AGM__AMUX) 
/* Bidirectional Enable */                                                        
#define Int_AGM_BIE                    (* (reg8 *) Int_AGM__BIE)
/* Bit-mask for Aliased Register Access */
#define Int_AGM_BIT_MASK               (* (reg8 *) Int_AGM__BIT_MASK)
/* Bypass Enable */
#define Int_AGM_BYP                    (* (reg8 *) Int_AGM__BYP)
/* Port wide control signals */                                                   
#define Int_AGM_CTL                    (* (reg8 *) Int_AGM__CTL)
/* Drive Modes */
#define Int_AGM_DM0                    (* (reg8 *) Int_AGM__DM0) 
#define Int_AGM_DM1                    (* (reg8 *) Int_AGM__DM1)
#define Int_AGM_DM2                    (* (reg8 *) Int_AGM__DM2) 
/* Input Buffer Disable Override */
#define Int_AGM_INP_DIS                (* (reg8 *) Int_AGM__INP_DIS)
/* LCD Common or Segment Drive */
#define Int_AGM_LCD_COM_SEG            (* (reg8 *) Int_AGM__LCD_COM_SEG)
/* Enable Segment LCD */
#define Int_AGM_LCD_EN                 (* (reg8 *) Int_AGM__LCD_EN)
/* Slew Rate Control */
#define Int_AGM_SLW                    (* (reg8 *) Int_AGM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Int_AGM_PRTDSI__CAPS_SEL       (* (reg8 *) Int_AGM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Int_AGM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Int_AGM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Int_AGM_PRTDSI__OE_SEL0        (* (reg8 *) Int_AGM__PRTDSI__OE_SEL0) 
#define Int_AGM_PRTDSI__OE_SEL1        (* (reg8 *) Int_AGM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Int_AGM_PRTDSI__OUT_SEL0       (* (reg8 *) Int_AGM__PRTDSI__OUT_SEL0) 
#define Int_AGM_PRTDSI__OUT_SEL1       (* (reg8 *) Int_AGM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Int_AGM_PRTDSI__SYNC_OUT       (* (reg8 *) Int_AGM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Int_AGM__SIO_CFG)
    #define Int_AGM_SIO_HYST_EN        (* (reg8 *) Int_AGM__SIO_HYST_EN)
    #define Int_AGM_SIO_REG_HIFREQ     (* (reg8 *) Int_AGM__SIO_REG_HIFREQ)
    #define Int_AGM_SIO_CFG            (* (reg8 *) Int_AGM__SIO_CFG)
    #define Int_AGM_SIO_DIFF           (* (reg8 *) Int_AGM__SIO_DIFF)
#endif /* (Int_AGM__SIO_CFG) */

/* Interrupt Registers */
#if defined(Int_AGM__INTSTAT)
    #define Int_AGM_INTSTAT            (* (reg8 *) Int_AGM__INTSTAT)
    #define Int_AGM_SNAP               (* (reg8 *) Int_AGM__SNAP)
    
	#define Int_AGM_0_INTTYPE_REG 		(* (reg8 *) Int_AGM__0__INTTYPE)
#endif /* (Int_AGM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Int_AGM_H */


/* [] END OF FILE */
