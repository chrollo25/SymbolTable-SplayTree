#include "SymbolTable.h"


SymbolTable::SymbolTable() {
    levelIndex = 0;
    scopeHead = new Scope();
    scopeCurrent = scopeHead;
}

SymbolTable::~SymbolTable() {
    while (scopeCurrent != scopeHead)
    {
        END();
    }

    Scope* temp = scopeCurrent;

    scopeCurrent = scopeCurrent->getPrev();

    delete temp;
}

void SymbolTable::run(string filename)
{
    ifstream fin;
    fin.open(filename);

    while (!fin.eof())
    {
        string temp;
        string ins;
        getline(fin, ins, '\n');

        //1 ve
        if (ins == "BEGIN")
        {
            BEGIN();
        }
        else if (ins == "END")
        {
            if (!END())
                throw UnknownBlock();
        }
        else if (ins == "PRINT")
        {
            cout << PRINT() << endl;
        }
        else
        {
            // n ve 
            size_t pos = ins.find_first_of(' ', 0);
            temp = ins.substr(0, pos);

            if (temp == "INSERT") // name, type, static
            {
                //cutting
                temp = ins.substr(pos + 1, ins.find_first_of(' ', pos + 1) - (pos + 1));
                pos = ins.find_first_of(' ', pos + 1);

                //valid name
                if (!isIden(temp))
                    throw InvalidInstruction(ins);

                /*
                if (temp[0] >= lowercaseLowerBound && temp[0] <= lowercaseUpperBound)
                {
                    for (int i = 0; i < temp.length(); i++)
                    {
                        if (temp[i] >= lowercaseLowerBound && temp[i] <= lowercaseUpperBound);
                        else if (temp[i] >= numericLowerBound && temp[i] <= numericUpperBound);
                        else if (temp[i] == underscore);
                        else
                            throw InvalidInstruction(ins);
                    }
                }
                else
                    throw InvalidInstruction(ins);
                */

                //cutting
                string data = ins.substr(pos + 1, ins.find_first_of(' ', pos + 1) - (pos + 1));
                pos = ins.find_first_of(' ', pos + 1);

                //valid datatype
                if (data == "number" || data == "string" || isFunc(data)); 
                {

                }
                else
                    throw InvalidInstruction(ins);

                //cutting
                int level = 0;
                string flag = ins.substr(pos + 1, ins.find_first_of('\n', pos + 1) - (pos + 1));
                pos = ins.find_first_of('\n', pos + 1);
                
                //valid static
                int level;

                if(flag == "true")
                {   
                    level = 0;
                }
                else if(flag == "false")
                {
                    level = levelCurrent;
                    if(isFunc())
                    {
                        throw InvalidDeclaration(ins);
                    }
                }
                else
                {
                    throw InvalidInstruction(ins);
                }


                if (INSERT(temp, data, level, flag))
                    cout << "success\n";
                else
                    throw Redeclared(ins);
            }
            else if (temp == "ASSIGN")
            {
                //scenario 0 = success, 1 = invalidinstruction, 2 = typemismatch
                int scenario = 0;

                //cutting
                temp = ins.substr(pos + 1, ins.find_first_of(' ', pos + 1) - (pos + 1));
                pos = ins.find_first_of(' ', pos + 1);

                //valid name
                if (!isIden(temp))
                    throw InvalidInstruction(ins);

                /*
                if (temp[0] >= lowercaseLowerBound && temp[0] <= lowercaseUpperBound)
                {
                    for (int i = 1; i < temp.length(); i++)
                    {
                        if (temp[i] >= lowercaseLowerBound && temp[i] <= lowercaseUpperBound);
                        else if (temp[i] >= numericLowerBound && temp[i] <= numericUpperBound);
                        else if (temp[i] == underscore);
                        else
                            throw InvalidInstruction(ins);
                    }
                }
                else
                    throw InvalidInstruction(ins);
                */

                string name = temp;

                //cutting
                temp = ins.substr(pos + 1, ins.find_first_of('\n', pos + 1) - (pos + 1));
                pos = ins.find_first_of('\n', pos + 1);

                scenario = ASSIGN(name, temp);

                //controller
                if (scenario == 1)
                    throw InvalidInstruction(ins);
                else if (scenario == 2)
                    throw TypeMismatch(ins);
                else if (scenario == 3)
                    throw Undeclared(ins);
                else
                    cout << "success\n";
            }
            else if (temp == "LOOKUP")
            {
                temp = ins.substr(pos + 1, ins.find_first_of('\n', pos + 1) - (pos + 1));
                pos = ins.find_first_of('\n', pos + 1);

                if (!isIden(temp))
                    throw InvalidInstruction(ins);

                if (LOOKUP(temp) != -1)
                {
                    cout << LOOKUP(temp);
                    cout << endl;
                }
                else
                    throw Undeclared(ins);
            }
            else
                throw InvalidInstruction(ins);
        }
    }

    // check scope level
    if (levelIndex != 0)
        throw UnclosedBlock(levelIndex);
}

