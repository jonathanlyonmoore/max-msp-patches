function [Value]=sigWrite(varargin)% SIGWRITE%% function [Value]=sigWrite(x,name,sr,{form,res,end})% % x       : vector to be output% name    : filename with or without extension. Complete filename must be %           provided (path included)% sr      : sampling rate% form    : file format, needed if RIname is provided without extension %              - 'snd': NeXT sound file format%              - 'au' : Sun sound file format (equivalent to NeXT)%              - 'rif': AMS format. Requires two files file.inf and%                       file.rif%              - 'inf': same as 'rif'%              - 'aif': AIFF format,(or AIFC without compression)%                       (Apple, SGI)%              - 'raw': raw data, requires precision argument. Sampling rate%                       argument won't be used%              - 'wav': Wav format (Windows95, Windows 98, Windows NT)% res     : resolution : 'int8, 'int16'(default), 'int32',float32, float64%% Parameters 3 to 6 are optionnal. Only mono files are considered now.%% See also : sigRead% 28 october 1998 : imnplementing WAV et Snd fileserror(nargchk(3, 6,nargin));Value = 1;msg = [];[msg, filedata, fileName, fileForm, fileSR, fileRes, fileEndian]=sigWritechk(varargin);if(~isempty(msg))  Value = 0;  error(msg);end  switch(fileForm)     case {'snd','sau'}        fid = fopen(fileName, 'w', 'ieee-be');        if(fid == -1)          fprintf('There was a problem creating the output file. Check your machine,\n')           fprintf('or have a serious talk with the system administrator\n');        else	switch(fileForm)          case 'snd',             fprintf('Writing NeXT snd file\n');          case 'sau'             fprintf('Writing Sun au file\m');          end          switch fileRes            case 'int8',              encode = 2;              fact = 1;            case 'int16',              encode = 3;              fact = 2;            case 'int32',              encode = 5;              fact = 4;            case 'float32',              encode = 6;              fact = 4;            case 'float64',              encode = 7;              fact = 8;            otherwise              fprintf('Unavailable encoding. Using default resolution : int16\n');              encode = 3;              fact = 2;          end           %%Magic number	          chunkletter = '.snd';          v = fwrite(fid, chunkletter, 'char');            %Header          toWrite = 28;          v = fwrite(fid,toWrite,'uint32');          toWrite = length(filedata)*fact;          v = fwrite(fid,toWrite,'uint32');          toWrite = encode;          v = fwrite(fid,toWrite,'uint32');          toWrite = fileSR;          v = fwrite(fid,toWrite,'uint32');          toWrite = 1; %channel          v = fwrite(fid,toWrite,'uint32');          toWrite = 0;          v = fwrite(fid,toWrite,'uint32');                            switch(fileRes)           case 'int8',               samplesize = 8;              tempvec = floor(filedata*(2^(samplesize -1)-1));               v = fwrite(fid,tempvec, fileRes);           case 'int16',              samplesize = 16;              tempvec = floor(filedata*(2^(samplesize -1)-1));                v = fwrite(fid,tempvec, fileRes);           case 'int32',              samplesize = 32;              tempvec = floor(filedata*(2^(samplesize -1)-1));               v = fwrite(fid,tempvec, fileRes);           otherwise,             tempvec = filedata;             v = fwrite(fid,tempvec, fileRes);          end           fclose(fid);        end     case 'aif',       switch(fileRes)          case 'int8',               samplesize = 8;          case 'int16',              samplesize = 16;          case 'int32',              samplesize = 32;          otherwise,              fprinf('Data types incompatible with AIFF/AIFC file format or not yet supported\n');                Value = 0; return;              fclose(fid);        end              fid = fopen(fileName, 'w', 'ieee-be');       if(fid == -1)         fprintf('There was a problem creating the output file. Check your machine,\n')          fprintf('or have a serious talk with the system administrator\n');         keyboard       else         s = length(filedata);         ts = s*samplesize/8;         if(mod(ts, 2) == 0)             flagadd = 0;         else            flagadd = 1;         end                  fprintf('Writing AIFC file without compression : %s\n', fileName);             %FORM chunk         chunkletter = 'FORM';         v = fwrite(fid, chunkletter, 'char');         ssize = 78+ts+flagadd;         v =  fwrite(fid, ssize,  'int32');         chunkletter = 'AIFC';         v = fwrite(fid, chunkletter, 'char');                     %FVER chunk         chunkletter = 'FVER';         v = fwrite(fid, chunkletter, 'char');         ssize = 4;         v =  fwrite(fid, ssize,  'int32');         timestamp = 27263184;         v =  fwrite(fid,timestamp,  'int32');                 %COMM         chunkletter = 'COMM';         v = fwrite(fid, chunkletter, 'char');         ssize = 38;         v =  fwrite(fid, ssize,  'int32');         numchan = 1;         v =  fwrite(fid, numchan,  'int16');         v =  fwrite(fid, s,  'int32');                         switch(fileRes)            case 'int8',               samplesize = 8;            case 'int16',              samplesize = 16;            case 'int32',              samplesize = 32;            otherwise,              fprintf('Data format not yet supported\n');              fclose(fid);          end          v =  fwrite(fid, samplesize,  'int16');                     %la frequence d'echantillonage          tfe = converttoIEEE(fileSR);          v =  fwrite(fid, tfe, 'ubit1');                   chunkletter = 'NONE';          v = fwrite(fid, chunkletter, 'char');          completter = 'not compressed';          v = fwrite(fid, completter, 'char');            ms = blanks(2);          v = fwrite(fid, ms, 'char');                 %SSND          chunkletter = 'SSND';          v = fwrite(fid, chunkletter, 'char');          ssize = ts+8;          v = fwrite(fid, ssize, 'int32');          val = 0;          fwrite(fid, val, 'int32');          fwrite(fid, val, 'int32');                 %ecrit le fichier                    %padding eventuel          tempvec = floor(filedata*(2^(samplesize -1)-1));          v = fwrite(fid,tempvec, fileRes);          if(flagadd == 1)            val = 0;            fwrite(fid, val, fileRes);          end         fclose(fid);       end     case 'rif',       kurzname = strtok(fileName, '.');       infname = [kurzname '.inf'];       rifname = [kurzname '.rif'];             fid1 = fopen(infname, 'w', 'ieee-le');       if(fid1 == -1)         fprintf('There was a problem creating the output file. Check your machine,\n')          fprintf('or have a serious talk with the system administrator\n');       else          type = char(0);fwrite(fid1,type,'char')          fwrite(fid1, rifname,'char'),l = length(rifname);diff = 64-l;Sempty = blanks(diff);fwrite(fid1,Sempty,'char'),          dummy = blanks(6);fwrite(fid1, dummy, 'char'),          mydate = datestr(now);l = length(mydate); diff = 26-l; Sempty = blanks(diff);          fwrite(fid1, mydate, 'char'), fwrite(fid1,  Sempty , 'char'),          myinfo = 'from Matlab'; l = length(myinfo); diff = 64-l; Sempty = blanks(diff);          fwrite(fid1, myinfo), fwrite(fid1, Sempty, 'char')           fwrite(fid1,fileSR ,'float32');                    mymax = max(abs(filedata));          fcoeff = mymax/32767          fwrite(fid1, fcoeff, 'float32');          offset = 0;          fwrite(fid1, offset, 'float32');                    mysize = size(filedata);          fwrite(fid1, mysize, 'uint32');                    sigmax = max(filedata)/fcoeff;          sigmin = min(filedata)/fcoeff;          fwrite(fid1, sigmin, 'int16');          fwrite(fid1, sigmax, 'int16');                fclose(fid1);       end       fid2 = fopen(rifname, 'w', 'ieee-le');       if(fid2 == -1)           fprintf('There was a problem creating the output file. Check your machine,\n')            fprintf('or have a serious talk with the system administrator\n');       else           fwrite(fid2, filedata/fcoeff, 'int16');           fclose(fid2);       end            case 'raw',       if(strcmp(fileEndian, 'little'))         fid =  fopen(fileName, 'w', 'ieee-le');       else          if(strcmp(fileEndian, 'big'))            fid =  fopen(fileName, 'w', 'ieee-be');         else             fid =  fopen(fileName, 'w', 'native');         end       end       if(fid == -1)         fprintf('There was a problem creating the output file. Check your machine,\n')          fprintf('or have a serious talk with the system administrator\n');       else          fprintf('Writing raw data for file %s.\n', fileName);          v = fwrite(fid,filedata,fileRes);          fclose(fid);       end            case 'wav',       switch(fileRes)           case 'int8',               samplesize = 8;           case 'int16',              samplesize = 16;           otherwise,              fprinf('Data types incompatible with WAV file format or not yet supported\n');              fprintf('Using default resolution : 16 bits\n');              samplesize = 16;        end        fprintf('Writing WAV file\n');        wavwrite(filedata, fileSR, samplesize, fileName);     otherwise,       Value = 0;       msg = 'Unknown file format';       error(msg);        endfunction [msg, data, name,form,sr, res, endian] = sigWritechk(P)     %P : (x,name,sr,form,res,end)lp = length(P); msg = [];v = (P(1));data = cat(1,v{:});if(isempty(data))  msg = 'No data given !!';  data = [];res = []; name = [];sr = 0; res = 0;endian =[];  return;endif(lp == 3) name = char(P(2));  if (isempty(name))   msg = 'No file name given; you''re too hard for me poor computer!!';   data = [];res = []; name = [];sr = 0; res = 0;endian =[];   return; end [t r] = strtok(name, '.');   if(isempty(r))   msg = 'No file format given. Either use an extension, or the form field!!';   data = [];res = []; name = [];sr = 0; res = 0;endian =[];   return; else   switch(r)    case '.aif',       form  = 'aif';       fprintf('Using default resolution 16 bits\n');       res = 'int16';       endian = 'big';    case '.snd',       form = 'snd';       fprintf('Using default resolution 16 bits\n');       res = 'int16';       endian = 'big';    case '.au',       form = 'sau';       fprintf('Using default resolution 16 bits\n');       res = 'int16';       endian = 'big';    case '.rif',       form = 'rif';          res = 'int16';       endian = 'little';    case '.inf'       form = 'rif';       res = 'int16';       endian = 'little';    case '.raw'       form = 'raw';       fprintf('Using default resolution 16 bits\n');       res = 'int16';       fprintf('Using machine endian ordering\n');       endian = 'mach';    case '.wav'       form = 'wav';       fprintf('Using default resolution 16 bits\n');       res='int16';       endian = 'little';    otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;   end end b = P(3); sr = cat(1,b{:}); if(isempty(sr) | (sr == 0))   fprintf('Using default sampling rate : 44100\n');   sr = 44100; end   end     if(lp == 4) %P : (x,name,sr,form) name = char(P(2)); form = char(P(4)); if(strcmp(form, 'au'))     form = 'sau'; end   if(isempty(form))    if (isempty(name))       msg = 'No file name given; you''re too hard for me poor computer!!';       data = [];res = []; name = [];sr = 0; res = 0;endian =[];       return;    end     [t r] = strtok(name, '.');        if(isempty(r))       msg = 'No file format given. Either use an extension, or the form field!!';       data = [];res = []; name = [];sr = 0; res = 0;endian =[];       return;    else     switch(r)      case '.aif',        form  = 'aif';        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'big';      case '.snd',        form = 'snd';        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'big';      case '.au',        form = 'sau';        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'big';      case '.rif',        form = 'rif';           res = 'int16';        endian = 'little';      case '.inf'        form = 'rif';        res = 'int16';        endian = 'little';      case '.raw'        form = 'raw';        fprintf('Using default resolution 16 bits\n');        res = 'int16';        fprintf('Using machine endian ordering\n');        endian = 'mach';      case '.wav'        form = 'wav';        fprintf('Using default resolution 16 bits\n');        endian = 'little';      otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;    end   end   else   if(isempty(name))      name = 'file';   end           [t r] = strtok(name, '.');     if(isempty(r))     switch(form)      case 'aif',        name = [name '.' form];        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'big';      case 'snd',        name = [name '.' form];        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'big';      case 'au',        name = [name '.au'];        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'big';      case 'rif',        res = 'int16';        endian = 'little';      case 'inf'        form = 'rif';        res = 'int16';        endian = 'little';      case 'raw'        name = [name '.' form];         fprintf('Using default resolution 16 bits\n');        res = 'int16';        fprintf('Using machine endian ordering\n');        endian = 'mach';      case 'wav'        name = [name '.' form];        fprintf('Using default resolution 16 bits\n');        res = 'int16';        endian = 'little';      otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;     end   else     testform = r(2:length(r));     if(strcmp(testform , 'au'))       testform = 'sau';     end           if(strcmp(form, testform) == 0)        msg = 'Incompatible extension and file format';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;     else        switch(form)          case 'aif',           fprintf('Using default resolution 16 bits\n');           res = 'int16';           endian = 'big';          case 'snd',           fprintf('Using default resolution 16 bits\n');           res = 'int16';           endian = 'big';          case 'sau',           fprintf('Using default resolution 16 bits\n');           res = 'int16';           endian = 'big';          case 'rif',           res = 'int16';           endian = 'little';          case 'inf'           form = 'rif';           res = 'int16';           endian = 'little';          case 'raw'           fprintf('Using default resolution 16 bits\n');           res = 'int16';           fprintf('Using machine endian ordering\n');           endian = 'mach';          case 'wav'           fprintf('Using default resolution 16 bits\n');           res = 'int16';           endian = 'little';          otherwise,           msg = 'File format unknown!!';           data = [];res = []; name = [];sr = 0; res = 0;endian =[];           return;      end     end   end end  if(isempty(name))   name = 'file'; end %srate   b = P(3); sr = cat(1,b{:}); if(isempty(sr) | (sr == 0))   fprintf('Using default sampling rate : 44100\n');   sr = 44100; endendif(lp == 5) %(x,name,sr,form,res)  name = char(P(2)); form = char(P(4)); res = char(P(5)); if(strcmp(form, 'au'))     form = 'sau'; end   if(isempty(form))    if (isempty(name))       msg = 'No file name given; you''re too hard for me poor computer!!';       data = [];res = []; name = [];sr = 0; res = 0;endian =[];       return;    end     [t r] = strtok(name, '.');        if(isempty(r))       msg = 'No file format given. Either use an extension, or the form field!!';       data = [];res = []; name = [];sr = 0; res = 0;endian =[];       return;    else     switch(r)      case '.aif',        form  = 'aif';        endian = 'big';      case '.snd',        form = 'snd';        endian = 'big';      case '.au',        form = 'sau';        endian = 'big';      case '.rif',        form = 'rif';           endian = 'little';      case '.inf'        form = 'rif';        endian = 'little';      case '.raw'        form = 'raw';        fprintf('Using machine endian ordering\n');        endian = 'mach';      case '.wav'        form = 'wav';        endian = 'little';      otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;    end   end   else   if(isempty(name))      name = 'file';   end   [t r] = strtok(name, '.');     if(isempty(r))     switch(form)      case 'aif',        name = [name '.' form];        endian = 'big';      case 'snd',        name = [name '.' form];        endian = 'big';      case 'au',        name = [name '.au'];        endian = 'big';      case 'rif',        res = 'int16';        endian = 'little';      case 'inf'        form = 'rif';        endian = 'little';      case 'raw'        name = [name '.' form];         fprintf('Using machine endian ordering\n');        endian = 'mach';      case 'wav'        name = [name '.' form];        endian = 'little';      otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;     end   else     testform = r(2:length(r));     if(strcmp(testform , 'au'))       testform = 'sau';     end           if(strcmp(form, testform) == 0)        msg = 'Incompatible extension and file format';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;     else        switch(form)          case 'aif',            endian = 'big';          case 'snd',            endian = 'big';          case 'sau',            endian = 'big';          case 'rif',            endian = 'little';          case 'inf'            endian = 'little';          case 'raw'            fprintf('Using machine endian ordering\n');           endian = 'mach';          case 'wav'            endian = 'little';          otherwise,           msg = 'File format unknown!!';           data = [];res = []; name = [];sr = 0; res = 0;endian =[];           return;      end     end   end end         %srate   b = P(3); sr = cat(1,b{:}); if(isempty(sr) | (sr == 0))   fprintf('Using default sampling rate : 44100\n');   sr = 44100; end if(isempty(res))   fprintf('Using default resolution : int16\n');   res = 'int16'; else   switch(res)     case 'int8',res = 'int8';     case 'int16', res = 'int16';     case 'int32', res = 'int32';     case 'float', res = 'float';     otherwise,      fprintf('Warning : unknown resolution, using default : int16\n');      res = 'int16';        end endendif(lp == 6)   %(x,name,sr,form,res)  name = char(P(2)); form = char(P(4)); res = char(P(5)); endian = char(P(6)); if(strcmp(form, 'au'))     form = 'sau'; end   if(isempty(form))    if (isempty(name))       msg = 'No file name given; you''re too hard for me poor computer!!';       data = [];res = []; name = [];sr = 0; res = 0;endian =[];       return;    end     [t r] = strtok(name, '.');        if(isempty(r))       msg = 'No file format given. Either use an extension, or the form field!!';       data = [];res = []; name = [];sr = 0; res = 0;endian =[];       return;    else     switch(r)      case '.aif',        form  = 'aif';        endian = 'big';      case '.snd',        form = 'snd';        endian = 'big';      case '.au',        form = 'sau';        endian = 'big';      case '.rif',        form = 'rif';           endian = 'little';      case '.inf'        form = 'rif';        endian = 'little';      case '.raw'        form = 'raw';      case '.wav'        form = 'wav';        endian = 'little';      otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;    end   end   else   if(isempty(name))      name = 'file';   end   [t r] = strtok(name, '.');     if(isempty(r))     switch(form)      case 'aif',        name = [name '.' form];        endian = 'big';      case 'snd',        name = [name '.' form];        endian = 'big';      case 'sau',        name = [name '.au'];        endian = 'big';      case 'rif',        res = 'int16';        endian = 'little';      case 'inf'        form = 'rif';        endian = 'little';      case 'raw'        name = [name '.' form];        case 'wav'        name = [name '.' form];        endian = 'little';      otherwise,        msg = 'File format unknown!!';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;     end   else     testform = r(2:length(r));     if(strcmp(testform , 'au'))       testform = 'sau';     end           if(strcmp(form, testform) == 0)        msg = 'Incompatible extension and file format';        data = [];res = []; name = [];sr = 0; res = 0;endian =[];        return;     else        switch(form)          case 'aif',            endian = 'big';          case 'snd',            endian = 'big';          case 'sau',            endian = 'big';          case 'rif',            endian = 'little';          case 'inf'            endian = 'little';          case 'raw'            endian = 'mach';          case 'wav'            endian = 'little';          otherwise,           msg = 'File format unknown!!';           data = [];res = []; name = [];sr = 0; res = 0;endian =[];           return;      end     end   end end         %srate   b = P(3); sr = cat(1,b{:}); if(isempty(sr) | (sr == 0))   fprintf('Using default sampling rate : 44100\n');   sr = 44100; end if(isempty(res))   fprintf('Using default resolution : int16\n');   res = 'int16'; else   switch(res)     case 'int8',res = 'int8';     case 'int16', res = 'int16';     case 'int32', res = 'int32';     case 'float32', res = 'float32';     case 'float64', res = 'float64';     otherwise,      fprintf('Warning : unknown resolution, using default : int16\n');      res = 'int16';        end end if(strcmp(form, 'raw') == 0)  if(~isempty(endian))   fprintf('Endian (les petits et grands indiens, whouloulou) not used for this file format\n');  end else   switch(endian)    case 'little',      endian = 'little';    case 'big',      endian = 'big';    case 'mach',      endian = 'mach';    otherwise,      fprintf('Unknown endian order; using machine endian order\n');      endian = 'mach';    end       endendfunction [ieeev ] = converttoIEEE(v)ieeev = zeros(1, 80);sign = zeros(1, 16);if(v<0)  sign(1) = 1;  v = -v;end if (v == 0)  expon = 0;  hiMant = 0;  lowMant = 0;else i = 0; val = 1;  while(v > val)   i = i+1;   val = 2^i; end  fMant = v/val; expon = i; if((expon > 16384) | (~(fMant < 1)))      a = [0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1];       b = a;      if(sign(1) == 1)        b(1) = 1;      end      expon = ar2num(b)      hiMant = 0;       loMant = 0;     else     expon = expon+16382;       if (expon < 0)      fMant = fMant*2^expon;      expon = 0;    end           c = dec2bin(expon);    d = zeros(1,16);    l = length(c);    k = 1;    for j = 16-l+1:16      if(strcmp(c(k), '1'))        d(j) = 1;      else        d(j) = 0;      end      k = k+1;      end        lexp = d;    if(sign(1) == 1)        lexp(1) = 1;    end     expon = ar2num(lexp);         fMant = fMant*2^32;    fsMant = floor(fMant);     hiMant = fsMant;    fMant = (fMant - fsMant)*2^32;     fsMant = floor(fMant);     loMant = fsMant;  end    if(length(expon) ~=16)     expon = binarray16(expon);  end  bHiMant = binarray32(hiMant);  bloMant = binarray32(loMant);    ieeev = [expon bHiMant bloMant]; endfunction [varray] = binarray16(val) varray = zeros(1,16) ; binval = dec2bin(val);  l = length(binval); for(i=1:l)  if(strcmp(binval(i), '1'))   varray(16-l+i) = 1;  else    varray(16-l+i) = 0;  end end    function [varray] = binarray32(val) varray = zeros(1,32);  binval = dec2bin(val);  l = length(binval); for(i=1:l)  if(strcmp(binval(i), '1'))   varray(32-l+i) = 1;  else    varray(32-l+i) = 0;  end end    function [vf]=ar2num(v)%msb is leftlg = length(v);for i=1:lg  if ((v(i) == 0) | (v(i) == 1))        %nothing    else       vf = NaN;       return;  endendw = fliplr([0:lg-1]);w2 = 2.^w;w2 = w2.*v;vf = sum(w2);