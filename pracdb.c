#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define COLUMN_EMAIL_SIZE 255
#define COLUMN_USERNAME_SIZE 32
// ((Struct *)0)->Attribute is used to access the value of the 'Attribute' member of a structure 'Struct' through a pointer to the structure
// sizeof(((Struct *)0)->Attribute) is the same as the size of the data type of the 'Attribute' member.
#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

// Since the structure passed here is Row; the size of ID_SIZE will be 4 bytes(int) because the attribute which is
// being passed here is id which is of type uint32_t
const uint32_t ID_SIZE = size_of_attribute(Row,id)
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;


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
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR
}PrepareResult;

typedef enum{
    STATEMENT_SELECT,
    STATEMENT_INSERT
}StatementType;

typedef struct{
    StatementType type;
    Row row_to_insert;
}Statement;

typedef struct{
uint32_t id;
char username[COLUMN_USERNAME_SIZE];
char email[COLUMN_EMAIL_SIZE]
}Row

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

void serialize_row(Row* source, void* destination)
{
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void * source, Row* destination)
{
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);

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
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s",&(statement->row_to_insert.id),&(statement->row_to_insert.username),&(statement->row_to_insert.email) );
        if(args_assigned>3)
        {
            return PREPARE_SYNTAX_ERROR;
        }
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