bool SymbolTable::INSERT(string name, string datatype, int level, bool staticFlag) 
{
    Symbol* target = new Symbol(name, datatype, level, staticFlag);
    Symbol* temp = new search(target);

    if(temp->name == name || temp->level == level)
        return false;
    
    insertSymbol(target);
    return true;
}

int SymbolTable::ASSIGN(string name, string value) {
    Scope* pScope = scopeCurrent;
    Symbol* identifier = NULL;

    // find name
    while (pScope != NULL) {
        if (pScope->findSymbol(name))
        {
            identifier = pScope->findSymbol(name);
            break;
        }
        else
            pScope = pScope->getPrev();
    }


    if (identifier == NULL)
        return 3;

    if (isStr(value))
    {
        if (identifier->getDatatype() == "string")
        {
            identifier->adjustData(value);
            return 0;
        }
        else
            return 2;
    }
    else if (isNum(value))
    {
        if (identifier->getDatatype() == "number")
        {
            identifier->adjustData(value);
            return 0;
        }
        else
            return 2;
    }
    else if (isIden(value))
    {
        Symbol* valueIden = findIden(value);

        if (valueIden == NULL)
        {
            return 3;
        }

        if (valueIden->getDatatype() == identifier->getDatatype())
        {
            identifier->adjustData(valueIden->getData());
            return 0;
        }
        else
            return 2;
    }
    else
        return 1;

    /*
    // number or iden
    else if (value.back() != apostrophe && value.front() != apostrophe)
    {
        bool isNumber = true;
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] < numericLowerBound || value[i] > numericUpperBound)
                isNumber = false;
        }

        // num
        if (isNumber)
        {
        }
        // iden?
        else
        {
            Symbol* valueIden = NULL;

            pScope = scopeCurrent;
            while (pScope != NULL)
            {
                if (pScope->findSymbol(value))
                {
                    valueIden = pScope->findSymbol(value);
                    break;
                }
                else
                    pScope = pScope->getPrev();
            }

            // iden
            if (valueIden != NULL)
            {
            }
            // not iden
            else
                return 3;
        }

    }
    // unknown
    else
        return 1;
    */
    /*
    // name not found
    if (identifier == NULL)
        return 3;
    else {
        if (identifier->getDatatype() == "number")
        {
            if (value.back() == apostrophe && value.front() == apostrophe)
                return 2;

            for (int i = 0; i < value.length(); i++)
            {
                if (value[i] >= numericLowerBound && value[i] <= numericUpperBound);
                else
                    return 1;
            }


            identifier->adjustData(value);
            return 0;
        }
        else
        {
            if (value.back() == apostrophe && value.front() == apostrophe)
            {
                for (int i = 1; i < value.length() - 1; i++)
                {
                    if (value[i] >= lowercaseLowerBound && value[i] <= lowercaseUpperBound);
                    else if (value[i] >= numericLowerBound && value[i] <= numericUpperBound);
                    else if (value[i] >= uppercaseLowerBound && value[i] <= uppercaseUpperBound);
                    else if (value[i] == space);
                    else
                        return 1;
                }

                identifier->adjustData(value);
                return 0;
            }
            else
                return 2;

        }
    }

    // find value as identifier
    pScope = scopeCurrent;
    while (pScope != NULL)
    {
        if (pScope->findSymbol(value))
        {
            valueIden = pScope->findSymbol(value);
            break;
        }
        else
            pScope = pScope->getPrev();
    }

    // if value is identifier
    if (valueIden != NULL)
    {
        if (valueIden->getDatatype() == identifier->getDatatype())
        {
            identifier->adjustData(valueIden->getData());
            return 0;
        }
        else
            return 2;
    }
    else
        return 3;
    */
    /*
    while (temp != NULL)
    {
        if (temp->findSymbol(name))
        {
            if (temp->findSymbol(name)->getDatatype() == "number")
            {
                if (value.back() == apostrophe && value.front() == apostrophe)
                    return 2;

                for (int i = 0; i < value.length(); i++)
                {
                    if (value[i] >= numericLowerBound && value[i] <= numericUpperBound);
                    else
                        return 1;
                }


                temp->assignSymbol(name, value);
                return 0;
            }
            else
            {
                if (value.back() == apostrophe && value.front() == apostrophe)
                {
                    for (int i = 1; i < value.length() - 1; i++)
                    {
                        if (value[i] >= lowercaseLowerBound && value[i] <= lowercaseUpperBound);
                        else if (value[i] >= numericLowerBound && value[i] <= numericUpperBound);
                        else if (value[i] >= uppercaseLowerBound && value[i] <= uppercaseUpperBound);
                        else if (value[i] == space);
                        else
                            return 1;
                    }
                    temp->assignSymbol(name, value);
                    return 0;
                }
                else
                    return 2;
            }
        }
        else
            temp = temp->getPrev();
    }
    // cant not find it
    return 3;
    */
}

