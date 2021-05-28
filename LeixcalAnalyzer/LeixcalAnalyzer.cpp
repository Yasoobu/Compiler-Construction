#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool lexicalError = false;
bool parserError = false;
int errorShown = 0;

struct LexicalPair {
	string tok;
	string lex;

	void print()
	{
		cout << "Token = " << tok << endl;
		cout << "Lexeme = " << lex << endl;
	}
};

bool isOperator(char c)
{
	if (c == '+' || c == '-' || c == '/' || c == '*') 
		return true;
	return false;
}

bool isInputOperator(char c)
{
	if (c == '>')
		return true;
	return false;
}

bool isRelationOperator(char c)
{
	if (c == '<' || c == '>' || c == '=')
		return true;
	return false;
}

bool isParenthesis(char c)
{
	if (c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']')
		return true;
	return false;
}

bool isVariableDeclaration(char c)
{
	if (c == ':')
		return true;
	return false;
}

bool isKeyword(string word, vector<string> keywords)
{
	for (int i = 0; i < keywords.size(); i++)
	{
		if (word == keywords[i])
			return true;
	}
	return false;
}

bool isCommaClass(char c)
{
	if (c == ',' || c == ';')
		return true;
	return false;
}

bool isLiteral(char c)
{
	if (c == '\'')
		return true;
	return false;
}

bool isString(char c)
{
	if (c == '\"')
		return true;
	return false;
}

bool isComment(char c1, char c2)
{
	if (c1 == '/' && c2 == '*')
		return true;
	return false;
}

string generateTokenLexemePair(string token, string lexeme)
{
	string pair = "(" + token + ", " + lexeme + ")";
	return pair;
}

void populateKeywordsArr(vector<string>& keywords)
{
	keywords.push_back("if");
	keywords.push_back("elif");
	keywords.push_back("else");
	keywords.push_back("while");
	keywords.push_back("func");
	keywords.push_back("in");
	keywords.push_back("In");
	keywords.push_back("print");
	keywords.push_back("println");
	keywords.push_back("Integer");
	keywords.push_back("integer");
	keywords.push_back("char");
	keywords.push_back("Char");
	keywords.push_back("write");
	keywords.push_back("ret");
}

