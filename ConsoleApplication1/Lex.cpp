#include <iostream>
#include <map>
#include <cmath>

using namespace std;

enum type_of_lex
{
    LEX_PROGRAM,
    LEX_INT,
    LEX_REAL,
    LEX_STRING,
    LEX_IF,
    LEX_ELSE,
    LEX_DO,
    LEX_WHILE,
    LEX_FOR,
    LEX_CONTINUE,
    LEX_BREAK,
    LEX_NOT,
    LEX_AND,
    LEX_OR,
    LEX_READ,
    LEX_WRITE,
    LEX_SEMICOLON,
    LEX_LPARENTHESIS,
    LEX_RPARENTHESIS,
    LEX_ASSIGNMENT,
    LEX_LESS,
    LEX_GREATER,
    LEX_LESSEQ,
    LEX_GREATEREQ,
    LEX_EQ,
    LEX_PLUS,
    LEX_MINUS,
    LEX_MULTIPLY,
    LEX_DIVIDE,
    LEX_NEQ,
    LEX_NULL,
    LEX_ERROR
};

string name_of_lex[] =
{
    "LEX_PROGRAM",
    "LEX_INT",
    "LEX_REAL",
    "LEX_STRING",
    "LEX_IF",
    "LEX_ELSE",
    "LEX_DO",
    "LEX_WHILE",
    "LEX_FOR",
    "LEX_CONTINUE",
    "LEX_BREAK",
    "LEX_NOT",
    "LEX_AND",
    "LEX_OR",
    "LEX_READ",
    "LEX_WRITE",
    "LEX_SEMICOLON",
    "LEX_LPARENTHESIS",
    "LEX_RPARENTHESIS",
    "LEX_ASSIGNMENT",
    "LEX_LESS",
    "LEX_GREATER",
    "LEX_LESSEQ",
    "LEX_GREATEREQ",
    "LEX_EQ",
    "LEX_PLUS",
    "LEX_MINUS",
    "LEX_MULTIPLY",
    "LEX_DIVIDE",
    "LEX_NEQ",
    "LEX_NULL",
    "LEX_ERROR"
};

map<string, type_of_lex> keywords =
{
    {"program", LEX_PROGRAM},
    {"int",     LEX_INT},
    {"real",    LEX_REAL},
    {"string",  LEX_STRING},
    {"if",      LEX_IF},
    {"else",    LEX_ELSE},
    {"do",      LEX_DO},
    {"while",   LEX_WHILE},
    {"for",     LEX_FOR},
    {"continue",LEX_CONTINUE},
    {"break",   LEX_BREAK},
    {"not",     LEX_NOT},
    {"and",     LEX_AND},
    {"or",      LEX_OR},
    {"read",    LEX_READ},
    {"write",   LEX_WRITE}
};

map<string, type_of_lex> operators =
{
    {";",  LEX_SEMICOLON},
    {"(",  LEX_LPARENTHESIS},
    {")",  LEX_RPARENTHESIS},
    {"=",  LEX_ASSIGNMENT},
    {"<",  LEX_LESS},
    {">",  LEX_GREATER},
    {"<=", LEX_LESSEQ},
    {">=", LEX_GREATEREQ},
    {"==", LEX_EQ},
    {"+",  LEX_PLUS},
    {"-",  LEX_MINUS},
    {"*",  LEX_MULTIPLY},
    {"/",  LEX_DIVIDE},
    {"!=", LEX_NEQ},
};

class Lex
{
    type_of_lex type_lex = LEX_NULL;
    int int_value;
    double real_value; // для вещественных чисел
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, double real_v = 0)
    {
        type_lex = t; int_value = v; real_value = real_v;
    }
    type_of_lex get_type () { return type_lex; }
    int get_value () { return int_value; }
    friend ostream& operator << ( ostream &s, Lex l )
    {
        s << '(' << l.type_lex << ',' << l.int_value << ");";
        return s;
    }
};


class Parser{
    enum state { H, ID, NUM, RNUM, STR}; // Состояния

