# Principles

Icons in grasshopper have two roles. 
The first is to support reading a script where the inputs and outputs are visible but the action the component takes might not be obvious.
The second is to support a script writer when searching for a component in the menu - something they do not know the name of yet but know what action they are trying to accomplish. 

## Principal 1: code is read more times then is written, so support the reader by making the action of the component clear

## Principal 2: all icons should be recognisably 'topologic', to help reading the script by 'paragraph of related thought'. 

## Principal 3: Following 2, all icons should follow a consistent visual language of colours, shapes and placement

## Principal 3: Following 1, some duplication of icons is better then all icons being identical, but the more similar two components are, the clearer the action needs to be (eg Impose vs Merge)

## Principal 4: Sufficent design space should be left to allow new functions to be added in the future. 


# Grasshopper viusual cues 

From Principal 3, what are the visual cues that grasshopper trains people in from basic functions?
* Dark Grey Hex is a data collection
* a grey rounded button outline is a abstract function (like add, subtract)
* a 3x3 black outline around a white pixel is a geometric point
* for surface components, a 2d surface is yellow-organge. 3d is indicated by a gradient in the colour
* for line modifying components: input is a dark red line, output is a thick black line
* for surfrace modifying components, input is grey, output is yellow.


# Basis of design

For KISS, the code used will use the most commonly available image and tabualtion software- MS Paint and MS excel.
File types for information storage will be PNG and CSV

Each icon will be created from 4 layers - two inputs, one action and one output.  Layers will be ranked based on their pixel size. solid white pixels will be rendered transparent

Actions that have been identifed are: Create, Modify, Inspect, Explore.
The difference between Inspect and Explore is wether the output is contained within the input. A component that returns the length of a wire 'inspects'. A compoenent that returns wires that connect to the input 'explores'.

A script has been provided by David Rutten that will identify all of the grasshopper components, inputs and outputs in the addin.

The naming scheme and data heirearchy of the project is consistent enough that some rules for action can be dervied:

All components of the form <datatype>.By<LowerDatatype> Create

All componenets of the form <datatype>.<LowerDatatype> Inspect 

All components of the form <datatype>.Adjacent<datatype> Explore


However, there is a large number of components that need inspection to record their action. it is assumed that when new components are added, the action should be clear to the creator and the icon can be generated on the spot in an adhoc format.

There are some components in a grey area, where the output is technically a modified input, but the component could be considered to create a new geometry. 

There are components with multiple inputs, but one of those is background to the others. Eg using a wire to create a face in a topology needs the topolgy to be input too, but the script writer/reader will be more interested in the component as something that takes a wire and outputs a face.

