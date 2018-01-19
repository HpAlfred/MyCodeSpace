#!/usr/bin/python3
# -*-coding:gbk-*-
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

img = Image.open('lena.jpg')
img = np.array(img)
if img.ndim == 3:
    img = img[:,:,0]

plt.imshow(img, cmap = plt.cm.gray_r)
plt.show()