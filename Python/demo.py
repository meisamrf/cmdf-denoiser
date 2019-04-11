import time
import numpy as np
import skimage.io
import matplotlib.pyplot as plt
import denoise_cmdf


def gray2rgb(im_rgb,img_gray):     
    ycbcr = skimage.color.rgb2ycbcr(im_rgb)
    ycbcr[:,:, 0] = img_gray
    rgb = skimage.color.ycbcr2rgb(ycbcr)
    rgb[rgb<0]=0
    rgb[rgb>1]=1
    return np.uint8(rgb*255)

# std of noise
sigma = 15

np.random.seed(2019)
# read RGB image
image =  skimage.io.imread('../dataset/peppers.png')
# convert to grayscale
im_gray = skimage.color.rgb2ycbcr(image)
im_gray = np.float32(im_gray[:,:,0])

# add noise
noise = np.random.normal(0,sigma,(im_gray.shape[0],im_gray.shape[1]))
y0 = np.float32(im_gray) + np.float32(noise)

# denoise with CMDF
y3, elapsed_time = denoise_cmdf.cmdfcall(y0, sigma)
mse_noisy = np.mean((y0 - np.float32(im_gray)) ** 2)
mse_denoised = np.mean((y3 - np.float32(im_gray)) ** 2)

print('processing time =',elapsed_time, ' seconds')

plt.figure(figsize=(14, 14))
plt.subplot(1, 2, 1)
plt.title("Noisy image PSNR = %2.2f dB"%(10*np.log10(255*255/mse_noisy)), fontsize=10)
plt.axis('off')
y0[y0<0]=0
y0[y0>255]=255
plt.imshow(gray2rgb(image, im_gray))
            
plt.subplot(1, 2, 2)
plt.title("Denoised image PSNR = %2.2f dB"%(10*np.log10(255*255/mse_denoised)), fontsize=10)
plt.axis('off')
plt.imshow(gray2rgb(image, im_gray))
plt.show()