int SymbolTable::LOOKUP(string name) {
    Scope* temp = scopeCurrent;
    for (int i = levelIndex; i >= 0; i--)
    {
        if (temp->findSymbol(name))
            return i;
        temp = temp->getPrev();
    }

    return -1;
}

string SymbolTable::PRINT() {
    Scope* scopeT = scopeCurrent;
    Scope* temp = new Scope();
    string print = "";

    for (int i = levelIndex; i >= 0; i--)
    {
        // per scope
        Symbol* symbolT = scopeT->getCurrentSymbol();
        while (symbolT != NULL)
        {
            // per symbol in scope
            if (!temp->findSymbol(symbolT->getName()))
            {
                temp->addSymbol(symbolT->getName(), symbolT->getDatatype());
                print = symbolT->getName() + "//" + to_string(i) + " " + print;
            }
            symbolT = symbolT->getPrev();
        }

        scopeT = scopeT->getPrev();
    }

    if (print.length() != 0)
        print.erase(print.length() - 1, 1);

    return print;
}

string SymbolTable::RPRINT() {
    Scope* scopeT = scopeCurrent;
    Scope* temp = new Scope();
    string rprint = "";

    for (int i = levelIndex; i >= 0; i--)
    {
        // per scope
        Symbol* symbolT = scopeT->getCurrentSymbol();
        while (symbolT != NULL)
        {
            // per symbol in scope
            if (!temp->findSymbol(symbolT->getName()))
            {
                temp->addSymbol(symbolT->getName(), symbolT->getDatatype());
                rprint = rprint + symbolT->getName() + "//" + to_string(i) + " ";
            }
            symbolT = symbolT->getPrev();
        }
        scopeT = scopeT->getPrev();
    }

    if (rprint.length() != 0)
        rprint.erase(rprint.length() - 1, 1);

    return rprint;
}

void SymbolTable::BEGIN() {
    levelCurrent++;
}

bool SymbolTable::END() {
    if (levelCurrent != 0)
    {
        //delete every Symbol in levelCurrent
        levelCurrent--;
        
        return true;
    }
    else
        return false;
}

bool SymbolTable::isNum(string str) {
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] < numericLowerBound || str[i] > numericUpperBound)
            return false;
    }
    return true;
}

bool SymbolTable::isStr(string value) {
    if (value.back() == apostrophe && value.front() == apostrophe)
    {
        for (size_t i = 1; i < value.length() - 1; i++)
        {
            if (value[i] >= lowercaseLowerBound && value[i] <= lowercaseUpperBound);
            else if (value[i] >= numericLowerBound && value[i] <= numericUpperBound);
            else if (value[i] >= uppercaseLowerBound && value[i] <= uppercaseUpperBound);
            else if (value[i] == space);
            else
                return false;
        }
        return true;
    }
    else
        return false;
}

