/**
 *   @file  mmw_config.h
 *
 *   @brief
 *      This is the header file that describes configurations for the Millimeter
 *   Wave Demo.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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
#ifndef MMW_CONFIG_H
#define MMW_CONFIG_H

/* MMWAVE library Include Files */
#include <ti/control/mmwave/mmwave.h>
#include <ti/common/sys_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Maximum number of 1D FFT bins in DC range antenna signature compensation */
#define DC_RANGE_SIGNATURE_COMP_MAX_BIN_SIZE 32

/**
 * @brief
 *  Millimeter Wave Demo CFAR Configuration
 *
 * @details
 *  The structure contains the cfar configuration used in data path
 */
typedef struct MmwDemo_CfarCfg_t
{
    /*! @brief    CFAR threshold scale */
    uint16_t       thresholdScale;

    /*! @brief    CFAR averagining mode 0-CFAR_CA, 1-CFAR_CAGO, 2-CFAR_CASO */
    uint8_t        averageMode;

    /*! @brief    CFAR noise avraging window length */
    uint8_t        winLen;

    /*! @brief    CFAR guard length*/
    uint8_t        guardLen;

    /*! @brief    CFAR cumulative noise sum divisor */
    uint8_t        noiseDivShift;

    /*! @brief    CFAR 0-cyclic mode disabled, 1-cyclic mode enabled */
    uint8_t        cyclicMode;

} MmwDemo_CfarCfg;

/**
 * @brief
 *  Millimeter Wave Demo Peak grouping Configuration
 *
 * @details
 *  The structure contains the Peak grouping configuration used in data path
 */
typedef struct MmwDemo_PeakGroupingCfg_t
{
    /*! @brief    minimum range index exported */
    uint16_t        minRangeIndex;

    /*! @brief    maximum range index exported */
    uint16_t        maxRangeIndex;

    /*! @brief    Peak grouping scheme 1-based on neighboring peaks from detection matrix
     *                                 2-based on on neighboring CFAR detected peaks */
    uint8_t        scheme;

    /*! @brief     Grouping in range direction, 0- disbled, 1-enabled */
    uint8_t        inRangeDirectionEn;

    /*! @brief     Grouping in Doppler direction,  0- disbled, 1-enabled */
    uint8_t        inDopplerDirectionEn;

} MmwDemo_PeakGroupingCfg;

/**
 * @brief
 *  Millimeter Wave Demo multi object beam formaing Configuration
 *
 * @details
 *  The structure contains the Peak grouping configuration used in data path
 */
typedef struct MmwDemo_MultiObjBeamFormingCfg_t
{
    /*! @brief    enabled flag:  1-enabled 0-disabled */
    uint8_t     enabled;

    /*! @brief    second peak detection threshold */
    float        multiPeakThrsScal;

} MmwDemo_MultiObjBeamFormingCfg;

/**
 * @brief
 *  Millimeter Wave Demo Velocity Disambiguation
 *
 * @details
 *  The structure contains Velocity Disambiguation configuration
 */
typedef struct MmwDemo_ExtendedMaxVelocityCfg_t
{
    /*! @brief    enabled flag:  1-enabled 0-disabled */
    uint8_t     enabled;

} MmwDemo_ExtendedMaxVelocityCfg;

/**
 * @brief
 *  Clutter removal configuration
 *
 * @details
 *  The structure contains clutter removal configuration
 */
typedef struct MmwDemo_ClutterRemovalCfg_t
{
    /*! @brief    enabled flag:  1-enabled 0-disabled */
    uint8_t     enabled;

} MmwDemo_ClutterRemovalCfg;

/**
 * @brief
 *  Millimeter Wave Demo DC range signature compensation
 *
 * @details
 *  The structure contains the DC range antenna signature removeal configuration used in data path
 */
