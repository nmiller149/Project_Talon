/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_SPI_Altitude_H)
#define CY_SPIM_PVT_SPI_Altitude_H

#include "SPI_Altitude.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 SPI_Altitude_swStatusTx;
extern volatile uint8 SPI_Altitude_swStatusRx;

#if(SPI_Altitude_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPI_Altitude_txBuffer[SPI_Altitude_TX_BUFFER_SIZE];
    extern volatile uint8 SPI_Altitude_txBufferRead;
    extern volatile uint8 SPI_Altitude_txBufferWrite;
    extern volatile uint8 SPI_Altitude_txBufferFull;
#endif /* (SPI_Altitude_TX_SOFTWARE_BUF_ENABLED) */

#if(SPI_Altitude_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPI_Altitude_rxBuffer[SPI_Altitude_RX_BUFFER_SIZE];
    extern volatile uint8 SPI_Altitude_rxBufferRead;
    extern volatile uint8 SPI_Altitude_rxBufferWrite;
    extern volatile uint8 SPI_Altitude_rxBufferFull;
#endif /* (SPI_Altitude_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_SPI_Altitude_H */


/* [] END OF FILE */
