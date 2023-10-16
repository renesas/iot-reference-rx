<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<smc>
    <general version="2.0.0.0">
        <configuration active="true" id="${configurationTypeReleaseID}">
            <property id="com.renesas.smc.service.project.buildArtefactType" values="com.renesas.smc.service.project.buildArtefactType.exe"/>
            <toolchain id="${toolChainID}">
                <option id="com.renesas.smc.toolchain.option.buildArtefactType" key="com.renesas.smc.toolchain.option.buildArtefactType.exe"/>
                <option id="com.renesas.smc.toolchain.option.rtos" key="com.renesas.smc.toolchain.option.rtos.freertoslts">
                    <item id="com.renesas.smc.toolchain.option.rtos.freertoslts" value="${packageVersion}"/>
                </option>
            </toolchain>
        </configuration>
        <platform id="${targetDevice}"/>
    </general>
    <tool id="SWComponent" version="1.0.0.0">
        <configuration inuse="true" name="r_bsp">
            <component display="r_bsp" id="r_bsp7.20" version="7.20">
                <gridItem id="BSP_CFG_HEAP_BYTES" selectedIndex="0x1000"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_ENABLE" selectedIndex="1"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_CHANNEL" selectedIndex="5"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_s12ad_rx">
            <component detailDescription="ADC Driver" display="r_s12ad_rx" id="r_s12ad_rx5.00" version="5.00">
                <gridItem id="AN115" selectedIndex="1"/>
                <gridItem id="AN117" selectedIndex="1"/>
                <gridItem id="S12AD1" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_flash_rx">
            <component display="r_flash_rx" id="r_flash_rx5.00" version="5.00">
                <gridItem id="FLASH_CFG_PARAM_CHECKING_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_DATA_FLASH_BGO" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_BGO" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_RUN_FROM_ROM" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_irq_rx">
            <component display="r_irq_rx" id="r_irq_rx4.00" version="4.00">
                <gridItem id="IRQ1" selectedIndex="1"/>
                <gridItem id="IRQ2" selectedIndex="1"/>
                <gridItem id="IRQ7" selectedIndex="1"/>
                <gridItem id="IRQ13" selectedIndex="1"/>
                <gridItem id="IRQ14" selectedIndex="1"/>
                <gridItem id="IRQ15" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_cellular">
            <component display="r_cellular" id="r_cellular1.10" version="1.10">
                <gridItem id="CELLULAR_CFG_DEBUGLOG" selectedIndex="4"/>
                <gridItem id="CELLULAR_CFG_AP_NAME" selectedIndex="ibasis.iot"/>
                <gridItem id="CELLULAR_CFG_AUTH_TYPE" selectedIndex="2"/>
                <gridItem id="CELLULAR_CFG_UART_SCI_CH" selectedIndex="6"/>
                <gridItem id="CELLULAR_CFG_CTS_SW_CTRL" selectedIndex="0"/>
                <gridItem id="CELLULAR_CFG_RTS_PORT" selectedIndex="0"/>
                <gridItem id="CELLULAR_CFG_RTS_PIN" selectedIndex="2"/>
                <gridItem id="CELLULAR_CFG_RESET_PORT" selectedIndex="5"/>
                <gridItem id="CELLULAR_CFG_RESET_PIN" selectedIndex="5"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_sci_rx">
            <component detailDescription="Multi-Channel Multi-Mode SCI Driver." display="r_sci_rx" id="r_sci_rx4.40" version="4.40">
                <gridItem id="RXD5/SMISO5/SSCL5" selectedIndex="1"/>
                <gridItem id="TXD5/SMOSI5/SSDA5" selectedIndex="1"/>
                <gridItem id="SCI5" selectedIndex="1"/>
                <gridItem id="RXD6/SMISO6/SSCL6" selectedIndex="1"/>
                <gridItem id="TXD6/SMOSI6/SSDA6" selectedIndex="1"/>
                <gridItem id="CTS6#/RTS6#/SS6#" selectedIndex="1"/>
                <gridItem id="SCI6" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH1_INCLUDED" selectedIndex="0"/>
                <gridItem id="SCI_CFG_CH5_INCLUDED" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH6_INCLUDED" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH6_TX_BUFSIZ" selectedIndex="2180"/>
                <gridItem id="SCI_CFG_CH6_RX_BUFSIZ" selectedIndex="8192"/>
                <gridItem id="SCI_CFG_TEI_INCLUDED" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_byteq">
            <component display="r_byteq" id="r_byteq2.00" version="2.00">
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_tsip_rx">
            <component display="r_tsip_rx" id="r_tsip_rx1.16.l" version="1.16.l"/>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS_Kernel">
            <component display="FreeRTOS Kernel" id="FreeRTOS_Kernel${packageVersion}" version="${packageVersion}">
                <gridItem id="configCOMMAND_INT_MAX_OUTPUT_SIZE" selectedIndex="850"/>
                <gridItem id="configUSE_TICK_HOOK" selectedIndex="1"/>
                <gridItem id="configUSE_IDLE_HOOK" selectedIndex="1"/>
                <gridItem id="configMINIMAL_STACK_SIZE" selectedIndex="768"/>
                <gridItem id="configCOMMAND_INT_MAX_OUTPUT_SIZE" selectedIndex="500"/>
                <gridItem id="configUSE_MALLOC_FAILED_HOOK" selectedIndex="1"/>
                <gridItem id="configCHECK_FOR_STACK_OVERFLOW" selectedIndex="2"/>
            </component>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS_LittleFS">
            <component display="FreeRTOS LittleFS" id="FreeRTOS_LittleFS${packageVersion}" version="${packageVersion}">
                <gridItem id="LFS_FLASH_READ_SIZE" selectedIndex="1"/>
                <gridItem id="LFS_FLASH_PROGRAM_SIZE" selectedIndex="4"/>
                <gridItem id="LFS_FLASH_BLOCK_SIZE" selectedIndex="128"/>
            </component>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS Object">
            <allocatable id="" name="" type=""/>
            <component description="" detailDescription="" display="FreeRTOS Object" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.object" version="1.0.112"/>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
    </tool>
</smc>
