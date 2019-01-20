/*******************************************************************************
* File Name: Counter_Duration_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_Duration.h"

static Counter_Duration_backupStruct Counter_Duration_backup;


/*******************************************************************************
* Function Name: Counter_Duration_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Duration_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_Duration_SaveConfig(void) 
{
    #if (!Counter_Duration_UsingFixedFunction)

        Counter_Duration_backup.CounterUdb = Counter_Duration_ReadCounter();

        #if(!Counter_Duration_ControlRegRemoved)
            Counter_Duration_backup.CounterControlRegister = Counter_Duration_ReadControlRegister();
        #endif /* (!Counter_Duration_ControlRegRemoved) */

    #endif /* (!Counter_Duration_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_Duration_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Duration_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_Duration_RestoreConfig(void) 
{      
    #if (!Counter_Duration_UsingFixedFunction)

       Counter_Duration_WriteCounter(Counter_Duration_backup.CounterUdb);

        #if(!Counter_Duration_ControlRegRemoved)
            Counter_Duration_WriteControlRegister(Counter_Duration_backup.CounterControlRegister);
        #endif /* (!Counter_Duration_ControlRegRemoved) */

    #endif /* (!Counter_Duration_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_Duration_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Duration_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_Duration_Sleep(void) 
{
    #if(!Counter_Duration_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_Duration_CTRL_ENABLE == (Counter_Duration_CONTROL & Counter_Duration_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_Duration_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_Duration_backup.CounterEnableState = 0u;
        }
    #else
        Counter_Duration_backup.CounterEnableState = 1u;
        if(Counter_Duration_backup.CounterEnableState != 0u)
        {
            Counter_Duration_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_Duration_ControlRegRemoved) */
    
    Counter_Duration_Stop();
    Counter_Duration_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_Duration_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Duration_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_Duration_Wakeup(void) 
{
    Counter_Duration_RestoreConfig();
    #if(!Counter_Duration_ControlRegRemoved)
        if(Counter_Duration_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_Duration_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_Duration_ControlRegRemoved) */
    
}


/* [] END OF FILE */
