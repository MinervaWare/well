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
 * @docgen: category
 * @name: argparse-internal
 * @brief: internal argparse behavior intended for programmers
 *
 * @embed structure: ArgparseArguments
 * @embed structure: ArgparseOptions
 * @embed structure: ArgparseOption
 * @embed structure: ArgparseArgument
 *
 * @description
 * @This document intends to describe the internals of argparse, and provide
 * @documentation for internal structures and other things used by argparse
 * @internally. As such, if you are a user of argparse, it is recommended that
 * @you do not use any of these.
 * @
 * @The main setup of a program using argparse is separated into these main steps:
 * @    - Initialization of an argument parser
 * @    - Configuration of the argument parser
 * @    - Adding options, and arguments
 * @    - Displaying a help message
 * @    - Performing error checks
 * @    - Doing stuff with the arguments
 * @    - Releasing the argument parser
 * @
 * @Out of everything in this list, error handling is the one main part that is
 * @the most complicated. To perform error handling, argparse needs to know about
 * @the expected structure of your program. Specifically, it needs to know:
 * @    - How many positional arguments to expect
 * @    - What counts as an option, and how many parameters it takes
 * @    - Whether or not there can be a variable number of arguments
 * @description
*/

#ifndef CWARE_ARGPARSE_INTERNAL_H
#define CWARE_ARGPARSE_INTERNAL_H

/* Types of options */
#define OPTION_TYPE_REGULAR 0
#define OPTION_TYPE_REPEAT 1

/* Internal libraries-- change if resolving duplicate dependencies */
#include "carray/carray.h"
#include "liberror/liberror.h"

/* Data structure properties */
#define OPTION_TYPE struct ArgparseOption
#define OPTION_HEAP 1
#define OPTION_FREE(value)

#define ARGUMENT_TYPE struct ArgparseArgument
#define ARGUMENT_HEAP 1
#define ARGUMENT_FREE(value)

/* Useful macros */
#define INIT_VARIABLE(variable) \
    memset(&(variable), 0, sizeof((variable)))

/*
 * @docgen: structure
 * @brief: an array of arguments
 * @name: ArgparseArguments
 *
 * @field length: the length of the array
 * @type: int
 *
 * @field capacity: the maximum capacity of the array
 * @type: int
 *
 * @field contents: the array of arguments
 * @type: struct ArgparseArgument *
*/
struct ArgparseArguments {
    int length;
    int capacity;
    struct ArgparseArgument *contents;
};

/*
 * @docgen: structure
 * @brief: an array of options
 * @name: ArgparseOptions
 *
 * @field length: the length of the array
 * @type: int
 *
 * @field capacity: the maximum capacity of the array
 * @type: int
 *
 * @field contents: the array of options
 * @type: struct ArgparseOptions *
*/
struct ArgparseOptions {
    int length;
    int capacity;
    struct ArgparseOption *contents;
};

/*
 * @docgen: structure
 * @brief: a positional argument that the program expects.
 * @name: ArgparseArgument
 *
 * @field name: the name of the positional argument
 * @type: const char *
*/
struct ArgparseArgument {
    const char *name;
};

/*
 * @docgen: structure
 * @brief: an option that the program can accept
 * @name: ArgparseOption
 *
 * @field name: one possible name for the option
 * @type: const char *
 *
 * @field alternate_name: another possible name for the option
 * @type: const char *
 *
 * @field parameters: the number of parameters it will accept
 * @type: int
 *
 * @field type: the type of option
 * @type: int
*/
struct ArgparseOption {
    const char *name;
    const char *alternate_name;
    int parameters;
    int type;
};

/*
 * @docgen: function
 * @brief: determine if an argument is an option
 * @name: argparse_is_option
 * @include: argparse_internal.h
 *
 * @description
 * @Determine if an argument takes the form of an option. Currently, this
 * @function only does UNIX-style options, however in the future it will
 * @use the type of argument style requested by the user as the basis.
 * @description
 *
 * @notes
 * @This function is meant for internal use only. Although it can only be
 * @accessed through including the argparse_internal.h header, it is very
 * @much advised that you do not use this directly.
 * @notes
 *
 * @error: argument is NULL
 *
 * @return: 1 if the argument is an option, and 0 if its not an option
 * @type: int
 *
 * @param parser: the parser to check-- currently unused
 * @type: struct ArgparseParser
 *
 * @param argument: the argument to check for option-ness
 * @type: const char *
 *
 * @reference: cware(cware)
 * @reference: argparse(cware)
 * @reference: argparse_internal(cware)
*/
int argparse_is_option(struct ArgparseParser parser, const char *argument);

