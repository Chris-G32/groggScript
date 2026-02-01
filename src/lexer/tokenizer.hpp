// #ifndef TOKENIZER_HPP
// #define TOKENIZER_HPP
// #include <sstream>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <iterator>
// #include "tokens.hpp"
//
// using GroggScript::Token;
// using GroggScript::TokenType;
// using std::string;
// using std::vector;
// namespace GroggScript
// {
//
//     class Tokenizer
//     {
//     public:
//         Tokenizer(const string &rawString) : _rawString(rawString), _it(_rawString.cbegin()),
//         _end(_rawString.cend()) {}; void generateTokens(); const vector<Token> getTokens() const
//         { return _tokens; } void debugPrint(std::ostream &out = std::cout)
//         {
//             for (const auto &token : _tokens)
//             {
//                 out << "TokenType: " << tokenTypeToString(token.token) << ". Stores: " <<
//                 token.value << '\n';
//             }
//         }
//         void printTags(std::ostream &out = std::cout, bool tagOnly = false)
//         {
//
//             for (const auto &token : _tokens)
//             {
//                 auto stringRepr = tokenTypeToString(token.token);
//                 if (!tagOnly)
//                 {
//                     out << '<' << stringRepr << '>' << token.value;
//                 }
//                 out << '<' << stringRepr << "/>" << '\n';
//             }
//             out << '\n';
//         }
//         string getLinePos()
//         {
//             string output = "Ln " + std::to_string(line) + ", Col " + std::to_string(col);
//             return output;
//         }
//
//     protected:
//         const string _rawString;
//
//         int line = 1;
//         int col = 1; // Mostly here for logs
//         void newLine()
//         {
//             line++;
//             col = 1;
//         }
//         std::string::const_iterator _it;
//         std::string::const_iterator _end;
//         char advance()
//         {
//             std::advance(_it, 1);
//             if (*_it == '\n')
//             {
//                 newLine();
//             }
//             else
//             {
//                 col++;
//             }
//             return *_it;
//         }
//         char peek() const
//         {
//             const auto next = std::next(_it);
//             if (next == _end)
//             {
//                 return '\0';
//             }
//             return *next;
//         }
//         bool nextMatches(const char match) const
//         {
//             return peek() == match;
//         }
//
//         void addDependingOnNext(const char expectedNext, TokenType nextDidntMatch, TokenType
//         nextMatchedToken)
//         {
//             if (nextMatches(expectedNext))
//             {
//                 _tokens.push_back({nextMatchedToken, string({*_it, expectedNext})});
//                 std::advance(_it, 2);
//                 return;
//             }
//             _tokens.push_back({nextDidntMatch, string(1, *_it)});
//             std::advance(_it, 1);
//         }
//         void addOneOrDouble(const char value, TokenType singleToken, TokenType doubleToken)
//         {
//             addDependingOnNext(value, singleToken, doubleToken);
//         }
//         std::vector<Token> _tokens;
//     };
// }
// #endif