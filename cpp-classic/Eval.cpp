#include "Absyn.H"
#include "Printer.H"
#include "Parser.H"
#include <set>
#include <iostream>
#include <string>

using namespace std;

set<string> usedNames;

bool isdigit(char ch) {
    return '0' <= ch && ch <= '9';
}

void incrementNumberSuffix(string& name) {
    int i = (int)name.size() - 1;
    bool carry = true;
    while (carry && i >= 0) {
        if (!isdigit(name[i])) break;
        if (name[i] == '9') {
            name[i] = '0';
            --i;
            continue;
        }
        ++name[i];
        carry = false;
    }
    if (carry) name.insert(name.begin() + (i + 1), '1');
}

string UniqueRename(string name) {
    while (usedNames.count(name))
        incrementNumberSuffix(name);
    usedNames.insert(name);
    return name;
}

int main(int argc, char** argv) {
    string name;
    getline(cin, name);
    usedNames.insert(name);
    while (true) {
        name = UniqueRename(name);
        cout << name;
        cin.ignore();
    }
}