    char c;
    string buf;
    int number;
    int flag;
    int fract;
    int count_10;
    state CS;
    FILE* fp;

public:

    Parser ( const char * program )
    {
        fp = fopen ( program, "r" );
        CS = H;
        gc();
    }

    char get_c()
    {
        return c;
    }

    void gc()
    {
        c = fgetc(fp);
    }

    Lex get_lex();
    Lex Check_keywords();
   // Lex Check_operators();
    //Lex Check_identificators();

};

Lex Parser::get_lex ()
{
   //  cout << c << endl;
    CS = H;
    do
    {
        switch ( CS )
        {
        case H:
            if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' )
                gc ();
            else if (isalpha(c))
            {
                buf = c;
                gc ();
                CS = ID;
            }
            else if (isdigit(c))
            {
                number =c-'0'; // вычисляем цифру
                gc();
                CS=NUM;
            }
            else if(c=='>')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_GREATEREQ);
                }
                return Lex(LEX_GREATER);
            }

            else if(c=='<')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_LESSEQ);
                }
                return Lex(LEX_LESS);
            }

            else if(c=='!')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_NEQ);
                }
               return Lex(LEX_ERROR);
            }

            else if(c=='=')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_EQ);
                }
               return Lex(LEX_ASSIGNMENT);
            }

            else if(c=='+')
            {
               gc();
               return Lex(LEX_PLUS);
            }

            else if(c=='-')
            {
               gc();
               return Lex(LEX_MINUS);
            }

            else if(c=='*')
            {
               gc();
               return Lex(LEX_MULTIPLY);
            }

            else if(c=='/')
            {
               gc();
               return Lex(LEX_DIVIDE);
            }

            else if(c=='(')
            {
               gc();
               return Lex(LEX_LPARENTHESIS);
            }

            else if(c==')')
            {
               gc();
               return Lex(LEX_RPARENTHESIS);
            }

            else if(c==';')
            {
               gc();
               return Lex(LEX_SEMICOLON);
            }

            else if(c=='\"')
            {
               gc();
               buf="";
               CS=STR;
            }
            else if (c == EOF)
            {
                return Lex();
            }
            else
            {
                return Lex(LEX_ERROR);
            }
            break;

        case ID:
            if ( isalpha(c) || isdigit(c))
            {
                buf += c;
                gc ();
            }
            else
            {
                return Check_keywords();
            }
            break;

        case NUM:
            if(isdigit(c))
            {
                gc();
                number=(number*10)+(c-'0');
            }
            else
            {
                if(c=='.')
                {
                    gc();
                    fract = 0;
                    CS=RNUM;
                }

                else
                {
                    return Lex(LEX_INT, number);
                }
            }
            break;

        case RNUM:
            if(isdigit(c))
            {
                fract=(fract*10)+(c-'0');
                count_10++;
                gc();
            }
            else
            {
                double r = number+((double)fract/pow(10,count_10));
                return Lex(LEX_REAL, 0, r);
            }
            break;

        case STR:
            if(c!='\"')
            {
                buf+=c;
                gc();
            }
            else
            {
                gc();
                // таблица идентификаторов
        //        cout << buf << endl;
                return Lex(LEX_STRING);
            }
            break;
        }
    }
    while (true);
}

Lex Parser::Check_keywords()
{
    std::map<string,type_of_lex>::iterator it; // указатель на map

    it = keywords.find(buf);
  //  cout << buf << endl;
    if (it == keywords.end())
    {
        return Lex();
    }
    else
    {
        return Lex(it->second);  // == keywords[buf]
    }
}


int main()
{
    //cout <<  static_cast<type_of_lex>(LEX_NULL) << endl;
    Parser p("test.txt");
    string cur_lex;
    while (p.get_c() != EOF && cur_lex != "LEX_ERROR")
    {
        cur_lex =  name_of_lex[p.get_lex().get_type()];
        cout << cur_lex << endl;
    }
    return 0;
}

