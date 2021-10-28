#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"


const int lowercaseLowerBound = 97;
const int lowercaseUpperBound = 122;
const int uppercaseLowerBound = 65;
const int uppercaseUpperBound = 90;
const int numericLowerBound = 48;
const int numericUpperBound = 57;
const int space = 32;
const int underscore = 95;
const int apostrophe = 39;



class SymbolTable
{
private:
    class Symbol;
    Symbol* head;
    int levelCurrent;

    class Symbol
    {
    private:
        string name;
        string datatype;
        string data;
        int level;
        Symbol* parent;
        Symbol* left;
        Symbol* right;
    public:
        Symbol(string name, string datatype, bool staticFlag, int level);

        void adjustName(string name);
        void adjustDatatype(string datatype);
        void adjustData(string data);
        void adjustLeft(Symbol* symbol);
        void adjustRight(Symbol* symbol);

        string getName();
        string getDatatype();
        string getData();
        Symbol* getLeft();
        Symbol* getRight();
    };

public:
    SymbolTable();
    ~SymbolTable();

    //splay tree
    bool keyCompare(Symbol* S1, Symbol* S2); //0: S1 < S2; 1: S1 > S2
    void rightRotate(Symbol* target); 
    void leftRotate(Symbol* target);
    void splay(Symbol* target);
    void insertSymbol(Symbol* root, string name, string datatype, bool staticFlag, int level);
    void deleteSymbol(int level);
    Symbol* search(string name, int level);

    void run(string filename);

    bool INSERT(string name, string datatype, int level, bool staticFlag);
    int ASSIGN(string name, string value);
    int LOOKUP(string name);
    string PRINT();
    string RPRINT();
    void BEGIN();
    bool END();

    void addScope();
    Symbol* findIden(string name);

    bool isNum(string str);
    bool isStr(string value);
    bool isIden(string value);
};

//splay check
//do search function without splaying the tree. only lookup do
//https://www.codesdope.com/course/data-structures-splay-trees/

#endif