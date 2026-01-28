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
 * This file contains implementations of the operations from argparse.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "ap_inter.h"

#define error_if_same_option(option, name, func)                             \
do {                                                                         \
    if((option) == NULL)                                                     \
        break;                                                               \
                                                                             \
    if((name) == NULL)                                                       \
        break;                                                               \
                                                                             \
    if(strcmp((option), (name)) == 0) {                                      \
        fprintf(stderr, "%s: option '%s' already exists\n", (func), (name)); \
        abort();                                                             \
    }                                                                        \
} while(0)

struct ArgparseParser argparse_init(const char *name, int argc, char **argv) {
    struct ArgparseParser parser;

    INIT_VARIABLE(parser);

    liberror_is_null(argparse_init, name);
    liberror_is_null(argparse_init, argv);
    liberror_is_negative(argparse_init, argc);

    parser.argc = argc;
    parser.argv = argv;

    parser.options = carray_init(parser.options, OPTION);
    parser.arguments = carray_init(parser.arguments, ARGUMENT);
    parser.name = name;

    return parser;
}

void argparse_add_argument(struct ArgparseParser *parser, const char *name) {
    int index = 0;
    struct ArgparseArgument argument;

    INIT_VARIABLE(argument);

    liberror_is_null(argparse_add_argument, name);
    liberror_is_null(argparse_add_argument, parser);
    liberror_is_null(argparse_add_argument, parser->arguments);
    liberror_is_null(argparse_add_argument, parser->arguments->contents);

    /* Detect if the argument already exists */
    for(index = 0; index < carray_length(parser->arguments); index++) {
        if(strcmp(parser->arguments->contents[index].name, name) != 0)
            continue;

        fprintf(stderr, "argparse_add_argument: attempt to add positional argument '%s' that"
                " already exists\n", name);
        abort();
    }

    argument.name = name;

    carray_append(parser->arguments, argument, ARGUMENT);
}

void argparse_add_option(struct ArgparseParser *parser, const char *name,
                         const char *alt_name, int parameters) {
    int index = 0;
    struct ArgparseOption option;

    liberror_is_null(argparse_add_option, name);
    liberror_is_null(argparse_add_option, parser);
    liberror_is_null(argparse_add_option, parser->options);
    liberror_is_null(argparse_add_option, parser->options->contents);
    liberror_is_negative(argparse_add_option, parser->options->length);

    /* Value must not be negative, and if it is, it cannot be a special value */
    if(parameters < 0) {
        if(parameters != ARGPARSE_VARIABLE && parameters != ARGPARSE_VARIABLE_ONE) {
            fprintf(stderr, "argparse_add_option: 'parameters' cannot be negative (%i)\n", parameters);
            abort();
        }
    }

    /* Make sure name and alt_name do not collide */
    if(alt_name != NULL && (strcmp(name, alt_name) == 0)) {
        fprintf(stderr, "%s", "argparse_add_option: name and alt_name parameters collide\n");
        abort();
    }

    /* This option is not allowed to exist somewhere else
     * TODO: When adding prefix options, add logic to detect
     * overlap between option names and prefixes */
    for(index = 0; index < carray_length(parser->options); index++) {
        struct ArgparseOption parser_option = parser->options->contents[index];

        error_if_same_option(parser_option.name, name, "argparse_add_option");
        error_if_same_option(parser_option.alternate_name, name, "argparse_add_option");
        error_if_same_option(parser_option.name, alt_name, "argparse_add_option");
        error_if_same_option(parser_option.alternate_name, alt_name, "argparse_add_option");
    }

    INIT_VARIABLE(option);

    option.name = name;
    option.alternate_name = alt_name;
    option.parameters = parameters;

    carray_append(parser->options, option, OPTION);
}

