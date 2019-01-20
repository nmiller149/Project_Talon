/*******************************************************************************
* File Name: Stabilizer.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_Stabilizer_H)
#define CY_PWM_Stabilizer_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Stabilizer_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define Stabilizer_Resolution                     (16u)
#define Stabilizer_UsingFixedFunction             (0u)
#define Stabilizer_DeadBandMode                   (0u)
#define Stabilizer_KillModeMinTime                (0u)
#define Stabilizer_KillMode                       (0u)
#define Stabilizer_PWMMode                        (1u)
#define Stabilizer_PWMModeIsCenterAligned         (0u)
#define Stabilizer_DeadBandUsed                   (0u)
#define Stabilizer_DeadBand2_4                    (0u)

#if !defined(Stabilizer_PWMUDB_genblk8_stsreg__REMOVED)
    #define Stabilizer_UseStatus                  (1u)
#else
    #define Stabilizer_UseStatus                  (0u)
#endif /* !defined(Stabilizer_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(Stabilizer_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define Stabilizer_UseControl                 (1u)
#else
    #define Stabilizer_UseControl                 (0u)
#endif /* !defined(Stabilizer_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define Stabilizer_UseOneCompareMode              (0u)
#define Stabilizer_MinimumKillTime                (1u)
#define Stabilizer_EnableMode                     (0u)

#define Stabilizer_CompareMode1SW                 (0u)
#define Stabilizer_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define Stabilizer__B_PWM__DISABLED 0
#define Stabilizer__B_PWM__ASYNCHRONOUS 1
#define Stabilizer__B_PWM__SINGLECYCLE 2
#define Stabilizer__B_PWM__LATCHED 3
#define Stabilizer__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define Stabilizer__B_PWM__DBMDISABLED 0
#define Stabilizer__B_PWM__DBM_2_4_CLOCKS 1
#define Stabilizer__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define Stabilizer__B_PWM__ONE_OUTPUT 0
#define Stabilizer__B_PWM__TWO_OUTPUTS 1
#define Stabilizer__B_PWM__DUAL_EDGE 2
#define Stabilizer__B_PWM__CENTER_ALIGN 3
#define Stabilizer__B_PWM__DITHER 5
#define Stabilizer__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define Stabilizer__B_PWM__LESS_THAN 1
#define Stabilizer__B_PWM__LESS_THAN_OR_EQUAL 2
#define Stabilizer__B_PWM__GREATER_THAN 3
#define Stabilizer__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define Stabilizer__B_PWM__EQUAL 0
#define Stabilizer__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!Stabilizer_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!Stabilizer_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!Stabilizer_PWMModeIsCenterAligned) */
        #if (Stabilizer_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (Stabilizer_UseStatus) */

        /* Backup for Deadband parameters */
        #if(Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_256_CLOCKS || \
            Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(Stabilizer_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (Stabilizer_KillModeMinTime) */

        /* Backup control register */
        #if(Stabilizer_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (Stabilizer_UseControl) */

    #endif /* (!Stabilizer_UsingFixedFunction) */

}Stabilizer_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    Stabilizer_Start(void) ;
void    Stabilizer_Stop(void) ;

#if (Stabilizer_UseStatus || Stabilizer_UsingFixedFunction)
    void  Stabilizer_SetInterruptMode(uint8 interruptMode) ;
    uint8 Stabilizer_ReadStatusRegister(void) ;
#endif /* (Stabilizer_UseStatus || Stabilizer_UsingFixedFunction) */

#define Stabilizer_GetInterruptSource() Stabilizer_ReadStatusRegister()

#if (Stabilizer_UseControl)
    uint8 Stabilizer_ReadControlRegister(void) ;
    void  Stabilizer_WriteControlRegister(uint8 control)
          ;
#endif /* (Stabilizer_UseControl) */

#if (Stabilizer_UseOneCompareMode)
   #if (Stabilizer_CompareMode1SW)
       void    Stabilizer_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (Stabilizer_CompareMode1SW) */
#else
    #if (Stabilizer_CompareMode1SW)
        void    Stabilizer_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (Stabilizer_CompareMode1SW) */
    #if (Stabilizer_CompareMode2SW)
        void    Stabilizer_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (Stabilizer_CompareMode2SW) */
