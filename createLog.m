%% create log file
% config
algoType = [0 1 2 3];
simSize = 0; % 0 Large , 1 Small
serverType = 0; %0 -homo,1  hetro *2 ,2  hetro *2
serverDist = 0; % 0 9-1 , 1 5-5 ,2 1-9
ratio = [0.6 0.7 0.8 0.83 0.86 0.89 0.9:0.015:0.99];

%% 
sizeVec = numel(algoType).*numel(ratio);
algoVec = repmat(algoType,(numel(ratio)),1);
algoVec =reshape(algoVec ,sizeVec,1);
simSizeVec =simSize.* ones(sizeVec,1);
serverTypeVec =serverType.* ones(sizeVec,1);
serverDistVec =serverDist.* ones(sizeVec,1);
ratioVec = repmat(ratio',1,(numel(algoType)));
ratioVec =reshape(ratioVec ,sizeVec,1);

mat = [algoVec simSizeVec serverTypeVec serverDistVec ratioVec];
%% write to log
filename = ['simConf_' , num2str(numel(algoType)),'Algo_', num2str(simSize),'_',num2str(serverType),...
    '_',num2str(serverDist),'_',num2str(numel(ratio)),'RatioS.txt'];
 dlmwrite(filename,mat,'delimiter',' ');


