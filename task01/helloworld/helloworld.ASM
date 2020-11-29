format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        hello db 'Hello, world!', 0

        NULL = 0

section '.code' code readable executable

        start:
                push hello
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