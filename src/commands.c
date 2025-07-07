#include "commands.h"
#include "screen.h"
#include "memory.h"
#include "string.h"

void handle_command(const char* cmd) {
    print("\n");

    if (strcmp(cmd, "help") == 0) {
        print("Available commands:\n");
        print(" help  - show this message\n");
        print(" clear - clear screen\n");
        print(" mem   - show memory usage\n");
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "mem") == 0) {
        print("Memory used: 1024 bytes (simulated)\n");
    } else {
        print("Unknown command.\n");
    }
}
