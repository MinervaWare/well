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
 * @docgen: project
 * @brief: argument parsing library for c
 * @name: argparse
 *
 * @embed structure: ArgparseParser
 *
 * @embed function: argparse_init
 * @embed function: argparse_add_argument
 * @embed function: argparse_add_option
 * @embed function: argparse_add_repeatable_option
 * @embed function: argparse_error
 * @embed function: argparse_get_argument
 * @embed function: argparse_get_option_parameter
 * @embed function: argparse_free
 * @embed function: argparse_option_exists
 * @embed function: argparse_count_arguments
 * @embed function: argparse_unrecognized_options
 * @embed function: argparse_parameter_count
 * @embed function: argparse_repeatable_parameter_count
 * @embed function: argparse_argument_count
 *
 * @embed macro_function: argparse_option_parameter_iter
 * @embed macro_function: argparse_argument_variable_iter
 * @embed macro_function: argparse_repeatable_option_iter
 *
 * @description
 * @This is a basic argument parsing library for C. It is intended to be used to
 * @create command line applications for UNIX operating systems, and thus supports
 * @UNIX style options. However, in the future, it will be possible to use other
 * @argument styles for other platforms.
 * @description
 *
 * @reference: cware(cware)
*/

#ifndef CWARE_ARGPARSE_H
#define CWARE_ARGPARSE_H

#define CWARE_ARGPARSE_VERSION  "1.0.1"

/* Possibilities for variable option parameters */
#define ARGPARSE_FLAG           0
#define ARGPARSE_NOT_FOUND      -1
#define ARGPARSE_VARIABLE       -2
#define ARGPARSE_VARIABLE_ONE   -3

/* Iterator macros */

/*
 * @docgen: macro_function
 * @brief: iterate over all parameters of an option
 * @name: argparse_option_parameter_iter
 *
 * @param parser: the parser to iterate through
 * @param option: the option to iterate over the parameters of
 * @param index: the index variable
*/
#define argparse_option_parameter_iter(parser, option, index)           \
    for(index = argparse_option_parameter_start(parser, option);        \
        argparse_option_parameter_stop(parser, option, index) != 1;     \
        index++)

/*
 * @docgen: macro_function
 * @brief: iterate over all variable arguments
 * @name: argparse_argument_variable_iter
 *
 * @param parser: the parser to iterate through
 * @param index: the index variable
*/
#define argparse_argument_variable_iter(parser, index)              \
    for(index = argparse_argument_variable_start(parser);           \
            index != ARGPARSE_NOT_FOUND;                            \
            index = argparse_argument_variable_next(parser, index))

/*
 * @docgen: macro_function
 * @brief: iterate over all occurrences of a repeatable option
 * @name: argparse_repeatable_option_iter
 *
 * @param parser: the parser to iterate through
 * @param option: the option to iterate over the parameters of
 * @param index: the index variable
*/
#define argparse_repeatable_option_iter(parser, option, index)         \
    for(index = argparse_repeatable_option_start(parser, "-i");        \
        index != ARGPARSE_NOT_FOUND;                                   \
        index = argparse_repeatable_option_next(parser, "-i", index))

/* Utility macros */
#define argparse_get_index(parser, index) \
    ((parser).argv[index])

#define argparse_variable_arguments(parser) \
    ((parser).variable_arguments = 1)

/*
 * @docgen: structure
 * @brief: the container for argument parsing information
 * @name: ArgparseParser
 *
 * @field argc: the argument count
 * @type: int
 *
 * @field argv: the argument vector
 * @type: char **
 *
 * @field options: the option array
 * @type: struct ArgparseOptions *
 *
 * @field arguments: the arguments array
 * @type: struct ArgparseArguments *
 *
 * @field variable_arguments: whether or not this program accepts variable arguments
 * @type: int
 *
 * @field name: the name of the program
 * @type: const char *
*/
struct ArgparseParser {
    int argc;
    char **argv;

    struct ArgparseOptions *options;
    struct ArgparseArguments *arguments;

    /* Program configuration */
    int variable_arguments;
    const char *name;
};

/*
 * @docgen: function
 * @brief: initialize a new argument parser
 * @name: argparse_init
 *
 * @description
 * @Initialize a new argument parser with the provided argc and argv.
 * @description
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    struct ArgparseParser parser = argparse_init(argc, argv);
 * @    
 * @    return 0;
 * @}
 * @example
 *
 * @error: argc is negative
 * @error: argv is NULL
 * @error: name is NULL
 *
 * @param name: the name of the program
 * @type: const char *
 *
 * @param argc: the argument count
 * @type: int
 *
 * @param argv: the argument vector
 * @type: char **
 *
 * @return: a new argument parser
 * @type: struct ArgparseParser
*/
struct ArgparseParser argparse_init(const char *name, int argc, char **argv);

