/**
 *   @file  task_mbox.c
 *
 *   @brief
 *     MSS main implementation of the millimeter wave Demo
 *
 *  Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>
#include <ti/sysbios/family/arm/v7r/vim/Hwi.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>

#include <ti/drivers/soc/soc.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/utils/cli/cli.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>

/* Demo Include Files */
#include "mss_mmw.h"
#include <common/mmw_messages.h>
#include <common/mmw_output.h>

#define RAD2DEGREE (180.f/3.1415926f)

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Global Variable for tracking information required by the mmw Demo
 */
extern MmwDemo_MCB    gMmwMssMCB;

/**************************************************************************
 ************************* Millimeter Wave Demo Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Function to send a message to peer through Mailbox virtural channel
 *
 *  @param[in]  message
 *      Pointer to the MMW demo message.
 *
 *  @retval
 *      Success    - 0
 *      Fail       < -1
 */
int32_t MmwDemo_mboxWrite(MmwDemo_message     * message)
{
    int32_t                  retVal = -1;

    retVal = Mailbox_write (gMmwMssMCB.peerMailbox, (uint8_t*)message, sizeof(MmwDemo_message));
    if (retVal == sizeof(MmwDemo_message))
    {
        retVal = 0;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The Task is used to handle  the mmw demo messages received from
 *      Mailbox virtual channel.
 *
 *  @param[in]  arg0
 *      arg0 of the Task. Not used
 *  @param[in]  arg1
 *      arg1 of the Task. Not used
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_mboxReadTask(UArg arg0, UArg arg1)
{
    MmwDemo_message message;
    int32_t         retVal = 0;
    uint32_t        timeStart;
    inputFromDSP    *srcAddress;
    uint16_t        numPoints, nPoint;
    int             n;
    uint16_t        *headerPtr;
    uint32_t        sum;
    MmwDemo_output_message_header outputMessage;
    _Bool sendDescr;

    outputMessage.magicWord[0] = 0x0102;
    outputMessage.magicWord[1] = 0x0304;
    outputMessage.magicWord[2] = 0x0506;
    outputMessage.magicWord[3] = 0x0708;
    outputMessage.platform = 0xA1642;
    outputMessage.version =    MMWAVE_SDK_VERSION_BUILD |   //DEBUG_VERSION
                                            (MMWAVE_SDK_VERSION_BUGFIX << 8) |
                                            (MMWAVE_SDK_VERSION_MINOR << 16) |
                                            (MMWAVE_SDK_VERSION_MAJOR << 24);
    outputMessage.subFrameNumber = 0;


    /* wait for new message and process all the messages received from the peer */
    while(1)
    {
        Semaphore_pend(gMmwMssMCB.mboxSemHandle, BIOS_WAIT_FOREVER);

        /* Read the message from the peer mailbox: We are not trying to protect the read
         * from the peer mailbox because this is only being invoked from a single thread */
        retVal = Mailbox_read(gMmwMssMCB.peerMailbox, (uint8_t*)&message, sizeof(MmwDemo_message));
        if (retVal < 0)
        {
            /* Error: Unable to read the message. Setup the error code and return values */
            System_printf ("Error: Mailbox read failed [Error code %d]\n", retVal);
        }
        else if (retVal == 0)
        {
            /* We are done: There are no messages available from the peer execution domain. */
            continue;
        }
        else
        {
            /* Flush out the contents of the mailbox to indicate that we are done with the message. This will
             * allow us to receive another message in the mailbox while we process the received message. */
            Mailbox_readFlush (gMmwMssMCB.peerMailbox);

            /* Process the received message: */
            switch (message.type)
            {
                case MMWDEMO_DSS2MSS_DETOBJ_READY:

                    timeStart = Cycleprofiler_getTimeStamp();
					
					gMmwMssMCB.mssDataPathObj.uartOutputCounter  = gMmwMssMCB.mssDataPathObj.uartOutputCounter + 1;

                	srcAddress = (inputFromDSP *)SOC_translateAddress(message.body.detObj.detObjOutAddress, SOC_TranslateAddr_Dir_FROM_OTHER_CPU,NULL);
                    outputMessage.frameNumber = srcAddress->header.frameNumber;
                    outputMessage.chirpProcessingMargin = srcAddress->header.chirpProcessingMarginInUsec;
                    outputMessage.frameProcessingMargin = srcAddress->header.frameProcessingMarginInUsec;
                    outputMessage.trackingProcessingTime = gMmwMssMCB.mssDataPathObj.cycleLog.trackingTimeCurrInusec;
                    outputMessage.uartSendingTime = gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeCurrInusec;


                    numPoints = srcAddress->outputToTracker.object_count;
                    if(numPoints > gMmwMssMCB.cfg.trackingCfg.config.maxNumPoints) {
                        numPoints = gMmwMssMCB.cfg.trackingCfg.config.maxNumPoints;
                    }
                    /* copy for tracker and classifier */
                    if(numPoints) {
                        gMmwMssMCB.pointCloud->header.length = sizeof(MmwDemo_output_message_tl) + numPoints*sizeof(MmwDemo_output_message_point);
                        for(nPoint = 0; nPoint < numPoints; nPoint++) {
                            gMmwMssMCB.pointCloud->point[nPoint].range = srcAddress->outputToTracker.range[nPoint];
                            gMmwMssMCB.pointCloud->point[nPoint].azimuth = srcAddress->outputToTracker.angle[nPoint] + gMmwMssMCB.cfg.applicationCfg.sensorAzimuthTilt;
                            gMmwMssMCB.pointCloud->point[nPoint].doppler = srcAddress->outputToTracker.doppler[nPoint];
                            gMmwMssMCB.pointCloud->point[nPoint].snr = srcAddress->outputToTracker.snr[nPoint];
                        }
                    }
                    else
                        gMmwMssMCB.pointCloud->header.length = 0;

                    /* copy for UART output */
                    if(numPoints) {
                        gMmwMssMCB.OutputPointCloud->header.length = sizeof(MmwDemo_output_message_tl) + sizeof(MmwDemo_output_message_point_uint) + numPoints*sizeof(MmwDemo_output_message_UARTpoint);
                        for(nPoint = 0; nPoint < numPoints; nPoint++) {
                            gMmwMssMCB.OutputPointCloud->point[nPoint].range = (uint16_t)(gMmwMssMCB.pointCloud->point[nPoint].range * gMmwMssMCB.mssDataPathObj.invRRes + 0.5f);
                            gMmwMssMCB.OutputPointCloud->point[nPoint].azimuth = (int8_t)(gMmwMssMCB.pointCloud->point[nPoint].azimuth * RAD2DEGREE * gMmwMssMCB.mssDataPathObj.invAzimRes + 0.5f);
                            gMmwMssMCB.OutputPointCloud->point[nPoint].doppler = (int8_t)(gMmwMssMCB.pointCloud->point[nPoint].doppler * gMmwMssMCB.mssDataPathObj.invDRes + 0.5f);
                            gMmwMssMCB.OutputPointCloud->point[nPoint].snr = (uint16_t)(gMmwMssMCB.pointCloud->point[nPoint].snr * gMmwMssMCB.mssDataPathObj.invSnrRes + 0.5f);
                        }
						if ((gMmwMssMCB.cfg.guiMonSel.reducePointCloudOutputSize) && (numPoints > gMmwMssMCB.cfg.guiMonSel.reducePointCloudOutputSize))
						{
							gMmwMssMCB.OutputPointCloud->header.length = sizeof(MmwDemo_output_message_tl) + sizeof(MmwDemo_output_message_point_uint) + gMmwMssMCB.cfg.guiMonSel.reducePointCloudOutputSize*sizeof(MmwDemo_output_message_UARTpoint);
						}
                    }
                    else
                        gMmwMssMCB.OutputPointCloud->header.length = 0;

                    gMmwMssMCB.mssDataPathObj.cycleLog.copyResultsTimeCurrInusec = ((float)(Cycleprofiler_getTimeStamp() - timeStart))/(float)R4F_CLOCK_MHZ;
                    if ((gMmwMssMCB.mssDataPathObj.cycleLog.copyResultsTimeCurrInusec > 0) && (gMmwMssMCB.mssDataPathObj.cycleLog.copyResultsTimeCurrInusec > gMmwMssMCB.mssDataPathObj.cycleLog.copyResultsTimeMaxInusec))
                        gMmwMssMCB.mssDataPathObj.cycleLog.copyResultsTimeMaxInusec = gMmwMssMCB.mssDataPathObj.cycleLog.copyResultsTimeCurrInusec;

                    /* Send a message to MSS to log the output data */
                    memset((void *)&message, 0, sizeof(MmwDemo_message));
                    message.type = MMWDEMO_MSS2DSS_DETOBJ_SHIPPED;
                    if (MmwDemo_mboxWrite(&message) != 0) {
                        System_printf ("Error: Mailbox send message id=%d failed \n", message.type);
                    }

                    /* Post a job for group tracker */
                    sendDescr = gMmwMssMCB.targetDescrHandle->currentDescr;
                    gMmwMssMCB.targetDescrHandle->currentDescr = !sendDescr;

                    if(gMmwMssMCB.mssDataPathObj.groupTrackerEnabled)
                        Semaphore_post(gMmwMssMCB.appSemHandle);

                    outputMessage.timeStamp = Cycleprofiler_getTimeStamp();
                    outputMessage.totalPacketLen = sizeof(MmwDemo_output_message_header);
                    outputMessage.numTLVs = 0;
                    outputMessage.checksum = 0;


                    if(gMmwMssMCB.OutputPointCloud->header.length) {
                        /* Add pointCloud TLV length */
                        outputMessage.totalPacketLen += gMmwMssMCB.OutputPointCloud->header.length;
                        outputMessage.numTLVs += 1;
                    }
                    if(gMmwMssMCB.targetDescrHandle->tList[sendDescr]->header.length) {
                        /* Add targetList TLV length */
                        outputMessage.totalPacketLen += gMmwMssMCB.targetDescrHandle->tList[sendDescr]->header.length;
                        outputMessage.numTLVs += 1;
                    }
                    if(gMmwMssMCB.targetDescrHandle->tIndex[sendDescr]->header.length) {
                        /* Add targetIndex TLV length */
                        outputMessage.totalPacketLen += gMmwMssMCB.targetDescrHandle->tIndex[sendDescr]->header.length;
                        outputMessage.numTLVs += 1;
                    }
                    if(gMmwMssMCB.classifierResults[sendDescr]->header.length) {
                        /* Add pointCloud TLV length */
                        outputMessage.totalPacketLen += gMmwMssMCB.classifierResults[sendDescr]->header.length;
                        outputMessage.numTLVs += 1;
                    }

                    /* Calculate header checksum */
                    headerPtr = (uint16_t *)&outputMessage;
                    for(n=0, sum = 0; n < sizeof(MmwDemo_output_message_header)/sizeof(uint16_t); n++)
                        sum += *headerPtr++;
                    outputMessage.checksum = ~((sum >> 16) + (sum & 0xFFFF));

					/* Always send a packet header */
					UART_write (gMmwMssMCB.loggingUartHandle, (uint8_t *)&outputMessage, sizeof(MmwDemo_output_message_header));

					if(gMmwMssMCB.OutputPointCloud->header.length) {
						/* If any points detected, send Point Cloud TLV */
						UART_write (gMmwMssMCB.loggingUartHandle, (uint8_t *)gMmwMssMCB.OutputPointCloud, gMmwMssMCB.OutputPointCloud->header.length);
					}

					if(gMmwMssMCB.targetDescrHandle->tList[sendDescr]->header.length) {
						/* If any targets tracked, send send target List TLV  */
						UART_write (gMmwMssMCB.loggingUartHandle, (uint8_t *)gMmwMssMCB.targetDescrHandle->tList[sendDescr], gMmwMssMCB.targetDescrHandle->tList[sendDescr]->header.length);
					}

					if(gMmwMssMCB.targetDescrHandle->tIndex[sendDescr]->header.length) {
						/* If exists, send target Index TLV  */
						UART_write (gMmwMssMCB.loggingUartHandle, (uint8_t *)gMmwMssMCB.targetDescrHandle->tIndex[sendDescr], gMmwMssMCB.targetDescrHandle->tIndex[sendDescr]->header.length);
					}
					if(gMmwMssMCB.classifierResults[sendDescr]->header.length) {
						/* If exists, send target Index TLV  */
						UART_write (gMmwMssMCB.loggingUartHandle, (uint8_t *)gMmwMssMCB.classifierResults[sendDescr], gMmwMssMCB.classifierResults[sendDescr]->header.length);
					}
					
                    gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeCurrInusec = ((float)(Cycleprofiler_getTimeStamp() - timeStart))/(float)R4F_CLOCK_MHZ;
                    if ((gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeCurrInusec > 0) && (gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeCurrInusec > gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeMaxInusec))
                        gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeMaxInusec = gMmwMssMCB.mssDataPathObj.cycleLog.sendingToUARTTimeCurrInusec;

                    break;

                case MMWDEMO_DSS2MSS_CONFIGDONE:
                	gMmwMssMCB.mssDataPathObj.heatmapAddress = (float *)SOC_translateAddress(message.body.dssStaticInfo.heatmapAddress, SOC_TranslateAddr_Dir_FROM_OTHER_CPU,NULL);
					gMmwMssMCB.mssDataPathObj.heatmapRowLen = message.body.dssStaticInfo.heatmapRowLen;
					gMmwMssMCB.mssDataPathObj.heatmapNumRows = message.body.dssStaticInfo.heatmapNumRows;

					gMmwMssMCB.OutputPointCloud->pointUint.rangeUnit = message.body.dssStaticInfo.rRes;
					gMmwMssMCB.OutputPointCloud->pointUint.azimuthUnit = message.body.dssStaticInfo.azimRes;
					gMmwMssMCB.OutputPointCloud->pointUint.dopplerUnit = message.body.dssStaticInfo.dRes;
					gMmwMssMCB.OutputPointCloud->pointUint.snrUint = message.body.dssStaticInfo.snrRes;

					gMmwMssMCB.mssDataPathObj.invRRes = 1.f/(message.body.dssStaticInfo.rRes);
					gMmwMssMCB.mssDataPathObj.invDRes = 1.f/(message.body.dssStaticInfo.dRes);
					gMmwMssMCB.mssDataPathObj.invAzimRes = 1.f/(message.body.dssStaticInfo.azimRes);
					gMmwMssMCB.mssDataPathObj.invSnrRes = 1.f/(message.body.dssStaticInfo.snrRes);
					break;
					
				default:
                {
                    /* Message not support */
                    System_printf ("Error: Unsupported Mailbox message id=%d\n", message.type);
                    break;
                }
            }
        }
    }
}

