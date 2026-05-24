#include "PatternMatching.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <tuple>
using namespace std;

void TestSingle() {
    std::ostringstream out;
    variant<int, float> a(4.5f);
    a | PatternMatch{
        [&out](int val) { out << "Integer " << val << '\n'; },
        [&out](float val) { out << "Float " << val << '\n'; }
    };
    if (out.str() != "Float 4.5\n") throw runtime_error("Failed TestSingle");
}

void TestDouble() {
    std::ostringstream out;
    variant<int, float> a(4.5f);
    variant<string, bool> b("hello");
    if ((make_tuple(a, b) | PatternMatch{
        [](int va, string vb) -> string {
            return "Integer with string";
        },
        [](int va, bool vb) -> string {
            return "Integer with bool";
        },
        [](float va, string vb) -> string {
            return "Float with string";
        },
        [](float va, bool vb) -> string {
            return "Float with bool";
        }
    }) != "Float with string")
        throw runtime_error("Failed TestDouble");
}

void TestAuto() {
    struct A{};
    struct B{};
    struct C{};
    using ABC = variant<A, B, C>;
    PatternMatch vis{
        [](A&&) { return "A"; },
        [](auto&&) { return "default"; }
    };
    if ((make_tuple(ABC(A())) | vis) != string("A"))
        throw runtime_error("Failed A() in TestAuto");
    if ((make_tuple(ABC(B())) | vis) != string("default"))
        throw runtime_error("Failed B() in TestAuto");
}

void TestDoubleAuto() {
    struct A{};
    struct B{};
    struct C{};
    struct D{};
    struct E{};
    using ABC = variant<A, B, C>;
    using DE = variant<D, E>;
    PatternMatch vis{
        [](A&&, D&&) { return "A and D"; },
        [](A&&, E&&) { return "A and E"; },
        [](B&&, auto&&) { return "B and something"; },
        [](C&&, D&&) { return "C and D"; },
        [](auto&&...) { return "something else"; }
    };
    if ((make_tuple(ABC(A()), DE(D())) | vis) != string("A and D"))
        throw runtime_error("Failed A() and D() in TestDoubleAuto");
    if ((make_tuple(ABC(C()), DE(E())) | vis) != string("something else"))
        throw runtime_error("Failed C() and E() in TestDoubleAuto");
    if ((make_tuple(ABC(B()), DE(E())) | vis) != string("B and something"))
        throw runtime_error("Failed B() and E() in TestDoubleAuto");
}

int main() {
    TestSingle();
    TestDouble();
    TestAuto();
    TestDoubleAuto();
}
