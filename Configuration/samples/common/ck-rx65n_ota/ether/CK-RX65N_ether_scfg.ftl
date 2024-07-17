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
                <gridItem id="AN000" selectedIndex="1"/>
                <gridItem id="ADTRG0#" selectedIndex="1"/>
                <gridItem id="S12AD0" selectedIndex="1"/>
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
        <configuration inuse="true" name="r_ether_rx">
            <component display="r_ether_rx" id="r_ether_rx1.23" version="1.23">
                <gridItem id="ET0_TX_CLK" selectedIndex="1"/>
                <gridItem id="ET0_RX_CLK" selectedIndex="1"/>
                <gridItem id="ET0_TX_EN" selectedIndex="1"/>
                <gridItem id="ET0_ETXD3" selectedIndex="1"/>
                <gridItem id="ET0_ETXD2" selectedIndex="1"/>
                <gridItem id="ET0_ETXD1" selectedIndex="1"/>
                <gridItem id="ET0_ETXD0" selectedIndex="1"/>
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
                <gridItem id="ETHER_CFG_EMAC_TX_DESCRIPTORS" selectedIndex="3"/>
                <gridItem id="ETHER_CFG_CH0_PHY_ACCESS" selectedIndex="0"/>
                <gridItem id="ETHER_CFG_LINK_PRESENT" selectedIndex="1"/>
                <gridItem id="ETHER_CFG_USE_PHY_ICS1894_32" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_sci_rx">
            <component detailDescription="Multi-Channel Multi-Mode SCI Driver." display="r_sci_rx" id="r_sci_rx5.00" version="5.00">
                <gridItem id="SCI1" selectedIndex="0"/>
                <gridItem id="RXD5/SMISO5/SSCL5" selectedIndex="1"/>
                <gridItem id="TXD5/SMOSI5/SSDA5" selectedIndex="1"/>
                <gridItem id="SCI5" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH1_INCLUDED" selectedIndex="0"/>
                <gridItem id="SCI_CFG_CH5_INCLUDED" selectedIndex="1"/>
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
        <configuration inuse="true" name="r_fwup">
            <component display="r_fwup" id="r_fwup2.03" version="2.03">
                <gridItem id="FWUP_CFG_UPDATE_MODE" selectedIndex="0"/>
                <gridItem id="FWUP_CFG_FUNCTION_MODE" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_MAIN_AREA_ADDR_L" selectedIndex="0xFFF00000U"/>
                <gridItem id="FWUP_CFG_BUF_AREA_ADDR_L" selectedIndex="0xFFE00000U"/>
                <gridItem id="FWUP_CFG_AREA_SIZE" selectedIndex="0xF0000U"/>
                <gridItem id="FWUP_CFG_CF_BLK_SIZE" selectedIndex="0x8000U"/>
                <gridItem id="FWUP_CFG_CF_W_UNIT_SIZE" selectedIndex="128U"/>
                <gridItem id="FWUP_CFG_EXT_BUF_AREA_ADDR_L" selectedIndex="0x00000U"/>
                <gridItem id="FWUP_CFG_EXT_BUF_AREA_BLK_SIZE" selectedIndex="4096U"/>
                <gridItem id="FWUP_CFG_DF_BLK_SIZE" selectedIndex="64U"/>
                <gridItem id="FWUP_CFG_DF_NUM_BLKS" selectedIndex="512U"/>
                <gridItem id="FWUP_CFG_SIGNATURE_VERIFICATION" selectedIndex="0"/>
                <gridItem id="FWUP_CFG_USER_SHA256_INIT_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_SHA256_INIT_FUNCTION" selectedIndex="ota_sha256_init_function"/>
                <gridItem id="FWUP_CFG_USER_SHA256_UPDATE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_SHA256_UPDATE_FUNCTION" selectedIndex="ota_sha256_update_function"/>
                <gridItem id="FWUP_CFG_USER_SHA256_FINAL_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_SHA256_FINAL_FUNCTION" selectedIndex="ota_sha256_final_function"/>
                <gridItem id="FWUP_CFG_USER_VERIFY_ECDSA_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_VERIFY_ECDSA_FUNCTION" selectedIndex="ota_verify_ecdsa_function"/>
                <gridItem id="FWUP_CFG_USER_GET_CRYPT_CONTEXT_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_GET_CRYPT_CONTEXT_FUNCTION" selectedIndex="ota_get_crypt_context_function"/>
                <gridItem id="FWUP_CFG_USER_FLASH_OPEN_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_OPEN_FUNCTION" selectedIndex="ota_flash_open_function"/>
                <gridItem id="FWUP_CFG_USER_FLASH_CLOSE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_CLOSE_FUNCTION" selectedIndex="ota_flash_close_function"/>
                <gridItem id="FWUP_CFG_USER_FLASH_ERASE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_ERASE_FUNCTION" selectedIndex="ota_flash_erase_function"/>
                <gridItem id="FWUP_CFG_USER_FLASH_WRITE_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_WRITE_FUNCTION" selectedIndex="ota_flash_write_function"/>
                <gridItem id="FWUP_CFG_USER_FLASH_READ_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_FLASH_READ_FUNCTION" selectedIndex="ota_flash_read_function"/>
                <gridItem id="FWUP_CFG_USER_BANK_SWAP_ENABLED" selectedIndex="1"/>
                <gridItem id="FWUP_CFG_USER_BANK_SWAP_FUNCTION" selectedIndex="ota_bank_swap_function"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
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
