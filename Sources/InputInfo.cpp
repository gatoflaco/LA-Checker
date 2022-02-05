/* LA-Checker by Isaac Jung
Last updated 02/04/2022

|===========================================================================================================|
|   This file contains definitions for methods used to process input via an InputInfo class. Should the     |
| input format change, these methods can be updated accordingly.                                            |
|===========================================================================================================| 
*/

#include "InputInfo.h"
#include <sstream>
#include <iostream>
#include <algorithm>

/* SUB METHOD: process_input - reads from standard in to initialize program data
 * 
 * parameters:
 * - none
 * 
 * returns:
 * - code representing success/failure
*/
int InputInfo::process_input()
{
    printf("TAKING INPUT....\n\n");
    int ret = 0;
    std::string cur_line;

    // v2.0
    std::getline(std::cin, cur_line);
    try {
        trim(cur_line);
        if (cur_line.compare("v2.0") != 0) {    // must strictly match, else error
            syntax_error(1, "v2.0", cur_line);
            return -1;
        }
    } catch (...) {
        other_error(1, cur_line);
        return -1;
    }
    
    // R C
    std::getline(std::cin, cur_line);
    try {
        std::istringstream iss(cur_line);
        if (!(iss >> num_rows)) throw 0;    // error when rows not given or not int
        if (!(iss >> num_cols)) throw 0;    // error when columns not given or not int
        if (num_rows < 1 || num_cols < 1) throw 0;  // error when values define impossible array
    } catch (...) {
        syntax_error(2, "R C", cur_line);
        return -1;
    }

    // levels
    std::getline(std::cin, cur_line);
    try {
        std::istringstream iss(cur_line);
        int level;
        for (int i = 0; i < num_cols; i++) {
            if (!(iss >> level)) throw 0;   // error when not enough levels given or not int
            levels.push_back(level);
        }
    } catch (...) {
        syntax_error(3, "L_1 L_2 ... L_C", cur_line);
        return -1;
    }

    // 0's
    for (int i = 0; i <= num_cols; i++) {
        std::getline(std::cin, cur_line);
        try {
            trim(cur_line);
            if (cur_line.compare("0") != 0) {   // must strictly match, else error
                syntax_error(i+4, "0", cur_line);
                return -1;
            }
        } catch (...) {
            other_error(i+4, cur_line);
            return -1;
        }
    }

    // array
    int *row;
    for (int i = 0; i < num_rows; i++) {
        std::getline(std::cin, cur_line);
        try {
            std::istringstream iss(cur_line);
            row = new int[num_cols];
            for (int j = 0; j < num_cols; j++) {
                if (!(iss >> row[j])) throw 0;
                if (row[j] < 0 || row[j] >= levels.at(j)) {  // error when array value out of range
                    semantic_error(i+num_cols+5, i+1, j+1, levels.at(j), row[j], false);
                    ret = 1;
                }
            }
            array.push_back(row);
        } catch (...) {
            other_error(i+num_cols+5, cur_line);
            return -1;
        }
    }

    return ret;
}

/* HELPER METHOD: trim - chops off all leading and trailing whitespace characters from a string
 * 
 * parameters:
 * - s: string to be trimmed
 * 
 * returns:
 * - void (the string will be changed at a global scope)
 * 
 * credit:
 * - this function was adapted from Evan Teran's post at https://stackoverflow.com/a/217605
*/
void InputInfo::trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
// ======================================================================================================= //

/* HELPER METHOD: syntax_error - prints an error message regarding input format
 * 
 * parameters:
 * - lineno: line number on which the input format was violated
 * - expected: the input format that was expected
 * - actual: the actual input received
 * - verbose: whether to print extra error output (true by default)
 * 
 * returns:
 * - void (caller should decide whether to quit or continue)
*/
void InputInfo::syntax_error(int lineno, std::string expected, std::string actual, bool verbose)
{
    printf("\t-- ERROR --\n\tInput format violated on line %d. Expected \"%s\" but got \"%s\" instead.\n",
        lineno, expected.c_str(), actual.c_str());
    if (verbose) printf("\tFor formatting details, please check the README.\n");
    printf("\n");
}

/* HELPER METHOD: semantic_error - prints an error message regarding array format
 * 
 * parameters:
 * - lineno: line number on which the array format was violated
 * - row: row number of the array in which the array format was violated
 * - col: column number of the array in which the array format was violated
 * - level: factor level corresponding to the column in which the array format was violated
 * - value: the array value which violated expected format
 * - verbose: whether to print extra error output (true by default)
 * 
 * returns:
 * - void (caller should decide whether to quit or continue)
*/
void InputInfo::semantic_error(int lineno, int row, int col, int level, int value, bool verbose)
{
    printf("\t-- ERROR --\n\tArray format violated at row %d, column %d, on line %d.\n", row, col, lineno);
    if (value < 0)
        printf("\tArray values should not be negative. Value in array was %d.\n", value);
    else
        printf("\tLevel for that factor was given as %d, but value in array was %d which is too large.\n",
            level, value);
    if (verbose) printf("\tFor formatting details, please check the README.\n");
    printf("\n");
}

/* HELPER METHOD: other_error - prints a general error message
 *
 * parameters:
 * - lineno: line number on which the error occurred
 * - line: the entire line which caused the error
 * - verbose: whether to print extra error output (true by default)
 * 
 * returns:
 * - void (caller should decide whether to quit or continue)
*/
void InputInfo::other_error(int lineno, std::string line, bool verbose)
{
    printf("\t-- ERROR --\n\tError with line %d: \"%s\".\n", lineno, line.c_str());
    if (verbose) printf("\tFor formatting details, please check the README.\n");
    printf("\n");
}

/* DECONSTRUCTOR - frees memory
*/
InputInfo::~InputInfo()
{
    for (int *row : array) delete[] row;
}