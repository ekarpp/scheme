### About ###
An interpreter for a Scheme-like language written in C. Done as a personal project during a university course regarding compilers. Base Scheme library implemented with test-driven development; once the front-end was working, test for a desired feature is written in Scheme and then the interpreter is modified until all tests pass.

The interpreter follows the Scheme [R7RS](https://small.r7rs.org/attachment/r7rs.pdf) specification but it does not fulfill all of the requirements (only supports 64 bit integers, no tail call optimization, some base library functions not implemented or implementation does not follow the specification).

### Features ###
* Lexer
* Parser
* Semantic analysis (during base library function calls)
* Arithmetic
* Boolean algebra
* Variables
* Recursive function calls
* Lambda functions
  * Still have bugs
