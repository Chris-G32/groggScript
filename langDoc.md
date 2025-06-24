
# Reserved Symbols
## Operators
### Math
- Addition: +
- Subtraction: -
- Division: /
- Multiplication: *
- Assignment: =
### Boolean
- Equality: is
- Negate: not
- Or: or
- Xor: either 
- And: and
## Type Keywords
- 
# Variables
## Primitives
- string: String values
- int | uint: only integer values
- float: decimal values
- bool: true or false
## Components
### Variable Declaration
Simple, variables do not require an initializer. camelCase preferred for variable names.
`(var | typespecifier) variableName [ = <value> ]`
### Access Specifiers
#### Literal
The `literal` specifier declares that this is not a variable, but rather a config like constant to be substituted during bundling process. Literals only support type inference and known compile time primitives. Convention says these should be all caps
##### Syntax
`literal MY_LITERAL_NAME = <primitive_value>`
#### Immutable
The `immutable` specifier declares that this symbol is to be assigned EXACTLY once. It only supports operations that do not change its state. Effectively getters only. It may be assigned after initial declaration, strictly syntactic sugar. These default to null if never assigned. Convention is to name these with a capital first.
##### Syntax
`immutable (var | typespecifier) VariableName [ = <value> ]`
## Default Passing Mechanism
