#include "headers.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "ccor/Core.h"

int PASCAL WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow) {
#if defined(_DEBUG)
        AllocConsole();

        int hCrt, i;
        FILE *hf;
        hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
        hf = _fdopen( hCrt, "w" );
        *stdout = *hf;
        i = setvbuf( stdout, NULL, _IONBF, 0 );
#endif

        CoreRunEngine(instance, prevInstance, cmdLine, cmdShow);
}
