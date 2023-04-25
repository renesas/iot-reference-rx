<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<smc>
<general version="2.0.0.0">
<configuration active="true" id="${configurationTypeReleaseID}">
<property id="com.renesas.smc.service.project.buildArtefactType" values="com.renesas.smc.service.project.buildArtefactType.exe"/>
<toolchain id="${toolChainID}">
<option id="com.renesas.smc.toolchain.option.buildArtefactType" key="com.renesas.smc.toolchain.option.buildArtefactType.exe"/>
<option id="com.renesas.smc.toolchain.option.rtos" key="com.renesas.smc.toolchain.option.rtos.freertos">
<item id="com.renesas.smc.toolchain.option.rtos.freertos" value="AmazonFreeRTOS_RX_${packageVersion}"/>
</option>
</toolchain>
</configuration>
<platform id="${targetDevice}"/>
<option id="board" value="RSKRX65N-2MB (V1.05)"/>
</general>
<tool id="SWComponent" version="1.0.0.0">
<configuration inuse="true" name="r_bsp">
<component display="r_bsp" id="r_bsp7.20" version="7.20">
<gridItem id="BSP_CFG_CODE_FLASH_BANK_MODE" selectedIndex="0"/>
<gridItem id="BSP_CFG_USER_STACK_ENABLE" selectedIndex="1"/>
<gridItem id="BSP_CFG_SWINT_UNIT2_ENABLE" selectedIndex="0"/>
<gridItem id="BSP_CFG_HEAP_BYTES" selectedIndex="0x1000"/>
<gridItem id="BSP_CFG_SCI_UART_TERMINAL_ENABLE" selectedIndex="1"/>
<gridItem id="BSP_CFG_SCI_UART_TERMINAL_INTERRUPT_PRIORITY" selectedIndex="15"/>
</component>
<source id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_s12ad_rx">
<component display="r_s12ad_rx" id="r_s12ad_rx5.00" version="5.00">
<gridItem id="S12AD0" selectedIndex="1"/>
<gridItem id="AN000" selectedIndex="1"/>
<gridItem id="ADTRG0#" selectedIndex="1"/>
</component>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_flash_rx">
<component display="r_flash_rx" id="r_flash_rx4.90" version="4.90">
<gridItem id="FLASH_CFG_PARAM_CHECKING_ENABLE" selectedIndex="1"/>
<gridItem id="FLASH_CFG_CODE_FLASH_ENABLE" selectedIndex="1"/>
<gridItem id="FLASH_CFG_DATA_FLASH_BGO" selectedIndex="1"/>
<gridItem id="FLASH_CFG_CODE_FLASH_BGO" selectedIndex="1"/>
<gridItem id="FLASH_CFG_CODE_FLASH_RUN_FROM_ROM" selectedIndex="1"/>
</component>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_sci_rx">
<component display="r_sci_rx" id="r_sci_rx4.40" version="4.40">
<gridItem id="SCI_CFG_CH1_INCLUDED" selectedIndex="0"/>
<gridItem id="SCI_CFG_CH8_INCLUDED" selectedIndex="1"/>
<gridItem id="SCK1" selectedIndex="1"/>
<gridItem id="SCI1" selectedIndex="1"/>
<gridItem id="SCK6" selectedIndex="1"/>
<gridItem id="SCI6" selectedIndex="1"/>
<!-- <gridItem id="RXD8/SMISO8/SSCL8" selectedIndex="1"/> -->
<!-- <gridItem id="RXD8" selectedIndex="1"/> -->
<gridItem id="SCI8" selectedIndex="1"/>
<gridItem id="TXD8/SMOSI8/SSDA8" selectedIndex="1"/>
<gridItem id="RXD8/SMISO8/SSCL8" selectedIndex="1"/>
<gridItem id="SCI_CFG_TEI_INCLUDED" selectedIndex="1"/>
</component>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_byteq">
<component display="r_byteq" id="r_byteq2.00" version="2.00"/>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_ether_rx">
<component display="r_ether_rx" id="r_ether_rx1.23" version="1.23">
<gridItem id="ETHER_CFG_CH0_PHY_ACCESS" selectedIndex="0"/>
<gridItem id="ETHER_CFG_CH1_PHY_ACCESS" selectedIndex="1"/>
<gridItem id="ETHER_CFG_CH0_PHY_ADDRESS" selectedIndex="30"/>
<gridItem id="ETHER_CFG_CH1_PHY_ADDRESS" selectedIndex="1"/>
<gridItem id="ETHER_CFG_EMAC_RX_DESCRIPTORS" selectedIndex="6"/>
<gridItem id="ETHER_CFG_EMAC_TX_DESCRIPTORS" selectedIndex="3"/>
<gridItem id="ET0_TX_CLK" selectedIndex="1"/>
<gridItem id="ET0_RX_CLK" selectedIndex="1"/>
<gridItem id="ET0_TX_EN" selectedIndex="1"/>
<gridItem id="ET0_ETXD3" selectedIndex="1"/>
<gridItem id="ET0_ETXD2" selectedIndex="1"/>
<gridItem id="ET0_ETXD1" selectedIndex="1"/>
<gridItem id="ET0_ETXD0" selectedIndex="1"/>
<!-- <gridItem id="ET0_TX_ER" selectedIndex="1"/> -->
<gridItem id="ET0_RX_DV" selectedIndex="1"/>
<gridItem id="ET0_ERXD3" selectedIndex="1"/>
<gridItem id="ET0_ERXD2" selectedIndex="1"/>
<gridItem id="ET0_ERXD1" selectedIndex="1"/>
<gridItem id="ET0_ERXD0" selectedIndex="1"/>
<gridItem id="ET0_RX_ER" selectedIndex="1"/>
<gridItem id="ET0_CRS" selectedIndex="1"/>
<gridItem id="ET0_COL" selectedIndex="1"/>
<gridItem id="ET0_MDC" selectedIndex="1"/>
<gridItem id="ET0_MDIO" selectedIndex="1"/>
<gridItem id="ET0_LINKSTA" selectedIndex="1"/>
<gridItem id="ETHERC0_MII" selectedIndex="1"/>
<gridItem id="ET0_MDC" selectedIndex="1"/>
<gridItem id="ET0_MDIO" selectedIndex="1"/>
<gridItem id="ET0_LINKSTA" selectedIndex="1"/>
</component>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_tsip_rx">
<component display="r_tsip_rx" id="r_tsip_rx1.15.l" version="1.15.l"/>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
</configuration>
<configuration inuse="true" name="r_dtc_rx">
<component display="r_dtc_rx" id="r_dtc_rx4.10" version="4.10"/>
<source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
<gridItem id="DTC_CFG_PARAM_CHECKING_ENABLE" selectedIndex="0"/>
<gridItem id="DTC_CFG_DISABLE_ALL_ACT_SOURCE" selectedIndex="1"/>
<gridItem id="DTC_CFG_SHORT_ADDRESS_MODE" selectedIndex="0"/>
<gridItem id="DTC_CFG_TRANSFER_DATA_READ_SKIP_EN" selectedIndex="1"/>
<gridItem id="DTC_CFG_USE_DMAC_FIT_MODULE" selectedIndex="1"/>
<gridItem id="DTC_CFG_USE_SEQUENCE_TRANSFER" selectedIndex="0"/>
</configuration>
<configuration inuse="true" name="FreeRTOS_Object">
<component display="FreeRTOS_Object" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.object"/>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
<configuration inuse="true" name="FreeRTOS_Kernel">
<component display="FreeRTOS_Kernel" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.amazon.kernel">
<gridItem id="configTOTAL_HEAP_SIZE" selectedIndex="( size_t ) ( 256U * 1024U )"/>
</component>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
<configuration inuse="true" name="AWS_device_shadow">
<component display="AWS_device_shadow" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.amazon.device_shadow"/>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
<configuration inuse="true" name="AWS_tcp_ip">
<component display="AWS_tcp_ip" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.amazon.tcp_ip">
<gridItem id="ipconfigINCLUDE_FULL_INET_ADDR" selectedIndex="0"/>
<gridItem id="ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS" selectedIndex="ETHER_CFG_EMAC_TX_DESCRIPTORS"/>
<gridItem id="ipconfigUSE_TCP_WIN" selectedIndex="0"/>
<gridItem id="ipconfigTCP_RX_BUFFER_LENGTH" selectedIndex="1460 * 8"/>
<gridItem id="ipconfigTCP_TX_BUFFER_LENGTH" selectedIndex="1460 * 8"/>
</component>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
<configuration inuse="true" name="AWS_mqtt">
<component display="AWS_mqtt" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.amazon.mqtt">
<gridItem id="mqttconfigMQTT_TASK_STACK_DEPTH" selectedIndex="6144"/>
</component>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
<configuration inuse="true" name="AWS_secure_socket">
<component display="AWS_secure_socket" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.amazon.secure_socket"/>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
<configuration inuse="true" name="AWS_ggd">
<component display="AWS_ggd" id="com.renesas.smc.tools.swcomponent.rtosconfigurator.freertos.amazon.ggd"/>
<source id="com.renesas.smc.tools.swcomponent.rtosconfigurator.source"/>
</configuration>
</tool>
<tool id="System">
    <section id="ocd">
        <option enabled="true" id="debugIF" selection="jtag">
            <item enabled="true" id="unused"/>
            <item enabled="true" id="fine"/>
            <item enabled="true" id="jtag"/>
            <item enabled="true" id="jtagTrace"/>
        </option>
    </section>
</tool>
</smc>