void argparse_add_repeatable_option(struct ArgparseParser *parser, const char *name,
                                    const char *alt_name) {
    int index = 0;
    struct ArgparseOption option;

    liberror_is_null(argparse_add_repeatable_option, name);
    liberror_is_null(argparse_add_repeatable_option, parser);
    liberror_is_null(argparse_add_repeatable_option, parser->options);
    liberror_is_null(argparse_add_repeatable_option, parser->options->contents);

    INIT_VARIABLE(option);

    /* Make sure name and alt_name do not collide */
    if(alt_name != NULL && (strcmp(name, alt_name) == 0)) {
        fprintf(stderr, "%s", "argparse_add_option: name and alt_name parameters collide\n");
        abort();
    }

    /* This option is not allowed to exist somewhere else
     * TODO: When adding prefix options, add logic to detect
     * overlap between option names and prefixes */
    for(index = 0; index < carray_length(parser->options); index++) {
        struct ArgparseOption parser_option = parser->options->contents[index];

        error_if_same_option(parser_option.name, name, "argparse_add_repeatable_option");
        error_if_same_option(parser_option.alternate_name, name, "argparse_add_repeatable_option");
        error_if_same_option(parser_option.name, alt_name, "argparse_add_repeatable_option");
        error_if_same_option(parser_option.alternate_name, alt_name, "argparse_add_repeatable_option");
    }

    option.name = name;
    option.alternate_name = alt_name;
    option.parameters = 1;
    option.type = OPTION_TYPE_REPEAT;

    carray_append(parser->options, option, OPTION);
}

int argparse_count_arguments(struct ArgparseParser parser) {
    int index = 0;
    int counted = 0;

    liberror_is_null(argparse_count_arguments, parser.argv);
    liberror_is_negative(argparse_count_arguments, parser.argc);

    for(index = 1; index < parser.argc; index++) {
        const char *argument = parser.argv[index];
        
        /* Skip past this option! (We can assume that the
         * number of indices skipped is correct because of the
         * parameter_count function being called beforehand. */
        if(argparse_is_option(parser, argument) == 1) {
            int parameters = argparse_option_parser_parameters(parser, argument);

            /* Skip past a variable number */
            if(parameters == ARGPARSE_VARIABLE || parameters == ARGPARSE_VARIABLE_ONE) {
                index += argparse_option_argv_parameters(parser, argument);

                continue;
            }

            index += parameters;

            continue;
        }

        counted++;
    }

    return counted;
}

void argparse_free(struct ArgparseParser parser) {
    liberror_is_null(argparse_free, parser.options);
    liberror_is_null(argparse_free, parser.arguments);
    liberror_is_negative(argparse_free, parser.options->length);
    liberror_is_negative(argparse_free, parser.arguments->length);

    carray_free(parser.options, OPTION);
    carray_free(parser.arguments, ARGUMENT);
}



/*
 * Error checking logic
*/

void argparse_unrecognized_options(struct ArgparseParser parser) {
    int index = 0;

    liberror_is_null(unrecognized_options, parser.argv);
    liberror_is_null(unrecognized_options, parser.name);
    liberror_is_negative(unrecognized_options, parser.argc);

    for(index = 1; index < parser.argc; index++) {
        const char *argument = parser.argv[index];

        if(argparse_is_option(parser, argument) == 0)
            continue;

        /* This is a valid option that this program expects; leave it alone */
        if(argparse_option_parser_index(parser, argument) != ARGPARSE_NOT_FOUND)
            continue;

        fprintf(stderr, "%s: unrecognized option '%s'\n", parser.name, argument);
        fprintf(stderr, "Try '%s --help' for more information.\n", parser.name);

        exit(EXIT_FAILURE);
    }
}

void argparse_parameter_count(struct ArgparseParser parser) {
    int index = 0;

    liberror_is_null(argparse_parameter_count, parser.argv);
    liberror_is_null(argparse_parameter_count, parser.name);
    liberror_is_null(argparse_parameter_count, parser.options);
    liberror_is_null(argparse_parameter_count, parser.options->contents);
    liberror_is_negative(argparse_parameter_count, parser.argc);

    for(index = 1; index < parser.argc; index++) {
        int argv_parameters = 0;
        int parser_parameters = 0;
        const char *argument = parser.argv[index];

        if(argparse_is_option(parser, argument) == 0)
            continue;

        /* For reference; no need to check if this is a parameter that the
         * program accepts here, as that was verified before this function
         * was called, or rather, it SHOULD be. :^) */
        /*
        if(argparse_option_parser_index(parser, argument) != ARGPARSE_NOT_FOUND)
            continue;
        */

        /* Ignore non-regular parameters; that is checked later */
        if(parser.options->contents[argparse_option_parser_index(parser, argument)].type != OPTION_TYPE_REGULAR)
            continue;

        argv_parameters = argparse_option_argv_parameters(parser, argument);
        parser_parameters = argparse_option_parser_parameters(parser, argument);

        /* Undecidably variable options do not need to be checked, because
         * because they are *variable* */
        if(parser_parameters == ARGPARSE_VARIABLE)
            continue;

        /* Requires at least one parameter, but does not care about any
         * further ones. */
        if(parser_parameters == ARGPARSE_VARIABLE_ONE) {
            if(argv_parameters != 0)
                continue;

            fprintf(stderr, "docgen: option '%s' expects at least one parameter\n", argument);
            fprintf(stderr, "Try '%s --help' for more information\n", parser.name);
            exit(EXIT_FAILURE);
        }

        /* Strict parameter requirements. Doing >= because the function to
         * compute argv parameters does not stop when the needed amount is
         * met. It does not matter anyway-- all that matters is that there is
         * enough. */
        if(argv_parameters >= parser_parameters)
            continue;

        fprintf(stderr, "docgen: option '%s' expects %i parameter(s), got %i\n",
                argument, parser_parameters, argv_parameters);
        fprintf(stderr, "Try '%s --help' for more information\n", parser.name);
        exit(EXIT_FAILURE);
    }
}