#endif /* (Stabilizer_UseOneCompareMode) */

#if (!Stabilizer_UsingFixedFunction)
    uint16   Stabilizer_ReadCounter(void) ;
    uint16 Stabilizer_ReadCapture(void) ;

    #if (Stabilizer_UseStatus)
            void Stabilizer_ClearFIFO(void) ;
    #endif /* (Stabilizer_UseStatus) */

    void    Stabilizer_WriteCounter(uint16 counter)
            ;
#endif /* (!Stabilizer_UsingFixedFunction) */

void    Stabilizer_WritePeriod(uint16 period)
        ;
uint16 Stabilizer_ReadPeriod(void) ;

#if (Stabilizer_UseOneCompareMode)
    void    Stabilizer_WriteCompare(uint16 compare)
            ;
    uint16 Stabilizer_ReadCompare(void) ;
#else
    void    Stabilizer_WriteCompare1(uint16 compare)
            ;
    uint16 Stabilizer_ReadCompare1(void) ;
    void    Stabilizer_WriteCompare2(uint16 compare)
            ;
    uint16 Stabilizer_ReadCompare2(void) ;
#endif /* (Stabilizer_UseOneCompareMode) */


#if (Stabilizer_DeadBandUsed)
    void    Stabilizer_WriteDeadTime(uint8 deadtime) ;
    uint8   Stabilizer_ReadDeadTime(void) ;
#endif /* (Stabilizer_DeadBandUsed) */

#if ( Stabilizer_KillModeMinTime)
    void Stabilizer_WriteKillTime(uint8 killtime) ;
    uint8 Stabilizer_ReadKillTime(void) ;
#endif /* ( Stabilizer_KillModeMinTime) */

void Stabilizer_Init(void) ;
void Stabilizer_Enable(void) ;
void Stabilizer_Sleep(void) ;
void Stabilizer_Wakeup(void) ;
void Stabilizer_SaveConfig(void) ;
void Stabilizer_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define Stabilizer_INIT_PERIOD_VALUE          (20000u)
#define Stabilizer_INIT_COMPARE_VALUE1        (1500u)
#define Stabilizer_INIT_COMPARE_VALUE2        (1510u)
#define Stabilizer_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    Stabilizer_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Stabilizer_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Stabilizer_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Stabilizer_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define Stabilizer_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  Stabilizer_CTRL_CMPMODE2_SHIFT)
#define Stabilizer_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  Stabilizer_CTRL_CMPMODE1_SHIFT)
#define Stabilizer_INIT_DEAD_TIME             (9u)


/********************************
*         Registers
******************************** */

#if (Stabilizer_UsingFixedFunction)
   #define Stabilizer_PERIOD_LSB              (*(reg16 *) Stabilizer_PWMHW__PER0)
   #define Stabilizer_PERIOD_LSB_PTR          ( (reg16 *) Stabilizer_PWMHW__PER0)
   #define Stabilizer_COMPARE1_LSB            (*(reg16 *) Stabilizer_PWMHW__CNT_CMP0)
   #define Stabilizer_COMPARE1_LSB_PTR        ( (reg16 *) Stabilizer_PWMHW__CNT_CMP0)
   #define Stabilizer_COMPARE2_LSB            (0x00u)
   #define Stabilizer_COMPARE2_LSB_PTR        (0x00u)
   #define Stabilizer_COUNTER_LSB             (*(reg16 *) Stabilizer_PWMHW__CNT_CMP0)
   #define Stabilizer_COUNTER_LSB_PTR         ( (reg16 *) Stabilizer_PWMHW__CNT_CMP0)
   #define Stabilizer_CAPTURE_LSB             (*(reg16 *) Stabilizer_PWMHW__CAP0)
   #define Stabilizer_CAPTURE_LSB_PTR         ( (reg16 *) Stabilizer_PWMHW__CAP0)
   #define Stabilizer_RT1                     (*(reg8 *)  Stabilizer_PWMHW__RT1)
   #define Stabilizer_RT1_PTR                 ( (reg8 *)  Stabilizer_PWMHW__RT1)

