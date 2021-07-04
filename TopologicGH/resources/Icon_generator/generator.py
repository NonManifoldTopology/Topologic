# -*- coding: utf-8 -*-
"""
Created on Sun Aug  2 15:03:19 2020

@author: Patrick.Barry
"""





#output the list of duplciate icons






#for icons generated, make white transparent
# make input blacks a red tint?

import os
from csv import reader
# reads in CSVs in an adaptable way
import glob
# good for getting lists of files of a type
from PIL import Image




def PixelCount(file):
    counter = 0
    im = Image.open(file)
    #ims = im.size  #(width, height) tuple
    for x in range(0, im.size[0]):
         for y in range (0, im.size[1]):
             cordinate = x, y
             p = im.getpixel(cordinate)
             if p != (255,255,255) and p != (255,255,255,255):  # possible error here if you are reading in a png with a transparent level too
                 counter += 1
    return counter

def makeIcon(details, layerdict):
    filename = details[0]
    layers=[]
    for thislayer in range(1,len(details)): #skip the filename
        layername = details[thislayer]
        lookup = str.lower(details[thislayer])
        layerrank = layerdict[lookup]
        layers.append([layername, layerrank])
    layers.sort(reverse = True, key=lambda i: i[1]) # sort by pix count, high to low
    new_icon = Image.new('RGBA', (24,24), (0, 0, 0, 255))  #grasshopper cions are 24x24, change final 255 to 0 to give solid white backgrounds
    for layer in layers:
        fileIm = os.path.join(os.getcwd(), "Layers", layer[0]) +".png"
        #print(fileIm)
        layerIm = Image.open(fileIm)
        for x in range (0, layerIm.size[0]):
            for y in range(0, layerIm.size[1]):
                cordinate = x,y
                p = layerIm.getpixel(cordinate)
                if p != (255,255,255) and p != (255,255,255,255):
                    pixelbox = Image.new('RGBA',(1,1), p)
                    new_icon.paste(pixelbox, cordinate)
    new_icon.save(os.path.join(os.getcwd(), "Generated", filename,)+".png")
    print('generated', filename)    
    return

# MAIN

    
#read in list of existing bepsoke icons
Bespoke = []
for file in glob.glob('Bespoke*/*.png', recursive=True):
    Bespoke.append(file)
print('**** BESPOKE read in as below *****')
print(Bespoke)
    
#read in list of layer pngs, and make dict of layer pixel count
Layer = {}
for file in glob.glob('Layers*/*.png', recursive=True):
    snip1 = len('Layers\'')
    snip2 = len(file) - len('.png')
    LayerKey = file[snip1:snip2]
#   print(LayerKey)
    LayerVal = PixelCount(file)
    #Layer[LayerKey] = LayerVal
    Layer[str.lower(LayerKey)] = LayerVal #incase of missing capital letters
    
    #overide values for action layers so they are always on top
    Layer['inspect'] = 0
    Layer['create'] = 0
    Layer['modify'] = 0
    Layer['explore'] = 0
          

    
print('**** Layers read in as below *****')
print(Layer)

#for each icon in list from csv, try to make
# by default skip any that already bespoke
# report any that could not be made
#read in list of cions needed from csv
with open('icon_table.csv', newline='') as csvfile:
    iconTable = reader(csvfile, delimiter=',', quotechar='|')
    print('**** CSV read in as below *****')
    for row in iconTable:
        if row[0] in Bespoke:
            print(row[0], "not generated as bespoke version exists")
        else:
            print(row)
            makeIcon(row, Layer)

    # PyName,Input1,Input2,action,output




    