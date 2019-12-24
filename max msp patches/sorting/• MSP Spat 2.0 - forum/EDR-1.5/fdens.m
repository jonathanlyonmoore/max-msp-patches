function [fd,fd_min,fd_max,fd_tdens,dmax,tdens] = fdens(Tr,dmin,N,tdens)% FDENS   % %   [fd,fd_min,fd_max,fd_tdens,dmax,tdens] = fdens(Tr,dmin,N,tdens)%%	Computes recommended value of modal density (average number of modes%	per Hz) for a feedback delay network reverberator, according to%	smallest delay length, reverberation time and number of delay units.%%	Arguments:%	Tr : mid-frequency reverberation time (s)%	dmin : smallest delay length (ms)%	N : number of delay units (optional).%	tdens : minimum echo density after 3 loops (kHz) --default = 3.%%	Results:%	fd_min,fd_max : min and max values for smooth time response%	fd_tdens : max value for satisfying tdens requirement%	dmax : longest delay length (ms)%	tdens : actual time density after 3 loops (kHz)%% See also :  EDRanalysis, RI2Spat, Ener2MatSpat, MatSpat2critif ~exist('Tr'),  fprintf(' error: 1 argument at least'); return;end%%% Modal density should be at least equal to Tr/4, and, if possible,% equal to Tr.fd = Tr;%% sort out other args:if ~exist('dmin'),  return;else  dmin = dmin * 0.001;		% convert dmin to secondsendif ~exist('N'),  return;else  if N<2,    return;  endendif ~exist('tdens'),  tdens = 1;else  tdens = tdens * 1000;		% convert tdens to Hzend%%% For smooth time response, the first and second loops should overlap.% => dmax > 2*dmin% Since fdens = total length of all delay units, we have approximately:% fdens = N * (dmin + dmax) / 2.% => dmax = 2*fdens/N - dminfd_min = 1.5 * N * dmin;%%% If we impose that the individual outputs of the first loop be% separated by less than dmin, we have:%    (dmax-dmin)/(N-1) < dmin% => 2*fdens/N - 2*dmin < (N-1)*dminfd_max = N * (N+1) * dmin/2;%%% The echo density after 3 loops is given by: N*N*(N-1) / (dmax-dmin),% which must be higher than tdens% => 2*fdens/N - 2*dmin < N*N*(N-1) / tdensfd_tdens = N * ( N*N*(N-1) / (2*tdens) + dmin );%%% Compute recommended value (initialized with fd = Tr).fd = min(fd,fd_max);				% less than fd_maxfd = min(fd,fd_tdens);				% less than fd_tdensfd = max(fd,fd_min);				% more than fd_minfd = max(fd,Tr/4);				% more than Tr/4%%% Additional resultsif nargout > 4,  dmax = (2*fd/N - dmin)*1000;			% dmax in msendif nargout > 5,  tdens = N*N*(N-1) / (dmax*0.001 - dmin);  tdens = 0.001 * tdens;			% tdens in kHzend