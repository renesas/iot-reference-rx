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
        <tool id="Pins" version="1.0.1.0">
        <pinItem allocation="10" comments="" direction="None" id="EMLE" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="20" comments="" direction="None" id="XTAL" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="134" comments="" direction="None" id="IRQ14" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="135" comments="" direction="None" id="IRQ13" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="4" comments="" direction="None" id="IRQ11" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="98" comments="" direction="None" id="IRQ15" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="124" comments="" direction="None" id="IRQ2" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="126" comments="" direction="None" id="IRQ0" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="102" comments="" direction="None" id="IRQ6" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="101" comments="" direction="None" id="IRQ7" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="70" comments="" direction="None" id="RXD5" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="7" comments="" direction="None" id="RXD6" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="129" comments="" direction="None" id="AN115" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="127" comments="" direction="None" id="AN117" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="8" comments="" direction="None" id="TXD6" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="67" comments="" direction="None" id="TXD5" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="22" comments="" direction="None" id="EXTAL" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="16" comments="" direction="None" id="FINED" isUsedBySoftware="true" locked="false" operationMode="" status="0"/>
        <pinItem allocation="13" comments="" direction="None" id="CTS6#" isUsedBySoftware="false" locked="false" operationMode="" status="0"/>
        <pinnumItem comment="LED2_B" id="88" symbolicname=""/>
        <pinnumItem comment="USBC-CTS" id="89" symbolicname=""/>
        <pinnumItem comment="Biometric-SDA" id="110" symbolicname=""/>
        <pinnumItem comment="OUTDOOR-AIR-QUALITY-RES_N" id="113" symbolicname=""/>
        <pinnumItem comment="INDOOR-AIR-CUALITY-RES_N" id="114" symbolicname=""/>
        <pinnumItem comment="VSS" id="116" symbolicname=""/>
        <pinnumItem comment="ETHERNET-LINKSTA" id="90" symbolicname=""/>
        <pinnumItem comment="VCC" id="91" symbolicname=""/>
        <pinnumItem comment="VCC" id="118" symbolicname=""/>
        <pinnumItem comment="PMOD1-GPIO0" id="92" symbolicname=""/>
        <pinnumItem comment="VSS" id="93" symbolicname=""/>
        <pinnumItem comment="LED4" id="94" symbolicname=""/>
        <pinnumItem comment="PMOD2-GPIO1" id="96" symbolicname=""/>
        <pinnumItem comment="EMLE" id="10" symbolicname=""/>
        <pinnumItem comment="Biometric-INT" id="98" symbolicname=""/>
        <pinnumItem comment="PMOD2-CTS" id="11" symbolicname=""/>
        <pinnumItem comment="VSS" id="12" symbolicname=""/>
        <pinnumItem comment="PMOD1-CTS" id="13" symbolicname=""/>
        <pinnumItem comment="VCL" id="14" symbolicname=""/>
        <pinnumItem comment="VBATT" id="15" symbolicname=""/>
        <pinnumItem comment="MD_FINED" id="16" symbolicname=""/>
        <pinnumItem comment="XCIN" id="17" symbolicname=""/>
        <pinnumItem comment="XCOUT" id="18" symbolicname=""/>
        <pinnumItem comment="S1(Reset)" id="19" symbolicname=""/>
        <pinnumItem comment="PowerHMI-PG" id="121" symbolicname=""/>
        <pinnumItem comment="AVSS0" id="1" symbolicname=""/>
        <pinnumItem comment="ETHERNET-IRQ" id="124" symbolicname=""/>
        <pinnumItem comment="AVCC1" id="3" symbolicname=""/>
        <pinnumItem comment="Barometric-Pressure-Sensor-INT" id="4" symbolicname=""/>
        <pinnumItem comment="S2(UserSwitch)" id="126" symbolicname=""/>
        <pinnumItem comment="AVSS1" id="5" symbolicname=""/>
        <pinnumItem comment="MICROPHONES2-OUT" id="127" symbolicname=""/>
        <pinnumItem comment="PMOD1-RXD" id="7" symbolicname=""/>
        <pinnumItem comment="MICROPHONES1-OUT" id="129" symbolicname=""/>
        <pinnumItem comment="PMOD1-TXD" id="8" symbolicname=""/>
        <pinnumItem comment="XTAL" id="20" symbolicname=""/>
        <pinnumItem comment="VSS" id="21" symbolicname=""/>
        <pinnumItem comment="EXTAL" id="22" symbolicname=""/>
        <pinnumItem comment="VCC" id="23" symbolicname=""/>
        <pinnumItem comment="UPSEL" id="24" symbolicname=""/>
        <pinnumItem comment="PMOD2-GPIO3" id="26" symbolicname=""/>
        <pinnumItem comment="PMOD2-GPIO2" id="27" symbolicname=""/>
        <pinnumItem comment="PMOD2-GPIO0" id="28" symbolicname=""/>
        <pinnumItem comment="SERIAL-RXD" id="29" symbolicname=""/>
        <pinnumItem comment="VSS" id="130" symbolicname=""/>
        <pinnumItem comment="VCC" id="132" symbolicname=""/>
        <pinnumItem comment="INDOOR-AIR-QUALITY-INT" id="134" symbolicname=""/>
        <pinnumItem comment="OUTDOOR-AIR-QUALITY-INT" id="135" symbolicname=""/>
        <pinnumItem comment="SERIAL-TXD" id="31" symbolicname=""/>
        <pinnumItem comment="LED6" id="32" symbolicname=""/>
        <pinnumItem comment="USB0_VBUSEN" id="33" symbolicname=""/>
        <pinnumItem comment="LED3" id="35" symbolicname=""/>
        <pinnumItem comment="Outdoor-Air-Quality-Sensor-SCL" id="36" symbolicname=""/>
        <pinnumItem comment="Outdoor-Air-Quality-Sensor-SDA" id="37" symbolicname=""/>
        <pinnumItem comment="LED2_R" id="38" symbolicname=""/>
        <pinnumItem comment="LED2_G" id="39" symbolicname=""/>
        <pinnumItem comment="VREFL0" id="140" symbolicname=""/>
        <pinnumItem comment="VREFH0" id="142" symbolicname=""/>
        <pinnumItem comment="AVCC0" id="143" symbolicname=""/>
        <pinnumItem comment="USB0_VBUS" id="40" symbolicname=""/>
        <pinnumItem comment="USB0_OVRCURA" id="43" symbolicname=""/>
        <pinnumItem comment="Temperature-Motion-Pressure-SDA" id="44" symbolicname=""/>
        <pinnumItem comment="Temperature-Motion-Pressure-SCL" id="45" symbolicname=""/>
        <pinnumItem comment="VCC_USB" id="46" symbolicname=""/>
        <pinnumItem comment="USBFS_DM" id="47" symbolicname=""/>
        <pinnumItem comment="USBFS_DP" id="48" symbolicname=""/>
        <pinnumItem comment="VSS_USB" id="49" symbolicname=""/>
        <pinnumItem comment="PMOD1-GPIO1" id="51" symbolicname=""/>
        <pinnumItem comment="PMOD2-RTS" id="52" symbolicname=""/>
        <pinnumItem comment="PMOD2-RXD" id="54" symbolicname=""/>
        <pinnumItem comment="PMOD2-TXD" id="56" symbolicname=""/>
        <pinnumItem comment="VSS" id="57" symbolicname=""/>
        <pinnumItem comment="ETHERNET-CRS_DV" id="58" symbolicname=""/>
        <pinnumItem comment="VCC" id="59" symbolicname=""/>
        <pinnumItem comment="UB" id="60" symbolicname=""/>
        <pinnumItem comment="ETHERNET-TXD1" id="63" symbolicname=""/>
        <pinnumItem comment="ETHERNET-TXD0" id="64" symbolicname=""/>
        <pinnumItem comment="ETHERNET-RST" id="65" symbolicname=""/>
        <pinnumItem comment="USBC-TXD" id="67" symbolicname=""/>
        <pinnumItem comment="ETHERNET-RX-ER" id="68" symbolicname=""/>
        <pinnumItem comment="ETHERNET-REF-50-CK" id="69" symbolicname=""/>
        <pinnumItem comment="USBC-RXD" id="70" symbolicname=""/>
        <pinnumItem comment="ETHERNET-RXD0" id="71" symbolicname=""/>
        <pinnumItem comment="ETHERNET-RXD1" id="72" symbolicname=""/>
        <pinnumItem comment="VCC" id="74" symbolicname=""/>
        <pinnumItem comment="USBC-RTS" id="75" symbolicname=""/>
        <pinnumItem comment="VSS" id="76" symbolicname=""/>
        <pinnumItem comment="PMOD1-GPIO2" id="78" symbolicname=""/>
        <pinnumItem comment="PMOD1-GPIO3" id="79" symbolicname=""/>
        <pinnumItem comment="Motion-INT" id="101" symbolicname=""/>
        <pinnumItem comment="Motion-INT" id="102" symbolicname=""/>
        <pinnumItem comment="VCC" id="103" symbolicname=""/>
        <pinnumItem comment="VSS" id="105" symbolicname=""/>
        <pinnumItem comment="ETHERNET-TXD_EN" id="81" symbolicname=""/>
        <pinnumItem comment="Biometric-SCL" id="109" symbolicname=""/>
        <pinnumItem comment="PMOD1-RTS" id="83" symbolicname=""/>
        <pinnumItem comment="ETHERNET-MDC" id="85" symbolicname=""/>
        <pinnumItem comment="ETHERNET-MDIO" id="86" symbolicname=""/>
    </tool>
    <tool id="SWComponent" version="1.0.0.0">
        <configuration inuse="true" name="r_byteq">
            <component display="r_byteq" id="r_byteq2.10" version="2.10">
                <gridItem id="BYTEQ_CFG_MAX_CTRL_BLKS" selectedIndex="32"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_s12ad_rx">
            <component detailDescription="ADC Driver" display="r_s12ad_rx" id="r_s12ad_rx5.40" version="5.40">
                <gridItem id="ADTRG0#" selectedIndex="1"/>
                <gridItem id="AN115" selectedIndex="1"/>
                <gridItem id="AN117" selectedIndex="1"/>
                <gridItem id="S12AD1" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_tsip_rx">
            <component display="r_tsip_rx" id="r_tsip_rx1.21" version="1.21"/>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
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
        <configuration inuse="true" name="r_fwup">
            <component display="r_fwup" id="r_fwup2.04" version="2.04">
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
        <configuration inuse="true" name="r_bsp">
            <component display="r_bsp" id="r_bsp7.52" version="7.52">
                <gridItem id="BSP_CFG_HEAP_BYTES" selectedIndex="0x2000"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_ENABLE" selectedIndex="1"/>
                <gridItem id="BSP_CFG_SCI_UART_TERMINAL_CHANNEL" selectedIndex="5"/>
                <gridItem id="BSP_CFG_USER_CHARPUT_ENABLED" selectedIndex="1"/>
                <gridItem id="BSP_CFG_USER_CHARPUT_FUNCTION" selectedIndex="vOutputChar"/>
                <gridItem id="BSP_CFG_IO_LIB_ENABLE" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_sci_rx">
            <component detailDescription="Multi-Channel Multi-Mode SCI Driver." display="r_sci_rx" id="r_sci_rx5.40" version="5.40">
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
                <gridItem id="SCI_CFG_CH6_EN_TXI_NESTED_INT" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH6_EN_RXI_NESTED_INT" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH6_EN_TEI_NESTED_INT" selectedIndex="1"/>
                <gridItem id="SCI_CFG_CH6_EN_ERI_NESTED_INT" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_flash_rx">
            <component display="r_flash_rx" id="r_flash_rx5.21" version="5.21">
                <gridItem id="FLASH_CFG_PARAM_CHECKING_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_ENABLE" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_DATA_FLASH_BGO" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_BGO" selectedIndex="1"/>
                <gridItem id="FLASH_CFG_CODE_FLASH_RUN_FROM_ROM" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
        </configuration>
        <configuration inuse="true" name="r_cellular">
            <component display="r_cellular" id="r_cellular1.12" version="1.12">
                <gridItem id="CELLULAR_CFG_AP_NAME" selectedIndex="iot.truphone.com"/>
                <gridItem id="CELLULAR_CFG_AUTH_TYPE" selectedIndex="2"/>
                <gridItem id="CELLULAR_CFG_DEBUGLOG" selectedIndex="4"/>
                <gridItem id="CELLULAR_CFG_UART_SCI_CH" selectedIndex="6"/>
                <gridItem id="CELLULAR_CFG_RTS_PORT" selectedIndex="0"/>
                <gridItem id="CELLULAR_CFG_RESET_PORT" selectedIndex="5"/>
                <gridItem id="CELLULAR_CFG_RESET_PIN" selectedIndex="5"/>
                <gridItem id="CELLULAR_CFG_IRQ_NUM" selectedIndex="4"/>
            </component>
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
        <configuration inuse="true" name="r_irq_rx">
            <component display="r_irq_rx" id="r_irq_rx4.60" version="4.60">
                <gridItem id="IRQ0" selectedIndex="1"/>
                <gridItem id="IRQ2" selectedIndex="1"/>
                <gridItem id="IRQ6" selectedIndex="1"/>
                <gridItem id="IRQ7" selectedIndex="1"/>
                <gridItem id="IRQ11" selectedIndex="1"/>
                <gridItem id="IRQ13" selectedIndex="1"/>
                <gridItem id="IRQ14" selectedIndex="1"/>
                <gridItem id="IRQ15" selectedIndex="1"/>
            </component>
            <source description="Components supporting Firmware Integration Technology" display="Firmware Integration Technology" id="com.renesas.smc.tools.swcomponent.fit.source"/>
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
