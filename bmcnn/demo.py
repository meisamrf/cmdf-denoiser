import numpy as np
import skimage.io
import matplotlib.pyplot as plt
import bmcnn
import time


def gray2rgb(im_rgb,img_gray):     
    ycbcr = skimage.color.rgb2ycbcr(im_rgb)
    ycbcr[:,:, 0] = img_gray
    rgb = skimage.color.ycbcr2rgb(ycbcr)
    rgb[rgb<0]=0
    rgb[rgb>1]=1
    return np.uint8(rgb*255)

# std of noise	
sigma = 15

# read RGB image
image =  skimage.io.imread('../dataset/peppers.png')
# convert to grayscale
im_gray = skimage.color.rgb2ycbcr(image)
im_gray = np.float32(im_gray[:,:,0])
# add noise
noise = np.random.normal(0,sigma,(im_gray.shape[0],im_gray.shape[1]))
y0 = np.float32(im_gray) + np.float32(noise)

handle = bmcnn.open('../bmcnn/models/bmcnn_16.bin')
bmcnn_out = np.zeros_like(y0)
y0 = y0/255
start_time = time.time()

bmcnn.predict(handle, y0, bmcnn_out)

elapsed_time = time.time()-start_time
bmcnn_out[bmcnn_out<0.0] = 0.0
bmcnn_out[bmcnn_out>1.0] = 1.0
bmcnn_out *=255 
y0 *=255

mse_noisy = np.mean((y0 - np.float32(im_gray)) ** 2)
mse_denoised = np.mean((bmcnn_out - np.float32(im_gray)) ** 2)

print('processing time =',elapsed_time, ' seconds')

plt.figure(figsize=(14, 14))
plt.subplot(1, 2, 1)
plt.title("Noisy image PSNR = %2.2f dB"%(10*np.log10(255*255/mse_noisy)), fontsize=10)
plt.axis('off')
y0[y0<0]=0
y0[y0>255]=255
plt.imshow(gray2rgb(image, y0))


plt.subplot(1, 2, 2)
plt.title("Denoised image PSNR = %2.2f dB"%(10*np.log10(255*255/mse_denoised)), fontsize=10)
plt.axis('off')
bmcnn_out[bmcnn_out<0]=0
bmcnn_out[bmcnn_out>255]=255
plt.imshow(gray2rgb(image, bmcnn_out))
plt.show()


