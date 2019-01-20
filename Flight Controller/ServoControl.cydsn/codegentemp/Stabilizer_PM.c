/*******************************************************************************
* File Name: Stabilizer_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Stabilizer.h"

static Stabilizer_backupStruct Stabilizer_backup;


/*******************************************************************************
* Function Name: Stabilizer_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Stabilizer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Stabilizer_SaveConfig(void) 
{

    #if(!Stabilizer_UsingFixedFunction)
        #if(!Stabilizer_PWMModeIsCenterAligned)
            Stabilizer_backup.PWMPeriod = Stabilizer_ReadPeriod();
        #endif /* (!Stabilizer_PWMModeIsCenterAligned) */
        Stabilizer_backup.PWMUdb = Stabilizer_ReadCounter();
        #if (Stabilizer_UseStatus)
            Stabilizer_backup.InterruptMaskValue = Stabilizer_STATUS_MASK;
        #endif /* (Stabilizer_UseStatus) */

        #if(Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_256_CLOCKS || \
            Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_2_4_CLOCKS)
            Stabilizer_backup.PWMdeadBandValue = Stabilizer_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Stabilizer_KillModeMinTime)
             Stabilizer_backup.PWMKillCounterPeriod = Stabilizer_ReadKillTime();
        #endif /* (Stabilizer_KillModeMinTime) */

        #if(Stabilizer_UseControl)
            Stabilizer_backup.PWMControlRegister = Stabilizer_ReadControlRegister();
        #endif /* (Stabilizer_UseControl) */
    #endif  /* (!Stabilizer_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Stabilizer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Stabilizer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Stabilizer_RestoreConfig(void) 
{
        #if(!Stabilizer_UsingFixedFunction)
            #if(!Stabilizer_PWMModeIsCenterAligned)
                Stabilizer_WritePeriod(Stabilizer_backup.PWMPeriod);
            #endif /* (!Stabilizer_PWMModeIsCenterAligned) */

            Stabilizer_WriteCounter(Stabilizer_backup.PWMUdb);

            #if (Stabilizer_UseStatus)
                Stabilizer_STATUS_MASK = Stabilizer_backup.InterruptMaskValue;
            #endif /* (Stabilizer_UseStatus) */

            #if(Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_256_CLOCKS || \
                Stabilizer_DeadBandMode == Stabilizer__B_PWM__DBM_2_4_CLOCKS)
                Stabilizer_WriteDeadTime(Stabilizer_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Stabilizer_KillModeMinTime)
                Stabilizer_WriteKillTime(Stabilizer_backup.PWMKillCounterPeriod);
            #endif /* (Stabilizer_KillModeMinTime) */

            #if(Stabilizer_UseControl)
                Stabilizer_WriteControlRegister(Stabilizer_backup.PWMControlRegister);
            #endif /* (Stabilizer_UseControl) */
        #endif  /* (!Stabilizer_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Stabilizer_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Stabilizer_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Stabilizer_Sleep(void) 
{
    #if(Stabilizer_UseControl)
        if(Stabilizer_CTRL_ENABLE == (Stabilizer_CONTROL & Stabilizer_CTRL_ENABLE))
        {
            /*Component is enabled */
            Stabilizer_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Stabilizer_backup.PWMEnableState = 0u;
        }
    #endif /* (Stabilizer_UseControl) */

    /* Stop component */
    Stabilizer_Stop();

    /* Save registers configuration */
    Stabilizer_SaveConfig();
}


/*******************************************************************************
* Function Name: Stabilizer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Stabilizer_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Stabilizer_Wakeup(void) 
{
     /* Restore registers values */
    Stabilizer_RestoreConfig();

    if(Stabilizer_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Stabilizer_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