void runLexicalAnalyzer()
{
	vector<string> keywords;
	populateKeywordsArr(keywords);

	cout << "Enter file name with full path = ";
	string fileName;
	cin >> fileName;

	ifstream ifile;
	ifile.open(fileName);

	if (!ifile)
	{
		lexicalError = true;
		cout << "File not found!" << endl;
		return;
	}

	ofstream ofile;
	ofile.open("words.txt");	//placed in same directory as .cpp file

	bool isMultiLineComment = false;	//for handling multi line comments
	string multiLineComment = "";

	while (!ifile.eof())
	{
		string s;
		getline(ifile, s);

		string word = "";


		for (int i = 0; i < s.size(); i++)
		{
			if (isComment(s[i], s[i+1]) || isMultiLineComment)
			{
				if (s[i + 1] == '*' || isMultiLineComment)
				{
					int j = 1;
					bool flag = true;
					string s1 = "";
					s1 += s[i];
					while (flag)
					{
						if (s[i + j] == '\n' || s[i + j] == '\0')
						{
							isMultiLineComment = true;
							break;
						}
						else if (s[i + j] == '*' && s[i + j + 1] == '/')
						{
							flag = false;
							s1 += s[i + j];
							s1 += s[i + j + 1];
						}
						else if (s[i + j] == '\n')
						{
							cout << "Error in comment syntax" << endl;
							lexicalError = true;
							break;
						}
						else
						{
							s1 += s[i + j];
						}
						j++;
					}

					i += j;

					if (!flag)
					{
						if (isMultiLineComment)
						{
							multiLineComment += s1;
							multiLineComment += "  ";
							cout << multiLineComment << " is a comment" << endl;
							ofile << generateTokenLexemePair("comment", "/**/") << endl;
							multiLineComment = "";
							isMultiLineComment = false;
						}
						else
						{
							cout << s1 << " is a comment" << endl;
							ofile << generateTokenLexemePair("comment", "/**/") << endl;
						}
					}
					else if (flag && isMultiLineComment)
					{
						multiLineComment += s1;
						multiLineComment += '\n';
					}
				}
			}
			else if (isOperator(s[i]))
			{
				if (s[i] == '/' && s[i + 1] == '=')
				{
					cout << s[i] << s[i + 1] << " is a relational operator" << endl;
					ofile << generateTokenLexemePair("RO", "NE") << endl;
					i++;
				}
				else
				{
					cout << s[i] << " is an operator" << endl;
					string op = "'";
					op += s[i];
					op += "'";
					ofile << generateTokenLexemePair(op, "^") << endl;
				}
			}
			else if (isInputOperator(s[i]) && isInputOperator(s[i+1]))
			{
				cout << s[i] << s[i + 1] << " is an input operator" << endl;
				ofile << generateTokenLexemePair(">>", "^") << endl;
				i++;
			}
			else if (isParenthesis(s[i]))
			{
				cout << s[i] << " is a parenthesis" << endl;
				string tok = "'";
				tok += s[i];
				tok += "'";
				ofile << generateTokenLexemePair(tok, "^") << endl;
			}
			else if (isVariableDeclaration(s[i]))
			{
				if (s[i + 1] != '=')
				{
					cout << s[i] << " is variable declaration" << endl;
					string tok = "'";
					tok += s[i];
					tok += "'";
					ofile << generateTokenLexemePair(tok, "^") << endl;
				}
				else
				{
					cout << s[i] << s[i + 1] << " is assignment operator" << endl;
					string tok = "'";
					tok += s[i];
					tok += s[i + 1];
					tok += "'";
					ofile << generateTokenLexemePair(tok, "^") << endl;
					i++;
				}
			}
			else if (isCommaClass(s[i]))
			{
				cout << s[i] << " is in comma class" << endl;
				string tok = "'";
				tok += s[i];
				tok += "'";
				ofile << generateTokenLexemePair(tok, "^") << endl;
			}
			else if (isLiteral(s[i]))
			{
				if (isalpha(s[i + 1]) && s[i + 2] == '\'')
				{
					cout << s[i] << s[i + 1] << s[i + 2] << " is a literal" << endl;
					string lex = "";
					lex += s[i + 1];
					ofile << generateTokenLexemePair("literal", lex) << endl;
					i += 2;
				}
				else
				{
					cout << "Error: Incorrect literal syntax!" << endl;
					lexicalError = true;
				}
			}
			else if (isRelationOperator(s[i]))
			{
				if (s[i + 1] == '=') // RO is <= or >= or ==
				{
					cout << s[i] << s[i + 1] << " is a relational operator" << endl;

					string lex = "";

					switch (s[i])
					{
					case '<':
						lex = "LE";
						break;
					case '>':
						lex = "GE";
						break;
					case '=':
						lex = "EE";
						break;
					default:
						break;
					}

					i++;
					ofile << generateTokenLexemePair("RO", lex) << endl;
				}
				else if (s[i + 1] == ' ' || isalnum(s[i + 1])) // RO is < or > or =
				{
					cout << s[i] << " is a relational operator" << endl;

					string lex = "";

					switch (s[i])
					{
					case '<':
						lex = "LT";
						break;
					case '>':
						lex = "GT";
						break;
					default:
						break;
					}

					ofile << generateTokenLexemePair("RO", lex) << endl;
				}
				else
				{
					cout << "Error in relational operator syntax" << endl;
					lexicalError = true;
				}

			}
			else if (isString(s[i]))    //assume string exists in 1 line only
			{
				int j = 1;
				bool flag = true;
				string s1 = "";
				while (flag)
				{
					if (s[i + j] == '\"')
					{
						flag = false;
						break;
					}
					else if (s[i + j] == '\n')
					{
						cout << "Error in string syntax" << endl;
						lexicalError = true;
						break;
					}
					else
					{
						s1 += s[i + j];
					}
					j++;
				}

				i += j;

				if (!flag)
				{
					cout << s1 << " is a string" << endl;
					ofile << generateTokenLexemePair("string", "^") << endl;
				}
			}
			else if (isdigit(s[i]))
			{
				int j = 0;
				bool flag = true;
				string s1 = "";
				s1 += s[i];
				bool multipleDigitis = false;    ////added multiple digit checking flag
				while (flag)
				{
					j++;
					if (isdigit(s[i + j]))  //error checking left
					{
						s1 += s[i + j];
						multipleDigitis = true;
					}
					else
					{
						flag = false;
					}
				}

				if (multipleDigitis)
					i += j;

				if (!flag)
				{
					cout << s1 << " is a constant" << endl;
					ofile << generateTokenLexemePair("constant", s1) << endl;
				}
			}

			else if (isalnum(s[i]) || s[i] == '_')
				word += s[i];

			else
			{
				if (s[i] != ' ' && word != "")
					word += s[i];

				if ((s[i] != ' ' && s[i] != '\n' && s[i] != '\t') && word == "")
					cout << "Illegal use of " << s[i] << endl;
			}


			if (!isMultiLineComment)
			{
				if (s[i] == ' ' || s[i] == '\n' || !isalnum(s[i + 1]))	//// for correcting operator coming before indentifier
				{
					if (isKeyword(word, keywords))
					{
						cout << word << " is a keyword" << endl;
						ofile << generateTokenLexemePair(word, "^") << endl;
					}
					else
					{
						if (word != " " && word != "")
						{
							bool isIdentifier = true;
							if (word[0] == '_' && word.size() == 1) //single underscore not allowed
								isIdentifier = false;
							else if ((isalnum(word[0]) || word[0] == '_') && word.size() > 1)
							{
								for (int k = 1; k < word.size(); k++)
								{
									if (!(word[k] == '_' || isalnum(word[k]))) //only underscore & alphanumerics allowed
									{
										isIdentifier = false;
										break;
									}
								}
							}
							else if (!(isalnum(word[0])))
								isIdentifier = false;

							if (isIdentifier)
							{
								cout << word << " is an identifier" << endl;
								ofile << generateTokenLexemePair("ID", word) << endl;
							}
							else
								cout << word << " is not a valid identifier" << endl;
						}
					}
					word = "";
				}
			}
		}

	}

	ifile.close();
	ofile.close();

}

