# CALCULATOR WITH SOCKETS
---
Description of the supported operations:

| Function    | Description     | Input    | Example   | Output|
| :------------- | :------------- | :------------- | :------------- | :------------- |
| add       | a + b       | add a b       |   add 5 7       | 12        |
| sub       | a - b       | sub a b       |   sub 5 7       | -2        |
| mult      | a * b       | mult a b       |   mult 4 10       | 40        |
| div       | a / b       | div a b       |   div 32 4       | 8         |
| sqrt      | (a)^1/2     | sqrt a        |   sqrt 25        | 5         |
| exp       | e^n         | exp a         |   exp 5         | 148       |
| mmult     | [[matA]] * [[matB]] | mmult [[a b c][d e f]] [[g h][i j][k l]] | mmult [[2 0 1][3 0 0][5 1 1]] [[1 0 1][1 2 1][1 1 0]]| [[3 1 2][3 0 3][7 3 6]]|
| mdet       | \|matA\|      | mdet [[a b c][d e f]]         |   mdet [[3 4 -7 6][1 2 -3 4][5 6-7 5][-8 -9 1 2]] | 8       |
| minv       | [[matA]]^-1         | minv [[a b c][d e f]] |  minv [[1 2][-1 3]]   | [[0.600000 -0.400000][0.200000 0.200000]] |

## Executing

Type in your terminal:

      make

    ./server

In other terminal:

      ./client add 5 7

Solution to possible compiling error:

      export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/utp/zmq/lib
