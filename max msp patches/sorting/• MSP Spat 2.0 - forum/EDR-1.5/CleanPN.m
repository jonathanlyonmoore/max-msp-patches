function [PNoiseOut, Index] = CleanPN(PNoiseIn)% CleanPN% %   [PN_New, index] = CleanP0(PNoise)%% This function smooths the energy signal of a corrupting noise%% PNoise : energy of the corrupting noiseafter smoothing%% PN_New : energy of the corrupting noiseafter smoothing% index  : vector of indexes where the PNoise has been corrected%% See Also : CleanP0, CleanTr, EDRanalysis_IterationIndex = [];PNoiseOut = [];[PNoise, Index] = CorrectNaN(PNoiseIn);vec = find(PNoise == 0);lv = length(vec);lp = length(PNoise);if(lp ~= lv) val = sum(PNoise)/(lp -lv); PNoise(vec) = val*ones(size(vec)); [PNoise, Index] = CorrectVal(PNoise, 0); [PNoise, Index] = CorrectVal(PNoise, eps);else val = eps; PNoise(vec) = val*ones(size(vec));end PNoiseOut = PNoise;