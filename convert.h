/*
 *  This program converts a .csv file into a table using the LaTeX syntax.
 *  Use the arguments: -h, -p, -t, -r, -e, -s and -a to specify what should be done to the file
 *
 *  (Based opon The MIT License (MIT))
 *
 *  Copyright (c) 2014 Jannik Gade
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#ifndef __csv_to_LaTeX_table__convert__
#define __csv_to_LaTeX_table__convert__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class latex_table {
private:
    int width;
    int height;
    string **array;    //array pointer
    string filename;
    string csv_string;      //holds string version of csv file
    void add_latex_table_header(string latex_label);
    void add_latex_tabular_header(void);
    void save_latex_table_to_file(void);
    int convert_exponential_to_math(string &target_srt, string start_str, string stop_str, string replacement_str_1, string replacement_str_2);
    void split_string_into_array(string &target_str, string delimiter_1, string delimeter_2);
    int number_of_occurences(string &target_str, string start_str, string stop_str="");
    int convert_exp_to_math(string **ptr);
    void print_array(string **ptr);
    void convert_array_to_string(string &target_str, string **ptr);
    void cleanup();
    void transpose();
    void reverse();
    void convert_array_to_string_alternating_rows(string &target_str, string **ptr);
public:
    void convert_csv_to_latex_table(string in_filename, bool help=false, bool print=false, bool trans=false, bool rev=false, bool exp=false, bool save=false, bool alter=false);
    void print_LaTeX_string();
    void print_help();
};

#endif /* defined(__csv_to_LaTeX_table__convert__) */
