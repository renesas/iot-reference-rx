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
            <component display="r_bsp" id="r_bsp7.51" version="7.51">
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_ether_rx">
            <component display="r_ether_rx" id="r_ether_rx1.23" version="1.23">
                <gridItem id="REF50CK0" selectedIndex="1"/>
                <gridItem id="RMII0_TXD_EN" selectedIndex="1"/>
                <gridItem id="RMII0_TXD1" selectedIndex="1"/>
                <gridItem id="RMII0_TXD0" selectedIndex="1"/>
                <gridItem id="RMII0_RXD1" selectedIndex="1"/>
                <gridItem id="RMII0_RXD0" selectedIndex="1"/>
                <gridItem id="RMII0_RX_ER" selectedIndex="1"/>
                <gridItem id="RMII0_CRS_DV" selectedIndex="1"/>
                <gridItem id="ET0_MDC" selectedIndex="1"/>
                <gridItem id="ET0_MDIO" selectedIndex="1"/>
                <gridItem id="ET0_LINKSTA" selectedIndex="1"/>
                <gridItem id="ETHERC0_RMII" selectedIndex="1"/>
                <gridItem id="ETHER_CFG_MODE_SEL" selectedIndex="1"/>
                <gridItem id="ETHER_CFG_CH0_PHY_ADDRESS" selectedIndex="5"/>
                <gridItem id="ETHER_CFG_EMAC_RX_DESCRIPTORS" selectedIndex="6"/>
                <gridItem id="ETHER_CFG_EMAC_TX_DESCRIPTORS" selectedIndex="10"/>
                <gridItem id="ETHER_CFG_CH0_PHY_ACCESS" selectedIndex="0"/>
                <gridItem id="ETHER_CFG_LINK_PRESENT" selectedIndex="1"/>
                <gridItem id="ETHER_CFG_USE_PHY_ICS1894_32" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS_Kernel">
            <component display="FreeRTOS Kernel" id="FreeRTOS_Kernel${packageVersion}" version="${packageVersion}">
                <gridItem id="configMINIMAL_STACK_SIZE" selectedIndex="( unsigned short ) 510"/>
                <gridItem id="configTOTAL_HEAP_SIZE_N" selectedIndex="8"/>
                <gridItem id="configUSE_TICK_HOOK" selectedIndex="1"/>
                <gridItem id="configUSE_IDLE_HOOK" selectedIndex="1"/>
                <gridItem id="configUSE_MALLOC_FAILED_HOOK" selectedIndex="1"/>
                <gridItem id="configCHECK_FOR_STACK_OVERFLOW" selectedIndex="2"/>
                <gridItem id="configPRINTF( X )" selectedIndex="printf X "/>
                <gridItem id="configPRINT( X )" selectedIndex="printf X "/>
                <gridItem id="configPRINT_STRING( x )" selectedIndex="printf x "/>
                <gridItem id="configPLATFORM_NAME" selectedIndex="&quot;RenesasRX65N&quot;"/>
            </component>
            <source description="Code generator for Real-time OS" display="RTOS Configurator" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
        </configuration>
        <configuration enable="true" exclude="false" inuse="true" name="FreeRTOS_IP">
            <component display="FreeRTOS_IP" id="FreeRTOS_IP${packageVersion}" version="${packageVersion}">
                <gridItem id="ipconfigUSE_DHCP" selectedIndex="1"/>
                <gridItem id="ipconfigUSE_DHCP_HOOK" selectedIndex="1"/>
                <gridItem id="ipconfigDHCP_REGISTER_HOSTNAME" selectedIndex="1"/>
                <gridItem id="ipconfigSUPPORT_OUTGOING_PINGS" selectedIndex="1"/>
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
                        <option defaultValue="512" id="StackSize" value="512"/>
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
