int csv_parser(char* filename, char* linkid, char *buffer) {
    char line[1024];
    char* tok;
    FILE* stream = fopen(filename, "r");

    if (stream == NULL) { 
        printf("Cannot open database_a file \n"); 
        exit(0); 
    } 

    while (fgets(line, 1024, stream)) {
	tok = strtok(line, ",");
	if (strcmp(tok, linkid) == 0)
    		for (;tok && *tok; ) {
			strcat (buffer, tok);
        		printf("Field 3 would be %s\n", tok);
			tok = strtok(NULL, ",\n");
		}
		return 1;
    	}
    return 0;
}