/*
 * @docgen: function
 * @brief: count the number of parameters passed to an option
 * @name: argparse_count_argv_parameters
 * @include: argparse_internal.h
 *
 * @description
 * @Count the number of parameters an option has provided to it *in* the
 * @argv, rather than how much it *should* have. This function counts all
 * @arguments after the occurrence of OPTION in the argv as a parameter,
 * @but functions which use this interface will only produce an error if
 * @the number returned by this functions is less than the expected number
 * @of parameters.
 * @description
 *
 * @notes
 * @This function is meant for internal use only. Although it can only be
 * @accessed through including the argparse_internal.h header, it is very
 * @much advised that you do not use this directly.
 * @notes
 *
 * @error: option is NULL
 * @error: parser.argv is NULL
 * @error: parser.argc is negative
 * @error: option does not exist in the argv
 *
 * @return: the number of real parameters
 * @type: int
 *
 * @param parser: the parser the option is apart of
 * @type: struct ArgparseParser
 *
 * @param option: the option to search for
 * @type: const char *
 *
 * @reference: cware(cware)
 * @reference: argparse(cware)
 * @reference: argparse_internal(cware)
*/
int argparse_option_argv_parameters(struct ArgparseParser parser,
                                   const char *option);

/*
 * @docgen: function
 * @brief: count the number of parameters that an option should have
 * @name: argparse_count_parser_parameters
 * @include: argparse_internal.h
 *
 * @description
 * @Calculate the number of paramters that an option should have, as opposed to
 * @how many it actually has in the argv of the program.
 * @description
 *
 * @notes
 * @This function is meant for internal use only. Although it can only be
 * @accessed through including the argparse_internal.h header, it is very
 * @much advised that you do not use this directly.
 * @notes
 *
 * @error: option is NULL
 * @error: parser.argv is NULL
 * @error: parser.argc is negative
 * @error: option does not exist in the argv
 *
 * @return: the number of real parameters
 * @type: int
 *
 * @param parser: the parser the option is apart of
 * @type: struct ArgparseParser
 *
 * @param option: the option to search for
 * @type: const char *
 *
 * @reference: cware(cware)
 * @reference: argparse(cware)
 * @reference: argparse_internal(cware)
*/
int argparse_option_parser_parameters(struct ArgparseParser parser,
                                     const char *option);

/*
 * @docgen: function
 * @brief: determine the index of an option in the argv of the parser
 * @name: argparse_option_argv_index
 *
 * @description
 * @Determine the index of an option in the argv of the parser. This function
 * @does not determine the location of the alternate name of the option, but
 * @rather just finds the option name given to it.
 * @description
 *
 * @notes
 * @This function is meant for internal use only. Although it can only be
 * @accessed through including the argparse_internal.h header, it is very
 * @much advised that you do not use this directly.
 * @notes
 *
 * @return: the index of the option in the argc, or ARGPARSE_NOT_FOUND
 * @type: int
 *
 * @error: option is NULL
 * @error: parser.argv is NULL
 * @error: parser.argc is negative
 * 
 * @param parser: the parser to search
 * @type: struct ArgparseParser
 *
 * @param option: the option to search for
 * @type: const char *
*/
int argparse_option_argv_index(struct ArgparseParser parser, const char *option);


/*
 * @docgen: function
 * @brief: determine the index of the option in the parser's option array
 * @name: argparse_option_parser_index
 *
 * @description
 * @Determine the index of an option in the option array of the parser. This
 * @function, unlike `argparse_option_real_index`, does consider alternative
 * @names, as well as main names when searching.
 * @description
 *
 * @notes
 * @This function is meant for internal use only. Although it can only be
 * @accessed through including the argparse_internal.h header, it is very
 * @much advised that you do not use this directly.
 * @notes
 *
 * @return: the index of the option in the parser array, or ARGPARSE_NOT_FOUND
 * @type: int
 *
 * @error: option is NULL
 * @error: parser.argv is NULL
 * @error: parser.argc is negative
 *
*/
int argparse_option_parser_index(struct ArgparseParser parser, const char *option);

#endif
