#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "engine.h"

int main() {

    start_game();

    command *new_command;
    while (1) {
        new_command = parse_command();

        if (strcmp(new_command->name, "INIT") == 0) {
            init(new_command->data[0],
                 new_command->data[1],
                 new_command->data[2],
                 new_command->data[3],
                 new_command->data[4],
                 new_command->data[5],
                 new_command->data[6]);
        }

        print_topleft();
    }

    end_game();

    return 0;
}