void argparse_repeatable_parameter_count(struct ArgparseParser parser) {
    int index = 0;

    liberror_is_null(argparse_parameter_count, parser.argv);
    liberror_is_null(argparse_parameter_count, parser.name);
    liberror_is_null(argparse_parameter_count, parser.options);
    liberror_is_null(argparse_parameter_count, parser.options->contents);
    liberror_is_negative(argparse_parameter_count, parser.argc);

    for(index = 1; index < parser.argc; index++) {
        int parser_index = 0;
        const char *argument = parser.argv[index];

        /* Ignore non-options */
        if(argparse_is_option(parser, argument) == 0)
            continue;

        parser_index = argparse_option_parser_index(parser, argument);

        /* Type of the option must be a repeatable one to be checked
           by this function */
        if(parser.options->contents[parser_index].type != OPTION_TYPE_REPEAT)
            continue;

        /* A valid repeatable option will have a non-option argument
           that is following it. */
        if(index + 1 < parser.argc && argparse_is_option(parser, parser.argv[index + 1]) == 0)
            continue;

        /* Uh oh, bad option. Release the hounds. */
        fprintf(stderr, "%s: repeatable option '%s' expects 1 option, got 0\n", parser.name,
                parser.argv[index]);
        exit(EXIT_FAILURE);
    }
}

void argparse_argument_count(struct ArgparseParser parser) {
    int index = 0;
    int arguments = 0;

    liberror_is_null(argument_count, parser.argv);
    liberror_is_null(argument_count, parser.name);
    liberror_is_null(argument_count, parser.arguments);
    liberror_is_negative(argument_count, parser.argc);
    liberror_is_negative(argument_count, parser.variable_arguments);
    liberror_is_negative(argument_count, parser.arguments->length);

    /* This field cannot be anything but 1 or 0 */
    if(parser.variable_arguments > 1) {
        fprintf(stderr, "argument_count: parser.variable_arguments must be 0 or 1 (is %i)\n",
                parser.variable_arguments);
        abort();
    }

    for(index = 1; index < parser.argc; index++) {
        const char *argument = parser.argv[index];

        /* Skip past this option! (We can assume that the
         * number of indices skipped is correct because of the
         * parameter_count function being called beforehand. */
        if(argparse_is_option(parser, argument) == 1) {
            int parameters = argparse_option_parser_parameters(parser, argument);

            /* Skip past a variable number */
            if(parameters == ARGPARSE_VARIABLE || parameters == ARGPARSE_VARIABLE_ONE) {
                index += argparse_option_argv_parameters(parser, argument);

                continue;
            }

            index += parameters;

            continue;
        }

        arguments++; 
    }

    /* Variable arguments only require a minimum number of arguments */
    if(parser.variable_arguments == 1) {
        if(arguments >= carray_length(parser.arguments))
            return;

        fprintf(stderr, "%s: expected at least %i argument(s), got %i\n", parser.name,
                carray_length(parser.arguments), arguments);
        fprintf(stderr, "Try '%s --help' for more information.\n", parser.name);

        exit(EXIT_FAILURE);
    }

    if(arguments == carray_length(parser.arguments))
        return;

    fprintf(stderr, "%s: expected %i argument(s), got %i\n", parser.name,
            carray_length(parser.arguments), arguments);
    fprintf(stderr, "Try '%s --help' for more information.\n", parser.name);
    exit(EXIT_FAILURE);
}

void argparse_error(struct ArgparseParser parser) {
    argparse_unrecognized_options(parser);
    argparse_parameter_count(parser);
    argparse_repeatable_parameter_count(parser);
    argparse_argument_count(parser);
}
