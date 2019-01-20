/*******************************************************************************
* File Name: Aileron.h
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

#if !defined(CY_PWM_Aileron_H)
#define CY_PWM_Aileron_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Aileron_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define Aileron_Resolution                     (16u)
#define Aileron_UsingFixedFunction             (0u)
#define Aileron_DeadBandMode                   (0u)
#define Aileron_KillModeMinTime                (0u)
#define Aileron_KillMode                       (0u)
#define Aileron_PWMMode                        (1u)
#define Aileron_PWMModeIsCenterAligned         (0u)
#define Aileron_DeadBandUsed                   (0u)
#define Aileron_DeadBand2_4                    (0u)

#if !defined(Aileron_PWMUDB_genblk8_stsreg__REMOVED)
    #define Aileron_UseStatus                  (1u)
#else
    #define Aileron_UseStatus                  (0u)
#endif /* !defined(Aileron_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(Aileron_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define Aileron_UseControl                 (1u)
#else
    #define Aileron_UseControl                 (0u)
#endif /* !defined(Aileron_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define Aileron_UseOneCompareMode              (0u)
#define Aileron_MinimumKillTime                (1u)
#define Aileron_EnableMode                     (0u)

#define Aileron_CompareMode1SW                 (0u)
#define Aileron_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define Aileron__B_PWM__DISABLED 0
#define Aileron__B_PWM__ASYNCHRONOUS 1
#define Aileron__B_PWM__SINGLECYCLE 2
#define Aileron__B_PWM__LATCHED 3
#define Aileron__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define Aileron__B_PWM__DBMDISABLED 0
#define Aileron__B_PWM__DBM_2_4_CLOCKS 1
#define Aileron__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define Aileron__B_PWM__ONE_OUTPUT 0
#define Aileron__B_PWM__TWO_OUTPUTS 1
#define Aileron__B_PWM__DUAL_EDGE 2
#define Aileron__B_PWM__CENTER_ALIGN 3
#define Aileron__B_PWM__DITHER 5
#define Aileron__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define Aileron__B_PWM__LESS_THAN 1
#define Aileron__B_PWM__LESS_THAN_OR_EQUAL 2
#define Aileron__B_PWM__GREATER_THAN 3
#define Aileron__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define Aileron__B_PWM__EQUAL 0
#define Aileron__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!Aileron_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!Aileron_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!Aileron_PWMModeIsCenterAligned) */
        #if (Aileron_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (Aileron_UseStatus) */

        /* Backup for Deadband parameters */
        #if(Aileron_DeadBandMode == Aileron__B_PWM__DBM_256_CLOCKS || \
            Aileron_DeadBandMode == Aileron__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(Aileron_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (Aileron_KillModeMinTime) */

        /* Backup control register */
        #if(Aileron_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (Aileron_UseControl) */

    #endif /* (!Aileron_UsingFixedFunction) */

}Aileron_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    Aileron_Start(void) ;
void    Aileron_Stop(void) ;

#if (Aileron_UseStatus || Aileron_UsingFixedFunction)
    void  Aileron_SetInterruptMode(uint8 interruptMode) ;
    uint8 Aileron_ReadStatusRegister(void) ;
#endif /* (Aileron_UseStatus || Aileron_UsingFixedFunction) */

#define Aileron_GetInterruptSource() Aileron_ReadStatusRegister()

#if (Aileron_UseControl)
    uint8 Aileron_ReadControlRegister(void) ;
    void  Aileron_WriteControlRegister(uint8 control)
          ;
#endif /* (Aileron_UseControl) */

#if (Aileron_UseOneCompareMode)
   #if (Aileron_CompareMode1SW)
       void    Aileron_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (Aileron_CompareMode1SW) */
#else
    #if (Aileron_CompareMode1SW)
        void    Aileron_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (Aileron_CompareMode1SW) */
    #if (Aileron_CompareMode2SW)
        void    Aileron_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (Aileron_CompareMode2SW) */
#endif /* (Aileron_UseOneCompareMode) */

#if (!Aileron_UsingFixedFunction)
    uint16   Aileron_ReadCounter(void) ;
    uint16 Aileron_ReadCapture(void) ;

    #if (Aileron_UseStatus)
            void Aileron_ClearFIFO(void) ;
    #endif /* (Aileron_UseStatus) */

    void    Aileron_WriteCounter(uint16 counter)
            ;
#endif /* (!Aileron_UsingFixedFunction) */

void    Aileron_WritePeriod(uint16 period)
        ;
uint16 Aileron_ReadPeriod(void) ;

#if (Aileron_UseOneCompareMode)
    void    Aileron_WriteCompare(uint16 compare)
            ;
    uint16 Aileron_ReadCompare(void) ;
