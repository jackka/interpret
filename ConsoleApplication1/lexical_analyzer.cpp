//Пример реализации на Си++ интерпретатора модельного языка
/* 02.04.2020 */
#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;


enum type_of_lex {
    LEX_NULL,                                                                                   /* 0*/
    LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,        /* 9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,  /*18*/
    LEX_FIN,                                                                                    /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,              /*35*/
    LEX_NUM,                                                                                    /*36*/
    LEX_ID,                                                                                     /*37*/
    POLIZ_LABEL,                                                                                /*38*/
    POLIZ_ADDRESS,                                                                              /*39*/
    POLIZ_GO,                                                                                   /*40*/
    POLIZ_FGO                                                                                   /*41*/
};

const char* type_of_lex_str[] = {
    "LEX_NULL",
    "LEX_AND","LEX_BEGIN","LEX_BOOL","LEX_DO","LEX_ELSE","LEX_END","LEX_IF","LEX_FALSE","LEX_INT",
    "LEX_NOT","LEX_OR","LEX_PROGRAM","LEX_READ","LEX_THEN","LEX_TRUE","LEX_VAR","LEX_WHILE","LEX_WRITE",
    "LEX_FIN",
    "LEX_SEMICOLON","LEX_COMMA","LEX_COLON","LEX_ASSIGN","LEX_LPAREN","LEX_RPAREN","LEX_EQ","LEX_LSS",
    "LEX_GTR","LEX_PLUS","LEX_MINUS","LEX_TIMES","LEX_SLASH","LEX_LEQ","LEX_NEQ","LEX_GEQ", 
    "LEX_NUM",
    "LEX_ID",
    "POLIZ_LABEL",
    "POLIZ_ADDRESS",
    "POLIZ_GO",
    "POLIZ_FGO"
};

/////////////////////////  Класс Lex  //////////////////////////

class Lex {
    type_of_lex   t_lex;
    int           v_lex;
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0) : t_lex(t), v_lex(v) { }
    type_of_lex  get_type() const {
        return t_lex;
    }
    int get_value() const {
        return v_lex;
    }
    friend ostream& operator<< (ostream& s, Lex l);
};

/////////////////////  Класс Ident  ////////////////////////////

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    bool operator== (const string& s) const {
        return name == s;
    }
    Ident(const string n) {
        name = n;
        declare = false;
        assign = false;
    }
    string get_name() const {
        return name;
    }
};

//////////////////////  TID  ///////////////////////

vector<Ident> TID;

int put(const string& buf) {
    vector<Ident>::iterator k;

    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

/////////////////////////////////////////////////////////////////

class Scanner {
    FILE* fp;
    char   c;
    int look(const string buf, const char** list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc() {
        c = fgetc(fp);
    }
public:
    static const char* TW[], * TD[];
    Scanner(const char* program) {
        fp = fopen(program, "r");
        if (!fp) {
            cout << "unable open file";
        }
    }
    Lex get_lex();
};

const char*
Scanner::TW[] = { "", "and", "begin", "bool", "do", "else", "end", "if", "false", "int", "not", "or", "program",
                      "read", "then", "true", "var", "while", "write", NULL };

const char*
Scanner::TD[] = { "@", ";", ",", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL };

Lex Scanner::get_lex() {
    enum    state { H, IDENT, NUMB, COM, ALE, AORE, NEQ };
    int     d, j;
    string  buf;
    state   CS = H;
    do {
        gc();
        switch (CS) {
        case H:
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t');
            else if ((c>=-1 && c<=255) && isalpha(c)) {
                buf.push_back(c);
                CS = IDENT;
            }
            else if ((c >= -1 && c <= 255) && isdigit(c)) {
                d = c - '0';
                CS = NUMB;
            }
            else if (c == '{') {
                CS = COM;
            }
            else if (c == '<' || c == '>') {
                buf.push_back(c);
                CS = ALE;
            }
            else if (c == '=') {
                buf.push_back(c);
                CS = AORE;
            }
            else if (c == '@')
                return Lex(LEX_FIN);
            else if (c == '!') {
                buf.push_back(c);
                CS = NEQ;
            }
            else {
                buf.push_back(c);
                if ((j = look(buf, TD))) {
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                else
                    throw c;
            }
            break;
        case IDENT:
            if (isalpha(c) || isdigit(c)) {
                buf.push_back(c);
            }
            else {
                ungetc(c, fp);
                if ((j = look(buf, TW))) {
                    return Lex((type_of_lex)j, j);
                }
                else {
                    j = put(buf);
                    return Lex(LEX_ID, j);
                }
            }
            break;
        case NUMB:
            if (isdigit(c)) {
                d = d * 10 + (c - '0');
            }
            else {
                ungetc(c, fp);
                return Lex(LEX_NUM, d);
            }
            break;
        case COM:
            if (c == '}') {
                CS = H;
            }
            else if (c == '@' || c == '{')
                throw c;
            break;
        case ALE:
            if (c == '=') {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            }
            else {
                ungetc(c, fp);
                j = look(buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            }
            break;
        case AORE:
            if (c == '=') {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            }
            else {
                ungetc(c, fp);
                j = look(buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            }
            break;
        case NEQ:
            if (c == '=') {
                buf.push_back(c);
                j = look(buf, TD);
                return Lex(LEX_NEQ, j);
            }
            else
                throw '!';
            break;
        } //end switch
    } while (true);
}

ostream& operator<< (ostream& s, Lex l) { // форматированный вывод
    string t;
    if (l.t_lex <= 18)
        t = Scanner::TW[l.t_lex];
    else if (l.t_lex >= 19 && l.t_lex <= 35)
        t = Scanner::TD[l.t_lex - 19];
    else if (l.t_lex == 36)
        t = "NUMB";
    else if (l.t_lex == 37)
        t = TID[l.v_lex].get_name();
    else if (l.t_lex == 38)
        t = "Label";
    else if (l.t_lex == 39)
        t = "Addr";
    else if (l.t_lex == 40)
        t = "!";
    else if (l.t_lex == 41)
        t = "!F";
    else
        throw l;
    s << type_of_lex_str[l.t_lex] << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

//////////////////////////  Класс Parser  /////////////////////////////////
template <class T, class T_EL>
void from_st(T st, T_EL& i) {
    i = st.top(); st.pop();
}

class Parser {
    Lex           curr_lex;
    type_of_lex   c_type;
    int           c_val;
    Scanner       scan;
    stack < int > st_int;
    stack < type_of_lex >  st_lex;
    void  gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    vector <Lex> poliz;
    Parser(const char* program) : scan(program) { }
    void  analyze();
};

void Parser::analyze() {
    while (true) {
        gl();
        cout << curr_lex << endl;
    }
}

class Interpretator {
    Parser   pars;
public:
    Interpretator(const char* program) : pars(program) {}
    void     interpretation();
};

void Interpretator::interpretation() {
    pars.analyze();
}


int main() {



     try {
        Interpretator I("test_file.txt");
        I.interpretation();
        return 0;
    }
    catch (char c) {
        if (c == EOF)  // лексичекский анализ закончивается с концом файла
            return 0;
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch (const char* source) {
        cout << source << endl;
        return 1;
    }
}