/*
 * @docgen: function
 * @brief: add a new argument to the argument parser
 * @name: argparse_add_argument
 *
 * @description
 * @Add a new positional argument to the argument parser of a certain name.
 * @Adding a new argument will require the program to accept a new argument
 * @to run correctly.
 * @description
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    struct ArgparseParser parser = argparse_init(argc, argv);
 * @
 * @    argparse_add_argument(&parser, "argument_a");
 * @    argparse_add_argument(&parser, "argument_b");
 * @    
 * @    return 0;
 * @}
 * @example
 *
 * @error: name is NULL
 * @error: parser is NULL
 * @error: parser->arguments is NULL
 * @error: name is already the name of an argument
 *
 * @param parser: the argument parser to add the argument to
 * @type: struct ArgparseParser *
 *
 * @param name: the name of the argument
 * @type: const char *
*/
void argparse_add_argument(struct ArgparseParser *parser, const char *name);

/*
 * @docgen: function
 * @brief: add a new option to the argument parser
 * @name: argparse_add_option
 *
 * @description
 * @Add a new option that the program can accept to the argument parser.
 * @The option can have both a name, and an alternative name (which emulates
 * @UNIX-like longform and shortform names) which can also be used as the option.
 * @
 * @The number of parameters must be a positive number, except for in the special
 * @cases that either ARGPARSE_VARIABLE, or ARGPARSE_VARIABLE_ONE, which should
 * @be negative integers. In cases like Windows, where switches / flags do not
 * @have alternative forms, the alt_name parameter can be NULL, but under no
 * @circumstances can the `name` parameter be NULL.
 * @description
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    struct ArgparseParser parser = argparse_init(argc, argv);
 * @
 * @    argparse_add_option(&parser, "-f", "--foo", 2);
 * @    argparse_add_option(&parser, "-b", "--bar", 1);
 * @    
 * @    return 0;
 * @}
 * @example
 *
 * @error: name is NULL
 * @error: parser is NULL
 * @error: parser->options is NULL
 * @error: parser->options->length is negative
 * @error: parameters is a non-special negative integer
 * @error: either of the names collide with themselves, or with any existing options
 *
 * @param parser: the argument parser to add the option to
 * @type: struct ArgparseParser *
 *
 * @param name: the name of the option
 * @type: const char *
 *
 * @param alt_name: the alternative name of the option
 * @type: const char *
 *
 * @param parameters: the number of parameters the option accepts
 * @type: int
*/
void argparse_add_option(struct ArgparseParser *parser, const char *name,
                         const char *alt_name, int parameters);

/*
 * @docgen: function
 * @brief: add a new repeatable option to the argument parser
 * @name: argparse_add_repeatable_option
 *
 * @description
 * @Add a new option that the program can accept to the argument parser.
 * @The option can have both a name, and an alternative name (which emulates
 * @UNIX-like longform and shortform names) which can also be used as the option.
 * @
 * @Unlike argparse_add_option, the option is repeatable. This allows you to
 * @pass the same option multiple times to the program. As an example, the options
 * @-L and -I in GCC. All repeatable options can only accept one parameter.
 * @description
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    struct ArgparseParser parser = argparse_init(argc, argv);
 * @
 * @    argparse_add_repeatable_option(&parser, "-f", "--foo");
 * @    argparse_add_repeatable_option(&parser, "-b", "--bar");
 * @    
 * @    return 0;
 * @}
 * @example
 *
 * @error: name is NULL
 * @error: parser is NULL
 * @error: parser->options is NULL
 * @error: either of the names collide with themselves, or with any existing options
 *
 * @param parser: the argument parser to add the option to
 * @type: struct ArgparseParser *
 *
 * @param name: the name of the option
 * @type: const char *
 *
 * @param alt_name: the alternative name of the option
 * @type: const char *
*/
void argparse_add_repeatable_option(struct ArgparseParser *parser, const char *name,
                                    const char *alt_name);

