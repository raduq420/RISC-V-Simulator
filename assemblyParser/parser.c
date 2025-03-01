#include "parser_functions.h"

int main(int argc, char *argv[])
{   
     // Check for the correct number of arguments
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Open the input assembly file to store labels
    FILE *input_file2 = fopen(argv[1], "r");
    if (input_file2 == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    char line[256];
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
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    // Open the output file for writing encoded instructions
    FILE *output_file = fopen("output.txt", "w");
    
    
    
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