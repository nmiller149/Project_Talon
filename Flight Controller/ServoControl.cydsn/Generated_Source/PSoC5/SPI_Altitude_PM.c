/*******************************************************************************
* File Name: SPI_Altitude_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_Altitude_PVT.h"

static SPI_Altitude_BACKUP_STRUCT SPI_Altitude_backup =
{
    SPI_Altitude_DISABLED,
    SPI_Altitude_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_Altitude_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_Altitude_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_Altitude_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_Altitude_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_Altitude_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_Altitude_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_Altitude_Sleep(void) 
{
    /* Save components enable state */
    SPI_Altitude_backup.enableState = ((uint8) SPI_Altitude_IS_ENABLED);

    SPI_Altitude_Stop();
}


/*******************************************************************************
* Function Name: SPI_Altitude_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_Altitude_backup - used when non-retention registers are restored.
*  SPI_Altitude_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_Altitude_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_Altitude_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_Altitude_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_Altitude_Wakeup(void) 
{
    #if(SPI_Altitude_RX_SOFTWARE_BUF_ENABLED)
        SPI_Altitude_rxBufferFull  = 0u;
        SPI_Altitude_rxBufferRead  = 0u;
        SPI_Altitude_rxBufferWrite = 0u;
    #endif /* (SPI_Altitude_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_Altitude_TX_SOFTWARE_BUF_ENABLED)
        SPI_Altitude_txBufferFull  = 0u;
        SPI_Altitude_txBufferRead  = 0u;
        SPI_Altitude_txBufferWrite = 0u;
    #endif /* (SPI_Altitude_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_Altitude_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_Altitude_backup.enableState)
    {
        SPI_Altitude_Enable();
    }
}


/* [] END OF FILE */
