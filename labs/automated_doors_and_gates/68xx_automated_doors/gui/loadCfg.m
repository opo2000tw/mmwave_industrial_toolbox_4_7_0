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
function [spCliHandle, cliCfg] = loadCfg(num_COM_UART, cliCfgFileName)

    spCliHandle = configureUserUARTPort(num_COM_UART);

    timeOut = get(spCliHandle,'Timeout');
    set(spCliHandle,'Timeout',1);

%     while 1
%         fprintf(spCliHandle, ''); 
%         cc=fread(spCliHandle,100);
%         cc = strrep(strrep(cc,char(10),''),char(13),'');
%         if ~isempty(cc)
%             break;
%         end
%         pause(0.1);
%     end
    set(spCliHandle,'Timeout', timeOut);
    
    %Read Configuration file
    [cliCfg] = readCfgFile(cliCfgFileName);

    
    %Send CLI configuration to XWR1xxx
    fprintf('Sending configuration to XWR1xxx %s ...\n', cliCfgFileName);
    
    
    for k=1:length(cliCfg)
        if isempty(strrep(strrep(cliCfg{k},char(9),''),char(32),''))
            continue;
        end
        if strcmp(cliCfg{k}(1),'%')
            continue;
        end
        fprintf(spCliHandle, cliCfg{k});
        fprintf('%s\n', cliCfg{k});
        for kk = 1:3
            cc = fgetl(spCliHandle);
            if strcmp(cc,'Done')
                fprintf('%s\n',cc);
                break;
            elseif ~isempty(strfind(cc, 'not recognized as a CLI command'))
                fprintf('%s\n',cc);
                return;
            elseif ~isempty(strfind(cc, 'Error'))
                fprintf('%s\n',cc);
                return;
            end
        end
        pause(0.2)
    end
    fclose(spCliHandle);
return

function [sphandle] = configureUserUARTPort(num_COM)
    comportnum_str=['COM' num2str(num_COM)];
       
    connectedPorts = instrfind('Type','serial');
    if(~isempty(connectedPorts))
        for i=1:numel(connectedPorts)
            if(strcmp(connectedPorts.Name(i), ['Serial-' comportnum_str]))
                delete(connectedPorts(i))
            end
        end
    end
    
    sphandle = serial(comportnum_str,'BaudRate',115200);
    set(sphandle,'Parity','none')    
    set(sphandle,'Terminator','LF')
    

        
    fopen(sphandle);
return