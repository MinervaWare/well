/*
 * C-Ware License
 * 
 * Copyright (c) 2022, C-Ware
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Redistributions of modified source code must append a copyright notice in
 *    the form of 'Copyright <YEAR> <NAME>' to each modified source file's
 *    copyright notice, and the standalone license file if one exists.
 *
 * A "redistribution" can be constituted as any version of the source code
 * that is intended to comprise some other derivative work of this code. A
 * fork created for the purpose of contributing to any version of the source
 * does not constitute a truly "derivative work" and does not require listing.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * This file contains functions related to extracting arguments and options
 * from the program's argv. These functions all assume that error checking has
 * been done. This file ALSO contains iterator logic for arguments and options.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "ap_inter.h"

int argparse_option_exists(struct ArgparseParser parser, const char *option) {
    if(argparse_option_argv_index(parser, option) == ARGPARSE_NOT_FOUND)
        return ARGPARSE_NOT_FOUND;

    return 1;
}

char *argparse_get_argument(struct ArgparseParser parser, const char *argument) {
    int argv_index = 0;
    int argument_index = 0;

    liberror_is_null(argparse_get_argument, argument);
    liberror_is_null(argparse_get_argument, parser.name);
    liberror_is_null(argparse_get_argument, parser.argv);
    liberror_is_null(argparse_get_argument, parser.arguments);
    liberror_is_null(argparse_get_argument, parser.arguments->contents);
    liberror_is_negative(argparse_get_argument, parser.arguments->length);
    liberror_is_negative(argparse_get_argument, parser.argc);

    for(argv_index = 1; argv_index < parser.argc && argument_index < carray_length(parser.arguments); argv_index++) {
        struct ArgparseArgument parser_argument = parser.arguments->contents[argument_index];

        /* Skip past this option! (We can assume that the number of
         * indices skipped is correct because error checking should have
         * been done before hand. */
        if(argparse_is_option(parser, argument) == 1) {
            int parameters = argparse_option_parser_parameters(parser, argument);

            /* Skip past a variable number */
            if(parameters == ARGPARSE_VARIABLE || parameters == ARGPARSE_VARIABLE_ONE) {
                argv_index += argparse_option_argv_parameters(parser, argument);

                continue;
            }

            argv_index += parameters;

            continue;
        }

        if(strcmp(parser_argument.name, argument) == 0)
            return parser.argv[argv_index];

        argument_index++;
    }

    fprintf(stderr, "argparse_get_argument: attempt to find positional argument '%s' that is"
            " unexpected by the program.\n", argument);
    exit(EXIT_FAILURE);

    return NULL;
}

char *argparse_get_option_parameter(struct ArgparseParser parser,
                                    const char *option, int index) {
    int argv_location = 0;
    int argv_parameters = 0;

    liberror_is_null(argparse_get_option_parameter, option);
    liberror_is_null(argparse_get_option_parameter, parser.argv);

    argv_location = argparse_option_argv_index(parser, option);
    argv_parameters = argparse_option_argv_parameters(parser, option);

    if(index < 0 || index >= argv_parameters) {
        fprintf(stderr, "argparse_get_option_parameter: attempt to get parameter %i"
                " of option '%s', which is out of bounds.\n", index, option);
        exit(EXIT_FAILURE);
    }


    return parser.argv[argv_location + 1 + index];
}




/*
 * #=============================#
 * #      ITERATOR LOGIC         #
 * #=============================#
*/
int argparse_option_parameter_start(struct ArgparseParser parser, const char *option) {
    int argv_location = 0;
    int parser_location = 0;
    int argv_parameters = 0;

    liberror_is_null(argparse_option_parameter_start, option);

    argv_location = argparse_option_argv_index(parser, option);
    parser_location = argparse_option_parser_index(parser, option);
    argv_parameters = argparse_option_argv_parameters(parser, option);

    if(argv_location == ARGPARSE_NOT_FOUND) {
        fprintf(stderr, "argparse_option_parameter_start: option '%s' not in the program's argv\n", option);
        abort();
    }

    if(argv_parameters == 0) {
        fprintf(stderr, "argparse_option_parameter_start: attempt to iterate over option '%s' which takes no parameters\n", option);
        abort();
    }

    if(parser_location == ARGPARSE_NOT_FOUND) {
        fprintf(stderr, "argparse_option_parameter_start: option '%s' is not one the program expects\n", option);
        abort();
    }

    return argv_location + 1;
}

