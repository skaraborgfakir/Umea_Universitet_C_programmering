/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-18 14:40:11 stefan>
 * File:         game_of_life_file_handler.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 * Input:        Choice of initial configuration and then instruction
 *               to step or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#ifndef GAME_OF_LIFE_FILE_HANDLER_H
#define GAME_OF_LIFE_FILE_HANDLER_H

int load_config_from_file( field *the_field,
			   FILE *fp);
int save_config_to_file( const field the_field,
			 FILE *fp);

#endif

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
