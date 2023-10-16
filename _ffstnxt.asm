.MODEL LARGE

_DATA SEGMENT WORD PUBLIC 'DATA'
_DATA ENDS

DGROUP GROUP _DATA

_FFSTNXT_TEXT   SEGMENT WORD PUBLIC 'CODE'
                ASSUME cs:_FFSTNXT_TEXT, ds:DGROUP, ss:DGROUP

PUBLIC __cl_findfirst
PUBLIC __cl_findnext

__cl_findfirst  PROC    FAR
                PUSH    BP
                MOV     BP, SP
                PUSH    DI
                PUSH    SI
                PUSH    DS

                ; Set the DTA address
                LDS     DX, DWORD PTR [BP + 0Ah]
                MOV     AH, 1Ah
                INT     21h

                ; Do a _dos_findfirst()
                MOV     CX, WORD PTR [BP + 0Eh]
                LDS     DX, DWORD PTR [BP + 06h]
                MOV     AH, 4Eh
                INT     21h

                MOV     AX, 1
                JNC     FindFirstRet

                XOR     AX, AX

FindFirstRet:   POP     DS
                POP     SI
                POP     DI
                POP     BP
                RETF
__cl_findfirst  ENDP

;

__cl_findnext   PROC    FAR
                PUSH    BP
                MOV     BP, SP
                PUSH    DI
                PUSH    SI
                PUSH    DS

                ; Set the DTA address
                LDS     DX, DWORD PTR [BP + 06h]
                MOV     AH, 1Ah
                INT     21h

                ; Do a _dos_findnext()
                MOV     AH, 4Fh
                INT     21h

                MOV     AX, 1
                JNC     FindNextRet

                XOR     AX, AX

FindNextRet:    POP     DS
                POP     SI
                POP     DI
                POP     BP
                RETF
__cl_findnext   ENDP

_FFSTNXT_TEXT   ENDS
                END
