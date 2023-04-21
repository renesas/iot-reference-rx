---------------
1. Usage Notes
---------------
1.1. This sample project requires a "DUAL" device setting.
After project generation, please change to "DUAl" device following steps:
 [Project] > [Change Device]
 At "Target Device" text box, append "_DUAL" (e.g. R5F565NEHxFB_DUAL)
 Click Next, Next and Finish
After device is changed, compiler setting is reset, please
 [Project] > [C/C++ Project Settings]
 For CC-RX, 
  go to Linker > Section, click Import... and select "linker_section_sample.esi" under src folder of current project
 For GCC RX,
  Copy and replace content of linker_script_sample.ld for current linker_script.ld under src folder of current project

1.2. Please also take note that the sample project is verified with C project.
If you create new project with C++ option, please confirm its behaviour by yourself.
For example, if you create project with C++ option and CC-RX compiler, you will need to add abort() function manually.

