function [Ssig] = istft(STFTtime, STFTfreq, STFT, nfft, origwin, roverlap)% ISTFT% %     [sig] = istft(STFTtime, STFTfreq, STFT, nfft, origwin, roverlap);% % Compute the inverse Fourier transform of a short-time Fourier spectrum% %	% STFTtime   : Time scale for the short-time Fourier transform (seconds)% STFTfreq   : Frequency scale for the short-time Fourier transform (Herz)% STFT       : Short-time Fourier transform signal% nfft       : Number of points in FFT computing of the short-time Fourier%              transform% originwind : window used in the short-time Fourier transform % roverlap   : overlap rate of the windows during % % sig        : output signal. Last samples (roverlap*nfft) are replaced with 0.%% See also : STFT, STFSlf = length(STFTfreq);lt = length(STFTtime);overlappoint = floor((1-roverlap)*nfft);product = (lt)*overlappoint+nfft;mylg = product - roverlap*nfft;sig =  zeros(1,product);sigwin = zeros(1,product);for i = 1:lt % fprintf('Computing inverse short-time Fourier transform : %d\r', i); vec = zeros(1,product); vecwin = zeros(1, product);  myvec = STFT(:,i); myvec = myvec(:)';  tmyvec = conj(fliplr(myvec(2:length(myvec)-1))); myvec = [myvec tmyvec]; sigi = ifft(myvec, nfft); w = zeros(size(sigi)); w(1:nfft-1) =real(sigi(2:nfft));  w = fliplr(w); vec(1+(i-1)*overlappoint:(i-1)*overlappoint+nfft) = w; vecwin(1+(i-1)*overlappoint:(i-1)*overlappoint+nfft) = origwin;  %fprintf('i %d,debut : %d, fin %d\n', i,1+(i-1)*overlappoint,(i-1)*overlappoint+nfft); sig = sig+vec; sigwin = sigwin+vecwin; endmoy = mean(sigwin(nfft:length(sigwin)-nfft));nsig = real(sig)/moy;%nsig = sig;lg = length(sig);Ssig = nsig(nfft/2+1: mylg+nfft/2);Ssig(length(Ssig)-nfft*roverlap:length(Ssig)) = zeros(size(Ssig(length(Ssig)-nfft*roverlap:length(Ssig))));fprintf('\n');