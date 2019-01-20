/*******************************************************************************
* File Name: Motor_Speed_PM.c
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

#include "Motor_Speed.h"

static Motor_Speed_backupStruct Motor_Speed_backup;


/*******************************************************************************
* Function Name: Motor_Speed_SaveConfig
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
*  Motor_Speed_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Motor_Speed_SaveConfig(void) 
{

    #if(!Motor_Speed_UsingFixedFunction)
        #if(!Motor_Speed_PWMModeIsCenterAligned)
            Motor_Speed_backup.PWMPeriod = Motor_Speed_ReadPeriod();
        #endif /* (!Motor_Speed_PWMModeIsCenterAligned) */
        Motor_Speed_backup.PWMUdb = Motor_Speed_ReadCounter();
        #if (Motor_Speed_UseStatus)
            Motor_Speed_backup.InterruptMaskValue = Motor_Speed_STATUS_MASK;
        #endif /* (Motor_Speed_UseStatus) */

        #if(Motor_Speed_DeadBandMode == Motor_Speed__B_PWM__DBM_256_CLOCKS || \
            Motor_Speed_DeadBandMode == Motor_Speed__B_PWM__DBM_2_4_CLOCKS)
            Motor_Speed_backup.PWMdeadBandValue = Motor_Speed_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Motor_Speed_KillModeMinTime)
             Motor_Speed_backup.PWMKillCounterPeriod = Motor_Speed_ReadKillTime();
        #endif /* (Motor_Speed_KillModeMinTime) */

        #if(Motor_Speed_UseControl)
            Motor_Speed_backup.PWMControlRegister = Motor_Speed_ReadControlRegister();
        #endif /* (Motor_Speed_UseControl) */
    #endif  /* (!Motor_Speed_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Motor_Speed_RestoreConfig
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
*  Motor_Speed_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Motor_Speed_RestoreConfig(void) 
{
        #if(!Motor_Speed_UsingFixedFunction)
            #if(!Motor_Speed_PWMModeIsCenterAligned)
                Motor_Speed_WritePeriod(Motor_Speed_backup.PWMPeriod);
            #endif /* (!Motor_Speed_PWMModeIsCenterAligned) */

            Motor_Speed_WriteCounter(Motor_Speed_backup.PWMUdb);

            #if (Motor_Speed_UseStatus)
                Motor_Speed_STATUS_MASK = Motor_Speed_backup.InterruptMaskValue;
            #endif /* (Motor_Speed_UseStatus) */

            #if(Motor_Speed_DeadBandMode == Motor_Speed__B_PWM__DBM_256_CLOCKS || \
                Motor_Speed_DeadBandMode == Motor_Speed__B_PWM__DBM_2_4_CLOCKS)
                Motor_Speed_WriteDeadTime(Motor_Speed_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Motor_Speed_KillModeMinTime)
                Motor_Speed_WriteKillTime(Motor_Speed_backup.PWMKillCounterPeriod);
            #endif /* (Motor_Speed_KillModeMinTime) */

            #if(Motor_Speed_UseControl)
                Motor_Speed_WriteControlRegister(Motor_Speed_backup.PWMControlRegister);
            #endif /* (Motor_Speed_UseControl) */
        #endif  /* (!Motor_Speed_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Motor_Speed_Sleep
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
*  Motor_Speed_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Motor_Speed_Sleep(void) 
{
    #if(Motor_Speed_UseControl)
        if(Motor_Speed_CTRL_ENABLE == (Motor_Speed_CONTROL & Motor_Speed_CTRL_ENABLE))
        {
            /*Component is enabled */
            Motor_Speed_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Motor_Speed_backup.PWMEnableState = 0u;
        }
    #endif /* (Motor_Speed_UseControl) */

    /* Stop component */
    Motor_Speed_Stop();

    /* Save registers configuration */
    Motor_Speed_SaveConfig();
}


/*******************************************************************************
* Function Name: Motor_Speed_Wakeup
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
*  Motor_Speed_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Motor_Speed_Wakeup(void) 
{
     /* Restore registers values */
    Motor_Speed_RestoreConfig();

    if(Motor_Speed_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Motor_Speed_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
