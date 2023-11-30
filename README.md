# C minus compiler

College project on the compiler course. Focus on developing the
analysis phase of a compiler (lexical analysis, syntax analysis and
semantic analysis) of a subset of C language, the C-

## C minus lexical conventions

Here it is defined the lexical conventions of the C- language. The
fixed lexemes are listed below

```
keywords: else if int return void while
special symbols: + - * / < <= > >= == != = ; , () [] {} /* */
```

Lexemes can also be classified as identifiers or numbers, those two
being defined as

```
ID = letter letter*
NUM = digit digit*

digit = [a-zA-Z]
digit = [0-9]
```

## C minus sintatic conventions

Here it is defined the sintatic conventions of the C- language. The
contex-free grammar on BNF is listed bellow. Note that <NUMBER> and
<IDENTIFIER> are tokens with semantic value, all other expressions
inside "<>" should be interpreted as non-terminals

```
<program>: <declaration-list>

<declaration-list>: <declaration-list> <declaration>
                  | <declaration>

<declaration>: <var-declaration>
             | <fun-declaration>

<var-declaration>: <type-specifier> <IDENTIFIER>;
                 | <type-specifier> <IDENTIFIER>[<NUMBER>];

<type-specifier>: int
                | void

<fun-declaration>: <type-specifier> <IDENTIFIER>(<params>) <composed-declaration>

<params>: <param-list>
        | void

<param-list>: <param-list>, <param>
            | <param>

<param>: <type-specifier> <IDENTIFIER>
       | <type-specifier> <IDENTIFIER>[]

<composed-declaration>: {<local-declarations> <statement-list>}

<local-declarations>: <local-declarations> <var-declaration>
                    | %epsilon

<statement-list>: <statement-list> <statement>
                | %epsilon

<statement>: <expression-declaration>
           | <composed-declaration>
           | <selection-declaration>
           | <iteration-declaration>
           | <return-declaration>

<expression-declaration>: <expression>;
                        | ;

<selection-declaration>: if(<expression>) <statement>
                       | if(<expression>) <statement> else <statement>

<iteration-declaration>: while(<expression>) <statement>

<return-declaration>: return;
                    | return <expression>;

<expression>: <var> = <expression>
            | <simple-expression>

<var>: <IDENTIFIER>
     | <IDENTIFIER>[<expression>]

<simple-expression>: <sum-expression> <relational> <sum-expression>
                   | <sum-expression>

<relational>: <=
            | <
            | >
            | >=
            | ==
            | !=

<sum-expression>: <sum-expression> <sum> <term>
                | <term>

<sum>: +
     | -

<term>: <term> <multiplication> <term>
      | <factor>

<multiplication>: *
                | /

<factor>: (<expression>)
        | <var>
        | <activation>
        | <NUMBER>

<activation>: <IDENTIFIER>(<args>)

<args>: <arg-list>
      | %epsilon

<arg-list>: <arg-list>, <expression>
          | <expression>

```
