---------------
1. Usage Notes
---------------
1.1. This sample project requires a "DUAL" device setting.
After project generation, please change to "DUAL" device following steps:
 [Project] > [Change Device]
 At "Target Device" text box, append "_DUAL" (e.g. R5F565NEHxFB_DUAL)
 Click Next, Next and Finish
After device is changed, compiler setting is reset, please
 [Project] > [C/C++ Project Settings]
 For CC-RX, 
  go to Linker > Section, click Import... and select "linker_section_sample.esi" under src folder of current project

1.2. Please make sure SCI6 is configured to use TXD6 and RXD6 pins, instead of SMOSI6/SMISO6.

1.3. Please take note that the sample project is verified with C project.
If you create new project with C++ option, please confirm its behaviour by yourself.
For example, if you create project with C++ option and CC-RX compiler, you will need to add abort() function manually.