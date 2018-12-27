clc;clear
%% load file
path = uigetdir('*txt'); 
logFiles = dir(path);
data =cell(size(logFiles,2));
for k = 3:size(logFiles,1)
    %if ~logFiles(i).isdir
    data{k-2}.vec = importdata([logFiles(k).folder '\' logFiles(k).name]);
    data{k-2}.name = logFiles(k).name;
end

%% 
idxRand =1;
idxjsq = 1;
for i=1:length(data)
    splName = strsplit(data{i}.name,'_');
    splName = splName(1:5);
    splName = cellfun(@str2num,splName,'UniformOutput',false);
    switch splName{1}
        case 0
        randData{idxRand}.data=data{i};
        randData{idxRand}.x= splName{5};
        randData{idxRand}.y = sum(data{i}.vec)/numel(data{i}.vec);
        idxRand = idxRand + 1;
        case 1
        jsqData{idxjsq}.data=data{i};
        jsqData{idxjsq}.x= splName{5};
        jsqData{idxjsq}.y = sum(data{i}.vec)/numel(data{i}.vec);
        idxjsq =idxjsq + 1;
    end
end
idxRand =idxRand-1;
idxjsq =idxjsq- 1;
%% plot data
jsqPlot.x =[];
jsqPlot.y =[];
randPlot.x =[];
randPlot.y =[];
for i =1 :length (jsqData)
    jsqPlot.x = [jsqPlot.x jsqData{i}.x];
    jsqPlot.y = [jsqPlot.y jsqData{i}.y];
    randPlot.x = [randPlot.x randData{i}.x];
    randPlot.y = [randPlot.y randData{i}.y];
end

%% plot
figure
random = plot( randPlot.x, randPlot.y,'b');
hold
jsq = plot(jsqPlot.x , jsqPlot.y,'r');
legend 