bool SymbolTable::isIden(string value) {
    if (value[0] >= lowercaseLowerBound && value[0] <= lowercaseUpperBound)
    {
        for (size_t i = 0; i < value.length(); i++)
        {
            if (value[i] >= lowercaseLowerBound && value[i] <= lowercaseUpperBound);
            else if (value[i] >= uppercaseLowerBound && value[i] <= uppercaseUpperBound);
            else if (value[i] >= numericLowerBound && value[i] <= numericUpperBound);
            else if (value[i] == underscore);
            else
                return false;
        }
        return true;
    }
    else
        return false;
}

bool SymbolTable::isFunc(string data)
{
    string ins = data;
    string temp = "";
    string list = "";
    size_t pos = 0;
    size_t lastComma = 0;
    
    if(ins[0] == '(')
    {
        list = ins.substr(pos + 1, ins.find_first_of(')', pos + 1) - (pos + 1));
        pos = ins.find_first_of(')', pos + 1);
        lastComma = ins.rfind(',');
        
        //cout << list << " " << pos << " " << lastComma << endl;
        
        bool flag = true;
        pos = 0;
        
        while(flag)
        {
            temp = ins.substr(pos + 1, ins.find_first_of(',', pos + 1) - (pos + 1));
            pos = ins.find_first_of(',', pos + 1);
                        
            if(!(temp == "string" || temp == "number" || temp == ""))
            {
                //cout << "false" << endl;
                return false;
            }
            
            //cout << "arguments: " << temp << " " << pos << endl; 
            
            if(pos == lastComma)
            {
                temp = ins.substr(pos + 1, ins.find_first_of(')', pos + 1) - (pos + 1));
                pos = ins.find_first_of(')', pos + 1);
                
                cout << "arguments: " << temp << " " << pos << endl; 
                if(!(temp == "string" || temp == "number" || temp == ""))
                {
                    //cout << "false" << endl;
                    return false;
                }
                
                flag = false;
            }
            
        }
        
        if(ins.substr(pos + 1, 2) == "->")
        {
            pos += 2;
            temp = ins.substr(pos + 1, ins.find_first_of('\n', pos + 1) - (pos + 1));
            pos = ins.find_first_of('\n', pos);
            
            if(!(temp == "string" || temp == "number"))
            {
                //cout << "false" << endl;
                return false;
            }
            else
            {
                //cout << "output: " << temp << " " << pos << endl;
            }
        }
    }
    else
    {
        //cout << false;
        return false;
    }

    return true;
}

SymbolTable::Symbol* SymbolTable::isMax(Symbol* root)
{
    while(root->right != NULL)
    {
        root = root->right;
    }
    return root;
}

SymbolTable::Symbol* SymbolTable::findIden(string name) {
    Symbol* iden = NULL;
    Scope* tmp = scopeCurrent;

    while (tmp != NULL)
    {
        if (tmp->findSymbol(name))
        {
            iden = tmp->findSymbol(name);
            break;
        }
        else
            tmp = tmp->getPrev();
    }

    return iden;
}

SymbolTable::Symbol::Symbol(string name, string datatype, bool staticFlag, int level) {
    this->name = name;
    this->datatype = datatype;
    this->staticFlag = staticFlag;
    this->level = level;
    left = NULL;
    right = NULL;
    parent = NULL;
    data = "";
}

void SymbolTable::Symbol::adjustName(string name) {
    this->name = name;
}

void SymbolTable::Symbol::adjustDatatype(string datatype) {
    this->datatype = datatype;
}

void SymbolTable::Symbol::adjustData(string data) {
    this->data = data;
}

void SymbolTable::Symbol::adjustParent(Symbol* symbol){
    parent = symbol;
}

void SymbolTable::Symbol::adjustRight(Symbol* symbol) {
    right = symbol;
}

void SymbolTable::Symbol::adjustLeft(Symbol* symbol)
{
    left = symbol;
}

string SymbolTable::Symbol::getName()
{
    return this->name;
}

