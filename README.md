# Visualization

## Introduction

<img src="https://raw.githubusercontent.com/wiki/irajkovic/visualization/images/cpu_monitor.png" alt="Example of CPU monitor configuration">

Application implements UDP server which reads incomming data and displays it
via set of virtual instruments. Any signal source that can deliver UDP package 
of given structure to Visualization server can easily be integrated and it's 
data visualized.

Idea is to help DIY builders and provide them with a generic application to build 
control panels for various purposes (smart home, systems monitoring, etc). 

Application is developed with C++ and Qt, with intent of being highly portable
to various devices and form factors.

Currently, only data readout is implemented, but plan is to implement control 
commands as well. 

Virtual instruments are dynamically configured during runtime. Various instrument 
properties (color, size, placement, etc) are read from configuration xml file. 
UDP port of server, window resolution and title are also configurable.
This file name should be given as first command line argument when starting the 
application. If no argument is given, default "config.xml" will be loaded.

## Wiki page

For more information, visit gihtub wiki page at:
https://github.com/irajkovic/visualization/wiki

## Author
Developed by Ivan Rajkovic (iivanrajkovic@gmail.com)







