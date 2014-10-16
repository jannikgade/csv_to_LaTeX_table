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

#include <iostream>
#include "convert.h"

#define NUMER_OF_KNOWN_ARGUMENTS 7

using namespace std;

int main(int argc, const char * argv[])
{
    latex_table test;
    bool valid_filename = false;
    
    if (argc==1)
    {
        cout << "Please specify a file!";
        cout << " Use -h to get a list of commands" << endl;
    }
    else if (argc>1)
    {
        string filename(argv[1]);
        if (filename.length()>4&&filename.substr(filename.length()-4)==".csv") {
            valid_filename = true;
        }
        
        if (valid_filename) {
            
            string list_of_known_arguments[NUMER_OF_KNOWN_ARGUMENTS] = {"-h","-p","-t","-r","-e","-s","-a"};
            bool arguments[NUMER_OF_KNOWN_ARGUMENTS] = {false};
            
            for (int i=0; i<NUMER_OF_KNOWN_ARGUMENTS; i++) {
                for (int j=2; j<argc; j++) {
                    if (list_of_known_arguments[i]==argv[j]) {
                        arguments[i] = true;
                    }
                }
            }
            test.convert_csv_to_latex_table(filename,arguments[0],arguments[1],arguments[2],arguments[3],arguments[4],arguments[5],arguments[6]);
            
        }
        else
        {
            test.print_help();
        }
         
    }
    
    return 0;
}

