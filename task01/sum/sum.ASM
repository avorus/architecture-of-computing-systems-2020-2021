format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        formatNum db '%d', 0

        A rd 1
        B rd 1

        aStr db 'A: ', 0
        bStr db 'B: ', 0
        resStr db 'A + B = %d', 0

        NULL = 0

section '.code' code readable executable

        start:
                push aStr
                call [printf]

                push A
                push formatNum
                call [scanf]

                push bStr
                call [printf]

                push B
                push formatNum
                call [scanf]

                mov ecx, [A]
                add ecx, [B]
                push ecx
                push resStr
                call [printf]

                call [getch]

                push NULL
                call [ExitProcess]

section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               getch, '_getch',\
               scanf, 'scanf'