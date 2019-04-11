function [y3,y2,y1,elapsed_time] = denoise_cmdf(im_n,sigma)

%=====================================================================
% Image noise reduction based on cascaded multi-domain flow (CMDF)
% Written by Meisam Rakhshnfar, Vidpro Lab
% (http://users.encs.concordia.ca/~amer/cmdf/)
% Revised: September 2016
%
% Copyright Notice:
% Copyright (c) 2011-2016 Concordia University
% All Rights Reserved.
%===================================================================== 

% denoise_cmdf(im_n,sigma)
 
%   im_n can be an M-by-N grayscale image array.
%   im_n cannot be RGB, an empty array or very small matrix.

%   sigma is the standard deviation of noise 

%  OUTPUTS:
%   y2 is the denoised image
%   y1 and y1 is the output of intermediate stages
%   elapsed_time: execution time

%%%
%%% parse input parameters
%%%
if (nargin~=2)
    error('not accuarte input parameters');
end
    
if (size(im_n,3)~=1)
    % only gray image
    error('Input image should be grayscale');
end

if ~isa(im_n,'single')
    im_n = single(im_n);
end
    
if (any(size(im_n)<64))
    error('Too Small Image');
end
if (size(im_n,1)>2160) || (size(im_n,2)>3840)
    error('Image size is not supported');
end
% check the size is divisible by 8
pad = [];
if any(mod(size(im_n),8))
    pad = 8*ceil(size(im_n)/8)-size(im_n);
    im_n = padarray(im_n,pad,'post','symmetric');
end

% call denoise function
tic;
[y3,y2,y1] = cascaded_mdf(im_n,sigma);
elapsed_time = toc;

if ~isempty(pad)
   y1 = y1(1:end-pad(1),1:end-pad(2));
   y2 = y2(1:end-pad(1),1:end-pad(2));
   y3 = y3(1:end-pad(1),1:end-pad(2));
end

function [y3,y2,y1] = cascaded_mdf(y0,sigma)
if ~isa(y0,'single')
    y0 = single(y0);
end

% stage 1

y1 = cmdfnl(single(y0),single(sigma));
alpha_2 = sqrt(0.55);
sigma_i2 = sigma*alpha_2;
%stage 2&3
[y3,y2] = cmdfldd(y1,single(sigma_i2));



