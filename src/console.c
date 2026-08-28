#include "ASCIIPCR/console.h"

#include <stdio.h>

HANDLE h_std_out; // initializing only on start
DWORD old_console_mode;
DWORD new_console_mode;

int windows_enable_ansi() {
    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_std_out == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "failed to get standard output handle!\n");
        return 23;
    }

    if (!GetConsoleMode(h_std_out, &old_console_mode)) {
        fprintf(stderr, "failed to get old console mode!\n");
        return 20;
    }

    new_console_mode = old_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING; // add virtual_terminal_processing to old mode
    if (!SetConsoleMode(h_std_out,  new_console_mode)) {
        fprintf(stderr, "failed to set new console mode!\n");
        return 25;
    }

    return 0;
}

int windows_disable_ansi() {
    if (!SetConsoleMode(h_std_out, old_console_mode)) {
        fprintf(stderr, "failed to restore console mode on exit! Good luck with ANSI-symbols :)\n");
        return 15;
    }

    return 0;
}