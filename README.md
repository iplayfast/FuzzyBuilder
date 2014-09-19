FuzzyBuilder
============

A Gui for designing Fuzzy Logic which will write out C++ Source

Really fast Tutorial
====================

This tutorial is to show how to condition joystick inputs, and ignore dead spots. By going though this tutorial you will get an understanding of how to use this system to build code.

The code is not quite up to the tutorial so this tutorial won't work. But it's enough to show you the direction the project is taking.

Step 1. Add components
----------------------
* Click on the Input Icon ![In](/images/in.png)
* Click on the Fuzzy Icon ![Fuzzy](/images/fuzzy.png)
* Click on the Output Icon ![Out](/images/out.png)

At this point you've got an input, output and fuzzy component on diagram.

* Left click on out to select, name it Output
* Left click on in to select, name it Joystick


Step 2. Connect Components
--------------------------
* Left click on Joystick (in) to select
* Right click on Fuzzy1 to connect
* Left click on Fuzzy1
* Right click on Output


Step 3. Adjust Values
---------------------

We have now connected the input to the fuzzy, and the fuzzy to the output. In the real world, inputs and outputs can have many different possible ranges. Between the inputs and outputs all values are between 0 and 1. 1 being true and 0 being false. All other values are a range of truthiness.

* Left click on Joystick (in)
* Min slider adjusts the minimum expected value from the joystick. For our example use 5. There are two slides, the top one is for scaling the bottom is for the value.
* Max slider adjusts the maximum expected value from the joystick. For our example use 240.
* Simulate slider sets the simulated input.
* Selecting the code tab shows the code that would be generated. You can change this as needed, or regenerate to start fresh.

* Left click on Fuzzy1
* Between all the inputs and outputs, the values always range from 0 to 1.
* So for example, if the Joysticks minimum is 5, the connection to the fuzzy1 will be 0 when the joystick is at it's minimum.

* Set the cause slider to 0, and the effect slider to 0. Click Set Data Point
* Set the cause slikder to 1, and the effect slider to 1, Click set data point.  What we have done here is specify that we are going to follow the input range in a 1 to 1 fashion.
* Suppose the joystick has a dead zone from 100 to 120 where there might be noise. We want to just ignore those valuse.
* Left click the Joystick and move the simulation slider to 100
Left click the Fuzzy1. Note that the cause slider has moved the appropriate scale. Click Set Data point.
Left clikc the joystick and move the simulation slider to 120
Left click the Fuzzy1. Note that the cuase slider has moved to the new position. Move the effect slider back to where it was at 100. Set data point. You can also type in the datapoint in the point list.

Set the output values to scale from 0 to 255. (min is 0, max is 255). 

Now when you select the input and modify the simulation the output scales correctly, while ignoring the input range of the joystick's dead zone.

Step 4. Get the Code
--------------------
Each component will generate it's own code and you can edit it if desired.
When you are ready, you can click the file menu and view or save the file as C or arduino source.



