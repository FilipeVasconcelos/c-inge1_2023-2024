#include<stdlib.h>
// return line index 0 to N-1 
// from char c, 'a' is the reference  
size_t sgftoline(char c){
    return (size_t) c - (size_t) 'a';
}
