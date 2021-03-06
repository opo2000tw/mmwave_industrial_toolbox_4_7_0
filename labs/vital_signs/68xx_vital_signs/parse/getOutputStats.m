%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 %      (C) Copyright 2016 Texas Instruments, Inc.
 %
 %  Redistribution and use in source and binary forms, with or without
 %  modification, are permitted provided that the following conditions
 %  are met:
 %
 %    Redistributions of source code must retain the above copyright
 %    notice, this list of conditions and the following disclaimer.
 %
 %    Redistributions in binary form must reproduce the above copyright
 %    notice, this list of conditions and the following disclaimer in the
 %    documentation and/or other materials provided with the
 %    distribution.
 %
 %    Neither the name of Texas Instruments Incorporated nor the names of
 %    its contributors may be used to endorse or promote products derived
 %    from this software without specific prior written permission.
 %
 %  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 %  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 %  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 %  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 %  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 %  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 %  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 %  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 %  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 %  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 %  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 %
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
function [outputStats] = getOutputStats(payload)
    [r c] = size(payload);
    
    if(r<c)
        payload = payload';
    end
    
    word = [1 256 65536 16777216]';
    % size of the parameters  
    outputStatsSizeOf = struct('rangeBinIndexMax', 2,...
        'rangeBinIndexPhase', 2,...
        'maxVal', 4,...
        'processingCyclesOut' , 4, ...
        'rangeBinStartIndex' , 2, ...
        'rangeBinEndIndex' , 2, ...
        'unwrapPhasePeak_mm' , 4, ...
        'outputFilterBreathOut' , 4, ...
        'outputFilterHeartOut' , 4, ...
        'heartRateEst_FFT' , 4, ...
        'heartRateEst_FFT_4Hz' , 4, ...
        'heartRateEst_xCorr' , 4, ...
        'heartRateEst_peakCount' , 4, ...
        'breathingRateEst_FFT' , 4, ...
        'breathingRateEst_xCorr' , 4, ...
        'breathingRateEst_peakCount' , 4, ...
        'confidenceMetricBreathOut' , 4, ...
        'confidenceMetricBreathOut_xCorr' , 4, ...
        'confidenceMetricHeartOut' , 4, ...
        'confidenceMetricHeartOut_4Hz' , 4, ...
        'confidenceMetricHeartOut_xCorr' , 4, ...
        'sumEnergyBreathWfm' , 4, ...
        'sumEnergyHeartWfm' , 4, ...
        'motionDetectedFlag' , 4, ...
        'reserved', 10*4);
    
    outputStatsType = struct('rangeBinIndexMax', 'uint16',...
        'rangeBinIndexPhase', 'uint16',...
        'maxVal', 'single',...
        'processingCyclesOut' , 'uint32', ...
        'rangeBinStartIndex' , 'uint16', ...
        'rangeBinEndIndex' , 'uint16', ...
        'unwrapPhasePeak_mm' , 'single', ...
        'outputFilterBreathOut' , 'single', ...
        'outputFilterHeartOut' , 'single', ...
        'heartRateEst_FFT' , 'single', ...
        'heartRateEst_FFT_4Hz' , 'single', ...
        'heartRateEst_xCorr' , 'single', ...
        'heartRateEst_peakCount' , 'single', ...
        'breathingRateEst_FFT' , 'single', ...
        'breathingRateEst_xCorr' , 'single', ...
        'breathingRateEst_peakCount' , 'single', ...
        'confidenceMetricBreathOut' , 'single', ...
        'confidenceMetricBreathOut_xCorr' , 'single', ...
        'confidenceMetricHeartOut' , 'single', ...
        'confidenceMetricHeartOut_4Hz' , 'single', ...
        'confidenceMetricHeartOut_xCorr' , 'single', ...
        'sumEnergyBreathWfm' , 'single', ...
        'sumEnergyHeartWfm' , 'single', ...
        'motionDetectedFlag' , 'single', ...
        'reserved', 'single');
   
     
    OUTPUT_STATS_SIZE_BYTES = 128;
    
    % get fieldnames of frame header struct
    structFieldNames = fieldnames(outputStatsSizeOf);
    
    % parse values from payload into struct
    outputStats = struct();
    idxPayload = 0;
    for i=1:numel(structFieldNames)
        if(~strcmp(structFieldNames{i},'reserved'))
            idxStart = idxPayload+1;
            idxEnd = idxStart + outputStatsSizeOf.(structFieldNames{i})-1;        
            outputStats.(structFieldNames{i}) = typecast(uint8([payload(idxStart:idxEnd)]), outputStatsType.(structFieldNames{i})); %sum(payload(idxStart:idxEnd) .* word(1:idxEnd-idxStart+1));
            idxPayload = idxEnd;
        else
            idxStart = idxPayload+1;
            idxEnd = idxStart + outputStatsSizeOf.(structFieldNames{i})-1;        
            outputStats.(structFieldNames{i}) = sum(payload(idxStart:idxEnd));
            idxPayload = idxEnd; 
        end
    end
return
