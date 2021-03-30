#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h>


int eval(char c){
  if(c == 't' || c == 'l' || c == 'n' || c == 'u' || c == 'm' || c == 'd' || c == 'a')
    return 1;
  return 0;
}