#else
   #if (Stabilizer_Resolution == 8u) /* 8bit - PWM */

       #if(Stabilizer_PWMModeIsCenterAligned)
           #define Stabilizer_PERIOD_LSB      (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define Stabilizer_PERIOD_LSB_PTR  ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define Stabilizer_PERIOD_LSB      (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define Stabilizer_PERIOD_LSB_PTR  ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (Stabilizer_PWMModeIsCenterAligned) */

       #define Stabilizer_COMPARE1_LSB        (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Stabilizer_COMPARE1_LSB_PTR    ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Stabilizer_COMPARE2_LSB        (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Stabilizer_COMPARE2_LSB_PTR    ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Stabilizer_COUNTERCAP_LSB      (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Stabilizer_COUNTERCAP_LSB_PTR  ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Stabilizer_COUNTER_LSB         (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Stabilizer_COUNTER_LSB_PTR     ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Stabilizer_CAPTURE_LSB         (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define Stabilizer_CAPTURE_LSB_PTR     ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(Stabilizer_PWMModeIsCenterAligned)
               #define Stabilizer_PERIOD_LSB      (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define Stabilizer_PERIOD_LSB_PTR  ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define Stabilizer_PERIOD_LSB      (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define Stabilizer_PERIOD_LSB_PTR  ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (Stabilizer_PWMModeIsCenterAligned) */

            #define Stabilizer_COMPARE1_LSB       (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Stabilizer_COMPARE1_LSB_PTR   ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Stabilizer_COMPARE2_LSB       (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Stabilizer_COMPARE2_LSB_PTR   ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Stabilizer_COUNTERCAP_LSB     (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Stabilizer_COUNTERCAP_LSB_PTR ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Stabilizer_COUNTER_LSB        (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Stabilizer_COUNTER_LSB_PTR    ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Stabilizer_CAPTURE_LSB        (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define Stabilizer_CAPTURE_LSB_PTR    ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(Stabilizer_PWMModeIsCenterAligned)
               #define Stabilizer_PERIOD_LSB      (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define Stabilizer_PERIOD_LSB_PTR  ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define Stabilizer_PERIOD_LSB      (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define Stabilizer_PERIOD_LSB_PTR  ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (Stabilizer_PWMModeIsCenterAligned) */

            #define Stabilizer_COMPARE1_LSB       (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Stabilizer_COMPARE1_LSB_PTR   ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Stabilizer_COMPARE2_LSB       (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Stabilizer_COMPARE2_LSB_PTR   ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Stabilizer_COUNTERCAP_LSB     (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Stabilizer_COUNTERCAP_LSB_PTR ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Stabilizer_COUNTER_LSB        (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Stabilizer_COUNTER_LSB_PTR    ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Stabilizer_CAPTURE_LSB        (*(reg16 *) Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define Stabilizer_CAPTURE_LSB_PTR    ((reg16 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define Stabilizer_AUX_CONTROLDP1          (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define Stabilizer_AUX_CONTROLDP1_PTR      ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (Stabilizer_Resolution == 8) */

   #define Stabilizer_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define Stabilizer_AUX_CONTROLDP0          (*(reg8 *)  Stabilizer_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define Stabilizer_AUX_CONTROLDP0_PTR      ((reg8 *)   Stabilizer_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (Stabilizer_UsingFixedFunction) */

#if(Stabilizer_KillModeMinTime )
    #define Stabilizer_KILLMODEMINTIME        (*(reg8 *)  Stabilizer_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define Stabilizer_KILLMODEMINTIME_PTR    ((reg8 *)   Stabilizer_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (Stabilizer_KillModeMinTime ) */

#if(Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_256_CLOCKS)
    #define Stabilizer_DEADBAND_COUNT         (*(reg8 *)  Stabilizer_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Stabilizer_DEADBAND_COUNT_PTR     ((reg8 *)   Stabilizer_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Stabilizer_DEADBAND_LSB_PTR       ((reg8 *)   Stabilizer_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define Stabilizer_DEADBAND_LSB           (*(reg8 *)  Stabilizer_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (Stabilizer_UsingFixedFunction)
        #define Stabilizer_DEADBAND_COUNT         (*(reg8 *)  Stabilizer_PWMHW__CFG0)
        #define Stabilizer_DEADBAND_COUNT_PTR     ((reg8 *)   Stabilizer_PWMHW__CFG0)
        #define Stabilizer_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Stabilizer_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define Stabilizer_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define Stabilizer_DEADBAND_COUNT         (*(reg8 *)  Stabilizer_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Stabilizer_DEADBAND_COUNT_PTR     ((reg8 *)   Stabilizer_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Stabilizer_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Stabilizer_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define Stabilizer_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (Stabilizer_UsingFixedFunction) */
