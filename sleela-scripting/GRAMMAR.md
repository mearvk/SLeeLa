# Sleela Script Grammar

This document gives the compact 1.0 grammar. Whitespace and comments are
ignored except inside strings.

```ebnf
program      = { declaration | statement } ;

declaration  = script_decl | function_decl ;
script_decl  = "script" string ;

function_decl = "fn" identifier "(" [ parameters ] ")" block ;
parameters   = identifier { "," identifier } ;

statement    = let_stmt | const_stmt | assignment | if_stmt | while_stmt
             | for_stmt | return_stmt | break_stmt | continue_stmt
             | expression_stmt | block ;

let_stmt     = "let" identifier "=" expression ;
const_stmt   = "const" identifier "=" expression ;
assignment   = identifier "=" expression ;

if_stmt      = "if" expression block [ "else" block ] ;
while_stmt   = "while" expression block ;
for_stmt     = "for" identifier "in" expression block ;

return_stmt  = "return" [ expression ] ;
break_stmt   = "break" ;
continue_stmt = "continue" ;

block        = "{" { statement } "}" ;
expression_stmt = expression ;

expression   = logical_or ;
logical_or   = logical_and { "or" logical_and } ;
logical_and  = equality { "and" equality } ;
equality     = comparison { ("==" | "!=") comparison } ;
comparison   = term { ("<" | "<=" | ">" | ">=") term } ;
term         = factor { ("+" | "-") factor } ;
factor       = unary { ("*" | "/" | "%") unary } ;
unary        = [ "not" | "-" ] primary ;
primary      = literal | identifier | call | list | map
             | "(" expression ")" ;

call         = identifier "(" [ arguments ] ")" ;
arguments    = expression { "," expression } ;

list         = "[" [ arguments ] "]" ;
map          = "{" [ map_entry { "," map_entry } ] "}" ;
map_entry    = expression ":" expression ;

literal      = number | string | "true" | "false" | "null" ;
```

Implementations may add qualified host calls such as `fs.read(...)` while
preserving the core grammar through member-qualified identifiers.
