/*******************************************************************************
* File Name: PPS_GPS.h  
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

#if !defined(CY_PINS_PPS_GPS_H) /* Pins PPS_GPS_H */
#define CY_PINS_PPS_GPS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PPS_GPS_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PPS_GPS__PORT == 15 && ((PPS_GPS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    PPS_GPS_Write(uint8 value);
void    PPS_GPS_SetDriveMode(uint8 mode);
uint8   PPS_GPS_ReadDataReg(void);
uint8   PPS_GPS_Read(void);
void    PPS_GPS_SetInterruptMode(uint16 position, uint16 mode);
uint8   PPS_GPS_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the PPS_GPS_SetDriveMode() function.
     *  @{
     */
        #define PPS_GPS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define PPS_GPS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define PPS_GPS_DM_RES_UP          PIN_DM_RES_UP
        #define PPS_GPS_DM_RES_DWN         PIN_DM_RES_DWN
        #define PPS_GPS_DM_OD_LO           PIN_DM_OD_LO
        #define PPS_GPS_DM_OD_HI           PIN_DM_OD_HI
        #define PPS_GPS_DM_STRONG          PIN_DM_STRONG
        #define PPS_GPS_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define PPS_GPS_MASK               PPS_GPS__MASK
#define PPS_GPS_SHIFT              PPS_GPS__SHIFT
#define PPS_GPS_WIDTH              1u

/* Interrupt constants */
#if defined(PPS_GPS__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PPS_GPS_SetInterruptMode() function.
     *  @{
     */
        #define PPS_GPS_INTR_NONE      (uint16)(0x0000u)
        #define PPS_GPS_INTR_RISING    (uint16)(0x0001u)
        #define PPS_GPS_INTR_FALLING   (uint16)(0x0002u)
        #define PPS_GPS_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define PPS_GPS_INTR_MASK      (0x01u) 
#endif /* (PPS_GPS__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PPS_GPS_PS                     (* (reg8 *) PPS_GPS__PS)
/* Data Register */
#define PPS_GPS_DR                     (* (reg8 *) PPS_GPS__DR)
/* Port Number */
#define PPS_GPS_PRT_NUM                (* (reg8 *) PPS_GPS__PRT) 
/* Connect to Analog Globals */                                                  
#define PPS_GPS_AG                     (* (reg8 *) PPS_GPS__AG)                       
/* Analog MUX bux enable */
#define PPS_GPS_AMUX                   (* (reg8 *) PPS_GPS__AMUX) 
/* Bidirectional Enable */                                                        
#define PPS_GPS_BIE                    (* (reg8 *) PPS_GPS__BIE)
/* Bit-mask for Aliased Register Access */
#define PPS_GPS_BIT_MASK               (* (reg8 *) PPS_GPS__BIT_MASK)
/* Bypass Enable */
#define PPS_GPS_BYP                    (* (reg8 *) PPS_GPS__BYP)
/* Port wide control signals */                                                   
#define PPS_GPS_CTL                    (* (reg8 *) PPS_GPS__CTL)
/* Drive Modes */
#define PPS_GPS_DM0                    (* (reg8 *) PPS_GPS__DM0) 
#define PPS_GPS_DM1                    (* (reg8 *) PPS_GPS__DM1)
#define PPS_GPS_DM2                    (* (reg8 *) PPS_GPS__DM2) 
/* Input Buffer Disable Override */
#define PPS_GPS_INP_DIS                (* (reg8 *) PPS_GPS__INP_DIS)
/* LCD Common or Segment Drive */
#define PPS_GPS_LCD_COM_SEG            (* (reg8 *) PPS_GPS__LCD_COM_SEG)
/* Enable Segment LCD */
#define PPS_GPS_LCD_EN                 (* (reg8 *) PPS_GPS__LCD_EN)
/* Slew Rate Control */
#define PPS_GPS_SLW                    (* (reg8 *) PPS_GPS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PPS_GPS_PRTDSI__CAPS_SEL       (* (reg8 *) PPS_GPS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PPS_GPS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PPS_GPS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PPS_GPS_PRTDSI__OE_SEL0        (* (reg8 *) PPS_GPS__PRTDSI__OE_SEL0) 
#define PPS_GPS_PRTDSI__OE_SEL1        (* (reg8 *) PPS_GPS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PPS_GPS_PRTDSI__OUT_SEL0       (* (reg8 *) PPS_GPS__PRTDSI__OUT_SEL0) 
#define PPS_GPS_PRTDSI__OUT_SEL1       (* (reg8 *) PPS_GPS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PPS_GPS_PRTDSI__SYNC_OUT       (* (reg8 *) PPS_GPS__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(PPS_GPS__SIO_CFG)
    #define PPS_GPS_SIO_HYST_EN        (* (reg8 *) PPS_GPS__SIO_HYST_EN)
    #define PPS_GPS_SIO_REG_HIFREQ     (* (reg8 *) PPS_GPS__SIO_REG_HIFREQ)
    #define PPS_GPS_SIO_CFG            (* (reg8 *) PPS_GPS__SIO_CFG)
    #define PPS_GPS_SIO_DIFF           (* (reg8 *) PPS_GPS__SIO_DIFF)
#endif /* (PPS_GPS__SIO_CFG) */

/* Interrupt Registers */
#if defined(PPS_GPS__INTSTAT)
    #define PPS_GPS_INTSTAT            (* (reg8 *) PPS_GPS__INTSTAT)
    #define PPS_GPS_SNAP               (* (reg8 *) PPS_GPS__SNAP)
    
	#define PPS_GPS_0_INTTYPE_REG 		(* (reg8 *) PPS_GPS__0__INTTYPE)
#endif /* (PPS_GPS__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PPS_GPS_H */


/* [] END OF FILE */
