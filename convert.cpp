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

#include "convert.h"

#define NUMER_OF_KNOWN_ARGUMENTS 7

using namespace std;

void latex_table::convert_csv_to_latex_table(string in_filename, bool help, bool print, bool trans, bool rev, bool exp, bool save, bool alter){
    
    if (help)
    {
        print_help();
    }
    else
    {
        filename = in_filename;
        cout << "Loading " << filename << "..." << endl;
        ifstream input_csv(filename);
        
        string temp_csv_string((std::istreambuf_iterator<char>(input_csv)), std::istreambuf_iterator<char>());
        csv_string = temp_csv_string;
        
        split_string_into_array(csv_string, ",", "\n");
        
        if (rev) {
            reverse();
        }
        if (trans) {
            transpose();
        }
        // convert to exponential notation on default
        if (!exp) {
            convert_exp_to_math(array);
        }
        if (alter) {
            convert_array_to_string_alternating_rows(csv_string, array);
        } else {
            convert_array_to_string(csv_string, array);
        }
        
        
        
        add_latex_table_header(filename);
        
        cleanup();
        
        if (print||save) {
            print_LaTeX_string();
        }
        
        if (!save) {
            cout << "Saving file.." << endl;
            save_latex_table_to_file();
        }
        
    }
    
}

void latex_table::add_latex_table_header(string latex_label){
    
    //remove spaces from name (spaces not allowen in LaTeX labels)
    char c;
    int i=0;
    while (latex_label[i])
    {
        c=latex_label[i];
        if (!isalpha(c)&&!isdigit(c)) {
            latex_label[i]='_';
        } c='_';
        i++;
    }
    
    string head_begin_table = "\\begin{table}[H]\n\\centering\n\\scriptsize\n\\begin{tabular}{|";
    
    string head_end_table = "\n\\end{tabular}\n\\caption{}\n\\label{tab:" + latex_label + "}\n\\end{table}";
    
    for (int i=0; i<width; i++) {
        head_begin_table+="l|";
    }
    head_begin_table+="}\n\\hline\n";
    
    csv_string = head_begin_table + csv_string + head_end_table;
}

void latex_table::save_latex_table_to_file(){
    
    ofstream output_file (filename+"_LaTeX.txt");
    
    if (output_file.is_open())
    {
        output_file << csv_string;
        output_file.close();
    }
    else cout << "Unable to open file";
}

int latex_table::convert_exponential_to_math(string &target_str, string start_str, string stop_str, string replacement_str_1, string replacement_str_2){
    
    //cout << target_str << endl;
    int replacements_made = 0;
    size_t keyword_start = target_str.find(start_str,0);
    size_t keyword_stop = target_str.find(stop_str,keyword_start);
    
    if (keyword_stop>target_str.length()) {
        keyword_stop=target_str.length();
    }
    
    char c=target_str[keyword_start-1];
    
    while (keyword_start!=string::npos) {
        if (isdigit(c)) {
            keyword_stop = target_str.find(stop_str,keyword_start);
            
            if (keyword_stop>target_str.length()) {
                keyword_stop=target_str.length();
            }
            
            target_str.insert(keyword_stop, replacement_str_2);
            target_str.replace(keyword_start, 1, replacement_str_1);
            
            replacements_made++;
            
        }
        
        keyword_start = target_str.find(start_str,keyword_start+replacement_str_1.length());
        c=target_str[keyword_start-1];
        
    }
    
    return replacements_made;
}

void latex_table::print_LaTeX_string() {
    cout << "\n\n" << csv_string << "\n\n";
}

int latex_table::number_of_occurences(string &target_str, string start_str, string stop_str){
    int number_of_items_found = 0;
    size_t keyword_start;
    size_t keyword_stop;
    
    //if no stop string is specified search entire file
    if (stop_str=="") {
        keyword_start = target_str.find(start_str,0);
        //find last element of the type
        keyword_stop = target_str.rfind(start_str);
    }
    else if (stop_str==""&&start_str=="") {
        number_of_items_found = (int)target_str.length();
        //skip while loop
        keyword_start = keyword_stop++;
    } else {
        keyword_start = target_str.find(start_str,0);
        keyword_stop = target_str.find(stop_str,0);
    }
    
    while ((keyword_start<keyword_stop)&&(keyword_start!=string::npos)){
        keyword_start = target_str.find(start_str,keyword_start);
        number_of_items_found++;
        keyword_start++;
    }
    
    return number_of_items_found;
    
}

void latex_table::split_string_into_array(string &target_str, string delimiter_1, string delimiter_2){
    
    size_t keyword_start;
    size_t keyword_stop;
    size_t keyword_linebreak;
    
    width = number_of_occurences(csv_string, ",", "\n");
    height = number_of_occurences(csv_string, "\n", "") + 1;    //last row won't have a newline
    
    //cout << "w: " << width << " h: " << height << endl;
    
    //create dynamic array the size of the picture
    array = new string*[height];
    for (int row=0; row<height; row++) {
        array[row] = new string[width];
    }
    
    //find delimiter 1
    keyword_start = -1;
    //find next occurence of delimiter 1 after delimiter 1
    keyword_stop = target_str.find(delimiter_1, 0);
    //find linebreak
    keyword_linebreak = target_str.find(delimiter_2, 0);
    
    for (int row = 0; row<height; row++) {
        for (int col=0; col<width; col++) {
            
            if (keyword_linebreak<keyword_stop) {
                array[row][col] = target_str.substr(keyword_start+1,keyword_linebreak-keyword_start-delimiter_2.length());
                
                keyword_start = keyword_linebreak;
                keyword_stop = target_str.find(delimiter_1, keyword_start+1);
                keyword_linebreak = target_str.find(delimiter_2, keyword_stop);
                
            } else {
                array[row][col] = target_str.substr(keyword_start+1,keyword_stop-keyword_start-delimiter_1.length());
                keyword_start = target_str.find(delimiter_1, keyword_start+1);
                keyword_stop = target_str.find(delimiter_1, keyword_start+1);
            }
            
        }
    }
    
}

