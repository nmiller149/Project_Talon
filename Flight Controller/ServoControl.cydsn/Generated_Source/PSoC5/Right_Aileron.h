/*******************************************************************************
* File Name: Right_Aileron.h  
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

#if !defined(CY_PINS_Right_Aileron_H) /* Pins Right_Aileron_H */
#define CY_PINS_Right_Aileron_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Right_Aileron_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Right_Aileron__PORT == 15 && ((Right_Aileron__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Right_Aileron_Write(uint8 value);
void    Right_Aileron_SetDriveMode(uint8 mode);
uint8   Right_Aileron_ReadDataReg(void);
uint8   Right_Aileron_Read(void);
void    Right_Aileron_SetInterruptMode(uint16 position, uint16 mode);
uint8   Right_Aileron_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Right_Aileron_SetDriveMode() function.
     *  @{
     */
        #define Right_Aileron_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Right_Aileron_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Right_Aileron_DM_RES_UP          PIN_DM_RES_UP
        #define Right_Aileron_DM_RES_DWN         PIN_DM_RES_DWN
        #define Right_Aileron_DM_OD_LO           PIN_DM_OD_LO
        #define Right_Aileron_DM_OD_HI           PIN_DM_OD_HI
        #define Right_Aileron_DM_STRONG          PIN_DM_STRONG
        #define Right_Aileron_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Right_Aileron_MASK               Right_Aileron__MASK
#define Right_Aileron_SHIFT              Right_Aileron__SHIFT
#define Right_Aileron_WIDTH              1u

/* Interrupt constants */
#if defined(Right_Aileron__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Right_Aileron_SetInterruptMode() function.
     *  @{
     */
        #define Right_Aileron_INTR_NONE      (uint16)(0x0000u)
        #define Right_Aileron_INTR_RISING    (uint16)(0x0001u)
        #define Right_Aileron_INTR_FALLING   (uint16)(0x0002u)
        #define Right_Aileron_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Right_Aileron_INTR_MASK      (0x01u) 
#endif /* (Right_Aileron__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Right_Aileron_PS                     (* (reg8 *) Right_Aileron__PS)
/* Data Register */
#define Right_Aileron_DR                     (* (reg8 *) Right_Aileron__DR)
/* Port Number */
#define Right_Aileron_PRT_NUM                (* (reg8 *) Right_Aileron__PRT) 
/* Connect to Analog Globals */                                                  
#define Right_Aileron_AG                     (* (reg8 *) Right_Aileron__AG)                       
/* Analog MUX bux enable */
#define Right_Aileron_AMUX                   (* (reg8 *) Right_Aileron__AMUX) 
/* Bidirectional Enable */                                                        
#define Right_Aileron_BIE                    (* (reg8 *) Right_Aileron__BIE)
/* Bit-mask for Aliased Register Access */
#define Right_Aileron_BIT_MASK               (* (reg8 *) Right_Aileron__BIT_MASK)
/* Bypass Enable */
#define Right_Aileron_BYP                    (* (reg8 *) Right_Aileron__BYP)
/* Port wide control signals */                                                   
#define Right_Aileron_CTL                    (* (reg8 *) Right_Aileron__CTL)
/* Drive Modes */
#define Right_Aileron_DM0                    (* (reg8 *) Right_Aileron__DM0) 
#define Right_Aileron_DM1                    (* (reg8 *) Right_Aileron__DM1)
#define Right_Aileron_DM2                    (* (reg8 *) Right_Aileron__DM2) 
/* Input Buffer Disable Override */
#define Right_Aileron_INP_DIS                (* (reg8 *) Right_Aileron__INP_DIS)
/* LCD Common or Segment Drive */
#define Right_Aileron_LCD_COM_SEG            (* (reg8 *) Right_Aileron__LCD_COM_SEG)
/* Enable Segment LCD */
#define Right_Aileron_LCD_EN                 (* (reg8 *) Right_Aileron__LCD_EN)
/* Slew Rate Control */
#define Right_Aileron_SLW                    (* (reg8 *) Right_Aileron__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Right_Aileron_PRTDSI__CAPS_SEL       (* (reg8 *) Right_Aileron__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Right_Aileron_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Right_Aileron__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Right_Aileron_PRTDSI__OE_SEL0        (* (reg8 *) Right_Aileron__PRTDSI__OE_SEL0) 
#define Right_Aileron_PRTDSI__OE_SEL1        (* (reg8 *) Right_Aileron__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Right_Aileron_PRTDSI__OUT_SEL0       (* (reg8 *) Right_Aileron__PRTDSI__OUT_SEL0) 
#define Right_Aileron_PRTDSI__OUT_SEL1       (* (reg8 *) Right_Aileron__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Right_Aileron_PRTDSI__SYNC_OUT       (* (reg8 *) Right_Aileron__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Right_Aileron__SIO_CFG)
    #define Right_Aileron_SIO_HYST_EN        (* (reg8 *) Right_Aileron__SIO_HYST_EN)
    #define Right_Aileron_SIO_REG_HIFREQ     (* (reg8 *) Right_Aileron__SIO_REG_HIFREQ)
    #define Right_Aileron_SIO_CFG            (* (reg8 *) Right_Aileron__SIO_CFG)
    #define Right_Aileron_SIO_DIFF           (* (reg8 *) Right_Aileron__SIO_DIFF)
#endif /* (Right_Aileron__SIO_CFG) */

/* Interrupt Registers */
#if defined(Right_Aileron__INTSTAT)
    #define Right_Aileron_INTSTAT            (* (reg8 *) Right_Aileron__INTSTAT)
    #define Right_Aileron_SNAP               (* (reg8 *) Right_Aileron__SNAP)
    
	#define Right_Aileron_0_INTTYPE_REG 		(* (reg8 *) Right_Aileron__0__INTTYPE)
#endif /* (Right_Aileron__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Right_Aileron_H */


/* [] END OF FILE */
