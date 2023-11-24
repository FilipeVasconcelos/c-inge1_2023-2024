#ifndef SGF_H_
#define SGF_H_
extern char sgfilename[MAX_LEN];
extern bool readingfromfile;
void read_sgfile(char* sgfilename, Board board, Stone stones[], size_t* played);
#endif /* SGF_H_ */