LexicalPair extractLexicalPair(string s)
{
	LexicalPair l;
	for (int i = 1; i < s.find_last_of(','); i++)
	{
		if (s[i] != '\'')
			l.tok += s[i];
	}

	for (int i = s.find_last_of(',') + 2; i < s.find_last_of(')'); i++)
	{
		l.lex += s[i];
	}

	return l;
}


//---------------------------------------------------Parser Implementation------------------------------------------------------

//E->T E'
//E' -> + T E' | -T E' | ^
//T->F T'
//T' -> * F T' | / F T' | ^
//F->id | num | (E)

vector<LexicalPair> tokArr;
int currTok = 0;
int tab = 0;
ofstream treeFile;

void printTab(int tab, string s)
{
	if (tab != 0)
		treeFile << "|";

	for (int i = 0; i < tab; i++)
	{
		treeFile << "----";
	}

	treeFile << s << endl;
}

string nextTok()
{
	if (currTok != tokArr.size())
		return tokArr[currTok++].tok; 
	else
		return "EOF";
}

string peekNextTok()
{
	if (currTok != tokArr.size())
		return tokArr[currTok + 1].tok;
	else
		return "EOF";
}

string peekOverTwiceTok()
{
	if (currTok + 1 < tokArr.size())
		return tokArr[currTok + 1].tok;
	else
		return "EOF";
}

string look;	// lookahead symbol
void match(string tok) {
	if (look == tok)
		look = nextTok();
	else
	{
		cout << "Bad token ..." << endl;
		parserError = true;
	}
}

void T();
void Eprime();
void Tprime();
void F();
void E();

void E() {
	printTab(tab, "E");
	++tab;
	printTab(tab, "T"); T();
	printTab(tab, "E'"); Eprime();
	--tab;
}

void T() {
	++tab;
	printTab(tab, "F"); F();
	printTab(tab, "T'"); Tprime();
	--tab;
}

