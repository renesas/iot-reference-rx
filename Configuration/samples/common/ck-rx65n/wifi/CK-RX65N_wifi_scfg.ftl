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
            <component display="r_bsp" id="r_bsp7.42" version="7.42">
                <gridItem id="BSP_CFG_HEAP_BYTES" selectedIndex="0x2000"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_ENABLE" selectedIndex="1"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_CHANNEL" selectedIndex="5"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_s12ad_rx">
            <component detailDescription="ADC Driver" display="r_s12ad_rx" id="r_s12ad_rx5.30" version="5.30">
                <gridItem id="AN115" selectedIndex="1"/>
                <gridItem id="AN117" selectedIndex="1"/>
                <gridItem id="ADTRG0#" selectedIndex="1"/>
                <gridItem id="S12AD1" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_flash_rx">
            <component display="r_flash_rx" id="r_flash_rx5.11" version="5.11">
                <gridItem id="FLASH_CFG_PARAM_CHECKING_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_DATA_FLASH_BGO" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_BGO" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_RUN_FROM_ROM" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_wifi_da16xxx">
            <component display="r_wifi_da16xxx" id="r_wifi_da16xxx1.20" version="1.20">
                <gridItem id="WIFI_CFG_TCP_CREATABLE_SOCKETS" selectedIndex="4"/>
                <gridItem id="WIFI_CFG_TCP_SOCKET_RECEIVE_BUFFER_SIZE" selectedIndex="8192"/>
                <gridItem id="WIFI_CFG_COUNTRY_CODE" selectedIndex="&quot;VN&quot;"/>
                <gridItem id="WIFI_CFG_AT_CMD_TX_BUFFER_SIZE" selectedIndex="1500"/>
                <gridItem id="WIFI_CFG_DA16600_SUPPORT" selectedIndex="1"/>
                <gridItem id="WIFI_CFG_SCI_CHANNEL" selectedIndex="6"/>
                <gridItem id="WIFI_CFG_CTS_SW_CTRL" selectedIndex="1"/>
                <gridItem id="WIFI_CFG_CTS_PORT" selectedIndex="18"/>
                <gridItem id="WIFI_CFG_RTS_PORT" selectedIndex="18"/>
                <gridItem id="WIFI_CFG_PFS_SET_VALUE" selectedIndex="0x0AU"/>
                <gridItem id="WIFI_CFG_RESET_PORT" selectedIndex="5"/>
                <gridItem id="WIFI_CFG_RESET_PIN" selectedIndex="5"/>
                <gridItem id="WIFI_CFG_CREATABLE_SOCKETS" selectedIndex="4"/>
                <gridItem id="WIFI_CFG_SNTP_ENABLE" selectedIndex="1"/>
                <gridItem id="WIFI_CFG_USE_FREERTOS_LOGGING" selectedIndex="1"/>
                <gridItem id="WIFI_CFG_DEBUG_LOG" selectedIndex="4"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_sci_rx">
            <component detailDescription="Multi-Channel Multi-Mode SCI Driver." display="r_sci_rx" id="r_sci_rx5.00" version="5.00">
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
            <component display="r_byteq" id="r_byteq2.10" version="2.10">
                <gridItem id="BYTEQ_CFG_PARAM_CHECKING_ENABLE" selectedIndex="0"/>
                <gridItem id="BYTEQ_CFG_USE_HEAP_FOR_CTRL_BLKS" selectedIndex="0"/>
                <gridItem id="BYTEQ_CFG_MAX_CTRL_BLKS" selectedIndex="32"/>
                <gridItem id="BYTEQ_CFG_PROTECT_QUEUE" selectedIndex="0"/>
                <gridItem id="BYTEQ_CFG_CRITICAL_SECTION" selectedIndex="0"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_tsip_rx">
            <component display="r_tsip_rx" id="r_tsip_rx1.20.l" version="1.20.l"/>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS_Kernel">
            <component display="FreeRTOS Kernel" id="FreeRTOS_Kernel${packageVersion}" version="${packageVersion}">
                <gridItem id="configMINIMAL_STACK_SIZE" selectedIndex="768"/>
                <gridItem id="configUSE_TICK_HOOK" selectedIndex="1"/>
                <gridItem id="configUSE_IDLE_HOOK" selectedIndex="1"/>
                <gridItem id="configUSE_MALLOC_FAILED_HOOK" selectedIndex="1"/>
                <gridItem id="configCHECK_FOR_STACK_OVERFLOW" selectedIndex="2"/>
                <gridItem id="configCOMMAND_INT_MAX_OUTPUT_SIZE" selectedIndex="850"/>
                <gridItem id="configIP_ADDR0" selectedIndex="192"/>
                <gridItem id="configIP_ADDR1" selectedIndex="168"/>
                <gridItem id="configIP_ADDR2" selectedIndex="11"/>
                <gridItem id="configIP_ADDR3" selectedIndex="12"/>
            </component>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="LittleFS">
            <component display="LittleFS" id="LittleFS${packageVersion}" version="${packageVersion}">
                <gridItem id="RM_LITTLEFS_FLASH_DATA_START" selectedIndex="FLASH_DF_BLOCK_0_MACRO"/>
            </component>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS Object">
            <allocatable id="OBJECT" name="OBJECT" type="">
                <allocatable id="TASK" name="TASK" type="">
                    <allocatable id="TASKOBJECTLINK1" name="TASK" type="" readonly="true">
                        <option defaultValue="1" id="Initialize" value="0"/>
                        <option defaultValue="task_[X]" id="TaskCode" value="main_task"/>
                        <option defaultValue="task_[X]" id="TaskName" value="MAIN_TASK"/>
                        <option defaultValue="2048" id="StackSize" value="2048"/>
                        <option defaultValue="NULL" id="TaskHandler" value="NULL"/>
                        <option defaultValue="NULL" id="Parameter" value="NULL"/>
                        <option defaultValue="1" id="Priority" value="1"/>
                    </allocatable>
                    <option defaultValue="0" id="LinkNum" value="1"/>
                </allocatable>
                <allocatable id="QUEUE" name="QUEUE" type="">
                    <option defaultValue="0" id="LinkNum" value=""/>
                </allocatable>
                <allocatable id="SEMAPHORE" name="SEMAPHORE" type="">
                    <option defaultValue="0" id="LinkNum" value=""/>
                </allocatable>
                <allocatable id="SWTIMER" name="SWTIMER" type="">
                    <option defaultValue="0" id="LinkNum" value=""/>
                </allocatable>
                <allocatable id="EVENTGROUP" name="EVENTGROUP" type="">
                    <option defaultValue="0" id="LinkNum" value=""/>
                </allocatable>
                <allocatable id="STREAMBUFFER" name="STREAMBUFFER" type="">
                    <option defaultValue="0" id="LinkNum" value=""/>
                </allocatable>
                <allocatable id="MESSAGEBUFFER" name="MESSAGEBUFFER" type="">
                    <option defaultValue="0" id="LinkNum" value=""/>
                </allocatable>
            </allocatable>
            <component description="" detailDescription="" display="FreeRTOS Object" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.object" version="1.0.120"/>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
    </tool>
    <tool id="System">
        <section id="ocd">
            <option enabled="true" id="debugIF" selection="fine">
                <item enabled="true" id="unused"/>
                <item enabled="true" id="fine"/>
                <item enabled="true" id="jtag"/>
                <item enabled="true" id="jtagTrace"/>
            </option>
        </section>
    </tool>
</smc>