typedef struct MmwDemo_CalibDcRangeSigCfg_t
{
    /*! @brief    enabled flag:  1-enabled 0-disabled */
    uint16_t    enabled;

    /*! @brief    maximum negative range bin (1D FFT index) to be compensated */
    int16_t     negativeBinIdx;

    /*! @brief    maximum positive range bin (1D FFT index) to be compensated */
    int16_t     positiveBinIdx;

    /*! @brief    number of chirps in the averaging phase */
    uint16_t    numAvgChirps;
} MmwDemo_CalibDcRangeSigCfg;

/**
 * @brief
 *  ADCBUF configuration
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  which is used to configure ADCBUF.
 */
typedef struct MmwDemo_ADCBufCfg_t
{
    /*! ADCBUF out format:
        0-Complex,
        1-Real */
    uint8_t     adcFmt;

    /*! ADCBUF IQ swap selection:
        0-I in LSB, Q in MSB,
        1-Q in LSB, I in MSB */
    uint8_t     iqSwapSel;

    /*! ADCBUF channel interleave configuration:
        0-interleaved(not supported on XWR16xx),
        1- non-interleaved */
    uint8_t     chInterleave;

    /**
     * @brief   Chirp Threshold configuration used for ADCBUF buffer
     */
    uint8_t     chirpThreshold;
}MmwDemo_ADCBufCfg;


typedef struct _highAccuRangeProc_config_
{
//    uint32_t     fft1DSize;                     /**< 1D FFT size*/
    //uint32_t     nSamplesPerChirp;              /**< number of samples per chirp*/
    //uint32_t     numChirpsPerFrame;             /**< number of chirp per frame*/
    //float        maxBeatFreq;                   /**< maximum beat frequency*/
    //float      chirpBandwidth;                  /**< chirp bandwidth.*/
    //float      chirpRampTime;                   /**< chirp ramp duration.*/
    //float      fc;                              /**< chirp start frequency.*/
    //float      chirpSlope;                      /**< chirp slope.*/
    //float      adcStartTimeConst;               /**< ADC start constant.*/
    //float      adcSampleRate;                   /**< ADC sampling rate.*/
    //float      * win1D;                         /**< pointer to 1D windowing function.*/
    //int16_t      win1DLength;                   /**< half length of the 1D windowing function.*/
//    uint8_t      numRangeBinZoomIn;             /**< number of bins to zoom in for frequenc estimation.*/
//    uint8_t      enablePhaseEst;                /**< enable estimation using phase correction.*/
//    uint8_t      enableLinearFit;               /**< enable linear fit in phase estimation.*/
    uint8_t      enableRangeLimit;                  /**< enable RangeLimit.*/
    float      skipMin;                      /**< number of samples to skip from 0 to min range(m).*/
    float      skipMax;                     /**< number of samples to skip from the max range(m) to faraway.*/
    uint16_t    skipMinRangeBin;            /**< number of Range bins to skip from 0 to min range(m).*/
    uint16_t    skipMaxRangeBin;            /**< number of Range bins to skip from the max range(m) to faraway.*/
//    float      * fft1DIn;                   /**< pointer to 1D FFT input.*/
} highAccuRangeProc_config;


/**
 * @brief
 *  Millimeter Wave Demo Gui Monitor Selection
 *
 * @details
 *  The structure contains the flags which select what information is placed to
 *  the output packet, and sent out to GUI. If the flag is set to 1, information
 *  is sent out. If the flag is set to 0, information is not sent out.
 *
 */
typedef struct MmwDemo_GuiMonSel_t
{
    /*! @brief   Send list of detected objects (see @ref MmwDemo_detectedObj_t) */
    uint8_t        detectedObjects;

    /*! @brief   Send log magnitude range array  */
    uint8_t        logMagRange;

    /*! @brief   Send noise floor profile */
    uint8_t        noiseProfile;

    /*! @brief   Send complex range bins at zero doppler, all antenna symbols for range-azimuth heat map */
    uint8_t        rangeAzimuthHeatMap;

    /*! @brief   Send complex range bins at zero doppler, (all antenna symbols), for range-azimuth heat map */
    uint8_t        rangeDopplerHeatMap;

    /*! @brief   Send stats */
    uint8_t        statsInfo;

} MmwDemo_GuiMonSel;

