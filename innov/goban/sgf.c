void readFile( const char * filename ) {
    int returnCode;
    int count;

    FILE * stream = fopen( filename, "r" );
    if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for reading\n" );
        exit( -1 );
    }

    printf( "How many blocks to read: " );
    scanf( "%d", &count );
    {
        char buffer[ BLOCK_SIZE * count + 1];
        if ( count != fread( buffer, BLOCK_SIZE, count, stream ) ) {
            fprintf( stderr, "Cannot read blocks in file\n" );
        }
        buffer[ BLOCK_SIZE * count ] = '\0';
        printf( buffer );
    }
    returnCode = fclose( stream );
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }
}



int read_sgf(char * filepath) {

    FILE * stream = fopen(filepath, "w" );

    if ( !stream ) {
        fprintf(stderr,"Cannot open file for writing\n" );
        exit( -1 );
    }

}
