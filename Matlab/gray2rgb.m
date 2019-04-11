function im_o = gray2rgb(im_rgb,img_gray)

im_gray = single(rgb2gray(im_rgb));
dif_gray = img_gray-im_gray;
imgf = single(im_rgb);
imgf(:,:,1) = imgf(:,:,1) + dif_gray;
imgf(:,:,2) = imgf(:,:,2) + dif_gray;
imgf(:,:,3) = imgf(:,:,3) + dif_gray;
imgf(imgf<0) = 0;imgf(imgf>255) = 255;imshow(imgf/255);
im_o = uint8(round(imgf));