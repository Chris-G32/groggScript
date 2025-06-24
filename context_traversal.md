# Traverse

NODE A: ACCEPTS NODES B AND C AN

1: TYPE_SPEC 
2: var_decl_str
3: VAR
4: EQUALS
5: SEMICOLON
6: DEFINED_SYMBOL
7: PRIMITIVE VALUE
8: FUNCTION_RETURNING_VALUE

VARIABLE DECL: (1 2 4 6|3) (3 2 2 4)

ALPHABET:
TYPE KEYWORD: string, int, bool, float
OPERATOR : =, -, +, /, %
STRING_

# SYNTAX
() is optional entirely
[] one of these contained options
! blacklist
+ at least 1
* 0 or more
<statement>
<int>=> [0-9+]
<float>=> [0-9].([0-9])
<string>"" contains basically anything you can type, " escaped by \, cant contain
<boolean> [true | false]
<primitive_value> => [<int>|<float>|<string>|<boolean>]

# Symbol name is used for function naming and custom types, basically anything user defined

<symbol_name> => _[_a-Z0-9]+ | [a-Z0-9][_a-Z0-9]*  
<type_declaration> => <primitive_value> | <symbol_name>
<variable_declaration> => 
<type_specifier> <variable_name> (<equals_symbol> <primitive_value>|)
<var_keyword> <variable_name> 
<function_keyword>=> function
<function_returns_keyword> ->
<function_arg_def>
<function_param_def> ( )
<function_call> => <function_keyword> <symbol_name>

# Use cases
int a = 1;
string _2a = "sS\"\"\"+_)(*&^%$#@!~`1234567890-=)"
boolean _A2 =f alse
var val=""
var val2=someVal

var val3 =otherIntVal + 1
int intVal = integerFunction(val,1,"")
if(intval is 1 )
{

}