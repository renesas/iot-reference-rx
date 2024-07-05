MEMORY
{
	RAM : ORIGIN = 0x4, LENGTH = 0x3fffc
	RAM2 : ORIGIN = 0x00800000, LENGTH = 393216
    DATAFLASH (!rx) : ORIGIN = 0x100000, LENGTH = 32768
	ROM : ORIGIN = 0xFFF00000, LENGTH = 1048576
	OFS : ORIGIN = 0xFE7F5D00, LENGTH = 128
}

SECTIONS
{
	.exvectors 0xFFFFFF80: AT(0xFFFFFF80)
	{
		"_exvectors_start" = .;
		KEEP(*(.exvectors))
	"_exvectors_end" = .;
	} >ROM
	
	.fvectors 0xFFFFFFFC: AT(0xFFFFFFFC)
	{
		KEEP(*(.fvectors))
	} > ROM
	
	.text 0xFFF35200 : AT(0xFFF35200)
	{
		*(.text)
		*(.text.*)
		*(P)
		etext = .;
	} > ROM
	
	.rvectors ALIGN(4):
	{
		_rvectors_start = .;
		INCLUDE ../src/smc_gen/r_bsp/mcu/all/linker_script_rvectors.inc
		_rvectors_end = .;
	} > ROM
	
	.init :
	{
		KEEP(*(.init))
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		__init_array_start = (. + 3) & ~ 3;
		KEEP(*(.init_array))
		KEEP(*(SORT(.init_array.*)))
		__init_array_end = .;
		__fini_array_start = .;
		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		__fini_array_end = .;
	} > ROM
	
	.fini :
	{
		KEEP(*(.fini))
	} > ROM
	
	.got :
	{
		*(.got)
		*(.got.plt)
	} > ROM
	
	.rodata 0xFFF00300: AT(0xFFF00300)
	{
		*(.rodata)
		*(.rodata.*)
		*(C_1)
		*(C_2)
		*(C)
		_erodata = .;
	} > ROM
	
	gcc_exceptions_table :
	{
	    KEEP (*(.gcc_except_table))
	    *(.gcc_except_table.*)
	} > ROM
	
	.eh_frame_hdr :
	{
		*(.eh_frame_hdr)
	} > ROM
	
	.eh_frame :
	{
		*(.eh_frame)
	} > ROM
	.jcr :
	{
		*(.jcr)
	} > ROM
	
	.tors :
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		*(.ctors)
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		*(.dtors)
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
		_mdata = .; 
		. += _edata - _data;
	} > ROM
	
	.pfram2 ALIGN(4):
	{ 
	 _PFRAM2_start = .;
	 . += _RPFRAM2_end - _RPFRAM2_start;
	 _PFRAM2_end = .;
	} > ROM
	
	.data : AT(_mdata)
	{
		_data = .;
		*(.data)
		*(.data.*)
		*(D)
		*(D_1)
		*(D_2)
		_edata = .;
	} > RAM
	
	.rpfram2 ALIGN(4): AT(_PFRAM2_start)
	{ 
	 _RPFRAM2_start = .;
	 *(PFRAM2)
	 . = ALIGN(4);
	 _RPFRAM2_end = .;
	} > RAM

       .data_exram : AT(LOADADDR(.data)+(_edata - _data))
    {
        PROVIDE( _mdata_exram = LOADADDR(.data_exram) );
        _data_exram = .;
        *(.data_exram)
        *(.data_exram.*)
        . = ALIGN(2);
        _edata_exram = .;
    } > RAM2

	.bss :
	{
		_bss = .;
		*MqttDemoExample.o(.bss.**)
		*(COMMON)
		*(B)
		*(B_1)
		*(B_2)
		_ebss = .;
		. = ALIGN(128);
		_end = .;
	} >RAM AT>RAM
	
	.bss_exram :
	{
		_bss_exram = .;
		*(.bss)
		*(.bss.**)
		_ebss_exram = .;
		_end = .;
	} > RAM2 AT>RAM2
	
	C_LITTLEFS_MANAGEMENT_AREA 0x00100000 : AT(0x00100000)
	{
	} >DATAFLASH
	C_USER_APPLICATION_AREA 0x00102300 : AT(0x00102300)
    {
    } >DATAFLASH		
	
	.ofs1 0xFE7F5D00: AT(0xFE7F5D00)
	{
		KEEP(*(.ofs1))
	} > OFS
	.ofs2 0xFE7F5D10: AT(0xFE7F5D10)
	{
		KEEP(*(.ofs2))
	} > OFS
	.ofs3 0xFE7F5D20: AT(0xFE7F5D20)
	{
		KEEP(*(.ofs3))
	} > OFS
	.ofs4 0xFE7F5D40: AT(0xFE7F5D40)
	{
		KEEP(*(.ofs4))
	} > OFS
	.ofs5 0xFE7F5D48: AT(0xFE7F5D48)
	{
		KEEP(*(.ofs5))
	} > OFS
	.ofs6 0xFE7F5D50: AT(0xFE7F5D50)
	{
		KEEP(*(.ofs6))
	} > OFS
	.ofs7 0xFE7F5D64: AT(0xFE7F5D64)
	{
		KEEP(*(.ofs7))
	} > OFS
	.ofs8 0xFE7F5D70: AT(0xFE7F5D70)
	{
		KEEP(*(.ofs8))
	} > OFS
	.r_bsp_NULL :
	{
		. += 0x100;
		"_r_bsp_NULL_end" = .;
	} >RAM AT>RAM
.r_bsp_istack ALIGN(0x4) (NOLOAD) :
	{
		KEEP(*(.r_bsp_istack))
	} >RAM AT>RAM
.istack :
	{
		"_istack" = .;
	} >RAM
.r_bsp_ustack ALIGN(0x4) (NOLOAD) :
	{
		KEEP(*(.r_bsp_ustack))
	} >RAM AT>RAM
.ustack :
	{
		"_ustack" = .;
	} >RAM
}
