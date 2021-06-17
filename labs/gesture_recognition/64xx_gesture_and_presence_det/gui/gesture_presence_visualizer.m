
clear, clc, close all
delete(instrfind)

%% GET User Inputs & Setup Options
SETUP_VIA_GUI = 1;
ANTENNA_TYPE = 1; %Hardcoded b/c only ISK/BOOST style currently supported.
if(SETUP_VIA_GUI)
    hApp = setup_gest();
    
    ENABLE_RECORD = 0;
    datFile.path = '';
    datFile.name = 'tm_demo_uart_stream.txt';
    

    logFile.path = '';
    logFile.name = ['gesture_demo_log_' datestr(now,'mmddyy_HHMM') '.txt'];
    offset.height = 0;
    offset.az = 0;
    offset.el = 0;
end


%% Set up file to record
if(ENABLE_RECORD && REAL_TIME_MODE)
    
    if(~isempty(logFile.path))
        % append slash if needed
        if(logFile.path(end)~='\' && logFile.path(end)~='/')
            logFile.path(end+1) = '\';
        end
        status = mkdir(logFile.path);
    else
        status = 1;
    end
    if(status)
        fid = fopen([logFile.path logFile.name],'w+');
    end
    if fid~= -1
        fprintf(['Opening ' logFile.name '. Ready to log data. \n']);
    else
        fprintf('Error with log file name or path. No logging. \n');
        ENABLE_RECORD = 0;
    end
else
    fid = -1;
    ENABLE_RECORD = 0;
end

%% SET and PARSE CFG FILE
try 
    [cliCfg] = readCfgFile([cfgFile.path cfgFile.name]);
catch ME
    fprintf('Error: Could not open CFG file. Quitting.');
    if(ENABLE_RECORD)
        fclose(fid);
    end
    return;
end

%Define supported CLI commands
sdkVersion = '03.03.00.01'; %TODO read this from device
demoType = 'TM';
[supported_cfgs] = defineCLICommands(sdkVersion,demoType);
    
%Parse CLI strings and calculate derived parameters
P = parseCLICommands2Struct(cliCfg, supported_cfgs);
%calc_P = calculateChirpParams(P);


%% INIT SERIAL PORTS
if(REAL_TIME_MODE)
    %Init Ports
    hDataPort = initDataPort(comPort.data);
    hCfgPort = initCfgPort(comPort.cfg);
    
    %Check Port Status
    if(comPort.status == 0) %Unknown status
        if(hCfgPort ~= -1 && hDataPort ~=-1)
            if(hDataPort.BytesAvailable)
                %TODO: remove warning when config reload w/o NRST is enabled
                comPort.status = -1;
                fprintf('Device appears to already be running. Will not be able to load a new configuration. To load a new config, press NRST on the EVM and try again.');    
            else       
                fprintf(hCfgPort, 'version');
                pause(0.5); % adding some delay to make sure bytes are received
                response = '';
                if(hCfgPort.BytesAvailable)
                    for i=1:10 % version command reports back 10 lines TODO: change if SDK changes response
                        rstr = fgets(hCfgPort);
                        response = join(response, rstr);
                    end
                    fprintf('Test successful: CFG Port Opened & Data Received');
                    comPort.status = 1;
                else
                    fprintf('Port opened but no response received. Check port # and SOP mode on EVM');
                    comPort.status = -2;
                    fclose(hDataPort);
                    fclose(hCfgPort);
                end
            end
        else
            comPort.status = -2;
            fprintf('Could not open ports. Check port # and that EVM is powered with correct SOP mode.');    
        end
    end
     
else %REPLAY MODE
    
    %Load Data file
end

 %% Set flags based on COM port status
global RUN_VIZ
if(~REAL_TIME_MODE)
    RUN_VIZ = 1;
    LOAD_CFG = 0;
elseif(comPort.status == 1)
    LOAD_CFG = 1;
    RUN_VIZ = 1;
elseif(comPort.status == -1)
    LOAD_CFG = 0;
    RUN_VIZ = 1;
else
    RUN_VIZ = 0;
    LOAD_CFG = 0;
end
    
%% Load Config
if(LOAD_CFG) 
    loadCfg(hCfgPort, cliCfg);
end
        
if(RUN_VIZ)

%% INIT GESTURE PRESENCE STATE
hFig = figure('Name', 'mmWave Gesture Presence Detection', 'color','k');
global hText
hText = annotation(hFig, 'textbox',[0.25 0.25 0.5 0.5],'LineStyle', 'none','FontUnits','normalized','FontSize',0.1, 'String',' ','FitBoxToText','on','color','b');
pause(0.1)
ax_gest_img = axes('pos',[.25 .25 .3 .3],'Color','None');
l2r = imread('swipe_l2r.png');
r2l = imread('swipe_r2l.png');
set(ax_gest_img, 'box','off','XTickLabel',[],'XTick',[],'YTickLabel',[],'YTick',[])
pause(0.1);
%% INIT PLOT Figure
SHOW_PT_CLOUD = 0;
SHOW_TRACKED_OBJ = 0;
SHOW_STATS = 0;
SHOW_LANES = 0;
SHOW_PLOT = 0;
if(SHOW_PLOT)
hFig = figure('Name', 'Gesture Debug Visualizer V2.0.0','Color','white','CloseRequestFcn',@plotfig_closereq);

% init plot axes
maxRange = max([calc_P.rangeMax_m]);
[hFig, hAx3D] = init3DPlot_TM(hFig,[-maxRange maxRange],[0 maxRange], [0 offset.height]);

% init fov lines - approximate guidelines only
if (ANTENNA_TYPE==1)
    azFOV = 120; 
    elFOV = 40;  
else
    azFOV = 160; 
    elFOV = 160;
end
hFOVLines = drawFOVLines(hAx3D, azFOV, elFOV, maxRange, offset);
%reset the axes limits based on FOV TODO: Allow UI for axis limits
hAx3D.XLim = [min(hFOVLines.azimuth.XData) max(hFOVLines.azimuth.XData)];
hAx3D.YLim = [0 max([hFOVLines.boresight.YData hFOVLines.azimuth.YData hFOVLines.elevation.YData])];
hAx3D.ZLim = [0 max([hFOVLines.boresight.ZData hFOVLines.azimuth.ZData hFOVLines.elevation.ZData])];
% init handle for visualizing pt cloud
stylePtCloud = {'LineStyle','none','Marker','.','Color','b','MarkerSize',20};
hPtCloud = line(hAx3D,0,0,0, stylePtCloud{:});

% init handle for visualizing tracked objects
styleTrackObj = {'LineStyle','none','Marker','o','MarkerEdge','k','MarkerSize',20,'LineWidth',4};
hTrackObj = line(hAx3D,0,0,0, styleTrackObj{:});
hTrackObjLabel = text([0 1],[0 1],[0 1],{'0', '1'},'FontSize',10);
% init handle for visualizing TM lanes
if(SHOW_LANES)
    [hLanes hLaneLabels] = initLanes(hAx3D, lanes.x, lanes.y, 0, lanes.w, lanes.h, lanes.numLanes, 'r');
end
% init handle for stats textbox
styleStats = {'LineStyle','none', 'FontUnits','normalized','FontSize',0.025,'FitBoxToText','off'};
statsString = {'PlaceHolder'};
hStats = annotation(hFig,'textbox',[0.01 0.8 0.3 0.2], 'String', statsString, styleStats{:},'Color','k'); 

% init uicontrols
selectView = uicontrol(hFig,'Style','popupmenu',...
    'Position',[10 10 100 25],...
    'String',{'X-Y View','Y-Z View','X-Z View','3D View'},...
    'Callback',@(selectView,event) selection(selectView,hAx3D));

end

%% Pre-compute transformation matrix
rotMat_az = [cosd(offset.az) -sind(offset.az) 0; sind(offset.az) cosd(offset.az) 0; 0 0 1];
rotMat_el = [1 0 0; 0 cosd(offset.el) -sind(offset.el); 0 sind(offset.el) cosd(offset.el)];
transMat = rotMat_az*rotMat_el;
 
%% main - parse UART and update plots
if(REAL_TIME_MODE)
    bytesBuffer = zeros(1,hDataPort.InputBufferSize);
    bytesBufferLen = 0;
    isBufferFull = 0;
    READ_MODE = 'FIFO';
end
PRESENCE_STATE = -1;
r2lCount = 0;
l2rCount  = 0;
timer = tic; % start timer
while (RUN_VIZ)    
    
    % get bytes from UART buffer or DATA file
    if(REAL_TIME_MODE)
        [bytesBuffer, bytesBufferLen, isBufferFull, bytesAvailableFlag] = readUARTtoBuffer(hDataPort, bytesBuffer, bytesBufferLen, ENABLE_RECORD, fid);
         % parse bytes to frame
        [newframe, bytesBuffer, bytesBufferLen, numFramesAvailable,validFrame] = parseBytes_TM(bytesBuffer, bytesBufferLen, READ_MODE);
        frameIndex = 1;
    end
    
   
    
    if(validFrame(frameIndex))
        statsString = {['Frame: ' num2str(newframe.header.frameNumber)], ['Num Frames in Buffer: ' num2str(numFramesAvailable)]}; %reinit stats string each new frame
        if(1)

            % set frame flags
            HAVE_VALID_PT_CLOUD = newframe.header.timestamp ~= 0; %timestamp is used to contain num points
            numPoints = newframe.header.timestamp;
            HAVE_VALID_TARGET_LIST = ~isempty(newframe.targets);
            
            if(SHOW_PT_CLOUD)            
                if(HAVE_VALID_PT_CLOUD)
                    % Pt cloud hasn't been transformed based on offset TODO: move transformation to device
                    rotatedPtCloud = transMat * [newframe.detObj.x; newframe.detObj.y; newframe.detObj.z;];
                    hPtCloud.XData = rotatedPtCloud(1,:);
                    hPtCloud.YData = rotatedPtCloud(2,:);
                    hPtCloud.ZData = rotatedPtCloud(3,:)+offset.height; 

                else
                    hPtCloud.XData = [];
                    hPtCloud.YData = [];
                    hPtCloud.ZData = [];
                end
            end

            if(SHOW_TRACKED_OBJ)
                if(HAVE_VALID_TARGET_LIST)
                    numTargets = numel(newframe.targets.tid);
                    if(numTargets > 0)
                        %Tracker coordinates have been transformed by
                        %azimuth rotation but not elevation.
                        %TODO: Add elevation rotation and height
                        %offset on device
                            
                        rotatedTargets = rotMat_el * [newframe.targets.posX; newframe.targets.posY; newframe.targets.posZ;];
                        hTrackObj.XData = rotatedTargets(1,:);
                        hTrackObj.YData = rotatedTargets(2,:);
                        hTrackObj.ZData = rotatedTargets(3,:)+offset.height;
                        delete(hTrackObjLabel);
                        hTrackObjLabel = text(hAx3D,hTrackObj.XData,hTrackObj.YData,...
                            hTrackObj.ZData ,arrayfun(@(x) {num2str(x)}, newframe.targets.tid),...
                            'FontSize',10,'HorizontalAlignment','center','VerticalAlignment','middle');
                        
                    else
                        delete(hTrackObjLabel);
                        hTrackObjLabel = [];
                        hTrackObj.XData = [];
                        hTrackObj.YData = [];
                        hTrackObj.ZData = [];  
                    end
                else
                        delete(hTrackObjLabel);
                        hTrackObj.XData = [];
                        hTrackObj.YData = [];
                        hTrackObj.ZData = []; 
                        prevAccX = 0;
                end
            end
            
            
            if(1)
                if(HAVE_VALID_PT_CLOUD)
                    PRESENCE_STATE = 1;
                else
                    if(PRESENCE_STATE > 0)
                        PRESENCE_STATE=-1;
                    else
                        PRESENCE_STATE=PRESENCE_STATE-1;
                    end
                end
                
                if(HAVE_VALID_TARGET_LIST)
                    numTargets = numel(newframe.targets.tid);
                    if(numTargets > 0)

                        if(newframe.targets.velX > 0.01 && abs(newframe.targets.velX) > abs(newframe.targets.velZ) && abs(newframe.targets.accX)>newframe.targets.accY && abs(newframe.targets.accX)>newframe.targets.accZ)
                            %disp('R to L')
                            r2lCount = r2lCount+1;
                            l2rCount  = 0;
                            if(r2lCount >=1)
                            hImg = imshow(r2l,'Parent',ax_gest_img);
                            drawnow
                            end
                            %ax_gest_img.Color = 'w';
                        elseif (newframe.targets.velX < -0.01 && abs(newframe.targets.velX) > abs(newframe.targets.velZ) && abs(newframe.targets.accX)>newframe.targets.accY && abs(newframe.targets.accX)>newframe.targets.accZ)
                            r2lCount = 0;
                            l2rCount  = l2rCount+1;
                            if(l2rCount >=1)
                            hImg = imshow(l2r,'Parent',ax_gest_img);
                            drawnow
                            end
                            %ax_gest_img.Color = 'w';
                            %disp('L to R')

                        else
                            r2lCount = 0;
                            l2rCount  = 0;
                        end
 
                        
                    else

                    end
                else

                end
            end

            if(SHOW_STATS)
               if(HAVE_VALID_PT_CLOUD)
                   statsString{end+1} = ['Pt Cloud: ' num2str(newframe.detObj.numDetectedObj)];
               else
                    statsString{end+1} = ['Pt Cloud: '];
               end
                              
               if(HAVE_VALID_TARGET_LIST)
                   statsString{end+1} = ['Num Tracked Obj: ' num2str(numTargets)];
               else
                   statsString{end+1} = ['Num Tracked Obj: '];
               end
               
               % update string
               hStats.String = statsString;
            end 
            

            
         end % have validFrame
%         if(REAL_TIME_MODE)
%             drawnow limitrate
%         end
        
    elapsedTime = toc(timer);
    if (elapsedTime > 0.5)
        if(PRESENCE_STATE>0)
            set(hFig, 'color', 'w');
            %ax_gest_img.Color = 'w';
            hText.String = 'Presence Detected';
        elseif(PRESENCE_STATE<=-20)
            set(hFig, 'color', 'k');
            hText.String = 'Sleep Mode';
            %ax_gest_img.Color = 'k';
            cla(ax_gest_img)
        end
        timer = tic; % reset timer
        drawnow
    end
    else % have data in newFrame
    end
end %while inf


%% close ports
if(REAL_TIME_MODE)
    delete(instrfind);
    if(ENABLE_RECORD)
        c = fclose(fid);
        if(c == 0)
            disp('Log file closed w/o error.')
        else
            disp('Error closing log file.');
        end
    end
    disp('Visualizer terminated.')
end
end

%% Helper functions
function plotfig_closereq(src,callbackdata)
global RUN_VIZ
% Close request function 
% to display a question dialog box 
   s = questdlg('Close This Figure?',...
      'Close Request Function',...
      'Yes','No','Yes'); 
   switch s 
      case 'Yes'
         RUN_VIZ = 0; 
         delete(gcf)
      case 'No'
         RUN_VIZ = 1;
      return 
   end
end

function selection(selectView,ax)
    val = selectView.Value;
    switch val
        case 1
            view(ax, 0,90);           
        case 2
            view(ax, 90,0);
        case 3
            view(ax, 0,0);
        case 4
            view(ax, 170,10);
    end
end