/**
 * @brief
 *  Millimeter Wave Demo CLI related configuration
 */
typedef struct MmwDemo_CliCfg_t_
{
    /*! @brief   ADCBUF Configuration. */
    MmwDemo_ADCBufCfg   adcBufCfg;

    /*! @brief   Gui Monitor Selection */
    MmwDemo_GuiMonSel   guiMonSel;

#ifdef SOC_XWR14XX
    MmwDemo_CfarCfg     cfarCfg;
#else
    /*! @brief   CFAR configuration for range */
    MmwDemo_CfarCfg     cfarCfgRange;
    
    /*! @brief   CFAR configuration for doppler */
    MmwDemo_CfarCfg     cfarCfgDoppler;

    /*! @brief   Velocity disambiguation configuration */
    MmwDemo_ExtendedMaxVelocityCfg extendedMaxVelocityCfg;
#endif

    /*! @brief   Peak grouping configuration */
    MmwDemo_PeakGroupingCfg peakGroupingCfg;    

    /*! @brief   Multi object beam forming configuration */
    MmwDemo_MultiObjBeamFormingCfg multiObjBeamFormingCfg;

    /*! @brief   Calibrate DC (zero) range signature */
    MmwDemo_CalibDcRangeSigCfg calibDcRangeSigCfg;

    /*! @brief   Clutter removal configuration */
    MmwDemo_ClutterRemovalCfg clutterRemovalCfg;

} MmwDemo_CliCfg_t;

/**
 * @brief Range Bias and rx channel gain/phase compensation configuration.
 *
 */
typedef struct MmwDemo_compRxChannelBiasCfg_t_
{

    /*! @brief  Compensation for range estimation bias */
    float rangeBias;

    /*! @brief  Compensation for Rx channel phase bias in Q15 format */
    cmplx16ImRe_t rxChPhaseComp[SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL];

} MmwDemo_compRxChannelBiasCfg_t;

/**
 * @brief Range Bias and rx channel gain/phase measurement configuration.
 *
 */
typedef struct MmwDemo_measureRxChannelBiasCfg_t_
{

    /*! @brief  1-enabled 0-disabled */
    uint8_t enabled;

    /*! @brief  Target distance during measurement (in meters) */
    float targetDistance;

    /*! @brief  Search window size (in meters), the search is done in range
     *          [-searchWinSize/2 + targetDistance, targetDistance + searchWinSize/2] */
    float searchWinSize;

} MmwDemo_measureRxChannelBiasCfg_t;


/**
 * @brief
 *  Millimeter Wave Demo CLI related configuration common across all subframes
 */
typedef struct MmwDemo_CliCommonCfg_t_
{
    /*! @brief   Configuration for compensation for range bias
                 and Rx channel phase offset */
    MmwDemo_compRxChannelBiasCfg_t compRxChanCfg;

    /*! @brief   Configuration for measurement of range bias
                 and Rx channel phase offset */
    MmwDemo_measureRxChannelBiasCfg_t measureRxChanCfg;
} MmwDemo_CliCommonCfg_t;

/**
 * @brief
 *  Millimeter Wave Demo configuration
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  which is used to execute the Millimeter Wave Demo.
 */
typedef struct MmwDemo_Cfg_t
{
    /*! @brief   CPU Clock Frequency. */
    uint32_t            sysClockFrequency;

    /*! @brief   UART Logging Baud Rate. */
    uint32_t            loggingBaudRate;

    /*! @brief   UART Command Baud Rate. */
    uint32_t            commandBaudRate;

    /*! @brief   mmWave Control Configuration. */
    MMWave_CtrlCfg      ctrlCfg;

    /*! @brief   mmWave Open Configuration. */
    MMWave_OpenCfg      openCfg;

    /*! @brief   Datapath output loggerSetting
                 0 (default): MSS UART logger
                 1: DSS UART logger
     */
    uint8_t              dataLogger;
} MmwDemo_Cfg;


#ifdef __cplusplus
}
#endif

#endif /* MMW_CONFIG_H */
