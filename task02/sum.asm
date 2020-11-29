;Surova Sofia
;Variant ¹3
;B[i]=A[i]+A[i+1]

format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strVecSize        db 'size of vector = ', 0
        strIncorrectSize  db 'Incorrect size of vector.', 10, 0
        strVecElemInput   db '[%d] = ', 0
        strScanInt        db '%d', 0
        strVecAOutput     db 'A:', 10, 0
        strVecBOutput     db 'B:', 10, 0
        strVecElemOutput  db '[%d] = %d', 10, 0

        vecA_size    dd 0
        vecB_size    dd 0
        sum          dd 0
        i            dd ?
        tmp          dd ?
        tmpStack     dd ?
        vecA         rd 100
        vecB         rd 100

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
; 1) vector A input
        call InputVector
; 2) get vector B
        call FormVectorB
; 3) vector A output
        push strVecAOutput
        call[printf]
        call VectorAOut
; 4) vector B output
        push strVecBOutput
        call[printf]
        call VectorBOut

finish:
        call [getch]
; 5) exit
        push 0
        call [ExitProcess]

;--------------------------------------------------------------------------
InputVector:
        ; size input
        push strVecSize
        call [printf]
        add esp, 4

        push vecA_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vecA_size]
        cmp eax, 1
        jg  getVector

        ; fail size (size <= 1)
        push strIncorrectSize
        call [printf]
        call [getch]
        push 0
        call [ExitProcess]

; else continue (size > 1) and start cycle
getVector:
        xor ecx, ecx             ; ecx = 0
        mov ebx, vecA            ; ebx = &vecA
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vecA_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemInput
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
;--------------------------------------------------------------------------
FormVectorB:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vecA           ; ebx = &vecA
        mov edx, vecB           ; edx = &vecB

        ;vecB_size = vecA_size - 1
        mov eax, [vecA_size]
        mov [vecB_size], eax
        dec [vecB_size]

;start cycle
sumVecLoop:
        cmp ecx, [vecB_size]
        je endSumVector        ; to end of loop

        ;eax = A[i]+A[i+1]
        mov eax, [ebx]
        inc ecx
        add ebx, 4
        add eax, [ebx]
        mov [edx], eax
        add edx, 4
        jmp sumVecLoop
endSumVector:
        ret
;--------------------------------------------------------------------------
VectorAOut:
        mov [tmpStack], esp
        xor ecx, ecx           ; ecx = 0
        mov ebx, vecA          ; ebx = &vecA
putVecALoop:
        mov [tmp], ebx
        cmp ecx, [vecA_size]
        je endOutputVectorA    ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOutput
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecALoop
endOutputVectorA:
        mov esp, [tmpStack]
        ret
;--------------------------------------------------------------------------
VectorBOut:
        mov [tmpStack], esp
        xor ecx, ecx           ; ecx = 0
        mov edx, vecB          ; edx = &vecB
putVecBLoop:
        mov [tmp], edx
        cmp ecx, [vecB_size]
        je endOutputVectorB    ; to end of loop
        mov [i], ecx

        ; output element
        push dword [edx]
        push ecx
        push strVecElemOutput
        call [printf]

        mov ecx, [i]
        inc ecx
        mov edx, [tmp]
        add edx, 4
        jmp putVecBLoop
endOutputVectorB:
        mov esp, [tmpStack]
        ret
;--------------------------------------------------------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'