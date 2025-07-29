<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<smc>
    <general version="2.0.0.0">
        <configuration active="true" id="${configurationTypeReleaseID}">
            <property id="com.renesas.smc.service.project.buildArtefactType" values="com.renesas.smc.service.project.buildArtefactType.exe"/>
            <toolchain id="${toolChainID}">
                <option id="com.renesas.smc.toolchain.option.buildArtefactType" key="com.renesas.smc.toolchain.option.buildArtefactType.exe"/>
                <option id="com.renesas.smc.toolchain.option.rtos" key="com.renesas.smc.toolchain.option.rtos.none">
                </option>
            </toolchain>
        </configuration>
        <platform id="${targetDevice}"/>
    </general>
    <tool id="SWComponent" version="1.0.0.0">
        <configuration inuse="true" name="r_bsp">
            <component display="r_bsp" id="r_bsp7.52" version="7.52">
                <gridItem id="BSP_CFG_ISTACK_BYTES" selectedIndex="0x600"/>
                <gridItem id="BSP_CFG_HEAP_BYTES" selectedIndex="0x600"/>
                <gridItem id="BSP_CFG_USER_STACK_ENABLE" selectedIndex="0"/>
                <gridItem id="BSP_CFG_BOOTLOADER_PROJECT" selectedIndex="1"/>
                <gridItem id="BSP_CFG_USER_CHARPUT_ENABLED" selectedIndex="1"/>
                <gridItem id="BSP_CFG_PARAM_CHECKING_ENABLE" selectedIndex="0"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_ENABLE" selectedIndex="1"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_CHANNEL" selectedIndex="5"/>
                <gridItem id="BSP_CFG_IO_LIB_ENABLE" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_byteq">
            <component description="Dependency : r_bsp version(s) 6.20&#10;This module allocates a queue control block in an Open() call to handle indexing for buffers provided by the user application. Additional functions are provided to put and get data from the buffer queue, inspect the number of bytes available or in use, and to flush the queue. There is no limit to the number of queues supported, and queue control blocks can be allocated statically at build time or dynamically at run time." detailDescription="Byte-based circular buffer library." display="r_byteq" id="r_byteq2.10" version="2.10">
                <gridItem id="BYTEQ_CFG_PARAM_CHECKING_ENABLE" selectedIndex="1"/>
                <gridItem id="BYTEQ_CFG_MAX_CTRL_BLKS" selectedIndex="4"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_sci_rx">
            <component detailDescription="Multi-Channel Multi-Mode SCI Driver." display="r_sci_rx" id="r_sci_rx5.40" version="5.40">
                <gridItem id="RXD5/SMISO5/SSCL5" selectedIndex="1"/>
                <gridItem id="TXD5/SMOSI5/SSDA5" selectedIndex="1"/>
                <gridItem id="SCI5" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH1_INCLUDED" selectedIndex="0"/>
                <gridItem id="SCI_CFG_CH5_INCLUDED" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_flash_rx">
            <component description="Dependency : r_bsp version(s) 5.20, 5.30, 5.40, 5.50, 5.52, 6.10, 6.20, 6.21, 7.00, 7.10, 7.20, 7.30&#10;This module allows the user to program, erase, write and blankcheck the Code and/or Data flash." detailDescription="Flash API for RX100, RX200, RX600. and RX700." display="r_flash_rx" id="r_flash_rx5.21" version="5.21">
                <gridItem id="FLASH_CFG_PARAM_CHECKING_ENABLE" selectedIndex="0"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_RUN_FROM_ROM" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_fwup">
            <component description="Dependency : r_flash_rx version(s) 4.80, 4.91&#10;This module is a software library for implementing the firmware update function in user applications." detailDescription="FWUP Library" display="r_fwup" id="r_fwup2.04" version="2.04">
                <gridItem id="FWUP_CFG_USER_FLASH_OPEN_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_CLOSE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_ERASE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_WRITE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_READ_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_BANK_SWAP_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_SIGNATURE_VERIFICATION" selectedIndex="0"/>
                <gridItem id="FWUP_CFG_UPDATE_MODE" selectedIndex="0"/>
                <gridItem id="FWUP_CFG_MAIN_AREA_ADDR_L" selectedIndex="0xFFF00000U"/>
                <gridItem id="FWUP_CFG_BUF_AREA_ADDR_L" selectedIndex="0xFFE00000U"/>
                <gridItem id="FWUP_CFG_AREA_SIZE" selectedIndex="0xF0000U"/>
                <gridItem id="FWUP_CFG_CF_BLK_SIZE" selectedIndex="0x8000U"/>
                <gridItem id="FWUP_CFG_CF_W_UNIT_SIZE" selectedIndex="128U"/>
                <gridItem id="FWUP_CFG_EXT_BUF_AREA_ADDR_L" selectedIndex="0x00000U"/>
                <gridItem id="FWUP_CFG_EXT_BUF_AREA_BLK_SIZE" selectedIndex="4096U"/>
                <gridItem id="FWUP_CFG_DF_BLK_SIZE" selectedIndex="64U"/>
                <gridItem id="FWUP_CFG_DF_NUM_BLKS" selectedIndex="512U"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
    </tool>
</smc>