//E' -> + T E' | -T E' | ^

void Eprime() 
{
	if (look == "+") {
		++tab;
		printTab(tab, "+"); match("+"); 
		printTab(tab, "T"); T(); 
		printTab(tab, "E'"); Eprime();
		--tab;
	}
	else if (look == "-") {
		++tab;
		printTab(tab, " -"); match("-"); 
		printTab(tab, "T"); T();
		printTab(tab, "E'"); Eprime();
		--tab;
	}
}

//F->id | num | (E)

void F() {
	if (look == "ID")
	{
		++tab;
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID");
		--tab;
	}
	else if (look == "constant")
	{
		++tab;
		string s = "constant(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("constant");
		--tab;
	}
	else if (look == "(") {
		++tab;
		printTab(tab, "("); match("("); 
		//printTab(tab, "E"); 
		E(); 
		printTab(tab, ")"); match(")");
		--tab;
	}
	else
		cout << "Bad token ..." << endl;
}


//T' -> * F T' | / F T' | ^

void Tprime() {
	if (look == "*") {
		++tab;
		printTab(tab, "*"); match("*"); 
		printTab(tab, "F"); F(); 
		printTab(tab, "T'"); Tprime();
		--tab;
	}
	else if (look == "/") {
		++tab;
		printTab(tab, "/"); match("/"); 
		printTab(tab, "F"); F(); 
		printTab(tab, "T'"); Tprime();
		--tab;
	}
}

//return operation
//RET->ret E
void RET()
{
	printTab(tab, "RET");
	++tab;
	printTab(tab, "ret"); match("ret"); 
	E();
	--tab;
}

//8. Input operation
//INP->in >> id A
//A ->, id A | ^

void A();

void INP()
{
	printTab(tab, "INP");
	++tab;
	printTab(tab, "in"); match("in"); 
	printTab(tab, ">>"); match(">>"); 
	printTab(tab, "T'"); match("ID"); 
	printTab(tab, "A"); A();
	--tab;
}

void A()
{
	if (look == ",")
	{
		++tab;
		printTab(tab, ","); match(","); 
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID");
		printTab(tab, "A"); A();
		--tab;
	}
}

//7. Assignment operation
//AS->id : = E

void AS()
{
	printTab(tab, "AS");
	++tab;
	string s = "ID(" + tokArr[currTok - 1].lex + ")";
	printTab(tab, s); match("ID"); 
	printTab(tab, ":="); match(":="); 
	E();
	--tab;
}

//5. Literal constants with assignment :
//LIT->id : = ‘ alphabet ‘
//alphabet -> (a | b | c | ... | z) | (A | B | C | ... | Z) | (+| -| *|/|......)

void LIT()
{
	printTab(tab, "LIT");
	++tab;
	string s = "ID(" + tokArr[currTok - 1].lex + ")";
	printTab(tab, s); match("ID"); 
	printTab(tab, ":="); match(":=");
	printTab(tab, "literal"); match("literal");
	--tab;
}

//4. Variable declaration :
//DEC->DT : id A
//DT->Integer | char
//A ->, id A | ^

void DT();
void A();

void DEC()
{
	printTab(tab, "DEC");
	++tab;
	printTab(tab, "DT"); DT(); 
	printTab(tab, ":"); match(":"); 
	string s = "ID(" + tokArr[currTok - 1].lex + ")";
	printTab(tab, s); match("ID");
	printTab(tab, "A"); A();
	--tab;
}

void DT()
{
	if (look == "Integer")
	{
		++tab;
		printTab(tab, "Integer"); match("Integer");
		--tab;
	}
	else if (look == "char")
	{
		++tab;
		printTab(tab, "char"); match("char");
		--tab;
	}
}

//Write Data Statement
//WRITE-> print(TBP) | println(TBP)
//TBP-> string | E

void TBP()
{
	if (look == "string")
	{
		++tab;
		printTab(tab, "string");	match("string");
		--tab;
	}
	else
	{
		++tab;
		E();
		--tab;
	}

}

