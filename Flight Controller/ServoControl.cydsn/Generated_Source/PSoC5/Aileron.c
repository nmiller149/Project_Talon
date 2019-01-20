/*******************************************************************************
* File Name: Aileron.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Aileron.h"

/* Error message for removed <resource> through optimization */
#ifdef Aileron_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* Aileron_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 Aileron_initVar = 0u;


/*******************************************************************************
* Function Name: Aileron_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Aileron_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Aileron_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Aileron_initVar == 0u)
    {
        Aileron_Init();
        Aileron_initVar = 1u;
    }
    Aileron_Enable();

}


/*******************************************************************************
* Function Name: Aileron_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Aileron_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Aileron_Init(void) 
{
    #if (Aileron_UsingFixedFunction || Aileron_UseControl)
        uint8 ctrl;
    #endif /* (Aileron_UsingFixedFunction || Aileron_UseControl) */

    #if(!Aileron_UsingFixedFunction)
        #if(Aileron_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Aileron_interruptState;
        #endif /* (Aileron_UseStatus) */
    #endif /* (!Aileron_UsingFixedFunction) */

    #if (Aileron_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Aileron_CONTROL |= Aileron_CFG0_MODE;
        #if (Aileron_DeadBand2_4)
            Aileron_CONTROL |= Aileron_CFG0_DB;
        #endif /* (Aileron_DeadBand2_4) */

        ctrl = Aileron_CONTROL3 & ((uint8 )(~Aileron_CTRL_CMPMODE1_MASK));
        Aileron_CONTROL3 = ctrl | Aileron_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Aileron_RT1 &= ((uint8)(~Aileron_RT1_MASK));
        Aileron_RT1 |= Aileron_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        Aileron_RT1 &= ((uint8)(~Aileron_SYNCDSI_MASK));
        Aileron_RT1 |= Aileron_SYNCDSI_EN;

    #elif (Aileron_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Aileron_CONTROL & ((uint8)(~Aileron_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~Aileron_CTRL_CMPMODE1_MASK));
        Aileron_CONTROL = ctrl | Aileron_DEFAULT_COMPARE2_MODE |
                                   Aileron_DEFAULT_COMPARE1_MODE;
    #endif /* (Aileron_UsingFixedFunction) */

    #if (!Aileron_UsingFixedFunction)
        #if (Aileron_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Aileron_AUX_CONTROLDP0 |= (Aileron_AUX_CTRL_FIFO0_CLR);
        #else /* (Aileron_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Aileron_AUX_CONTROLDP0 |= (Aileron_AUX_CTRL_FIFO0_CLR);
            Aileron_AUX_CONTROLDP1 |= (Aileron_AUX_CTRL_FIFO0_CLR);
        #endif /* (Aileron_Resolution == 8) */

        Aileron_WriteCounter(Aileron_INIT_PERIOD_VALUE);
    #endif /* (!Aileron_UsingFixedFunction) */

    Aileron_WritePeriod(Aileron_INIT_PERIOD_VALUE);

        #if (Aileron_UseOneCompareMode)
            Aileron_WriteCompare(Aileron_INIT_COMPARE_VALUE1);
        #else
            Aileron_WriteCompare1(Aileron_INIT_COMPARE_VALUE1);
            Aileron_WriteCompare2(Aileron_INIT_COMPARE_VALUE2);
        #endif /* (Aileron_UseOneCompareMode) */

        #if (Aileron_KillModeMinTime)
            Aileron_WriteKillTime(Aileron_MinimumKillTime);
        #endif /* (Aileron_KillModeMinTime) */

        #if (Aileron_DeadBandUsed)
            Aileron_WriteDeadTime(Aileron_INIT_DEAD_TIME);
        #endif /* (Aileron_DeadBandUsed) */

    #if (Aileron_UseStatus || Aileron_UsingFixedFunction)
        Aileron_SetInterruptMode(Aileron_INIT_INTERRUPTS_MODE);
    #endif /* (Aileron_UseStatus || Aileron_UsingFixedFunction) */

    #if (Aileron_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Aileron_GLOBAL_ENABLE |= Aileron_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Aileron_CONTROL2 |= Aileron_CTRL2_IRQ_SEL;
    #else
        #if(Aileron_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Aileron_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Aileron_STATUS_AUX_CTRL |= Aileron_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(Aileron_interruptState);

            /* Clear the FIFO to enable the Aileron_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Aileron_ClearFIFO();
        #endif /* (Aileron_UseStatus) */
    #endif /* (Aileron_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Aileron_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void Aileron_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Aileron_UsingFixedFunction)
        Aileron_GLOBAL_ENABLE |= Aileron_BLOCK_EN_MASK;
        Aileron_GLOBAL_STBY_ENABLE |= Aileron_BLOCK_STBY_EN_MASK;
    #endif /* (Aileron_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (Aileron_UseControl || Aileron_UsingFixedFunction)
        Aileron_CONTROL |= Aileron_CTRL_ENABLE;
    #endif /* (Aileron_UseControl || Aileron_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Aileron_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void Aileron_Stop(void) 
{
    #if (Aileron_UseControl || Aileron_UsingFixedFunction)
        Aileron_CONTROL &= ((uint8)(~Aileron_CTRL_ENABLE));
    #endif /* (Aileron_UseControl || Aileron_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (Aileron_UsingFixedFunction)
        Aileron_GLOBAL_ENABLE &= ((uint8)(~Aileron_BLOCK_EN_MASK));
        Aileron_GLOBAL_STBY_ENABLE &= ((uint8)(~Aileron_BLOCK_STBY_EN_MASK));
    #endif /* (Aileron_UsingFixedFunction) */
}

