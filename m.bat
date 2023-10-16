  @Echo Off

  If Exist clffn.lib del clffn.lib

  cl /c /Oealtc /FPa /Zln /AL /Gs /J /W3 /WX /Gt1 ffstnxt.c
  If ErrorLevel 1 Goto Oops

  masm /mL _ffstnxt.asm;
  If ErrorLevel 1 Goto Oops

  Lib clffn.lib ffstnxt.obj+_ffstnxt.obj, clffn.lst;
  If ErrorLevel 1 Goto Oops

  Clipper test /m/n/w/p
  If ErrorLevel 1 Goto Oops

  Blinker file test lib clffn
  If ErrorLevel 1 Goto Oops

  Goto End

:Oops
  Echo Something terrible has happened

:End
