% demo for image denoising (AWGN)
clc
clear
close all
rng('default');

% read RGB image
image = imread('peppers.png');
% convert to grayscale
im_gray = single(rgb2gray(image));

% add noise
sigma  = 15;
noise = sigma*randn(size(im_gray));
y0 = im_gray + single(noise);
% denoise
[y3,y2,y1,elapsed_time] = denoise_cmdf(y0,sigma);

%show results
subplot(2,2,1);title('y0')
imshow(gray2rgb(image,y0));
xlabel(['PSNR = ' num2str(10*log10(255*255/mean((y0(:)-im_gray(:)).^2)),'%2.2f')]);


subplot(2,2,2);title('y1')
imshow(gray2rgb(image,y1));
xlabel(['PSNR = ' num2str(10*log10(255*255/mean((y1(:)-im_gray(:)).^2)),'%2.2f')]);


subplot(2,2,3);title('y2')
imshow(gray2rgb(image,y2));
xlabel(['PSNR = ' num2str(10*log10(255*255/mean((y2(:)-im_gray(:)).^2)),'%2.2f')]);


subplot(2,2,4);title('y3')
imshow(gray2rgb(image,y3));
xlabel(['PSNR = ' num2str(10*log10(255*255/mean((y3(:)-im_gray(:)).^2)),'%2.2f')]);


disp(['processing time = ' num2str(elapsed_time,'%2.2f seconds')])
