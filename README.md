# Compiler
Compiler for GO-- language that generates token lexeme pairs.

Functionalities implemented so far:
-Lexical Analyzer
 Gets sample code from another file and converts the code into token and lexeme pairs and stores in another file.
 
-Parser
 Gets sample code from another file and evaluates the code according to CFG to check if the code has syntax errors.
 If the code is error free, then makes a parse tree for the code and writes to another file.


To run this, you need to install Visual Studio 2015 or above.
Open the solution in Visual Studio and press Ctrl + F5 to run.

The command prompt will ask to enter the name of the input file which contains the source code to be checked. 
Input the file name along with extension and press enter. It will then parse the whole code and output the tokens and lexemes and will write the parse tree into another file.

