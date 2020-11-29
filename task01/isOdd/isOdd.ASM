format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        formatNum db '%d', 0

        A rd 2

        aStr db 'A: ', 0
        oddStr db '%d is odd', 0
        evenStr db '%d is even', 0

        NULL = 0

section '.code' code readable executable

        start:
                push aStr
                call [printf]

                push A
                push formatNum
                call [scanf]

                mov eax, [A]
                test eax, 1  ;если 0, то число чётное
                jz even

               ;нечетное
                 push [A]
                 push oddStr
                 call [printf]
                 jmp finish

               ;чётное
                even:
                 push [A]
                 push evenStr
                 call [printf]

               ;завершение программы
                finish:
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