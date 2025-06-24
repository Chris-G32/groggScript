#ifndef GRAMMAR_NODE_HPP
#define GRAMMAR_NODE_HPP
#include <vector>
using std::vector;
class GrammarVisitor{
    
}
class GrammarNode
{
protected:
    GrammarNode *parent;
    vector<GrammarNode *> children;
};
#endif