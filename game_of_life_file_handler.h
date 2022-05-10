/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-28 00:12:01 stefan>
 * File:         game_of_life_file_handler.h
 * Description:  A simple implementation of Conway's Game of Life.
 *               Definitioner för I/O-funktioner
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 */

#ifndef GAME_OF_LIFE_FILE_HANDLER_H
#define GAME_OF_LIFE_FILE_HANDLER_H

/* Krav från uppgiftsektionen för Mästarprov 9:
 *   3: implementera funktionen load_config_from_file
 *   3: implementera funktionen save_config_to_file
 */
int load_config_from_file( field *the_field, FILE *fp);
int save_config_to_file( const field the_field, FILE *fp);

#endif

/*
 * Local Variables:
 * c-file-style: "ellemtel"
 * compile-command: "make"
 * End:
 */
