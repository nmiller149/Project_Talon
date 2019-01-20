/*******************************************************************************
* File Name: BNO055_scl.h  
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

#if !defined(CY_PINS_BNO055_scl_H) /* Pins BNO055_scl_H */
#define CY_PINS_BNO055_scl_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BNO055_scl_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BNO055_scl__PORT == 15 && ((BNO055_scl__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BNO055_scl_Write(uint8 value);
void    BNO055_scl_SetDriveMode(uint8 mode);
uint8   BNO055_scl_ReadDataReg(void);
uint8   BNO055_scl_Read(void);
void    BNO055_scl_SetInterruptMode(uint16 position, uint16 mode);
uint8   BNO055_scl_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BNO055_scl_SetDriveMode() function.
     *  @{
     */
        #define BNO055_scl_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BNO055_scl_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BNO055_scl_DM_RES_UP          PIN_DM_RES_UP
        #define BNO055_scl_DM_RES_DWN         PIN_DM_RES_DWN
        #define BNO055_scl_DM_OD_LO           PIN_DM_OD_LO
        #define BNO055_scl_DM_OD_HI           PIN_DM_OD_HI
        #define BNO055_scl_DM_STRONG          PIN_DM_STRONG
        #define BNO055_scl_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BNO055_scl_MASK               BNO055_scl__MASK
#define BNO055_scl_SHIFT              BNO055_scl__SHIFT
#define BNO055_scl_WIDTH              1u

/* Interrupt constants */
#if defined(BNO055_scl__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BNO055_scl_SetInterruptMode() function.
     *  @{
     */
        #define BNO055_scl_INTR_NONE      (uint16)(0x0000u)
        #define BNO055_scl_INTR_RISING    (uint16)(0x0001u)
        #define BNO055_scl_INTR_FALLING   (uint16)(0x0002u)
        #define BNO055_scl_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BNO055_scl_INTR_MASK      (0x01u) 
#endif /* (BNO055_scl__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BNO055_scl_PS                     (* (reg8 *) BNO055_scl__PS)
/* Data Register */
#define BNO055_scl_DR                     (* (reg8 *) BNO055_scl__DR)
/* Port Number */
#define BNO055_scl_PRT_NUM                (* (reg8 *) BNO055_scl__PRT) 
/* Connect to Analog Globals */                                                  
#define BNO055_scl_AG                     (* (reg8 *) BNO055_scl__AG)                       
/* Analog MUX bux enable */
#define BNO055_scl_AMUX                   (* (reg8 *) BNO055_scl__AMUX) 
/* Bidirectional Enable */                                                        
#define BNO055_scl_BIE                    (* (reg8 *) BNO055_scl__BIE)
/* Bit-mask for Aliased Register Access */
#define BNO055_scl_BIT_MASK               (* (reg8 *) BNO055_scl__BIT_MASK)
/* Bypass Enable */
#define BNO055_scl_BYP                    (* (reg8 *) BNO055_scl__BYP)
/* Port wide control signals */                                                   
#define BNO055_scl_CTL                    (* (reg8 *) BNO055_scl__CTL)
/* Drive Modes */
#define BNO055_scl_DM0                    (* (reg8 *) BNO055_scl__DM0) 
#define BNO055_scl_DM1                    (* (reg8 *) BNO055_scl__DM1)
#define BNO055_scl_DM2                    (* (reg8 *) BNO055_scl__DM2) 
/* Input Buffer Disable Override */
#define BNO055_scl_INP_DIS                (* (reg8 *) BNO055_scl__INP_DIS)
/* LCD Common or Segment Drive */
#define BNO055_scl_LCD_COM_SEG            (* (reg8 *) BNO055_scl__LCD_COM_SEG)
/* Enable Segment LCD */
#define BNO055_scl_LCD_EN                 (* (reg8 *) BNO055_scl__LCD_EN)
/* Slew Rate Control */
#define BNO055_scl_SLW                    (* (reg8 *) BNO055_scl__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BNO055_scl_PRTDSI__CAPS_SEL       (* (reg8 *) BNO055_scl__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BNO055_scl_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BNO055_scl__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BNO055_scl_PRTDSI__OE_SEL0        (* (reg8 *) BNO055_scl__PRTDSI__OE_SEL0) 
#define BNO055_scl_PRTDSI__OE_SEL1        (* (reg8 *) BNO055_scl__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BNO055_scl_PRTDSI__OUT_SEL0       (* (reg8 *) BNO055_scl__PRTDSI__OUT_SEL0) 
#define BNO055_scl_PRTDSI__OUT_SEL1       (* (reg8 *) BNO055_scl__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BNO055_scl_PRTDSI__SYNC_OUT       (* (reg8 *) BNO055_scl__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BNO055_scl__SIO_CFG)
    #define BNO055_scl_SIO_HYST_EN        (* (reg8 *) BNO055_scl__SIO_HYST_EN)
    #define BNO055_scl_SIO_REG_HIFREQ     (* (reg8 *) BNO055_scl__SIO_REG_HIFREQ)
    #define BNO055_scl_SIO_CFG            (* (reg8 *) BNO055_scl__SIO_CFG)
    #define BNO055_scl_SIO_DIFF           (* (reg8 *) BNO055_scl__SIO_DIFF)
#endif /* (BNO055_scl__SIO_CFG) */

/* Interrupt Registers */
#if defined(BNO055_scl__INTSTAT)
    #define BNO055_scl_INTSTAT            (* (reg8 *) BNO055_scl__INTSTAT)
    #define BNO055_scl_SNAP               (* (reg8 *) BNO055_scl__SNAP)
    
	#define BNO055_scl_0_INTTYPE_REG 		(* (reg8 *) BNO055_scl__0__INTTYPE)
#endif /* (BNO055_scl__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BNO055_scl_H */


/* [] END OF FILE */