/*
 * @docgen: function
 * @brief: perform error checks on the argument parser
 * @name: argparse_error
 *
 * @description
 * @Perform error checks on the argv of a program, and raise an error to the
 * @stderr and exit the program if there is one. An error can be caused by:
 * @    - Too many, or too few positional arguments
 * @    - Too many, or too few parameters to an option
 * @    - An unknown option is found
 * @
 * @This function is a wrapper around the existing error checking functions.
 * @description
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    struct ArgparseParser parser = argparse_init(argc, argv);
 * @
 * @    argparse_add_option(&parser, "-f", "--foo", 2);
 * @    argparse_add_option(&parser, "-b", "--bar", 1);
 * @
 * @    argparse_error(parser);
 * @    
 * @    return 0;
 * @}
 * @example
 *
 * @param parser: the parser to perform error checking on
 * @type: struct ArgparseParser
*/
void argparse_error(struct ArgparseParser parser);











/*
 * Functions involving argument extraction are beyond this point!
*/

/*
 * @docgen: function
 * @brief: determine if an option exists in the argv
 * @name: argparse_option_exists
 *
 * @description
 * @This function will determine if an option exists in the argv. This function
 * @is simply a wrapper around argparse_option_argv_index(cware), with a more
 * @friendly interface.
 * @description
 *
 * @param parser: the parser to check
 * @type: struct ArgparseParser
 *
 * @param option: the option to find
 * @type: const char *
 *
 * @return: 1 if the option exists within the argv, and 0 if it does not
 * @type: int
*/
int argparse_option_exists(struct ArgparseParser parser, const char *option);

/*
 * @docgen: function
 * @brief: extract an argument from the parser
 * @name: argparse_get_argument
 *
 * @description
 * @This function will extract the argument in the argv represented by the
 * @argument provided.
 * @description
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    char *argument = NULL;
 * @    struct ArgparseParser parser = argparse_init("prog", argc, argv);
 * @    
 * @    // Program accepts three arguments
 * @    argparse_add_argument(&parser, "foo");
 * @    argparse_add_argument(&parser, "bar");
 * @    argparse_add_argument(&parser, "baz");
 * @
 * @    // Perform error checking
 * @    argparse_error(&parser);
 * @
 * @    // Get the 2nd positional argument (represented by 'bar')
 * @    argument = argparse_get_argument(parser, "bar");
 * @
 * @    return 0;
 * @}
 * @example
 *
 * @error: argument is NULL
 * @error: parser.name is NULL
 * @error: parser.argv is NULL
 * @error: parser.arguments is NULL
 * @error: parser.arguments->contents is NULL
 * @error: parser.arguments->length is negative
 * @error: parser.argc is negative
 *
 * @return: the argument assigned to the name
 * @type: char *
 *
 * @param parser: the parser to get an argument from
 * @type: struct ArgparseParser
 *
 * @param argument: the name of the argument to retrieve
 * @type: const char *
*/
char *argparse_get_argument(struct ArgparseParser parser, const char *argument);

/*
 * @docgen: function
 * @brief: get the Nth parameter of an option
 * @name: argparse_get_option_parameter
 *
 * @description
 * @Retrieve the Nth parameter of an option. Will work for variable, or fixed-
 * @length options. N is 0-indexed.
 * @description
 *
 * @error: option is NULL
 * @error: parser.argv is NULL
 *
 * @param parser: the parser to get the option from
 * @type: struct ArgparseParser
 *
 * @param option: the option to get the parameter of
 * @type: const char *
 *
 * @param index: the parameter to get
 * @type: int
 *
 * @return: the parameter at that index
 * @type: char *
*/
char *argparse_get_option_parameter(struct ArgparseParser parser,
                                    const char *option, int index);


/*
 * @docgen: function
 * @brief: start iteration of parameter options
 * @name: argparse_option_parameter_start
 *
 * @description
 * @Begin iteration over an option's parameters. This will work for both options
 * @with variable parameters, as well as a fixed number of parameters. All this
 * @function will do is return the index of the first parameter option. Do note,
 * @however, that this function will not consider alt_names when searching for
 * @the option to iterate over. You must have separate logic for both names.
 * @description
 *
 * @notes
 * @This function, along with all functions in extractors.c, will ASSUME that
 * @error checking has BEEN DONE. Make sure this is done before this function
 * @is invoked.
 * @notes
 *
 * @error: parser.argv is NULL
 * @error: option is NULL
 * @error: the option is not in the argv
 * @error: the option is not one that the program expects
 * @error: the option accepts no parameters
 *
 * @param parser: the parser the option is inside
 * @type: struct ArgparseParser
 *
 * @param option: the option to iterate over
 * @type: const char *
 *
 * @return: the index of the first parameter
 * @type: int
*/
int argparse_option_parameter_start(struct ArgparseParser parser, const char *option);

