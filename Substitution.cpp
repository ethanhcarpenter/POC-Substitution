#include <iostream>
#include <string>
#include <chrono>
#include "Lexer.h"
using namespace std;
using namespace chrono;
////define Variable class
//class vb {
//private:
//    char name;
//public:
//    //initialise input variable to property
//    vb(char n) :name(n) {}
//    //return name of variable
//    char getName() { return name; }
//};
////define Abstraction class
//class Abstraction {
//private:
//    vb* bound;
//    string expression;
//public:
//    //initialise input variables to properties
//    Abstraction(vb* b, string e) :bound(b), expression(e) {}
//    //return the variable pointer
//    vb* getBoundVariable() { return bound; }
//    //return the string expression
//    string getExpression() { return expression; }
//};
//class Application {
//private:
//    Abstraction* function;
//    string input;
//public:
//    //initialise input variables to properties
//    Application(Abstraction* f, string i) :function(f), input(i) {}
//    string solve() {
//        //get the variable name
//        char var = function->getBoundVariable()->getName();
//        //get the expression
//        string expr = function->getExpression();
//        //create empty string
//        string iterationStr = "";
//        //loop through each letter of the expression
//        for (int i = 0; i < expr.length(); i++) {
//            //if the current char is the variable replace it with the input in the answer string
//            //add parenthesis around input
//            if (expr[i] == var) { iterationStr += "(" + input + ")"; }
//            //otherwise keep as is
//            else { iterationStr += expr[i]; }
//        }
//        //return answer
//        return iterationStr;
//    }
//};
////create variable 
//vb c = { 'c' };
//Abstraction f1 = { &c,"c*a+4-b" };
//Application eq1 = { &f1,"5" };

//vb b = { 'b' };
//Abstraction f2 = { &b,eq1.solve() };
//Application eq2 = { &f2,"1" };

//vb a = { 'a' };
//Abstraction f3 = { &a,eq2.solve() };
//Application eq3 = { &f3,"4" };

//string solvedEq = eq3.solve();

////output answer
//cout << solvedEq << endl;
//std::cout << "\n";
////lexer
int main()
{//{LAMBDA g. LAMBDA x. g x}
    std::string expression = "[LAMBDA n. LAMBDA f. LAMBDA x. f(n f x)]abcedf";

    Parser parser = {};
    parser.getLexer()->tokenize(expression);
    parser.evaluate();
    
    int a = 12;





}
