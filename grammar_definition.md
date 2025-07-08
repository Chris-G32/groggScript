# BNF Grammar Definition
## Notation
- Parentheses denote an optional thing
- Brackets can denote that we must pick between one of the things, it is also sometimes used in a regex manner, like for ranges of chars
- + is like regex and so is *





# Basic Components
dot: '.'
colon: ':'
open_bracket: '['
close_bracket: ']'
alphabetical: [a-zA-Z]
numeric: [0-9]
alnum: alphabetical | numeric
symbol_components: [ _ | alnum ]+
symbol: 
| alphabetical+
| alphabetical alnum+
| '_' alphabetical alnum*

operator: '+' | '-' | '*' | '/'
assignment: '='
infer_type: 'var'
primitive_type: 
|'string' 
| 'int' 
| 'float'
| 'boolean' 

integer: numeric
float: numeric+ dot numeric*
boolean: 'true' | 'false'
// Strings can be any character, in practice \0 is always going to terminate the string
string: *
primitive_value: 
| integer
| float 
| boolean
| string

type_name: primitive_type | symbol
type_specifier: colon type_name
expression:
| [symbol | primitive_value] operator expression 
| [symbol | primitive_value]
variable_assignment: symbol assignment expression
var_declaration: 
| infer_type symbol (type_specifier) assignment expression // Type specifier must match the primitive or symbols type
| infer_type symbol type_specifier

statement: 
| var_declaration 
| variable_assignment
| expression
statements: statement*

a + b + c / 2 + b / c
prog:
statements EOF

var a:string = "abcd" + "123"
a = "123"+ "abcx" / "123"

statements
    statement
        var_declaration
            infer_type 
            symbol 
            type_specifier
                colon
                type_name
                    primitive_type
            assignment
            expression
                primitive_value
                operator
                primitive_value
    statement
        variable_assignment
            symbol
            assignment
            expression
                primitive_value
                operator
                expression
                    primitive_value
                    operator
                    expression
                        primitive_value

# AST Construction Pseudocode
tokens= getTokens();
parse_statements:
    while not end of tokens
        attempt_parse_statement

attempt_parse_statement:
    while not end of tokens
        if(attempt_parse_var_decl)
            return result
        else if (attempt_parse_variable_assignment)
            return result
        else if (attempt_parse_expression)
            return result
        else
            ERROR

attempt_parse_var_decl:
    if(current_token is not infer_type )
        return fail
    current_token=next_token
    if current_token is not symbol
        throw expected_a_symbol_name
    current_token=next_token
    if current_token is not type_specifier
        if(next_token is not assignment)
            throw must_initialize_inferred_type_variables
        current_token=next_token
        return attempt_parse_expression
    return non_initialized_variable

symbol operator symbol operator primitive
attempt_parse_expression:
    if current token is not symbol or primitive value
        return fail
    current_token=next_token
    if current token is not operator // technically needs an attempt parse thing
        return simple_expression
        current_token=next_token
    if(attempt_parse_expression is fail)
        throw invalid expression
    return attempt_parse_expression
    
