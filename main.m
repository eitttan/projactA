clc;clear
%% load file
path = uigetdir('*txt');
logFiles = dir(path);
data =cell(size(logFiles,2));
algoType = [];

for k = 3:size(logFiles,1)
    %if ~logFiles(i).isdir
    data{k-2}.vec = importdata([logFiles(k).folder '\' logFiles(k).name]);
    data{k-2}.name = logFiles(k).name;
    algoType =[algoType str2double(logFiles(k).name(1))];
end

%%
[C,ir,ic ] = unique(algoType);
numAlgo = numel(C);
numPointInAlgo = length(data)/numAlgo;
numRuns = length(data);

%% collect data
leg={};
x =[];y=[];
simParam = -1*(ones(numRuns,5));
%figure; hold on;
for j=1:numAlgo
    leg = [leg ,num2str(C(j))];
    for k =1 :numPointInAlgo
        i = numPointInAlgo*(j-1)+k;
        splName = strsplit(data{i}.name,'_');
        splName = splName(1:5);
        splName = cellfun(@str2num,splName,'UniformOutput',false);
        simParam(i,:)=cell2mat(splName);
        x(j,k) =splName{5};
        y(j,k) =sum(data{i}.vec)/numel(data{i}.vec);
    end
end

%% plot
figure;hold on
for j =1 :numAlgo
    plot (x(j,:),y(j,:))
end
legend (algoEnum(leg))
xlabel ('ratio'); ylabel ('avg per job [sec]');

if (numel(unique(simParam(:,2))== 1))
    switch unique(simParam(:,2))
        case -1
            errordlg('not equal size sim');
        case 0 
            simSize = 'Large';
        case 1
            simSize = 'Small';
    end     
end
if (numel(unique(simParam(:,3))== 1))
    switch unique(simParam(:,3))
        case -1
            errordlg
        case 0 
            typeServers = 'homogenous';
        case 1
            typeServers = 'hetrogenous *2';
        case 2
            typeServers = 'hetrogenous *10';
    end     
end
if (numel(unique(simParam(:,4))== 1))
    switch unique(simParam(:,4))
        case -1
            errordlg('not equal ratio');;%{ FAST_9_SLOW_1, FAST_5_SLOW_5, FAST_1_SLOW_9 
        case 0 
            speedServers = 'FAST_9_SLOW_1';
        case 1
            speedServers = 'FAST_5_SLOW_5';
        case 2
            speedServers = 'FAST_1_SLOW_9';
    end     
end
tit = ['avg time per job \newline' , simSize,' - ',typeServers];
if ~strcmp(typeServers,'homogenous')
    tit =[tit , ' - ' , speedServers];
end
title(tit);

%%  convergence  graph

% for jj =0:3:9
%     figure;hold on
%     for j =1 :numAlgo
%         d = data{ir(j)+jj}.vec;
%         dd =1:length(d);
%         d=d(dd);
%         xd = (1:numel(d));
%         cd = cumsum(d);
%         yd =cd(:) ./ xd(:) ;
%         % yd = (cumsum(d)./xd);
%         plot (xd,yd)
%     end
% end
% legend (algoEnum(leg))




