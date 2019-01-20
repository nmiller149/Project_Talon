/*******************************************************************************
* File Name: Aileron_PM.c
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

#include "Aileron.h"

static Aileron_backupStruct Aileron_backup;


/*******************************************************************************
* Function Name: Aileron_SaveConfig
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
*  Aileron_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Aileron_SaveConfig(void) 
{

    #if(!Aileron_UsingFixedFunction)
        #if(!Aileron_PWMModeIsCenterAligned)
            Aileron_backup.PWMPeriod = Aileron_ReadPeriod();
        #endif /* (!Aileron_PWMModeIsCenterAligned) */
        Aileron_backup.PWMUdb = Aileron_ReadCounter();
        #if (Aileron_UseStatus)
            Aileron_backup.InterruptMaskValue = Aileron_STATUS_MASK;
        #endif /* (Aileron_UseStatus) */

        #if(Aileron_DeadBandMode == Aileron__B_PWM__DBM_256_CLOCKS || \
            Aileron_DeadBandMode == Aileron__B_PWM__DBM_2_4_CLOCKS)
            Aileron_backup.PWMdeadBandValue = Aileron_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Aileron_KillModeMinTime)
             Aileron_backup.PWMKillCounterPeriod = Aileron_ReadKillTime();
        #endif /* (Aileron_KillModeMinTime) */

        #if(Aileron_UseControl)
            Aileron_backup.PWMControlRegister = Aileron_ReadControlRegister();
        #endif /* (Aileron_UseControl) */
    #endif  /* (!Aileron_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Aileron_RestoreConfig
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
*  Aileron_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Aileron_RestoreConfig(void) 
{
        #if(!Aileron_UsingFixedFunction)
            #if(!Aileron_PWMModeIsCenterAligned)
                Aileron_WritePeriod(Aileron_backup.PWMPeriod);
            #endif /* (!Aileron_PWMModeIsCenterAligned) */

            Aileron_WriteCounter(Aileron_backup.PWMUdb);

            #if (Aileron_UseStatus)
                Aileron_STATUS_MASK = Aileron_backup.InterruptMaskValue;
            #endif /* (Aileron_UseStatus) */

            #if(Aileron_DeadBandMode == Aileron__B_PWM__DBM_256_CLOCKS || \
                Aileron_DeadBandMode == Aileron__B_PWM__DBM_2_4_CLOCKS)
                Aileron_WriteDeadTime(Aileron_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Aileron_KillModeMinTime)
                Aileron_WriteKillTime(Aileron_backup.PWMKillCounterPeriod);
            #endif /* (Aileron_KillModeMinTime) */

            #if(Aileron_UseControl)
                Aileron_WriteControlRegister(Aileron_backup.PWMControlRegister);
            #endif /* (Aileron_UseControl) */
        #endif  /* (!Aileron_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Aileron_Sleep
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
*  Aileron_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Aileron_Sleep(void) 
{
    #if(Aileron_UseControl)
        if(Aileron_CTRL_ENABLE == (Aileron_CONTROL & Aileron_CTRL_ENABLE))
        {
            /*Component is enabled */
            Aileron_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Aileron_backup.PWMEnableState = 0u;
        }
    #endif /* (Aileron_UseControl) */

    /* Stop component */
    Aileron_Stop();

    /* Save registers configuration */
    Aileron_SaveConfig();
}


/*******************************************************************************
* Function Name: Aileron_Wakeup
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
*  Aileron_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Aileron_Wakeup(void) 
{
     /* Restore registers values */
    Aileron_RestoreConfig();

    if(Aileron_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Aileron_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
