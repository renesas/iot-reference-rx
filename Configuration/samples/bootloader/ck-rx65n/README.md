---------------
1. Usage Notes
---------------
  
1.1. Please take note that the sample project is verified with C project.  
If you create new project with C++ option, please confirm its behaviour by yourself.  
For example, if you create project with C++ option and CC-RX compiler, you will need to add abort() function manually.  
  
1.2. This sample project requires a "DUAL" device setting.  
If using e2studio_2024-01 and earlier, and "non-DUAL" target board (e.g: CK-RX65N) is selected in the PG wizard, please change to "DUAL" device following the steps below:  
###
        1. In the Smart Configurator window, select the Board tab to open the Device selection window.
        2. Click the ... button next to Board: to open the Change Device window.
        3. In the Change Device window, change the value of the Target Device: item to R5F565NEHxFB_DUAL.
           (You can click the ... button to the right of the text entry field to choose from a list of candidates.
            Leave the Target Board: setting of Custom unchanged.)
        4. After changing the device, click the Next > button.
           On the information window that appears, click the Next > button again without making changes.
        5. Finally, a window appears asking you to confirm the change. Click the Finish button.
           (If a window asking you to confirm a change of target board appears, click the Yes button.)
    
1.3 If using e2studio_2024-04 and later, and Bank Mode is configured as "Single Bank" in the PG wizard, please change to "Dual Bank" following the steps below:
###
        1. In the Smart Configurator window, select the Board tab to open the Device configuration window.
        2. Change the value of the Bank Mode: item to "Dual Bank".
        3. Click "Generate Code" to apply the changes.
  
1.4. After device/bank mode is changed, compiler setting is reset, please update the linker settings following the steps below:
###
      - Under [Project] > [C/C++ Project Settings]:
      - For CC-RX,
        1. Go to Linker > Section, click Import...
        2. Select "linker_section_sample.esi" under src folder of current project.
  
1.5. Please make sure to configure Smart Configurator 'Code generation behavior' to 'Update configuration files'.  
You can do so in e2studio Windows > Preferences > Renesas > Smart Configurator > Component.  
This is necessary because the FWUP module uses custom wrapper code for this application, this Smart Configurator setting ensures that wrapper code is not overwritten