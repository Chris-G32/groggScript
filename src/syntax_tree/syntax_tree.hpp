// #pragma once
// #include <vector>
// #include "tokenizer.hpp"
// #include "grammar_node.hpp"
// #include "program.hpp"
// class SyntaxTree
// {
// public:
//     SyntaxTree(const std::vector<Token> &tokens) : _tokens(std::move(tokens))
//     {
//     }

// protected:
//     void load()
//     {
//         TokenStream it = _tokens.cbegin();
//         head = new ProgramNode();
//         while (it != _tokens.cend())
//         {
//             head->parse();
//         }
//     }
//     void dispose() {}

// protected:
//     ProgramNode *head = nullptr;
//     const std::vector<Token> _tokens;
//     void parseTree() {}
// };