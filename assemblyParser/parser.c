#include "parser_functions.h"

int main(int argc, char *argv[])
{   
     // Check for the correct number of arguments
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    //Create the environment
    char output_filename[1024];
    snprintf(output_filename, sizeof(output_filename), "%s_completed.asm", argv[1]);


    FILE *input = fopen(argv[1], "r");
    FILE *base = fopen("basetest.asm", "r");
    FILE *out = fopen(output_filename, "w");

    if (!input || !base || !out) {
        perror("Error opening file");
        if (input) fclose(input);
        if (base) fclose(base);
        if (out) fclose(out);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), input)) {
        printf("%s ", line);
        fputs(line, out); // write base line to output
        
    }

    while (fgets(line, sizeof(line), base)) {
        fputs(line, out);
    }

    fclose(input);
    fclose(base);
    fclose(out);

    printf("Created %s\n", output_filename);

    // Open the input assembly file to store data section
    FILE *input_file3 = fopen(output_filename, "r");
    if (input_file3 == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    //Get to the data section and save data
    int in_data_section = 0;

    while (fgets(line, sizeof(line), input_file3) != NULL) {
        line[strcspn(line, "\n")] = 0;  // Strip newline

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        // Detect transition into .data section
        char *peek = strdup(line);
        char *tok1 = strtok(peek, " \t,");
        char *tok2 = strtok(NULL, " \t,");
        if ((tok1 && strcmp(tok1, ".section") == 0 && tok2 && strcmp(tok2, ".data") == 0) ||
            (tok1 && strcmp(tok1, ".data") == 0)) {
            in_data_section = 1;
            free(peek);
            continue;
        }
        free(peek);

        if (in_data_section)
        {
            // Data section — handle numeric data declarations
            // Expected format: label: .directive value[, value...]
            char *label = strtok(line, " \t");
            if (!label || label[strlen(label) - 1] != ':') continue;

            char *directive = strtok(NULL, " \t");
            char *value_str = strtok(NULL, ""); // Remainder of line

            if (!directive || !value_str) continue;

            // Check for supported numeric directives
            if (strcmp(directive, ".word") == 0 ||
                strcmp(directive, ".byte") == 0 ||
                strcmp(directive, ".half") == 0 ||
                strcmp(directive, ".dword") == 0) {

                int value = atoi(value_str);
                save_data(label, value);
                 
            }
        }
    }


    // Open the input assembly file to store labels
    FILE *input_file2 = fopen(output_filename, "r");
    if (input_file2 == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    //Get to the code section
    while (fgets(line, sizeof(line), input_file2) != NULL) {
        // Strip newline characters

        line[strcspn(line, "\n")] = 0;

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        char *token = strtok(line, " ,\t");
        
        // Check for .section .text or .text
        if ((strcmp(token, ".section") == 0 && strcmp(strtok(NULL, " ,\t"), ".text") == 0) || strcmp(token, ".text") == 0)
        {
            printf("Found .section .text or .text — stopping.\n");
            break;
        }
        
    }

    unsigned int offset = 0;
    
    while (fgets(line, sizeof(line), input_file2) != NULL) {
        // Strip newline characters

        line[strcspn(line, "\n")] = 0;

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        char *token = strtok(line, " ,\t");
        
        // Check if the line is a label
        if(token[strlen(token) - 1] == ':')
        {
            save_label(token, offset+1);
        }
        else offset++;
        
    }

    // Open the input assembly file
    FILE *input_file = fopen(output_filename, "r");
    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    // Open the output file for writing encoded instructions
    FILE *output_file = fopen("output.txt", "w");
    
    
    //Get to the code section
    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Strip newline characters

        line[strcspn(line, "\n")] = 0;

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        char *token = strtok(line, " ,\t");
        
        // Check for .section .text or .text
        if ((strcmp(token, ".section") == 0 && strcmp(strtok(NULL, " ,\t"), ".text") == 0) || strcmp(token, ".text") == 0)
        {
            printf("Found .section .text or .text — stopping.\n");
            break;
        }
        
    }
    
    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Strip newline characters

        printf("%s\n", line);
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        // Parse and encode the instruction
        parse_and_encode_instruction(line, output_file);
    }
    
    // Close the files
    fclose(input_file);
    fclose(output_file);

    printf("Encoding completed successfully.\n");
    return 0;
    
    return 0;

}