int argparse_option_parameter_stop(struct ArgparseParser parser, const char *option, int index) {
    int argv_location = 0;
    int argv_parameters = 0;

    liberror_is_null(argparse_option_parameter_stop, option);
    liberror_is_negative(argparse_option_parameter_stop, index);

    argv_location = argparse_option_argv_index(parser, option);;
    argv_parameters = argparse_option_argv_parameters(parser, option);

    if(index >= (argv_location + 1 + argv_parameters))
        return 1;

    return 0;
}

int argparse_argument_variable_start(struct ArgparseParser parser) {
    int argv_index = 0;
    int argument_index = 0;

    liberror_is_null(argparse_argument_variable_start, parser.argv);
    liberror_is_null(argparse_argument_variable_start, parser.arguments);
    liberror_is_null(argparse_argument_variable_start, parser.arguments->contents);
    liberror_is_negative(argparse_argument_variable_start, parser.argc);
    liberror_is_negative(argparse_argument_variable_start, parser.arguments->length);
    liberror_is_negative(argparse_argument_variable_start, parser.variable_arguments);

    /* This field cannot be anything but 1 or 0 */
    if(parser.variable_arguments > 1) {
        fprintf(stderr, "argparse_argument_variable_start: parser.variable_arguments must be 0 or 1 (is %i)\n",
                parser.variable_arguments);
        abort();
    }

    /* Find the first variable argument in the parser */
    for(argv_index = 1; argv_index < parser.argc; argv_index++) {
        const char *argument = parser.argv[argv_index];

        /* Skip past this option! (We can assume that the number of
         * indices skipped is correct because error checking should have
         * been done before hand. */
        if(argparse_is_option(parser, argument) == 1) {
            int parameters = argparse_option_parser_parameters(parser, argument);

            /* Skip past a variable number */
            if(parameters == ARGPARSE_VARIABLE || parameters == ARGPARSE_VARIABLE_ONE) {
                argv_index += argparse_option_argv_parameters(parser, argument);

                continue;
            }

            argv_index += parameters;

            continue;
        }

        if(argument_index == carray_length(parser.arguments))
            return argv_index;

        argument_index++;
    }

    /* No variable arguments */
    return ARGPARSE_NOT_FOUND;
}

int argparse_argument_variable_next(struct ArgparseParser parser, int index) {
    int argv_index = 0;

    if(index < 0 || index >= parser.argc) {
        fprintf(stderr, "argparse_argument_variable_next: index %i is out of bounds of argc\n", index);
        abort();
    }

    for(argv_index = index + 1; argv_index < parser.argc; argv_index++) {
        const char *argument = parser.argv[argv_index];

        /* Skip past this option! (We can assume that the number of
         * indices skipped is correct because error checking should have
         * been done before hand. */
        if(argparse_is_option(parser, argument) == 1) {
            int parameters = argparse_option_parser_parameters(parser, argument);

            /* Skip past a variable number */
            if(parameters == ARGPARSE_VARIABLE || parameters == ARGPARSE_VARIABLE_ONE) {
                argv_index += argparse_option_argv_parameters(parser, argument);

                continue;
            }

            argv_index += parameters;

            continue;
        }

        return argv_index;
    }

    return ARGPARSE_NOT_FOUND;
}

int argparse_repeatable_option_start(struct ArgparseParser parser, const char *option) {
    int option_index = -1;

    if(argparse_option_parser_index(parser, option) == ARGPARSE_NOT_FOUND) {
        fprintf(stderr, "argparse_repeatable_option_start: option '%s' not accepted by the program.\n", option);
        abort();
    }

    option_index = argparse_option_argv_index(parser, option);

    if(option_index == ARGPARSE_NOT_FOUND)
        return ARGPARSE_NOT_FOUND;

    for(; option_index < parser.argc; option_index++) {
        if(argparse_is_option(parser, parser.argv[option_index]) == 0)
            continue;

        /* Options must match */
        if(strcmp(option, parser.argv[option_index]) != 0)
            continue;

        return option_index + 1;
    }

    return ARGPARSE_NOT_FOUND;
}

int argparse_repeatable_option_next(struct ArgparseParser parser, const char *option, int index) {
    int option_index = 0;

    for(option_index = index; option_index < parser.argc; option_index++) {
        if(argparse_is_option(parser, parser.argv[option_index]) == 0)
            continue;

        /* Options must match */
        if(strcmp(option, parser.argv[option_index]) != 0)
            continue;

        return option_index + 1;
    }
    
    return ARGPARSE_NOT_FOUND;
}
