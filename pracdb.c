#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef enum{
    STATEMENT_SELECT,
    STATEMENT_INSERT
}StatementType;

typedef struct{
    StatementType type;
}Statement;

InputBuffer *new_input_buffer()
{
    InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt()
{
    printf("pracdb > ");
}

void read_input(InputBuffer *input_buffer)
{
    // getline returns number of bytes read ;if -1 then it means error; n is a pointer to save the size of allocated buffer; we are using signed size_t (ssize_t); our File stream is stdin
    // ssize_t getline(char **lineptr, size_t *n, FILE *stream);
    // if it returns -1 then free the allocated buffer
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <= 0)
    {
        printf("Error reading input");
        exit(EXIT_FAILURE);
    }

    // To ignore training newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

// Function to close input buffer on .exit string
void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if(strcmp(input_buffer->buffer , ".exit") == 0){
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    if (strncmp(input_buffer->buffer , "insert",6) == 0){
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer , "select")==0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void exucute_statement(Statement* statement){
    switch(statement->type){
        case(STATEMENT_INSERT):
        printf("Perform an insert statement.\n");
        break;
        case(STATEMENT_SELECT):
        printf("Perform a select statement.\n");
        break;
    }
}

int main(int argc, char *argv[])
{
    InputBuffer *input_buffer = new_input_buffer();
    while (true)
    {
        print_prompt();
        read_input(input_buffer);

        
            if(input_buffer->buffer[0] == '.')
            {
                switch(do_meta_command(input_buffer))
                {
                    case (META_COMMAND_SUCCESS):
                    continue;
                    case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'.\n",input_buffer->buffer);
                    continue;
                }
            }
            Statement statement;
            switch(prepare_statement(input_buffer,&statement)){
            case(PREPARE_SUCCESS):
            break;
            case(PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyboard at start of '%s'.\n",input_buffer->buffer);
            continue;
    }
    exucute_statement(&statement);
        printf("Executed.\n");
    }
        
}