/*
 * @docgen: function
 * @brief: determine if iteration of option parameters should be stopped
 * @name: argparse_option_parameter_stop
 *
 * @description
 * @Determine if the iteration over an option's parameters should be stopped.
 * @Simply checks if the current index is in the range of the number of
 * @parameters the option has in the argv.
 * @description
 *
 * @notes
 * @This function, along with all functions in extractors.c, will ASSUME that
 * @error checking has BEEN DONE. Make sure this is done before this function
 * @is invoked.
 * @
 * @It should also be noted that this function should NOT be called out of
 * @conjunction with argparse_option_parameter_start. This can lead to errors
 * @due to argparse_option_parameter_start performing error checks that this
 * @function does not.
 * @notes
 *
 * @error: option is NULL
 * @error: index is negative
 *
 * @param parser: the parser the option is inside
 * @type: struct ArgparseParser
 *
 * @param option: the option to iterate over
 * @type: const char *
 *
 * @param index: the current index in the argv
 * @type: const char *
 *
 * @return: 1 if iteration should stop, and 0 if it should continue
 * @type: int
*/
int argparse_option_parameter_stop(struct ArgparseParser parser, const char *option, int index);

/*
 * @docgen: function
 * @brief: begin iteration of variable arguments
 * @name: argparse_argument_variable_start
 *
 * @description
 * @This function will determine the location of the first variadic argument
 * @in the argv.
 * @description
 *
 * @notes
 * @This function, along with all functions in extractors.c, will ASSUME that
 * @error checking has BEEN DONE. Make sure this is done before this function
 * @is invoked.
 * @notes
 *
 * @error: parser.argv is NULL
 * @error: parser.arguments is NULL
 * @error: parser.arguments->contents is NULL
 * @error: parser.argc is negative
 * @error: parser.arguments->length is negative
 * @error: parser.variable_arguments is not 1
 *
 * @param parser: the parser the option is inside
 * @type: struct ArgparseParser
 *
 * @return: the index of the first variable argument, or -1 if there is none
 * @type: int
*/
int argparse_argument_variable_start(struct ArgparseParser parser);


/*
 * @docgen: function
 * @brief: find the next variable argument
 * @name: argparse_argument_variable_next
 *
 * @description
 * @This function will determine the location of the next variadic argument
 * @in the argv based off of the index of the previous one.
 * @description
 *
 * @notes
 * @This function, along with all functions in extractors.c, will ASSUME that
 * @error checking has BEEN DONE. Make sure this is done before this function
 * @is invoked.
 * @
 * @It should also be noted that this function should NOT be called out of
 * @conjunction with argparse_argument_variable_start. This can lead to errors
 * @due to argparse_argument_variable_start performing error checks that this
 * @function does not.
 * @notes
 *
 * @error: index is outside the bounds of the argc
 *
 * @param parser: the parser the option is inside
 * @type: struct ArgparseParser
 *
 * @param index: the index of the previous argument
 * @type: int
 *
 * @return: the index of the first variable argument, or -1 if there is none
 * @type: int
*/
int argparse_argument_variable_next(struct ArgparseParser parser, int index);

/*
 * @docgen: function
 * @brief: begin iteration over repeatable options
 * @name: argparse_repeatable_option_start
 *
 * @description
 * @Begin iteration over repeatable options by finding the first repeatable
 * @option in the argv.
 * @description
 *
 * @notes
 * @This function, along with all functions in extractors.c, will ASSUME that
 * @error checking has BEEN DONE. Make sure this is done before this function
 * @is invoked.
 * @notes
 *
 * @error: the option is not in the argv
 * @error: the option is not one that the program expects
 * @error: option is NULL
 * @error: parser.argv is NULL
 * @error: parser.argc is negative
 *
 * @param parser: the parser to iterate through
 * @type: struct ArgparseParser
 *
 * @param option: the repeatable option to search for
 * @type: const char *
 *
 * @return: the index of the first occurrence of the repeatable option, or ARGPARSE_NOT_FOUND
 * @type: int
*/
int argparse_repeatable_option_start(struct ArgparseParser parser, const char *option);

