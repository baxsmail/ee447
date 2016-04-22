/*MEMORY
*{
*    ram : ORIGIN = 0x0000, LENGTH = 12000
*}*/

SECTIONS
{
    .init   0x0000 : { *(.init    ) } 
    .text   0x1000 : { *(.text*   ) } 
    .data   0x3000 : { *(.data*    ) } 
    .rodata 0x5000 : { *(.rodata*  ) } 
    .bss    0x6000 : { *(.bss*     ) } 
    COMMON  0x7000 : { *(COMMON*   ) } 
  
}