void latex_table::print_array(string **ptr){
    // print array
    for (int row = 0; row<height; row++) {
        for (int col=0; col<width; col++) {
            cout << ptr[row][col] << "\t";
        }
        cout << endl;
    }
}

int latex_table::convert_exp_to_math(string **ptr){
    int replacements = 0;
    //print_array(ptr);
    
    //find all exponents
    for (int row = 0; row<height; row++) {
        for (int col=0; col<width; col++) {
            replacements += convert_exponential_to_math(ptr[row][col], "e", "\t", "$\\cdot 10^{", "}$");
        }
    }
    
    //print_array(ptr);
    
    return replacements;
}

void latex_table::convert_array_to_string(string &target_str, string **ptr) {
    
    
    
    target_str.clear();
    
    for (int row = 0; row<height; row++) {
        for (int col=0; col<width; col++) {
            
            if (col<width-1) {
                target_str +=  array[row][col] + "&\t";
            } else {
                target_str +=  array[row][col];
            }
        }
        target_str += "\\\\\t\\hline\n";
    }
}

void latex_table::convert_array_to_string_alternating_rows(string &target_str, string **ptr) {
    
    
    target_str.clear();
    
    for (int row = 0; row<height; row++) {
        if ((row%2)) {
            target_str += "\\rowcolor[gray]{0.8}";
        }
        for (int col=0; col<width; col++) {
            
            if (col<width-1) {
                target_str +=  array[row][col] + "&\t";
            } else {
                target_str +=  array[row][col];
            }
        }
        target_str += "\\\\\t\\hline\n";
    }
}

void latex_table::reverse(){
    
    string **temp_array;
    
    //create dynamic array
    temp_array = new string*[height];
    for (int row=0; row<height; row++) {
        temp_array[row] = new string[width];
    }
    //copy array to temp_array in reverse order
    for (int row = height-1; row>=0; row--) {
        for (int col=width-1; col>=0; col--) {
            temp_array[height-1-row][width-1-col] = array[row][col];
        }
    }
    
    // deallocate memory to prevent memory leak
    for (int i = 0; i < width; ++i) {
        delete [] array[i];
        array[i] = nullptr;
    }
    delete [] array;
    array = nullptr;
    
    // array will point to new location
    array = temp_array;
    
}

void latex_table::transpose(){
    
    string **temp_array;
    int temp_heigth = height;
    int temp_width = width;
    //create dynamic array that has: new width = old height and new heigth = old width
    temp_array = new string*[width];
    for (int row=0; row<width; row++) {
        temp_array[row] = new string[height];
    }
    
    //copy array to temp_array in transposed order
    for (int col = 0; col<width; col++) {
        for (int row=0; row<height; row++) {
            temp_array[col][row] = array[row][col];
        }
    }
    
    // deallocate memory to prevent memory leak
    for (int i = 0; i < width; ++i) {
        delete [] array[i];
        array[i] = nullptr;
    }
    delete [] array;
    array = nullptr;
    
    // array will point to new location
    array = temp_array;
    
    width = temp_heigth;
    height = temp_width;
    
}

void latex_table::cleanup(){
    
    // deallocate memory to prevent memory leak
    for (int i = 0; i < height; ++i) {
        delete [] array[i];
        array[i] = nullptr;
    }
    delete [] array;
    array = nullptr;
}

void latex_table::print_help(){
    string top = "----------------------------------------------------------\n";
    string copyright_and_usage = "This program converts a .csv file into a table using the LaTeX syntax.\nUse the arguments listed below to specify what should be done to the file.\n\nThe following arguments are available:";
    string commands[NUMER_OF_KNOWN_ARGUMENTS];
    
    commands[0] = "\n-h\tHelp\t\tDisplays the functionality of the program (This is what you're currently reading).";
    commands[1] = "\n-p\tPrint\t\tPrints the LaTeX table to the terminal. This is useful if you can copy text directly from the terminal.";
    commands[2] = "\n-t\tTranspose\tRotates the table 90degrees. eg. rows become columns and vice versa";
    commands[3] = "\n-r\tReverse\t\tReverses the csv table. eg. first element becomes the last and so on.";
    commands[4] = "\n-e\tExponent\tReplaces exponential notation with LaTeX syntax. eg. 6.022e23 becomes 6.022$\\cdot 10^{23}$. On by default";
    commands[5] = "\n-s\tSave\t\tSaves the latex table as <original filname>_LaTeX.txt. On by default";
    commands[5] = "\n-a\tAlternating\tAlternating row colors.";
    
    cout << top + copyright_and_usage;
    
    for (int i=0; i<NUMER_OF_KNOWN_ARGUMENTS; i++) {
        cout << commands[i];
    }
    
    cout << "\n\nThe order of the arguments is not important." << endl;
    
    cout << top << endl;
}