/*******************************************************************************
* File Name: BMP280_SCL.h  
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

#if !defined(CY_PINS_BMP280_SCL_H) /* Pins BMP280_SCL_H */
#define CY_PINS_BMP280_SCL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BMP280_SCL_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BMP280_SCL__PORT == 15 && ((BMP280_SCL__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BMP280_SCL_Write(uint8 value);
void    BMP280_SCL_SetDriveMode(uint8 mode);
uint8   BMP280_SCL_ReadDataReg(void);
uint8   BMP280_SCL_Read(void);
void    BMP280_SCL_SetInterruptMode(uint16 position, uint16 mode);
uint8   BMP280_SCL_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BMP280_SCL_SetDriveMode() function.
     *  @{
     */
        #define BMP280_SCL_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BMP280_SCL_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BMP280_SCL_DM_RES_UP          PIN_DM_RES_UP
        #define BMP280_SCL_DM_RES_DWN         PIN_DM_RES_DWN
        #define BMP280_SCL_DM_OD_LO           PIN_DM_OD_LO
        #define BMP280_SCL_DM_OD_HI           PIN_DM_OD_HI
        #define BMP280_SCL_DM_STRONG          PIN_DM_STRONG
        #define BMP280_SCL_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BMP280_SCL_MASK               BMP280_SCL__MASK
#define BMP280_SCL_SHIFT              BMP280_SCL__SHIFT
#define BMP280_SCL_WIDTH              1u

/* Interrupt constants */
#if defined(BMP280_SCL__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BMP280_SCL_SetInterruptMode() function.
     *  @{
     */
        #define BMP280_SCL_INTR_NONE      (uint16)(0x0000u)
        #define BMP280_SCL_INTR_RISING    (uint16)(0x0001u)
        #define BMP280_SCL_INTR_FALLING   (uint16)(0x0002u)
        #define BMP280_SCL_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BMP280_SCL_INTR_MASK      (0x01u) 
#endif /* (BMP280_SCL__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BMP280_SCL_PS                     (* (reg8 *) BMP280_SCL__PS)
/* Data Register */
#define BMP280_SCL_DR                     (* (reg8 *) BMP280_SCL__DR)
/* Port Number */
#define BMP280_SCL_PRT_NUM                (* (reg8 *) BMP280_SCL__PRT) 
/* Connect to Analog Globals */                                                  
#define BMP280_SCL_AG                     (* (reg8 *) BMP280_SCL__AG)                       
/* Analog MUX bux enable */
#define BMP280_SCL_AMUX                   (* (reg8 *) BMP280_SCL__AMUX) 
/* Bidirectional Enable */                                                        
#define BMP280_SCL_BIE                    (* (reg8 *) BMP280_SCL__BIE)
/* Bit-mask for Aliased Register Access */
#define BMP280_SCL_BIT_MASK               (* (reg8 *) BMP280_SCL__BIT_MASK)
/* Bypass Enable */
#define BMP280_SCL_BYP                    (* (reg8 *) BMP280_SCL__BYP)
/* Port wide control signals */                                                   
#define BMP280_SCL_CTL                    (* (reg8 *) BMP280_SCL__CTL)
/* Drive Modes */
#define BMP280_SCL_DM0                    (* (reg8 *) BMP280_SCL__DM0) 
#define BMP280_SCL_DM1                    (* (reg8 *) BMP280_SCL__DM1)
#define BMP280_SCL_DM2                    (* (reg8 *) BMP280_SCL__DM2) 
/* Input Buffer Disable Override */
#define BMP280_SCL_INP_DIS                (* (reg8 *) BMP280_SCL__INP_DIS)
/* LCD Common or Segment Drive */
#define BMP280_SCL_LCD_COM_SEG            (* (reg8 *) BMP280_SCL__LCD_COM_SEG)
/* Enable Segment LCD */
#define BMP280_SCL_LCD_EN                 (* (reg8 *) BMP280_SCL__LCD_EN)
/* Slew Rate Control */
#define BMP280_SCL_SLW                    (* (reg8 *) BMP280_SCL__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BMP280_SCL_PRTDSI__CAPS_SEL       (* (reg8 *) BMP280_SCL__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BMP280_SCL_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BMP280_SCL__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BMP280_SCL_PRTDSI__OE_SEL0        (* (reg8 *) BMP280_SCL__PRTDSI__OE_SEL0) 
#define BMP280_SCL_PRTDSI__OE_SEL1        (* (reg8 *) BMP280_SCL__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BMP280_SCL_PRTDSI__OUT_SEL0       (* (reg8 *) BMP280_SCL__PRTDSI__OUT_SEL0) 
#define BMP280_SCL_PRTDSI__OUT_SEL1       (* (reg8 *) BMP280_SCL__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BMP280_SCL_PRTDSI__SYNC_OUT       (* (reg8 *) BMP280_SCL__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BMP280_SCL__SIO_CFG)
    #define BMP280_SCL_SIO_HYST_EN        (* (reg8 *) BMP280_SCL__SIO_HYST_EN)
    #define BMP280_SCL_SIO_REG_HIFREQ     (* (reg8 *) BMP280_SCL__SIO_REG_HIFREQ)
    #define BMP280_SCL_SIO_CFG            (* (reg8 *) BMP280_SCL__SIO_CFG)
    #define BMP280_SCL_SIO_DIFF           (* (reg8 *) BMP280_SCL__SIO_DIFF)
#endif /* (BMP280_SCL__SIO_CFG) */

/* Interrupt Registers */
#if defined(BMP280_SCL__INTSTAT)
    #define BMP280_SCL_INTSTAT            (* (reg8 *) BMP280_SCL__INTSTAT)
    #define BMP280_SCL_SNAP               (* (reg8 *) BMP280_SCL__SNAP)
    
	#define BMP280_SCL_0_INTTYPE_REG 		(* (reg8 *) BMP280_SCL__0__INTTYPE)
#endif /* (BMP280_SCL__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BMP280_SCL_H */


/* [] END OF FILE */