#if (Aileron_UseOneCompareMode)
    #if (Aileron_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Aileron_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Aileron_SetCompareMode(uint8 comparemode) 
        {
            #if(Aileron_UsingFixedFunction)

                #if(0 != Aileron_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Aileron_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Aileron_CTRL_CMPMODE1_SHIFT) */

                Aileron_CONTROL3 &= ((uint8)(~Aileron_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                Aileron_CONTROL3 |= comparemodemasked;

            #elif (Aileron_UseControl)

                #if(0 != Aileron_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Aileron_CTRL_CMPMODE1_SHIFT)) &
                                                Aileron_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Aileron_CTRL_CMPMODE1_MASK;
                #endif /* (0 != Aileron_CTRL_CMPMODE1_SHIFT) */

                #if(0 != Aileron_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << Aileron_CTRL_CMPMODE2_SHIFT)) &
                                               Aileron_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & Aileron_CTRL_CMPMODE2_MASK;
                #endif /* (0 != Aileron_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                Aileron_CONTROL &= ((uint8)(~(Aileron_CTRL_CMPMODE1_MASK |
                                            Aileron_CTRL_CMPMODE2_MASK)));
                Aileron_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (Aileron_UsingFixedFunction) */
        }
    #endif /* Aileron_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (Aileron_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Aileron_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Aileron_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != Aileron_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Aileron_CTRL_CMPMODE1_SHIFT)) &
                                           Aileron_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & Aileron_CTRL_CMPMODE1_MASK;
            #endif /* (0 != Aileron_CTRL_CMPMODE1_SHIFT) */

            #if (Aileron_UseControl)
                Aileron_CONTROL &= ((uint8)(~Aileron_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                Aileron_CONTROL |= comparemodemasked;
            #endif /* (Aileron_UseControl) */
        }
    #endif /* Aileron_CompareMode1SW */

#if (Aileron_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Aileron_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Aileron_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Aileron_CTRL_CMPMODE2_SHIFT)) &
                                                 Aileron_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Aileron_CTRL_CMPMODE2_MASK;
        #endif /* (0 != Aileron_CTRL_CMPMODE2_SHIFT) */

        #if (Aileron_UseControl)
            Aileron_CONTROL &= ((uint8)(~Aileron_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Aileron_CONTROL |= comparemodemasked;
        #endif /* (Aileron_UseControl) */
    }
    #endif /*Aileron_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Aileron_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Aileron_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void Aileron_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(Aileron_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Aileron_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 Aileron_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(Aileron_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(Aileron_CAPTURE_LSB_PTR));
    }

    #if (Aileron_UseStatus)


        /*******************************************************************************
        * Function Name: Aileron_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Aileron_ClearFIFO(void) 
        {
            while(0u != (Aileron_ReadStatusRegister() & Aileron_STATUS_FIFONEMPTY))
            {
                (void)Aileron_ReadCapture();
            }
        }

    #endif /* Aileron_UseStatus */

#endif /* !Aileron_UsingFixedFunction */


/*******************************************************************************
* Function Name: Aileron_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void Aileron_WritePeriod(uint16 period) 
{
    #if(Aileron_UsingFixedFunction)
        CY_SET_REG16(Aileron_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(Aileron_PERIOD_LSB_PTR, period);
    #endif /* (Aileron_UsingFixedFunction) */
}

