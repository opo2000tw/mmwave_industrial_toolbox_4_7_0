function [hFig, hAx] = init3DPlot(hFig, xLimits, yLimits, zLimits)
    
    % setup axes with manual limits
    hAx = axes('parent',hFig,'Color','w','Position',[0.05 0.03 0.9 0.9],'DataAspectRatio', [1 1 1]);
    hAx.XLim = xLimits;
    hAx.YLim = yLimits;
    hAx.ZLim = zLimits;
    hAx.XLimMode = 'manual';
    hAx.YLimMode = 'manual';
    hAx.ZLimMode = 'manual';
    hAx.XColor = 'blue';
    hAx.YColor = 'black';
    hAx.ZColor = 'magenta';
    xlabel('X [m]'), ylabel('Y [m]'), zlabel('Z [m]')
    
    % draw reference lines for the axes at the origin
    refLineWidth = 1;
    line(hAx, xLimits, [0 0],'Color','blue','LineWidth', refLineWidth);
    %line(hAx, [0 0], yLimits,'Color','k','LineWidth', refLineWidth);
    line(hAx, [0 0], [0,5],'Color','k','LineWidth', refLineWidth);

    line(hAx, [0 0], [0 0], zLimits,'Color','magenta','LineWidth', refLineWidth);

    %Dotted Lines for inner Zone
    line(hAx, [-2 -1], [.5,.5], 'Color', 'r', 'LineStyle', '--','LineWidth', refLineWidth);
    line(hAx, [1 2], [.5,.5], 'Color', 'r', 'LineStyle', '--','LineWidth', refLineWidth);

    line(hAx, [-1 -1], [-1,.5], 'Color', 'r', 'LineStyle', '--','LineWidth', refLineWidth);
    line(hAx, [1 1], [-1,.5], 'Color', 'r', 'LineStyle', '--','LineWidth', refLineWidth);
    
    %Dotted Line for Activation Zone 
    line(hAx, [-2 2], [1.5,1.5], 'Color', 'g', 'LineStyle', '-','LineWidth', 4);

    
    % setup grid
    set(hAx, 'XGrid', 'on', 'YGrid', 'on', 'ZGrid', 'on', 'GridColor', [0 0 0])
    %set(hAx, 'XMinorGrid', 'on', 'YMinorGrid', 'on', 'ZMinorGrid', 'on', 'MinorGridColor', [0.95 0.95 0.95])

    % setup camera view and persepctive
    %set(hAx,'DataAspectRatio', [1 1 1], 'DataAspectRatioMode', 'manual');
    %set(hAx, 'CameraUpVector', [0 0 1], 'CameraUpVectorMode', 'manual');
    %view(hAx,210,10)
    view(2)

end