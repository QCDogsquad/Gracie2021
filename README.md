# Vex PROS Basic
This is our basic and foundational codebase for the VEX Robotics Competition. We use the PROS kernel.

## Building notes
The build system is set up in a very unorthodox way. Files are built through executing a single build.bat file. 
We use a unity build in order to remove the signficant complications of traditional build systems. 
Using a unity build means that functions do not need to be forward declared in header files. 
Since functions aren't forward declared, if you want to use this code in a project that uses a traditional build system,
you will need to create your own forward declarations.
To build this you need to specify multiple things:
  WIP -> LIST THINGS HERE.
