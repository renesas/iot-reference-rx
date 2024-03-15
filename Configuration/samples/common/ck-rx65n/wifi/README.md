---------------
1. Usage Notes
---------------
1.1. Please make sure SCI6 is configured to use CTS#6, instead of RTS6#.
This is required by the DA16600 Wi-Fi PMOD module.

1.2. Please take note that the sample project is verified with C project.
If you create new project with C++ option, please confirm its behaviour by yourself.
For example, if you create project with C++ option and CC-RX compiler, you will need to add abort() function manually.

1.3. Please make sure to configure Smart Configurator 'Code generation behavior' to 'Update configuration files'.
You can do so in e2studio Windows > Preferences > Renesas > Smart Configurator > Component.
This is necessary because the FWUP module uses custom wrapper code for this application, 
this Smart Configurator setting ensures that wrapper code is not overwritten.

1.4. Issue with added sections when generating code.
After generating code by Smart Configurator, sections (C_FIRMWARE_UPDATE_CONTROL_BLOCK and C_FIRMWARE_UPDATE_CONTROL_BLOCK_MIRROR) 
are added at address 0x00100000 due to the TSIP module (v1.17.l and above).
They are empty sections and will not use any additional memory.