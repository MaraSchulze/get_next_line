# get_next_line
get_next_line is a function that reads from a file descriptor line by line or until EOF. The 3 files are get_next_line.c, get_next_line_utils.c, get_next_line.h.  
The bonus reads from several fds and only uses one static variable. The 3 bonus files have extension "_bonus".  
The project description is in the pdf file.  

## I passed the project with 125%
<img src="ScreenshotSuccess.png" alt="Success Photo" width="200">

## Usage
This project was written for and compiled on a Mac.  
Include the header file:
```
#include "get_next_line.h"
```
Then compile
```
gcc main.c get_next_line.c get_next_line_utils.c
```

## Caveats
This project was a 42School project, the exact description being in the pdf. We had to program according to a strict Norm, for instance using only while loops, having only 25 lines per function, not using inline comments etc.