void WRITE()
{
	printTab(tab, "WRITE");
	if (look == "print")
	{
		++tab;
		printTab(tab, "print"); match("print");
		printTab(tab, "("); match("(");
		printTab(tab, "TBP"); TBP();
		printTab(tab, ")"); match(")");
		--tab;
	}
	else if (look == "println")
	{
		++tab;
		printTab(tab, "println"); match("println");
		printTab(tab, "("); match("(");
		printTab(tab, "TBP"); TBP();
		printTab(tab, ")"); match(")");
		--tab;
	}
}

//Function Declaration Statement
//FUNC->func DT : id(Parameters)
//DT->Integer | char
//Parameters->DT : id Parameters’ | ^
//Parameters’ ->, DT : id Parameters’ | ^

void Parameters();
void ParametersPrime();
void Action();

void FUNC()
{
	if (look == "comment")
		match("comment");

	printTab(tab, "FUNC");
	
	++tab;
	printTab(tab, "func"); match("func"); 
	printTab(tab, "DT"); DT(); 
	printTab(tab, ":"); match(":"); 
	string s = "ID(" + tokArr[currTok - 1].lex + ")";
	printTab(tab, s); match("ID");
	printTab(tab, "("); match("("); 
	printTab(tab, "Parameters"); Parameters(); 
	printTab(tab, ")"); match(")"); 
	printTab(tab, "{"); match("{"); 
	printTab(tab, "Action"); Action(); 
	printTab(tab, "}"); match("}");
	--tab;
}

void Parameters()
{
	++tab;
	if (look == "Integer")
	{
		printTab(tab, "Integer"); match("Integer"); 
		printTab(tab, ":"); match(":"); 
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID");
		printTab(tab, "Parameters'"); ParametersPrime();
	}
	else if (look == "char")
	{
		printTab(tab, "char"); match("char"); 
		printTab(tab, ":"); match(":"); 
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID");
		printTab(tab, "Parameters'"); ParametersPrime();
	}
	--tab;
}

void ParametersPrime()
{
	if (look == ",")
	{
		++tab;
		printTab(tab, ","); match(","); 
		printTab(tab, "DT'"); DT(); 
		printTab(tab, ":'"); match(":"); 
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID");
		printTab(tab, "Parameters'"); ParametersPrime();
		--tab;
	}
}

//6. parenthesis, braces, square brackets with matching
//P->(P) P | ^
//B->{B} B | ^
//SQ-> [SQ] SQ | ^

void P()
{
	if (look == "(")
	{
		match("("); P(); match(")"); P();
	}
}

void B()
{
	if (look == "{")
	{
		match("{"); B(); match("}"); B();
	}
}

void SQ()
{
	if (look == "(")
	{
		match("["); SQ(); match("]"); SQ();
	}
}

//3. Relational Operations
//ROP->C EE
//EE -> RO C
//C->E | id(ARG)

//ARG->id ARG' | ^
//ARG' -> , id ARG’ | ^

void C();
void EE();
void ARG();
void ARGPrime();

void ROP()
{
	++tab;
	printTab(tab, "C"); C(); 
	printTab(tab, "EE"); EE();
	--tab;
}

void EE()
{
	++tab;
	printTab(tab, "RO"); match("RO"); 
	printTab(tab, "C"); C();
	--tab;
}

void C()
{
	++tab;
	if (look == "ID" && peekNextTok()=="(")	//function call
	{
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID"); 
		printTab(tab, "("); match("("); 
		printTab(tab, "ARG"); ARG(); 
		printTab(tab, ")"); match(")");
	}
	else
		E();
	--tab;
}

void ARG()
{
	if (look == "ID")
	{
		++tab;
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID"); 
		printTab(tab, "ARG'"); ARGPrime();
		--tab;
	}
}

void ARGPrime()
{
	if (look == ",")
	{
		++tab;
		printTab(tab, ","); match(","); 
		string s = "ID(" + tokArr[currTok - 1].lex + ")";
		printTab(tab, s); match("ID");
		printTab(tab, "ARG'"); ARGPrime();
		--tab;
	}
}

