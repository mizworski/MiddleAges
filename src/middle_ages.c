#include <stdbool.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"

#define GAME_OVER                                   1
#define ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE     42

int main() {
    bool game_over = false;

    start_game();

    command *new_command;

    while (!game_over) {
        new_command = parse_command();
        int return_value = 0;
        switch (new_command->command_id) {
            case PARSE_ERROR:
                end_game();
                break;
            case INIT:
                return_value = init(new_command->data[0], new_command->data[1],
                                    new_command->data[2], new_command->data[3],
                                    new_command->data[4], new_command->data[5],
                                    new_command->data[6]);
                break;
            case MOVE:
                return_value = move(new_command->data[0], new_command->data[1],
                                    new_command->data[2], new_command->data[3]);
                break;
            case PRODUCE_KNIGHT:
                return_value = produce_knight(new_command->data[0], new_command->data[1],
                                              new_command->data[2], new_command->data[3]);
                break;
            case PRODUCE_PEASANT:
                return_value = produce_peasant(new_command->data[0], new_command->data[1],
                                               new_command->data[2], new_command->data[3]);
                break;
            case END_TURN:
                return_value = end_turn();
                break;
            default:
                return_value = PARSE_ERROR;
                break;
        }

        free(new_command);

        if (return_value == PARSE_ERROR) {
            end_game();
            return ANSWER_TO_THE_ULTIMATE_QUESTION_OF_LIFE;
        }

        if (return_value == GAME_OVER) {
            game_over = true;
        }

        print_topleft();
    }

    end_game();

    return 0;
}
