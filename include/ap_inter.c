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
 * This file contains implementations of the operations from
 * ap_inter.h
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "ap_inter.h"

int argparse_is_option(struct ArgparseParser parser, const char *argument) {
    int hyphon_index = -1;

    if(strlen(argument) < 2)
        return 0;

    if(argument[0] == '-') {
        hyphon_index = 0;

        if(argument[1] == '-')
            hyphon_index = 1;
    }


    /* Argument must start with a hyphon */
    if(hyphon_index == -1)
        return 0;

    
    /*
     * Make sure we do not index out of bounds after the hyphons
     *
     * --f <-- This is a valid option
     *  length = 3
     *  hyphon_index: 1
     *
     * -- <-- This, however, is not a valid option
     *  length = 2
     *  hyphon_index: 1
     *
     *  If ((hyphon_index + 1) == length), then we can be sure
     *  that indexing at argument[hyphon_index + 1] will end up
     *  indexing out of the bounds of the string, and thus is not
     *  a valid option.
     *
     *  Same applies for options of the form (-a) too.
     *
     *  -a <-- This is a valid option
     *  length = 2
     *  hyphon_index = 0
     *
     *  ((hyphon_index + 1) == length) #=> False
    */
    if(hyphon_index + 1 == strlen(argument))
        return 0;

    /* First character after the hyphon must be an alphabetical
     * character or an underscore. */
    if(isalpha(argument[hyphon_index + 1]) == 0 &&
       argument[hyphon_index + 1] != '_')
        return 0;

    return 1;
}

int argparse_option_argv_parameters(struct ArgparseParser parser,
                                    const char *option) {
    int index = 0;
    int parameters = 0;

    /* Locate the option; if index == option.argc, then that means the option
     * was NOT found. For example,
     *
     * "foo", "--bar", "baz"
     * i: 0   i: 1     i: 2
     *
     * argc: 3
    */
    for(index = 0; index < parser.argc; index++) {
        const char *argument = parser.argv[index];

        if(strcmp(option, argument) != 0)
            continue;

        break;
    }

    if(index == parser.argc) {
        fprintf(stderr, "argparse_option_argv_parameters: attempt to find parameters of option '%s'"
                " which does not appear in parser argv.\n", option);
        exit(EXIT_FAILURE);
    }

    /* Count the number of parameters (start after the index
     * so we do not count the option as a parameter) */
    for(index = index + 1; index < parser.argc; index++) {
        const char *argument = parser.argv[index];

        /* Found another option-- stop now counting */
        if(argparse_is_option(parser, argument) == 1)
            break;

        parameters++;
    }   
        
    return parameters;
}

int argparse_option_parser_parameters(struct ArgparseParser parser,
                                      const char *option) {
    int index = argparse_option_parser_index(parser, option);

    if(index == ARGPARSE_NOT_FOUND) {
        fprintf(stderr, "argparse_option_parser_parameters: attempt to find parameters of option '%s'"
                " which does not appear in parser's expected options.\n", option);
        exit(EXIT_FAILURE);
    }

    return parser.options->contents[index].parameters;
}

int argparse_option_argv_index(struct ArgparseParser parser, const char *option) {
    int index = 1;

    for(index = 1; index < parser.argc; index++) {
        if(strcmp(parser.argv[index], option) != 0)
            continue;

        return index;
    }

    return ARGPARSE_NOT_FOUND;
}

int argparse_option_parser_index(struct ArgparseParser parser, const char *option) {
    int index = 1;

    for(index = 0; index < carray_length(parser.options); index++) {
        struct ArgparseOption option_structure = parser.options->contents[index];

        /* option_structure.alt_name CAN be NULL, so we need to do a bit of
         * extra checking before doing the comparison. If it's NULL, we
         * only need to consider the first name. */
        if(option_structure.alternate_name == NULL) {
            if(strcmp(option_structure.name, option) != 0)
                continue;
        } else {
            if(strcmp(option_structure.name, option) != 0 &&
               strcmp(option_structure.alternate_name, option) != 0)
                continue;
        }

        return index;
    }

    return ARGPARSE_NOT_FOUND;
}