#endif /* (Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_256_CLOCKS) */



#if (Stabilizer_UsingFixedFunction)
    #define Stabilizer_STATUS                 (*(reg8 *) Stabilizer_PWMHW__SR0)
    #define Stabilizer_STATUS_PTR             ((reg8 *) Stabilizer_PWMHW__SR0)
    #define Stabilizer_STATUS_MASK            (*(reg8 *) Stabilizer_PWMHW__SR0)
    #define Stabilizer_STATUS_MASK_PTR        ((reg8 *) Stabilizer_PWMHW__SR0)
    #define Stabilizer_CONTROL                (*(reg8 *) Stabilizer_PWMHW__CFG0)
    #define Stabilizer_CONTROL_PTR            ((reg8 *) Stabilizer_PWMHW__CFG0)
    #define Stabilizer_CONTROL2               (*(reg8 *) Stabilizer_PWMHW__CFG1)
    #define Stabilizer_CONTROL3               (*(reg8 *) Stabilizer_PWMHW__CFG2)
    #define Stabilizer_GLOBAL_ENABLE          (*(reg8 *) Stabilizer_PWMHW__PM_ACT_CFG)
    #define Stabilizer_GLOBAL_ENABLE_PTR      ( (reg8 *) Stabilizer_PWMHW__PM_ACT_CFG)
    #define Stabilizer_GLOBAL_STBY_ENABLE     (*(reg8 *) Stabilizer_PWMHW__PM_STBY_CFG)
    #define Stabilizer_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) Stabilizer_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define Stabilizer_BLOCK_EN_MASK          (Stabilizer_PWMHW__PM_ACT_MSK)
    #define Stabilizer_BLOCK_STBY_EN_MASK     (Stabilizer_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define Stabilizer_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define Stabilizer_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define Stabilizer_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define Stabilizer_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define Stabilizer_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define Stabilizer_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define Stabilizer_CTRL_ENABLE            (uint8)((uint8)0x01u << Stabilizer_CTRL_ENABLE_SHIFT)
    #define Stabilizer_CTRL_RESET             (uint8)((uint8)0x01u << Stabilizer_CTRL_RESET_SHIFT)
    #define Stabilizer_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Stabilizer_CTRL_CMPMODE2_SHIFT)
    #define Stabilizer_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Stabilizer_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Stabilizer_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define Stabilizer_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << Stabilizer_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define Stabilizer_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define Stabilizer_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define Stabilizer_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define Stabilizer_STATUS_TC_INT_EN_MASK_SHIFT            (Stabilizer_STATUS_TC_SHIFT - 4u)
    #define Stabilizer_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define Stabilizer_STATUS_CMP1_INT_EN_MASK_SHIFT          (Stabilizer_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define Stabilizer_STATUS_TC              (uint8)((uint8)0x01u << Stabilizer_STATUS_TC_SHIFT)
    #define Stabilizer_STATUS_CMP1            (uint8)((uint8)0x01u << Stabilizer_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define Stabilizer_STATUS_TC_INT_EN_MASK              (uint8)((uint8)Stabilizer_STATUS_TC >> 4u)
    #define Stabilizer_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)Stabilizer_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define Stabilizer_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define Stabilizer_RT1_MASK              (uint8)((uint8)0x03u << Stabilizer_RT1_SHIFT)
    #define Stabilizer_SYNC                  (uint8)((uint8)0x03u << Stabilizer_RT1_SHIFT)
    #define Stabilizer_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define Stabilizer_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << Stabilizer_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define Stabilizer_SYNCDSI_EN            (uint8)((uint8)0x0Fu << Stabilizer_SYNCDSI_SHIFT)