#else
    void    Aileron_WriteCompare1(uint16 compare)
            ;
    uint16 Aileron_ReadCompare1(void) ;
    void    Aileron_WriteCompare2(uint16 compare)
            ;
    uint16 Aileron_ReadCompare2(void) ;
#endif /* (Aileron_UseOneCompareMode) */


#if (Aileron_DeadBandUsed)
    void    Aileron_WriteDeadTime(uint8 deadtime) ;
    uint8   Aileron_ReadDeadTime(void) ;
#endif /* (Aileron_DeadBandUsed) */

#if ( Aileron_KillModeMinTime)
    void Aileron_WriteKillTime(uint8 killtime) ;
    uint8 Aileron_ReadKillTime(void) ;
#endif /* ( Aileron_KillModeMinTime) */

void Aileron_Init(void) ;
void Aileron_Enable(void) ;
void Aileron_Sleep(void) ;
void Aileron_Wakeup(void) ;
void Aileron_SaveConfig(void) ;
void Aileron_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define Aileron_INIT_PERIOD_VALUE          (20000u)
#define Aileron_INIT_COMPARE_VALUE1        (1500u)
#define Aileron_INIT_COMPARE_VALUE2        (1505u)
#define Aileron_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    Aileron_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Aileron_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Aileron_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    Aileron_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define Aileron_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  Aileron_CTRL_CMPMODE2_SHIFT)
#define Aileron_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  Aileron_CTRL_CMPMODE1_SHIFT)
#define Aileron_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (Aileron_UsingFixedFunction)
   #define Aileron_PERIOD_LSB              (*(reg16 *) Aileron_PWMHW__PER0)
   #define Aileron_PERIOD_LSB_PTR          ( (reg16 *) Aileron_PWMHW__PER0)
   #define Aileron_COMPARE1_LSB            (*(reg16 *) Aileron_PWMHW__CNT_CMP0)
   #define Aileron_COMPARE1_LSB_PTR        ( (reg16 *) Aileron_PWMHW__CNT_CMP0)
   #define Aileron_COMPARE2_LSB            (0x00u)
   #define Aileron_COMPARE2_LSB_PTR        (0x00u)
   #define Aileron_COUNTER_LSB             (*(reg16 *) Aileron_PWMHW__CNT_CMP0)
   #define Aileron_COUNTER_LSB_PTR         ( (reg16 *) Aileron_PWMHW__CNT_CMP0)
   #define Aileron_CAPTURE_LSB             (*(reg16 *) Aileron_PWMHW__CAP0)
   #define Aileron_CAPTURE_LSB_PTR         ( (reg16 *) Aileron_PWMHW__CAP0)
   #define Aileron_RT1                     (*(reg8 *)  Aileron_PWMHW__RT1)
   #define Aileron_RT1_PTR                 ( (reg8 *)  Aileron_PWMHW__RT1)

#else
   #if (Aileron_Resolution == 8u) /* 8bit - PWM */

       #if(Aileron_PWMModeIsCenterAligned)
           #define Aileron_PERIOD_LSB      (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define Aileron_PERIOD_LSB_PTR  ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define Aileron_PERIOD_LSB      (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define Aileron_PERIOD_LSB_PTR  ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (Aileron_PWMModeIsCenterAligned) */

       #define Aileron_COMPARE1_LSB        (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Aileron_COMPARE1_LSB_PTR    ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define Aileron_COMPARE2_LSB        (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Aileron_COMPARE2_LSB_PTR    ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define Aileron_COUNTERCAP_LSB      (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Aileron_COUNTERCAP_LSB_PTR  ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define Aileron_COUNTER_LSB         (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Aileron_COUNTER_LSB_PTR     ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define Aileron_CAPTURE_LSB         (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define Aileron_CAPTURE_LSB_PTR     ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(Aileron_PWMModeIsCenterAligned)
               #define Aileron_PERIOD_LSB      (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define Aileron_PERIOD_LSB_PTR  ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define Aileron_PERIOD_LSB      (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define Aileron_PERIOD_LSB_PTR  ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (Aileron_PWMModeIsCenterAligned) */

            #define Aileron_COMPARE1_LSB       (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Aileron_COMPARE1_LSB_PTR   ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define Aileron_COMPARE2_LSB       (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Aileron_COMPARE2_LSB_PTR   ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define Aileron_COUNTERCAP_LSB     (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Aileron_COUNTERCAP_LSB_PTR ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define Aileron_COUNTER_LSB        (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Aileron_COUNTER_LSB_PTR    ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define Aileron_CAPTURE_LSB        (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define Aileron_CAPTURE_LSB_PTR    ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(Aileron_PWMModeIsCenterAligned)
               #define Aileron_PERIOD_LSB      (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define Aileron_PERIOD_LSB_PTR  ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define Aileron_PERIOD_LSB      (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define Aileron_PERIOD_LSB_PTR  ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (Aileron_PWMModeIsCenterAligned) */

            #define Aileron_COMPARE1_LSB       (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Aileron_COMPARE1_LSB_PTR   ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define Aileron_COMPARE2_LSB       (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Aileron_COMPARE2_LSB_PTR   ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define Aileron_COUNTERCAP_LSB     (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Aileron_COUNTERCAP_LSB_PTR ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define Aileron_COUNTER_LSB        (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Aileron_COUNTER_LSB_PTR    ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define Aileron_CAPTURE_LSB        (*(reg16 *) Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define Aileron_CAPTURE_LSB_PTR    ((reg16 *)  Aileron_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define Aileron_AUX_CONTROLDP1          (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define Aileron_AUX_CONTROLDP1_PTR      ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (Aileron_Resolution == 8) */

   #define Aileron_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define Aileron_AUX_CONTROLDP0          (*(reg8 *)  Aileron_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define Aileron_AUX_CONTROLDP0_PTR      ((reg8 *)   Aileron_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (Aileron_UsingFixedFunction) */

