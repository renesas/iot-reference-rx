---------------
Usage Notes (for RSKRX65N-2MB only)
---------------
This project requires a "DUAL" device setting.
After project generation, please change to "DUAL" device following steps:
 [Project] > [Change Device]
 At "Target Device" text box, append "_DUAL" (e.g. R5F565NEDxFC_DUAL)
 Click Next, Next and Finish
After device is changed, compiler setting is reset, please
 [Project] > [C/C++ Project Settings]
 For CC-RX, 
  go to Linker > Section, click Import... and select "linker_section_sample.esi" under src folder of current project
  go to Linker > Section > Symbol file, add PFRAM2=RPFRAM2
 For GCC RX,
  Copy and replace conent of linker_script_sample.ld for current linker_script.ld under src folder of current project
