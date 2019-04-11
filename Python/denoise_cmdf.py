import cmdftools
import numpy as np
import time
import skimage.color


def cmdfcall(im_n,sigma):

#=====================================================================
# Image noise redustion based on cascaded multi-domain flow (CMDF)
# Written by Meisam Rakhshnfar, Vidpro Lab
# (http://users.encs.concordia.ca/~amer/cmdf/)
# Revised: September 2016
#
# Copyright Notice:
# Copyright (c) 2011-2016 Concordia University
# All Rights Reserved.
#===================================================================== 

# denoise_cmdf(im_n,sigma)
 
#   im_n can be an M-by-N grayscale image  array.
#   im_n cannot be RGB, an empty array or very small matrix.

#   sigma the standard deviation of noise 

#  OUTPUTS:
#   y3: denoised image
#   y1, y2: output of intermediate stages
#   elapsed_time: execution time

###
### parse input parameters
###
 
    if (im_n.ndim!=2):
        # only gray image
        sys.stderr.write('Input image should be grayscale')
        sys.exit(1)

    if im_n.dtype==np.dtype('float32'):
        im_n = np.float32(im_n)
    
    if (im_n.shape[0]<64 or im_n.shape[1]<64):
        sys.stderr.write('Too Small Image')
        sys.exit(1)

    if (im_n.shape[0]>2160 or im_n.shape[1]>3840):
        sys.stderr.write('Image size is not supported')
        sys.exit(1)    
   
    # call denoiser function
    # check the size is divisible by 8
    pad_r, pad_c = 0,0
    if (im_n.shape[0] % 8 or im_n.shape[1] % 8):
        pad_r = np.int32(8*np.ceil(im_n.shape[0]/8.0)-im_n.shape[0])
        pad_c = np.int32(8*np.ceil(im_n.shape[1]/8.0)-im_n.shape[1])
        im_n = np.pad(im_n,((0,pad_r),(0,pad_c)),'symmetric')

    # initial outputs
    y1 = np.zeros_like(im_n)
    y2 = np.zeros_like(im_n)
    y3 = np.zeros_like(im_n)

    start_time = time.time()
    cascaded_mdf(im_n,y1,y2,y3,sigma)
    elapsed_time = time.time() - start_time

    if (pad_r or pad_c):
        y3 = y3[0:y3.shape[0]-pad_r,0:y3.shape[1]-pad_c]

    #clip
    y3[y3<0] = 0
    y3[y3>255] = 255

    return np.uint8(y3), elapsed_time

def cascaded_mdf(y0,y1,y2,y3,sigma):
    '''
    Cascading stages 1,2, and 3 of filters
    INPUTS:
        y0: noisy image
        sigma: the standard deviation of noise  
    OUTPUTS:
        y3: denoised image
        y1, y2: output of intermediate stages
    '''

    if y0.dtype==np.dtype('float32'):
        y0 = np.float32(y0)

    # stage 1
    cmdftools.filter1(y0, y1 ,np.float32(sigma))
    alpha_2 = np.sqrt(0.55);
    sigma_i2 = sigma*alpha_2;
    #stage 2&3
    res = cmdftools.filter2(y1,y2,y3,np.float32(sigma_i2))
    return res