#if(Aileron_KillModeMinTime )
    #define Aileron_KILLMODEMINTIME        (*(reg8 *)  Aileron_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define Aileron_KILLMODEMINTIME_PTR    ((reg8 *)   Aileron_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (Aileron_KillModeMinTime ) */

#if(Aileron_DeadBandMode == Aileron__B_PWM__DBM_256_CLOCKS)
    #define Aileron_DEADBAND_COUNT         (*(reg8 *)  Aileron_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Aileron_DEADBAND_COUNT_PTR     ((reg8 *)   Aileron_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define Aileron_DEADBAND_LSB_PTR       ((reg8 *)   Aileron_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define Aileron_DEADBAND_LSB           (*(reg8 *)  Aileron_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(Aileron_DeadBandMode == Aileron__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (Aileron_UsingFixedFunction)
        #define Aileron_DEADBAND_COUNT         (*(reg8 *)  Aileron_PWMHW__CFG0)
        #define Aileron_DEADBAND_COUNT_PTR     ((reg8 *)   Aileron_PWMHW__CFG0)
        #define Aileron_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Aileron_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define Aileron_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define Aileron_DEADBAND_COUNT         (*(reg8 *)  Aileron_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Aileron_DEADBAND_COUNT_PTR     ((reg8 *)   Aileron_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define Aileron_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << Aileron_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define Aileron_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (Aileron_UsingFixedFunction) */
#endif /* (Aileron_DeadBandMode == Aileron__B_PWM__DBM_256_CLOCKS) */



#if (Aileron_UsingFixedFunction)
    #define Aileron_STATUS                 (*(reg8 *) Aileron_PWMHW__SR0)
    #define Aileron_STATUS_PTR             ((reg8 *) Aileron_PWMHW__SR0)
    #define Aileron_STATUS_MASK            (*(reg8 *) Aileron_PWMHW__SR0)
    #define Aileron_STATUS_MASK_PTR        ((reg8 *) Aileron_PWMHW__SR0)
    #define Aileron_CONTROL                (*(reg8 *) Aileron_PWMHW__CFG0)
    #define Aileron_CONTROL_PTR            ((reg8 *) Aileron_PWMHW__CFG0)
    #define Aileron_CONTROL2               (*(reg8 *) Aileron_PWMHW__CFG1)
    #define Aileron_CONTROL3               (*(reg8 *) Aileron_PWMHW__CFG2)
    #define Aileron_GLOBAL_ENABLE          (*(reg8 *) Aileron_PWMHW__PM_ACT_CFG)
    #define Aileron_GLOBAL_ENABLE_PTR      ( (reg8 *) Aileron_PWMHW__PM_ACT_CFG)
    #define Aileron_GLOBAL_STBY_ENABLE     (*(reg8 *) Aileron_PWMHW__PM_STBY_CFG)
    #define Aileron_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) Aileron_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define Aileron_BLOCK_EN_MASK          (Aileron_PWMHW__PM_ACT_MSK)
    #define Aileron_BLOCK_STBY_EN_MASK     (Aileron_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define Aileron_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define Aileron_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define Aileron_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define Aileron_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define Aileron_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define Aileron_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define Aileron_CTRL_ENABLE            (uint8)((uint8)0x01u << Aileron_CTRL_ENABLE_SHIFT)
    #define Aileron_CTRL_RESET             (uint8)((uint8)0x01u << Aileron_CTRL_RESET_SHIFT)
    #define Aileron_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Aileron_CTRL_CMPMODE2_SHIFT)
    #define Aileron_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Aileron_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Aileron_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define Aileron_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << Aileron_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define Aileron_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define Aileron_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define Aileron_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define Aileron_STATUS_TC_INT_EN_MASK_SHIFT            (Aileron_STATUS_TC_SHIFT - 4u)
    #define Aileron_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define Aileron_STATUS_CMP1_INT_EN_MASK_SHIFT          (Aileron_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define Aileron_STATUS_TC              (uint8)((uint8)0x01u << Aileron_STATUS_TC_SHIFT)
    #define Aileron_STATUS_CMP1            (uint8)((uint8)0x01u << Aileron_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define Aileron_STATUS_TC_INT_EN_MASK              (uint8)((uint8)Aileron_STATUS_TC >> 4u)
    #define Aileron_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)Aileron_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define Aileron_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define Aileron_RT1_MASK              (uint8)((uint8)0x03u << Aileron_RT1_SHIFT)
    #define Aileron_SYNC                  (uint8)((uint8)0x03u << Aileron_RT1_SHIFT)
    #define Aileron_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define Aileron_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << Aileron_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define Aileron_SYNCDSI_EN            (uint8)((uint8)0x0Fu << Aileron_SYNCDSI_SHIFT)


