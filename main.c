#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void shell_loop();
char* read_line();
char** split_line(char* line);
int execute(char** args);

int main(int argc, char **argv) {
    shell_loop();

    return 0;
}

/*
 * Builds input char by char and dynamically reallocates memory if buffer is full.
 */
char* read_line(){
    int l = 32; // length of string
    char* input = (char*) malloc(sizeof(char) * l); // str pointer
    int i = 0; // index on string
    int c; // character obtained from user

    if(!input){ // couldn't get memory.
        printf("Failed to allocate memory.\n");
        exit(-1);
    }

    // Loop executes until eof or \n char is obtained.
    while(1){
        c = getchar(); // get the char
        if(c == EOF || c == '\n'){
            c = '\0';
            *(input + i) = c;
            return input;
        }else{
            *(input + i) = c;
        }

        i++;
        if(i >= l){
            l += 32; // dynamically increase buffer by 32 bytes.
            input = (char*) realloc(input, l);
            if(!input){
                printf("There was an error reallocating memory.\n");
                exit(-1);
            }
        }
    }
}

/*
 * Splits line using ' ' as delimiter. Can dynamically grow size of array being returned.
 */
char** split_line(char* line){
    char* token;
    int size = 16;
    char** lines = (char**) malloc(sizeof(char*) * size);
    int pos = 0;
    char* delim = " ";

    if(!lines){
        printf("Issue allocating memory.\n");
        exit(-1);
    }

    token = strtok(line, delim); // tokenizing line using ' ' as delimiter.
    while(token != NULL){  // loops while there are still tokens in line.
        *(lines + pos) = token;
        pos++;

        if(pos + 1 >= size){ // dynamically reallocating memory if size is too small for list of tokens.
            size += 32;
            lines = (char**) realloc(lines, sizeof(char*) * size);
            if(!lines){
                printf("error reallocating memory.\n");
                exit(-1);
            }
        }
        token = strtok(NULL, delim);
    }
    *(lines + pos) = NULL;
    return lines;
}

int execute(char** args){
	int p = fork();
	
	if(p < 0){
		printf("Error forking!\n");
		exit(-1);
	}

	if (p == 0){ // child
		execvp(*(args), args);
	}else{ // parent
		while(wait(NULL) != -1){}
		//printf("\n");
	}
	return 1;
}

void shell_loop(){
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while(status);
}
