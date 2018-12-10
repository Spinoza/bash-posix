#ifndef OPTIONS_H
#define OPTIONS_H

#define TRUE 1
#define FALS 0

#include <stdio.h>

/**
 *\file options.h
 *\brief
 *All information about options
 */

/**
 *\struct option
 *\brief Struct of the options used for 42sh
 *
 *This struct gathers information about the options woth which
 *42sh must be used
 */
struct option
{
    int c; /**< Set to 1 if option -c activated */
    int minus_O; /**< Set to 1 if option -0 activated */
    int plus_O; /**< Set to 1 if option +0 activated */
    int norc; /**< Set to 1 if option --norc activated */
    int ast_print; /**< Set to 1 if option --ast-print activated */
    int dotglob;
    int expand_aliases;
    int extglob;
    int nocaseglob;
    int nullglob;
    int sourcepath;
    int xpg_echo;
    int version; /**< Set to 1 if option --vesrion activated */

    char *arg_c; /**< Keeps arg for option -c */
};

/**
 *\fn struct option *option_init(void)
 *\brief Creates struct option to save information about options
 *\param void
 *\return a struct option if everything works properly, NULL otherwise
 */
struct option *option_init(void);

/**
 *\fn void handle_option(struct option *options)
 *\brief handle the use of different options
 *\param struct option *options, options to be used
 *\return void
 */
void handle_option(struct option *options);

/**
 *\fn int options_parser(int argc, char *argv, struct option *options)
 *\brief Sets options to be used
 *\param char *argv[] array of arguments, int argc size of array, struct
 *option *options struct to fill
 *\return the index of first element in argv that is not an option if
 *everything works properly, 0 otherwise
 */
int options_parser(int argc, char *argv[], struct option *options);

#endif /* ! OPTIONS_H */
