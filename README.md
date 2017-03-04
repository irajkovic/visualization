# Visualization

## Introduction

<img src="https://raw.githubusercontent.com/wiki/irajkovic/visualization/images/cpu_monitor.png" alt="Example of CPU monitor configuration">

Application is created as a general purpose control panel. It can communicate
with sensors and control units via UDP and serial protocols. 
Idea behind application is generic control panel creation software flexible
enough for any purpose (smart home, system monitoring, etc). 

Incoming data is shown via several types of virtual instrument widgets, while 
command widgets are used to send out commands. Image widgets are supported as
well, to provid deep integration into domain of use. Each widget is highly 
customizable, providing different attributes to fine tune it's appearance and 
function.

Any number of widgets can be arranged on the predefined area, forming layout of
a control panel. This layout can be stored into XML file which contains a 
description off all widget properties, as well as signals that are monitored.
Application will be runtime configured based on this XML file. This means that
there are no hardcoded values and entire look&feel of the control panel, as well
as it's functionality comes from user created XML file. Such XML file is called
configuration. 

One major part of application is built in WYSIWYG configuration editor. This 
allows easy and efficient creation and modification of configurations.

Any signal source that can send or receive UDP or serial package of given 
structure can be easily integrated. 

Application is developed with C++ and Qt, with intent of being highly portable
to various devices and form factors.

## Wiki page

For more information, visit github wiki page at:
https://github.com/irajkovic/visualization/wiki

## Author
Developed by Ivan Rajkovic (iivanrajkovic@gmail.com)

