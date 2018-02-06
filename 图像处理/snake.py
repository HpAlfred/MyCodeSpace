#!/usr/bin/python3
# -*-coding:gbk-*-
import turtle
def drawSnake(rad,angle,len,neckrad):
    for i in range(len):
        turtle.circle(rad,angle)
        turtle.circle(-rad, angle)
    turtle.cir
