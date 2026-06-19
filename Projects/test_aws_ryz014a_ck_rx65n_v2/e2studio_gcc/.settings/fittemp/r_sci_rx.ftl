<#--
/***********************************************************************************************************************
* Copyright (c) 2015 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
-->
<#-- = DECLARE FUNCTION INFORMATION HERE =================== -->
<#-- 
   (Step 1) Explanation: These variables are necessary information for the function header. 
   Please fill up or leave blank, but do not delete
-->
<#assign Function_Base_Name = "R_SCI_PinSet">
<#assign Function_Description = "This function initializes pins for r_sci_rx module">
<#assign Function_Arg = "none">
<#assign Function_Ret = "none">
<#assign Version = 1.00>

<#-- = DECLARE FUNCTION CONTENT HERE ======================= -->
<#-- 
   (Step 2) Explanation: Function content.
    - Macro [initialsection] : 
      Any text that goes into this section will be printed out 1 time per function
      input [postfix]   :Use this variable to add the channel number to the function base name.
-->
<#macro initialsection postfix>
<#assign Function_Name = "${Function_Base_Name}${postfix}">
<#include "lib/functionheader.ftl">
void ${Function_Name}()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
</#macro>

<#-- 
   (Step 3) Explanation: Function content.
    - Macro [peripheralpincode] : Any text that goes into this section will be printed out 1 time per peripheral
    - input [pin] : Available info includes:
        pin.pinName             :The name of pin, eg “SSLA0”
        pin.assignedPinName     :The pin assigned to, eg “P32”
        pin.pinMPC              :The port number of assigned pin, eg “P32” has portNume = “3”
        pin.portNum             :The bit number of the assigned pin, eg “P32” has pinBitNum = “2”
        pin.pinBitNum           :The value of MPC 
-->
<#macro peripheralpincode pin>
</#macro> 

<#-- 
   (Step 4) Explanation: Function content.
    - Macro [channelpincode] : Any text that goes into this section will be printed out 1 time per channel
    - input [pin] : Same as above
-->
<#macro channelpincode pin>

    /* Set ${pin.pinName} pin */
    MPC.${pin.assignedPinName}PFS.BYTE = 0x${pin.pinMPC}U;
    PORT${pin.portNum}.PMR.BIT.B${pin.pinBitNum} = 1U;
</#macro> 

<#macro channelpincodeextra pin postfix>
</#macro> 

<#-- 
   (Step 5) Explanation: Function content.
    - Macro [endsection] : Any text that goes into this section will be printed out 1 time last
-->
<#macro endsection>

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
</#macro> 

<#-- 
   (Step 6) Explanation: Header file content
    - Macro [headerfilesection] : Any text that goes into this section will be printed out 1 time in the header file
    - input [postfix]   :Use this variable to add the channel number to the function base name.
-->
<#macro headerfilesection postfix>
void ${Function_Base_Name}${postfix}();
</#macro> 

<#macro headerfilesectionExtra postfix>
</#macro> 

<#-- = END OF FILE ========================================= -->