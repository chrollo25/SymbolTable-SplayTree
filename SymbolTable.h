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
        bool staticFlag;
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
        void adjustParent(Symbol* symbol);

        string getName();
        string getDatatype();
        string getData();
        int getLevel();
        bool getStaticFlah();
        Symbol* getParent();
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
    void splay(Symbol* root, Symbol* target);
    void insertSymbol(Symbol* target);
    void deleteSymbol(Symbol* target);
    Symbol* search(Symbol* temp, Symbol* target);

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
    bool isFunc(string str);
    bool isFuncCall(string str);
    Symbol* isMax(Symbol* root);
};

//search function do not splay the tree. when use lookup then splay
//https://www.codesdope.com/course/data-structures-splay-trees/
//NUM_COMP and NUM_SPLAY
//Begin End 
//IsFuncCall 
//

#endif