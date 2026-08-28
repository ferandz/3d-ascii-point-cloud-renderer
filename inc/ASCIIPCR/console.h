/*
    console.h - console functions header
    (this header is shit) 
*/

#ifdef _WIN32

#ifndef WINANSI_H
#define WINANSI_H
    #include <windows.h>

    // enable ANSI escape
    int windows_enable_ansi();
    int windows_disable_ansi();

#else
    #include <unistd.h>
#endif // WinANSI
#endif // _WIN32