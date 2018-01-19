#!/usr/bin/python3
# -*-coding:gbk-*-
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
im=np.array(Image.open('D:\MyProject\MyCodeSpace\图像处理\lena.jpg'))
x = 0
y = 0
while ( y < 200):
    print('\n')
    while ( x < 200 ):
        Gray = (im[x,y,0] * 76 + im[x,y,1] * 151 + im[x,y,2] * 28) >> 8;
        im[x, y, 0] = Gray
        im[x, y, 1] = Gray
        im[x, y, 2] = Gray
        x=x+1
    y=y+1
    x=0
plt.figure("dog")
plt.imshow(im)
plt.axis('off')
plt.show()
