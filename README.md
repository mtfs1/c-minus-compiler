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

