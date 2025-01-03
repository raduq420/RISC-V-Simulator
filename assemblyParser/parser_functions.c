#include "parser_functions.h"

//imm[11:5] rs2 rs1 010 imm[4:0] 0100011 SW
R_type R_type_list[] =
{
    //OPCODE  - FUNCT3 - FUNCT 7
    //0110011 -  000   - 0000000 ADD
    {0b0110011, 0, 0b0, 0, 0, 0b0}
};

I_type I_type_list[] = 
{
    //OPCODE  - FUNCT3
    //0010011 - 000 ADDI
    {0b0010011, 0, 0b000, 0, 0},
    //0000011 - 010 LW
    {0b0000011, 0, 0b010, 0, 0}

};

S_type S_type_list[] =
{
    //OPCODE - FUNCT3
    //0100011 - 010 SW
    {0b0100011, 0 , 0b010, 0, 0}
};

instruction_mnemonics instruction_mnemonics_list[] =
{
    {"add", 'r', 0},
    {"addi", 'i', 0},
    {"lw", 'I', 1},
    {"sw", 's', 0}
};

void print_binary(unsigned int num, FILE* output_file, int number_size) {
    for (int i = number_size - 1; i >= 0; i--) {
        unsigned int bit = (num >> i) & 1;
        fprintf(output_file, "%u", bit);
    }
}

void print_R_type(R_type instruction, FILE* output_file)
{
    print_binary(instruction.funct7, output_file, 7);
    print_binary(instruction.rs2, output_file, 5);
    print_binary(instruction.rs1, output_file, 5);
    print_binary(instruction.funct3, output_file, 3);
    print_binary(instruction.rd, output_file, 5);
    print_binary(instruction.opcode, output_file, 7);
    fprintf(output_file, "\n");
}

void print_I_type(I_type instruction, FILE* output_file)
{
    print_binary(instruction.imm12, output_file, 12);
    print_binary(instruction.rs1, output_file, 5);
    print_binary(instruction.funct3, output_file, 3);
    print_binary(instruction.rd, output_file, 5);
    print_binary(instruction.opcode, output_file, 7);
    fprintf(output_file, "\n");
}

void print_S_type(S_type instruction, FILE* output_file)
{
    print_binary((instruction.imm12>>5), output_file, 7);
    print_binary(instruction.rs2, output_file, 5);
    print_binary(instruction.rs1, output_file, 5);
    print_binary(instruction.funct3, output_file, 3);
    print_binary((instruction.imm12), output_file, 5);
    print_binary(instruction.opcode, output_file, 7);
    fprintf(output_file, "\n");
}

void encode_R_type(char* token, FILE* output_file, int list_index)
{
    R_type encoded_instruction = R_type_list[list_index];

    token = strtok(NULL, " ,\t"); // Get the next token
    //Setting the destination register
    encoded_instruction.rd = atoi((token+1));

    token = strtok(NULL, " ,\t"); // Get the next token
    //Setting first source register
    encoded_instruction.rs1 = atoi((token+1));

    token = strtok(NULL, " ,\t"); // Get the next token
    //Setting second source register
    encoded_instruction.rs2 = atoi((token+1));
    

    print_R_type(encoded_instruction, output_file);
}

void encode_I_type(char* token, FILE* output_file, int list_index)
{
    I_type encoded_instruction = I_type_list[list_index];

    token = strtok(NULL, " ,\t"); // Get the next token
    //Setting the destination register
    encoded_instruction.rd = atoi((token+1));

    token = strtok(NULL, " ,\t"); // Get the next token
    //Setting source register
    encoded_instruction.rs1 = atoi((token+1));

    token = strtok(NULL, " ,\t"); // Get the next token
    //Setting the immediate value
    encoded_instruction.imm12 = atoi(token);


    print_I_type(encoded_instruction, output_file);
}

void encode_ILOAD_type(char* token, FILE* output_file, int list_index)
{
    I_type encoded_instruction = I_type_list[list_index];

    // Parse the destination register (rd)
    token = strtok(NULL, " ,\t"); // Get the next token
    encoded_instruction.rd = atoi((token + 1)); // Skip the 'x' prefix

    // Parse the immediate value and source register (rs1)
    token = strtok(NULL, " ,\t"); // Get the next token
    char* open_paren = strchr(token, '('); // Find '(' in the token
    if (open_paren != NULL) {
        *open_paren = '\0'; // Null-terminate the immediate part
        encoded_instruction.imm12 = atoi(token); // Immediate value
        encoded_instruction.rs1 = atoi((open_paren + 2)); // Source register (skip '(' and 'x')
    } else {
        fprintf(stderr, "Error: Invalid LW instruction format\n");
        return;
    }

    // Print the encoded instruction
    print_I_type(encoded_instruction, output_file);
}


void encode_S_type(char* token, FILE* output_file, int list_index) 
{
    S_type encoded_instruction = S_type_list[list_index];

    // Parse the source register (rs2)
    token = strtok(NULL, " ,\t"); // Get the next token
    encoded_instruction.rs2 = atoi((token + 1)); // Skip the 'x' prefix

    // Parse the immediate value and the base register (rs1)
    token = strtok(NULL, " ,\t"); // Get the next token
    char* open_paren = strchr(token, '('); // Find '(' in the token
    if (open_paren != NULL) {
        *open_paren = '\0'; // Null-terminate the immediate part
        encoded_instruction.imm12 = atoi(token); // Immediate value
        encoded_instruction.rs1 = atoi((open_paren + 2)); // Base register (skip '(' and 'x')
    } else {
        fprintf(stderr, "Error: Invalid SW instruction format\n");
        return;
    }

    // Print the encoded instruction
    print_S_type(encoded_instruction, output_file);
}

void parse_and_encode_instruction(char line[], FILE* output_file)
{
    char buffer[256];
    strncpy(buffer, line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

    // Tokenize the line (separated by spaces, commas, or tabs)
    char *token = strtok(buffer, " ,\t");

    for(int i = 0 ; i < mnemonic_list_size ; i++)
    {
        if(strcmp(token, instruction_mnemonics_list[i].mnemonic) == 0)
        {
            
            switch(instruction_mnemonics_list[i].format)
            {
                case 'r':
                    encode_R_type(token, output_file, instruction_mnemonics_list[i].index);
                    break;
                case 'I':
                    encode_ILOAD_type(token, output_file, instruction_mnemonics_list[i].index);
                    break;
                case 'i':
                    encode_I_type(token, output_file, instruction_mnemonics_list[i].index);
                    break;
                case 's':
                    encode_S_type(token, output_file, instruction_mnemonics_list[i].index);
                    break;
            }
            break;
        }

    }

}

