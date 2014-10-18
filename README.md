#csv_to_LaTeX_table

##Introduction
------------------
This simple program converts a .csv file into a table using the LaTeX syntax. Use the arguments listed below to specify what should be done to the .csv file. By default the program converts exponential notation into LaTeX math-mode and saves the table as a .txt file. This can ofcourse be altered by setting the -s and -e flag respectively.   


##List of Arguments
* **-h** *Displays the help information of the program*

* **-p** *Prints the LaTeX table to the terminal. This is useful if you can copy text directly from the terminal.*

* **-t** *Rotates the table 90degrees. eg. rows become columns and vice versa*

* **-r** *Reverses the csv table. eg. first element becomes the last and so on.*

* **-e** *Replaces exponential notation with LaTeX syntax. eg. 6.022e23 becomes 6.022$\\cdot 10^{23}$. On by default*

* **-s** *Saves the latex table as <original filname>_LaTeX.txt. On by default*

* **-a** *Alternating row colors.*

##Examples of Usage
The following examples will call the converter with the test file: test.csv. Set the flags as needed.

###Linux and OSX
    cd /Users/<username>/Downloads/
    ./csv_to_LaTeX_table_v.0.1 test.csv -p -e

###Windows
    cd \Users\<username>\Downloads\
    csv_to_LaTeX_table_v.0.1.exe test.csv -p -e