#if (Aileron_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Aileron_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void Aileron_WriteCompare(uint16 compare) \
                                       
    {
        #if(Aileron_UsingFixedFunction)
            CY_SET_REG16(Aileron_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Aileron_COMPARE1_LSB_PTR, compare);
        #endif /* (Aileron_UsingFixedFunction) */

        #if (Aileron_PWMMode == Aileron__B_PWM__DITHER)
            #if(Aileron_UsingFixedFunction)
                CY_SET_REG16(Aileron_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(Aileron_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (Aileron_UsingFixedFunction) */
        #endif /* (Aileron_PWMMode == Aileron__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Aileron_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_WriteCompare1(uint16 compare) \
                                        
    {
        #if(Aileron_UsingFixedFunction)
            CY_SET_REG16(Aileron_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(Aileron_COMPARE1_LSB_PTR, compare);
        #endif /* (Aileron_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Aileron_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_WriteCompare2(uint16 compare) \
                                        
    {
        #if(Aileron_UsingFixedFunction)
            CY_SET_REG16(Aileron_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(Aileron_COMPARE2_LSB_PTR, compare);
        #endif /* (Aileron_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Aileron_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Aileron_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Aileron_DeadBand2_4)
            CY_SET_REG8(Aileron_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            Aileron_DEADBAND_COUNT &= ((uint8)(~Aileron_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(Aileron_DEADBAND_COUNT_SHIFT)
                Aileron_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Aileron_DEADBAND_COUNT_SHIFT)) &
                                                    Aileron_DEADBAND_COUNT_MASK;
            #else
                Aileron_DEADBAND_COUNT |= deadtime & Aileron_DEADBAND_COUNT_MASK;
            #endif /* (Aileron_DEADBAND_COUNT_SHIFT) */

        #endif /* (!Aileron_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Aileron_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 Aileron_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Aileron_DeadBand2_4)
            return (CY_GET_REG8(Aileron_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(Aileron_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(Aileron_DEADBAND_COUNT & Aileron_DEADBAND_COUNT_MASK)) >>
                                                                           Aileron_DEADBAND_COUNT_SHIFT));
            #else
                return (Aileron_DEADBAND_COUNT & Aileron_DEADBAND_COUNT_MASK);
            #endif /* (Aileron_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Aileron_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Aileron_UseStatus || Aileron_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Aileron_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(Aileron_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Aileron_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 Aileron_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(Aileron_STATUS_PTR));
    }

#endif /* (Aileron_UseStatus || Aileron_UsingFixedFunction) */


#if (Aileron_UseControl)


    /*******************************************************************************
    * Function Name: Aileron_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 Aileron_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(Aileron_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: Aileron_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(Aileron_CONTROL_PTR, control);
    }

#endif /* (Aileron_UseControl) */


#if (!Aileron_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Aileron_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 Aileron_ReadCapture(void) 
    {
        return (CY_GET_REG16(Aileron_CAPTURE_LSB_PTR));
    }

#endif /* (!Aileron_UsingFixedFunction) */


#if (Aileron_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Aileron_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 Aileron_ReadCompare(void) 
    {
        #if(Aileron_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(Aileron_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(Aileron_COMPARE1_LSB_PTR));
        #endif /* (Aileron_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Aileron_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 Aileron_ReadCompare1(void) 
    {
        return (CY_GET_REG16(Aileron_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Aileron_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 Aileron_ReadCompare2(void) 
    {
        return (CY_GET_REG16(Aileron_COMPARE2_LSB_PTR));
    }

#endif /* (Aileron_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Aileron_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 Aileron_ReadPeriod(void) 
{
    #if(Aileron_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(Aileron_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(Aileron_PERIOD_LSB_PTR));
    #endif /* (Aileron_UsingFixedFunction) */
}

#if ( Aileron_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Aileron_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Aileron_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(Aileron_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Aileron_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 Aileron_ReadKillTime(void) 
    {
        return (CY_GET_REG8(Aileron_KILLMODEMINTIME_PTR));
    }

#endif /* ( Aileron_KillModeMinTime) */

/* [] END OF FILE */