//Decision Statement
//DS -> if ROP : { Action } DS’ DS’’
//DS’->elif ROP : { Action } DS’ | ^
//DS’’ -> else { Action } | ^
//ROP->ROP | id(ARG) //ROP can have relational operation or boolean function call
//ARG->id ARG' | ^
//ARG' -> , id ARG’ | ^
//Action->ST Action | ^ //statement (ST) defined below

void DSPrime();
void DSPrimePrime();
void Action();
void ROP();
bool ST();
void LOOP();

void DS()
{
	printTab(tab, "DS");
	++tab;
	printTab(tab, "if"); match("if"); 
	printTab(tab, "ROP"); ROP(); 
	printTab(tab, ":"); match(":"); 
	printTab(tab, "{"); match("{"); 
	printTab(tab, "Action"); Action(); 
	printTab(tab, "}"); match("}"); 
	printTab(tab, "DS'"); DSPrime(); 
	printTab(tab, "DS''"); DSPrimePrime();
	--tab;
}

void DSPrime()
{
	if (look == "elif")
	{
		++tab;
		printTab(tab, "Action"); match("elif"); 
		printTab(tab, "ROP"); ROP(); 
		printTab(tab, ":"); match(":"); 
		printTab(tab, "{"); match("{"); 
		printTab(tab, "Action"); Action(); 
		printTab(tab, "}"); match("}"); 
		printTab(tab, "DS'"); DSPrime();
		--tab;
	}
}
void DSPrimePrime()
{
	if (look == "else")
	{
		++tab;
		printTab(tab, "else"); match("else"); 
		printTab(tab, "{"); match("{"); 
		printTab(tab, "Action"); Action(); 
		printTab(tab, "}"); match("}");
		--tab;
	}
}
void Action()
{
	if (ST())
	{
		Action();
	}
}
bool ST()
{
	if (parserError)
	{
		++errorShown;
		if (errorShown == 1)
		{
			cout << endl << endl;
			cout << "Parser detected an error........" << endl;
			cout << "Halting further execution......." << endl;
		}
		return false;
	}

	if (look == "ID" && peekOverTwiceTok() == "literal")
	{
		LIT(); match(";"); return true;
	}
	else if (look == "ID")
	{
		AS(); match(";"); return true;
	}
	else if (look == "in")
	{
		INP(); match(";"); return true;
	}
	else if (look == "char" || look == "Integer")
	{
		DEC(); match(";"); return true;
	}
	else if (look == "print" || look == "println")
	{
		WRITE(); match(";"); return true;
	}
	else if (look == "func")
	{
		FUNC();  return true;
	}
	else if (look == "while")
	{
		LOOP();  return true;
	}
	else if (look == "if")
	{
		DS();  return true;
	}
	else if (look == "comment")
	{
		look = nextTok(); return true;
	}
	else if (look == "ret")
	{
		RET(); match(";"); return true;
	}
	else
	{
		return false;
	}
}

//Looping Statement
//LOOP -> while ROP : { Action }

void LOOP()
{
	printTab(tab, "LOOP");
	++tab;
	printTab(tab, "while"); match("while"); 
	printTab(tab, "ROP"); ROP(); 
	printTab(tab, ":"); match(":"); 
	printTab(tab, "{"); match("{"); 
	printTab(tab, "Action"); Action(); 
	printTab(tab, "}"); match("}");
	--tab;
}

void runParser()
{
	ifstream ifile;
	ifile.open("words.txt");
	treeFile.open("parsetree.txt");
	if (!ifile)
	{
		cout << "Error: Parser cannot find file generated by Lexical Analyzer!" << endl;
		return;
	}

	cout << endl << endl;
	while (!ifile.eof())
	{
		string s;
		getline(ifile, s);

		if (s != "")
		{
			LexicalPair l = extractLexicalPair(s);
			tokArr.push_back(l);
		}
	}

	ifile.close();

	look = nextTok();
	Action();

	treeFile.close();
}


int main()
{
	runLexicalAnalyzer();
	
	if (!lexicalError)
		runParser();
	else
	{
		cout << endl << endl;
		cout << "Error detected by Lexical Analyzer...." << endl;
		cout << "Halting further execution............." << endl << endl;
		return -1;
	}
	

	return 0;
}


