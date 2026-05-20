#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

#include "Absyn.H"
#include "Parser.H"
#include "Printer.H"

using namespace std;

set<string> usedNames;

bool isdigit(char ch) { return '0' <= ch && ch <= '9'; }

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
    while (usedNames.count(name)) incrementNumberSuffix(name);
    usedNames.insert(name);
    return name;
}

bool verbose = false;

class VCheckNames : public LC::Visitor {
    map<string, int> names;

public:
    bool Error = false;
    void visitProgram(LC::Program* p) override {};
    void visitExpr(LC::Expr* p) override {};
    void visitInteger(LC::Integer x) override {};
    void visitChar(LC::Char x) override {};
    void visitDouble(LC::Double x) override {};
    void visitString(LC::String x) override {};
    void visitIdent(LC::Ident x) override {};

    void visitListExpr(LC::ListExpr* p) override {
        for (LC::Expr* e : *p) e->accept(this);
    }

    void visitAProgram(LC::AProgram* p) override { p->listexpr_->accept(this); }

    void visitAbstraction(LC::Abstraction* p) override {
        names[p->ident_]++;
        usedNames.insert(p->ident_);
        p->expr_->accept(this);
        auto iter = names.find(p->ident_);
        if (--iter->second == 0) names.erase(iter);
    }

    void visitApplication(LC::Application* p) override {
        p->expr_1->accept(this);
        p->expr_2->accept(this);
    }

    void visitVariable(LC::Variable* p) override {
        if (names.count(p->ident_) == 0) {
            Error = true;
            cerr << "Variable " << p->ident_ << " undefined" << endl;
        }
    }
};

class VRename : public LC::Visitor {
    string what, into;

public:
    VRename(const std::string& what) : what(what), into(UniqueRename(what)) {}

    void visitProgram(LC::Program* p) override {};
    void visitExpr(LC::Expr* p) override {};
    void visitInteger(LC::Integer x) override {};
    void visitChar(LC::Char x) override {};
    void visitDouble(LC::Double x) override {};
    void visitString(LC::String x) override {};
    void visitIdent(LC::Ident x) override {};

    void visitListExpr(LC::ListExpr* p) override {
        for (LC::Expr* e : *p) e->accept(this);
    }

    void visitAProgram(LC::AProgram* p) override { p->listexpr_->accept(this); }

    void visitAbstraction(LC::Abstraction* p) override {
        if (p->ident_ != what) p->expr_->accept(this);
    }

    void visitApplication(LC::Application* p) override {
        p->expr_1->accept(this);
        p->expr_2->accept(this);
    }

    void visitVariable(LC::Variable* p) override {
        if (p->ident_ == what) p->ident_ = into;
    }
};

// Consumes arguments; duplicates `into`
class VSubstitute : public LC::Visitor {
    string what;
    LC::Expr* into;

public:
    LC::Expr* Result = nullptr;

    VSubstitute(const string& what, LC::Expr* into)
        : what(what), into(into) {}

    void visitProgram(LC::Program* p) override {}
    void visitExpr(LC::Expr* p) override {}
    void visitInteger(LC::Integer x) override {}
    void visitChar(LC::Char x) override {}
    void visitDouble(LC::Double x) override {}
    void visitString(LC::String x) override {}
    void visitIdent(LC::Ident x) override {}
    void visitListExpr(LC::ListExpr* p) override {}
    void visitAProgram(LC::AProgram* p) override {}

    void visitAbstraction(LC::Abstraction* p) override {
        Result = p;
        if (p->ident_ == what) return;
        LC::Expr* newInto = into->clone();
        {
            VRename renamer(p->ident_);
            newInto->accept(&renamer);
        }
        {
            VSubstitute subst(what, newInto);
            p->expr_->accept(&subst);
            p->expr_ = subst.Result;
        }
        delete newInto;
    }

    void visitApplication(LC::Application* p) override {
        Result = p;
        p->expr_1->accept(this);
        p->expr_1 = Result;
        p->expr_2->accept(this);
        p->expr_2 = Result;
    }

    void visitVariable(LC::Variable* p) override {
        if (p->ident_ != what) {
            Result = p;
            return;
        }
        Result = into->clone();
        delete p;
    }
};

// consumes arguments
class VEvaluate : public LC::Visitor {
public:
    LC::Expr* Result = nullptr;

    void visitProgram(LC::Program* p) override {}
    void visitExpr(LC::Expr* p) override {}
    void visitInteger(LC::Integer x) override {}
    void visitChar(LC::Char x) override {}
    void visitDouble(LC::Double x) override {}
    void visitString(LC::String x) override {}
    void visitIdent(LC::Ident x) override {}
    void visitListExpr(LC::ListExpr* p) override {}
    void visitAProgram(LC::AProgram* p) override {}

    void visitAbstraction(LC::Abstraction* p) override {
        Result = p;
    }

    void visitApplication(LC::Application* p) override {
        p->expr_1->accept(this);
        LC::Expr* _func = Result;
        Result = p->expr_1 = nullptr;
        LC::Abstraction* func = dynamic_cast<LC::Abstraction*>(_func);
        if (!func)
            throw runtime_error("Applying a non-function");
        p->expr_2->accept(this);
        LC::Expr* arg = Result;
        Result = p->expr_2 = nullptr;
        delete p;
        VSubstitute subst(func->ident_, arg);
        func->expr_->accept(&subst);
        Result = subst.Result;
        func->expr_ = nullptr;
        delete func;
    }

    void visitVariable(LC::Variable* p) override {
        Result = p;
    }
};

int main(int argc, char** argv) {
    if (argc == 0) return 1;
    vector<char*> files;
    bool needhelp = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == 0) continue;
        if (argv[i][0] != '-') {
            files.push_back(argv[i]);
            continue;
        }
        if (!strcmp(argv[i] + 1, "v") || !strcmp(argv[i] + 1, "verbose"))
            verbose = true;
        else if (!strcmp(argv[i] + 1, "h") || !strcmp(argv[i] + 1, "help"))
            needhelp = true;
        else {
            cerr << "Unrecognized option: " << argv[i] << endl;
            return 1;
        }
    }
    if (needhelp) {
        cerr << "Usage: " << argv[0] << " [-v|-verbose] [-h|-help] files...\n"
             << endl;
    }
    if (files.empty() && !needhelp) {
        cerr << "Nothing to do. Execute '" << argv[0] << " -h' for usage"
             << endl;
        return 0;
    }

    VEvaluate eval;
    LC::PrintAbsyn printer;
    for (char* file : files) {
        FILE* f = fopen(file, "r");
        if (!f) {
            perror(("Could not open file " + string(file)).c_str());
            continue;
        }
        cerr << "Entering " << file << endl;
        LC::AProgram* prog = static_cast<LC::AProgram*>(LC::pProgram(f));
        for (auto& expr : *prog->listexpr_) {
            usedNames.clear();
            {
                VCheckNames check;
                expr->accept(&check);
                if (check.Error) continue;
            }
            expr->accept(&eval);
            expr = eval.Result;
            eval.Result = nullptr;
            cout << printer.print(expr) << '\n' << endl;
        }
    }
}
