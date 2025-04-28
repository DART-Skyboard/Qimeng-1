OUTPUT_ARCH( "riscv" )
ENTRY(_start)


MEMORY
{
  flash (wxa!ri) : ORIGIN = 0x00000000, LENGTH = 1M
  ram (wxa!ri) :   ORIGIN = 0x83800000, LENGTH = 1M
}


SECTIONS
{
  .init           :
  {
    *(.init)
  } >flash AT>flash 
  /DISCARD/ : {*(*)}

}
