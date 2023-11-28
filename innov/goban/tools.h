#ifndef TOOLS_H_
#define TOOLS_H_
#define MAX_LEN 300
//Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// return line index 0 to N-1 
// from char c, 'a' is the reference  
size_t sgftoline(char c);
#endif /* TOOLS_H_ */