/*
 * @docgen: function
 * @brief: begin iteration over repeatable options
 * @name: argparse_repeatable_option_start
 *
 * @description
 * @Continue iteration by finding the next repeatable option in the argv
 * @of the parser.
 * @description
 *
 * @notes
 * @This function, along with all functions in extractors.c, will ASSUME that
 * @error checking has BEEN DONE. Make sure this is done before this function
 * @is invoked.
 * @
 * @It should also be noted that this function should NOT be called out of
 * @conjunction with argparse_repeatable_option_start. This can lead to errors
 * @due to argparse_repeatable_option_start performing error checks that this
 * @function does not.
 * @notes
 *
 * @error: the option is not in the argv
 * @error: the option is not one that the program expects
 * @error: option is NULL
 * @error: parser.argv is NULL
 * @error: parser.argc is negative
 *
 * @param parser: the parser to iterate through
 * @type: struct ArgparseParser
 *
 * @param option: the repeatable option to search for
 * @type: const char *
 *
 * @param index: the previous index left off at
 * @type: int
 *
 * @return: the index of the first occurrence of the repeatable option, or ARGPARSE_NOT_FOUND
 * @type: int
*/
int argparse_repeatable_option_next(struct ArgparseParser parser, const char *option, int index);

/* 
 * @docgen: function
 * @brief: release an argument parser from memory
 * @name: argparse_free
 * 
 * @description
 * @Release an argument parser from memory. Specifically, it releases the
 * @arguments and options array from the parser.
 * @description
 *
 * @error: parser.options is NULL
 * @error: parser.arguments is NULL
 *
 * @param parser: the argument parser to release from memory
 * @type: struct ArgparseParser
 *
 * @example
 * @#include "argparse.h"
 * @
 * @int main(int argc, char **argv) {
 * @    struct ArgparseParser parser = argparse_init("prog", argc, argv);
 * @
 * @    // Add arguments to the argument array
 * @    argparse_add_argument(&parser, "foo");
 * @    argparse_add_argument(&parser, "bar");
 * @    argparse_add_argument(&parser, "baz");
 * @
 * @    // Add options to the option array
 * @    argparse_add_option(&parser, "--opt", "-o", 1);
 * @
 * @    // Release the argument parser from memory
 * @    argparse_free(parser);
 * @
 * @    return 0;
 * @}
 * @example
*/
void argparse_free(struct ArgparseParser parser);

/*
 * @docgen: function
 * @brief: detect unrecognized options
 * @name: argparse_unrecognized_options
 *
 * @description
 * @Perform error checks for detecting unrecognized options in the program's
 * @argv.
 * @description
 *
 * @error: parser.argv is NULL
 * @error: parser.name is NULL
 * @error: parser.argc is negative
 *
 * @param parser: the argument parser
 * @type: struct ArgparseParser
*/
void argparse_unrecognized_options(struct ArgparseParser parser);

/*
 * @docgen: function
 * @brief: detect option parameter counts
 * @name: argparse_parameter_count
 *
 * @description
 * @Perform error checks for detecting the number of parameters that an
 * @option has.
 * @description
 *
 * @error: parser.argv is NULL
 * @error: parser.name is NULL
 * @error: parser.options is NULL
 * @error: parser.options->contents is NULL
 * @error: parser.argc is negative
 *
 * @param parser: the argument parser
 * @type: struct ArgparseParser
*/
void argparse_parameter_count(struct ArgparseParser parser);

/*
 * @docgen: function
 * @brief: detect repeatable option parameters
 * @name: argparse_repeatable_parameter_count
 *
 * @description
 * @Perform error checks when detecting the number of parameters that a
 * @repeatable option has.
 * @description
 *
 * @error: parser.argv is NULL
 * @error: parser.name is NULL
 * @error: parser.options is NULL
 * @error: parser.options->contents is NULL
 * @error: parser.argc is negative
 *
 * @param parser: the argument parser
 * @type: struct ArgparseParser
*/
void argparse_repeatable_parameter_count(struct ArgparseParser parser);

/*
 * @docgen: function
 * @brief: detect too few or too mnay arguments
 * @name: argparse_argument_count
 *
 * @description
 * @Perform error checks for verifying the number of arguments in the
 * @program's argv match the expected number of positional arguments.
 * @description
 *
 * @error: parser.argv is NULL
 * @error: parser.name is NULL
 * @error: parser.arguments is NULL
 * @error: parser.argc is negative
 * @error: parser.variable_arguments is negative
 * @error: parser.arguments->length is negative
 *
 * @param parser: the argument parser
 * @type: struct ArgparseParser
*/
void argparse_argument_count(struct ArgparseParser parser);

/*
 * @docgen: function
 * @brief: count the number of arguments in the argv
 * @name: argparse_count_arguments
 *
 * @description
 * @Count the number of arguments, both required and variable, in the argv
 * @of the program.
 * @description
 *
 * @error: parser.argc is negative
 * @error: parser.argv is NULL
 *
 * @param parser: the argument parser
 * @type: struct ArgparseParser
 *
 * @return: the number of arguments
 * @type: int
*/
int argparse_count_arguments(struct ArgparseParser parser);

#endif