#else
    #define Stabilizer_STATUS                (*(reg8 *)   Stabilizer_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Stabilizer_STATUS_PTR            ((reg8 *)    Stabilizer_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Stabilizer_STATUS_MASK           (*(reg8 *)   Stabilizer_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Stabilizer_STATUS_MASK_PTR       ((reg8 *)    Stabilizer_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Stabilizer_STATUS_AUX_CTRL       (*(reg8 *)   Stabilizer_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define Stabilizer_CONTROL               (*(reg8 *)   Stabilizer_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define Stabilizer_CONTROL_PTR           ((reg8 *)    Stabilizer_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define Stabilizer_CTRL_ENABLE_SHIFT      (0x07u)
    #define Stabilizer_CTRL_RESET_SHIFT       (0x06u)
    #define Stabilizer_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define Stabilizer_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define Stabilizer_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define Stabilizer_CTRL_ENABLE            (uint8)((uint8)0x01u << Stabilizer_CTRL_ENABLE_SHIFT)
    #define Stabilizer_CTRL_RESET             (uint8)((uint8)0x01u << Stabilizer_CTRL_RESET_SHIFT)
    #define Stabilizer_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Stabilizer_CTRL_CMPMODE2_SHIFT)
    #define Stabilizer_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Stabilizer_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define Stabilizer_STATUS_KILL_SHIFT          (0x05u)
    #define Stabilizer_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define Stabilizer_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define Stabilizer_STATUS_TC_SHIFT            (0x02u)
    #define Stabilizer_STATUS_CMP2_SHIFT          (0x01u)
    #define Stabilizer_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define Stabilizer_STATUS_KILL_INT_EN_MASK_SHIFT          (Stabilizer_STATUS_KILL_SHIFT)
    #define Stabilizer_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (Stabilizer_STATUS_FIFONEMPTY_SHIFT)
    #define Stabilizer_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (Stabilizer_STATUS_FIFOFULL_SHIFT)
    #define Stabilizer_STATUS_TC_INT_EN_MASK_SHIFT            (Stabilizer_STATUS_TC_SHIFT)
    #define Stabilizer_STATUS_CMP2_INT_EN_MASK_SHIFT          (Stabilizer_STATUS_CMP2_SHIFT)
    #define Stabilizer_STATUS_CMP1_INT_EN_MASK_SHIFT          (Stabilizer_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define Stabilizer_STATUS_KILL            (uint8)((uint8)0x00u << Stabilizer_STATUS_KILL_SHIFT )
    #define Stabilizer_STATUS_FIFOFULL        (uint8)((uint8)0x01u << Stabilizer_STATUS_FIFOFULL_SHIFT)
    #define Stabilizer_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << Stabilizer_STATUS_FIFONEMPTY_SHIFT)
    #define Stabilizer_STATUS_TC              (uint8)((uint8)0x01u << Stabilizer_STATUS_TC_SHIFT)
    #define Stabilizer_STATUS_CMP2            (uint8)((uint8)0x01u << Stabilizer_STATUS_CMP2_SHIFT)
    #define Stabilizer_STATUS_CMP1            (uint8)((uint8)0x01u << Stabilizer_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define Stabilizer_STATUS_KILL_INT_EN_MASK            (Stabilizer_STATUS_KILL)
    #define Stabilizer_STATUS_FIFOFULL_INT_EN_MASK        (Stabilizer_STATUS_FIFOFULL)
    #define Stabilizer_STATUS_FIFONEMPTY_INT_EN_MASK      (Stabilizer_STATUS_FIFONEMPTY)
    #define Stabilizer_STATUS_TC_INT_EN_MASK              (Stabilizer_STATUS_TC)
    #define Stabilizer_STATUS_CMP2_INT_EN_MASK            (Stabilizer_STATUS_CMP2)
    #define Stabilizer_STATUS_CMP1_INT_EN_MASK            (Stabilizer_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define Stabilizer_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define Stabilizer_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define Stabilizer_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define Stabilizer_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define Stabilizer_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* Stabilizer_UsingFixedFunction */

#endif  /* CY_PWM_Stabilizer_H */


/* [] END OF FILE */
