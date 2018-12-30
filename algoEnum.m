function algoName = algoEnum(varargin)

algo = cellfun(@str2num ,varargin{1});
algoName = cell(size(algo));
for ia =1:numel(algo)
    switch algo(ia)
        case 0
            algoName{ia} = 'Random';
        case 1
            algoName{ia} = 'JSQ';
        case 2
            algoName{ia} = 'JIQ';
        case 3
            algoName{ia} = 'PI';
    end
end
end