string SymbolTable::Symbol::getDatatype()
{
    return this->datatype;
}

string SymbolTable::Symbol::getData()
{
    return this->data;
}

SymbolTable::Symbol* SymbolTable::Symbol::getParent()
{
    return parent;
}

SymbolTable::Symbol* SymbolTable::Symbol::getLeft()
{
    return left;
}

SymbolTable::Symbol* SymbolTable::Symbol::getRight()
{
    return right;
}

//splay tree

void SymbolTable::rightRotate(Symbol* target)
{
    Symbol* temp = target->left;
    target->left = temp->right;
    
    if(temp->right != NULL)
    {
        temp->right->parent = target;
    }
    
    temp->parent = target->parent;
    if(target->parent == NULL) //target is root
    {
        head = temp;
    }
    else if(target == target->parent->right) // target is left child
    {
        target->parent->right = temp;
    }
    else //target is right child
    {
        target->parent->left = temp;
    }

    temp->right = target;
    target->parent = temp;
}

void SymbolTable::leftRotate(Symbol* target)
{
    Symbol* temp = target->right;
    target->right = temp->left;
    
    if(temp->left != NULL)
    {
        temp->left->parent = target;
    }
    
    temp->parent = target->parent;
    if(target->parent == NULL) // target is root
    {
        head = temp;
    }
    else if(target == target->parent->left) // target is left child
    {
        target->parent->left = temp;
    }
    else // target is right child
    {
        target->parent->right = temp; 
    }

    temp->left = target;
    target->parent = temp;
}

bool SymbolTable::keyCompare(Symbol* root, string name, int level) //0: root < target; 1: root > target 
{
    //string compare
}

void SymbolTable::splay(Symbol* root, Symbol* target)
{
    while(target->parent != NULL) //target is not root
    {
        if(target->parent == root) //target is child of root, one rotation
        {
            if(target == target->parent->left)
            {
                rightRotate(target->parent); 
            }
            else 
            {
                leftRotate(target->parent)
            }
        }
        else 
        {
            Symbol* p = target->parent; //temp parent
            Symbol* g = p->parent; //temp grandparent

            if((target->parent->left == target) && (p->parent->left == p))
            {
                rightRotate(g);
                rightRotate(p);
            }
            else if((target->parent->right == target) && (p->parent->right == p))
            {
                leftRotate(g);
                leftRotate(p);
            }
            else if((target->parent->right == target) && (p->parent->left == p))
            {
                leftRotate(p);
                rightRotate(g);
            }
            else if((target->parent->left == target) && (p->parent->right == p))
            {
                rightRotate(p);
                leftRotate(g);
            }
        }
    }
}


void SymbolTable::insertSymbol(Symbol* target)
{
    Symbol* y = NULL;
    Symbol* temp = head;
    
    while(temp != NULL)
    {
        y = temp;
        //if(target->data < temp->data)
        if(!(keyCompare(target,temp)))
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    target->parent = y;

    if(y == NULL) // newly added Symbol is root
    {
        head = target;
    }
    else if(!(keyCompare(target,y)))
    {
        y->left = target;
    }
    else
    {
        y->right = target;
    }

    splay(head, target);
}

void SymbolTable::deleteSymbol(Symbol *target)
{
    splay(head, target);

    Symbol* leftSubtree = head->left;
    Symbol* rightSubtree = head->right;

    if(leftSubtree == NULL)
    {
        leftSubtree->parent = NULL;
    }

    if(rightSubtree == NULL)
    {
        rightSubtree->parent = NULL;
    }

    free(target);

    if(leftSubtree != NULL)
    {
        Symbol* max = isMax(leftSubtree);
        splay(leftSubtree, max);
        leftSubtree->right = rightSubtree;
        head = leftSubtree;
    }    
    else
    {
        head = rightSubtree;
    }
}

SymbolTable::Symbol* SymbolTable::search(Symbol* temp, Symbol* target)
{
    if(temp->name == target->name)
    {
        return temp;
    }
    else if(keyCompare(target, temp))
    {
        return serach(temp->left, target);
    }
    else if(!(keyCompare(target,temp)))
    {
        return search(temp->right, target);
    }
    else 
    {
        return NULL;
    }
}
