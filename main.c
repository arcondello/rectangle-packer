#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Return allocated pointer to each line in file,
 * returns NULL pointer after EOF */
char *line_alloc(FILE *fp)
{
    char *line = NULL;
    char *line_temp = NULL;
    size_t size = 0;
    int c = 0;

    if( fp == NULL ){
        fprintf(stderr, "Error. File pointer is NULL.\n");
        return NULL;
    }
    while(1){
        /* Read a char, return null if file is already EOF,
         * else try to realloc line, return if character is newline or
         * EOF, else add character to line. */
        c = fgetc(fp);
        if(c == EOF && size == 0){
            return NULL;
        }
        size++;
        if( (line_temp = realloc(line, size*sizeof(*line))) == NULL ){
            fprintf(stderr, "Error. Could not reallocate memory for line.\n");
            free(line);
            return NULL;
        }
        else{
            line = line_temp;
        }
        if(c == '\n' || c == EOF){
            line[size - 1] = '\0';
            return line;
        }
        else{
            line[size - 1] = c;
        }
    }
    return NULL;
}

/* Return allocated list of integers parsed from csv string */
int *csvint_alloc(char *line, int *elements)
{
    int *csvint_alloc = NULL;
    char *tok = NULL;
    int i;

    /* Count elements = nr commas + 1 */
    *elements = 1;
    for(i = 0; line[i] != '\0'; i++){
        if(line[i] == ','){
            (*elements)++;
        }
    }

    if( (csvint_alloc = calloc((size_t)*elements, sizeof(int))) == NULL ){
        fprintf(stderr, "Error. Could not allocate memory for int list.\n");
        return NULL;
    }

    tok = strtok(line, ",");
    for(i = 0; tok != NULL; i++){
        csvint_alloc[i] = atoi(tok);
        tok = strtok(NULL, ",");
    }

    return csvint_alloc;
}

struct box{
    int height;
    int width;
};

struct box *boxlist_alloc(FILE *fp, int *nr_boxes)
{
    struct box *boxpt = NULL;
    struct box *boxpt_temp = NULL;
    size_t size = 0;
    char *line = NULL;

    if(fp == NULL){
        fprintf(stderr, "Error. File pointer is null.\n");
        return NULL;
    }

    while( (line = line_alloc(fp)) != NULL ){
        int *csv_integers;
        struct box temp;
        int elements;

        size++;
        if( (boxpt_temp = realloc(boxpt, size*sizeof(*boxpt))) == NULL ){
            fprintf(stderr, "Error. Could not reallocate memory for box list.\n");
            free(boxpt);
            free(line);
            return NULL;
        }
        else{
            boxpt = boxpt_temp;
        }
        csv_integers = csvint_alloc(line, &elements);
        temp.height = csv_integers[0];
        temp.width = csv_integers[1];
        boxpt[size - 1] = temp;
        free(csv_integers);
        free(line);
    }
    *nr_boxes = (int) size;
    return boxpt;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "No input file found!\n");
    }
    FILE *fp;
    if( (fp = fopen(argv[1], "r")) == NULL ){
        fprintf(stderr, "Error. File could not be opened.\n");
        return 0;
    }

    /*
     * FUNGERAR
    char *line = NULL;
    int *numbers = NULL;
    while( (line = line_alloc(fp)) != NULL ){
        printf("line: %s\n", line);
        int elements;
        numbers = csvint_alloc(line, &elements); 
        int i;
        for(i = 0; i < elements; i++){
            printf("int: %d, ", numbers[i]);
        }
        free(numbers);
        free(line);
    }*/

    struct box *list;
    int length;
    int i;
    list = boxlist_alloc(fp, &length);
    printf("boxlist_alloc length %d\n", length);
    for(i = 0; i < length; i++){
        printf("h = %d, w = %d\n", list[i].height, list[i].width);
    }
    free(list);

    fclose(fp);
    return 0;
}
