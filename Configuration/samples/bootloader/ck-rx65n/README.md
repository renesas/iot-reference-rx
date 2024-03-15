---------------
1. Usage Notes
---------------
1.1. Please take note that the sample project is verified with C project.
If you create new project with C++ option, please confirm its behaviour by yourself.
For example, if you create project with C++ option and CC-RX compiler, you will need to add abort() function manually.

1.2. Please make sure to configure Smart Configurator 'Code generation behavior' to 'Update configuration files'.
You can do so in e2studio Windows > Preferences > Renesas > Smart Configurator > Component.
This is necessary because the FWUP module uses custom wrapper code for this application, 
this Smart Configurator setting ensures that wrapper code is not overwritten.