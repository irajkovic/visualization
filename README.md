# visualization

## introduction
Application implements UDP server which reads incomming data and displays it
via set of virtual instruments. Idea is to help DIY builders and provide them
with generic application to build control panels for various purposes (smart 
home / systems monitoring). 

Application is developed with C++ and Qt, with intent of being highly portable
to various devices and form factors.

Currently, only data readout is implemented, but plan is to implement control 
commands as well. 

Instruments are dynamically configured during runtime. Various instrument 
properties (color, size, placement, etc) are red from configuration xml file. 
UDP port of server, window resoultion and title are also configurable.
This file name should be given as first command line argument when starting the 
application. If no argument is given, default "config.xml" will be loaded.

## UDP interface

Incomming UDP packets must have following format:

quint16 signalId;   
    // signal identifier
    
quint16 packetNumber;      
    // number of package so missed packages can be identified. Not used
    // at the moment.
    
quint64 timestamp;
    // time when package was send. Currently, used only on time plotter instrument.
    
quint64 rawValue;
    // raw value of measured signal. This value is then converted to actuall
    // value via expression: value = raw_value * factor + offset,
    // Factor and offset are red from signal configuration.
    
quint8 checksum;
    // checksum byte, when xor-ed with all other bytes of configuration,
    // result should be zero

## Author
Developed by Ivan Rajkovic (iivanrajkovic@gmail.com)