#else
    #define Aileron_STATUS                (*(reg8 *)   Aileron_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Aileron_STATUS_PTR            ((reg8 *)    Aileron_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define Aileron_STATUS_MASK           (*(reg8 *)   Aileron_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Aileron_STATUS_MASK_PTR       ((reg8 *)    Aileron_PWMUDB_genblk8_stsreg__MASK_REG)
    #define Aileron_STATUS_AUX_CTRL       (*(reg8 *)   Aileron_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define Aileron_CONTROL               (*(reg8 *)   Aileron_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define Aileron_CONTROL_PTR           ((reg8 *)    Aileron_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define Aileron_CTRL_ENABLE_SHIFT      (0x07u)
    #define Aileron_CTRL_RESET_SHIFT       (0x06u)
    #define Aileron_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define Aileron_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define Aileron_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define Aileron_CTRL_ENABLE            (uint8)((uint8)0x01u << Aileron_CTRL_ENABLE_SHIFT)
    #define Aileron_CTRL_RESET             (uint8)((uint8)0x01u << Aileron_CTRL_RESET_SHIFT)
    #define Aileron_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << Aileron_CTRL_CMPMODE2_SHIFT)
    #define Aileron_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << Aileron_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define Aileron_STATUS_KILL_SHIFT          (0x05u)
    #define Aileron_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define Aileron_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define Aileron_STATUS_TC_SHIFT            (0x02u)
    #define Aileron_STATUS_CMP2_SHIFT          (0x01u)
    #define Aileron_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define Aileron_STATUS_KILL_INT_EN_MASK_SHIFT          (Aileron_STATUS_KILL_SHIFT)
    #define Aileron_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (Aileron_STATUS_FIFONEMPTY_SHIFT)
    #define Aileron_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (Aileron_STATUS_FIFOFULL_SHIFT)
    #define Aileron_STATUS_TC_INT_EN_MASK_SHIFT            (Aileron_STATUS_TC_SHIFT)
    #define Aileron_STATUS_CMP2_INT_EN_MASK_SHIFT          (Aileron_STATUS_CMP2_SHIFT)
    #define Aileron_STATUS_CMP1_INT_EN_MASK_SHIFT          (Aileron_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define Aileron_STATUS_KILL            (uint8)((uint8)0x00u << Aileron_STATUS_KILL_SHIFT )
    #define Aileron_STATUS_FIFOFULL        (uint8)((uint8)0x01u << Aileron_STATUS_FIFOFULL_SHIFT)
    #define Aileron_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << Aileron_STATUS_FIFONEMPTY_SHIFT)
    #define Aileron_STATUS_TC              (uint8)((uint8)0x01u << Aileron_STATUS_TC_SHIFT)
    #define Aileron_STATUS_CMP2            (uint8)((uint8)0x01u << Aileron_STATUS_CMP2_SHIFT)
    #define Aileron_STATUS_CMP1            (uint8)((uint8)0x01u << Aileron_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define Aileron_STATUS_KILL_INT_EN_MASK            (Aileron_STATUS_KILL)
    #define Aileron_STATUS_FIFOFULL_INT_EN_MASK        (Aileron_STATUS_FIFOFULL)
    #define Aileron_STATUS_FIFONEMPTY_INT_EN_MASK      (Aileron_STATUS_FIFONEMPTY)
    #define Aileron_STATUS_TC_INT_EN_MASK              (Aileron_STATUS_TC)
    #define Aileron_STATUS_CMP2_INT_EN_MASK            (Aileron_STATUS_CMP2)
    #define Aileron_STATUS_CMP1_INT_EN_MASK            (Aileron_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define Aileron_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define Aileron_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define Aileron_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define Aileron_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define Aileron_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* Aileron_UsingFixedFunction */

#endif  /* CY_PWM_Aileron_H */


/* [] END OF FILE */
