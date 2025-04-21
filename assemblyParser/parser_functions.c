#include "parser_functions.h"

//imm[11:5] rs2 rs1 010 imm[4:0] 0100011 SW



R_type R_type_list[] =
{
    //OPCODE  - FUNCT3 - FUNCT 7
    //0110011 -  000   - 0000000 ADD
    {0b0110011, 0, 0b0, 0, 0, 0b0},
    //0110011 -  000   - 0100000 SUB
    {0b0110011, 0, 0b0, 0, 0, 0b0100000},
    //0110011 -  001   - 0000000 SLL
    {0b0110011, 0, 0b001, 0, 0, 0b0},
    //0110011 -  100   - 0000000 XOR
    {0b0110011, 0, 0b100, 0, 0, 0b0},
    //0110011 -  010   - 0000000 SLT
    {0b0110011, 0, 0b010, 0, 0, 0b0},
    //0110011 -  111   - 0000000 AND
    {0b0110011, 0, 0b111, 0, 0, 0b0},
    //0110011 -  110   - 0000000 OR
    {0b0110011, 0, 0b110, 0, 0, 0b0},
    //0110011 -  011   - 0000000 SLTU
    {0b0110011, 0, 0b011, 0, 0, 0b0},
    //0110011 -  101   - 0000000 SRL
    {0b0110011, 0, 0b101, 0, 0, 0b0}
};

I_type I_type_list[] = 
{
    //OPCODE  - FUNCT3
    //0010011 - 000 ADDI
    {0b0010011, 0, 0b000, 0, 0},
    //0000011 - 010 LW
    {0b0000011, 0, 0b010, 0, 0},
    //0010011 - 001 SLLI
    {0b0010011, 0, 0b001, 0, 0},
    //0000011 - 000 LB
    {0b0000011, 0, 0b000, 0, 0},
    //0000011 - 001 LH
    {0b0000011, 0, 0b001, 0, 0},
    //0000011 - 101 LHU
    {0b0000011, 0, 0b101, 0, 0},
    //0000011 - 100 LBU
    {0b0000011, 0, 0b100, 0, 0},
    //0010011 - 010 SLTI
    {0b0010011, 0, 0b010, 0, 0},
    //0010011 - 011 SLTIU
    {0b0010011, 0, 0b011, 0, 0},
    //0010011 - 100 XORI
    {0b0010011, 0, 0b100, 0, 0},
    //0010011 - 110 ORI
    {0b0010011, 0, 0b110, 0, 0},
    //0010011 - 111 ANDI
    {0b0010011, 0, 0b111, 0, 0},
    //0010011 - 101 SRLI
    {0b0010011, 0, 0b101, 0, 0},
    //0000011 - 110 LWU
    {0b0000011, 0, 0b110, 0, 0},
    //0000011 - 011 LD
    {0b0000011, 0, 0b011, 0, 0}
};

S_type S_type_list[] =
{
    //OPCODE - FUNCT3
    //0100011 - 010 SW
    {0b0100011, 0, 0b010, 0, 0},
    //0100011 - 000 SB
    {0b0100011, 0, 0b000, 0, 0},
    //0100011 - 001 SH
    {0b0100011, 0, 0b001, 0, 0},
    //0100011 - 011 SD
    {0b0100011, 0, 0b011, 0, 0}
};

B_type B_type_list[] = 
{
    //OPCODE - FUNCT3
    //1100011 - 000 BEQ
    {0b1100011, 0b000, 0, 0, 0},
    //1100011 - 001 BNE
    {0b1100011, 0b001, 0, 0, 0},
    //1100011 - 100 BLT
    {0b1100011, 0b100, 0, 0, 0},
    //1100011 - 101 BGE
    {0b1100011, 0b101, 0, 0, 0},
    //1100011 - 110 BLTU
    {0b1100011, 0b110, 0, 0, 0},
    //1100011 - 111 BGEu
    {0b1100011, 0b111, 0, 0, 0}
};

instruction_mnemonics instruction_mnemonics_list[] =
{
    {"add", 'r', 0},
    {"sub", 'r', 1},
    {"sll", 'r', 2},
    {"xor", 'r', 3},
    {"slt", 'r', 4},
    {"and", 'r', 5},
    {"or", 'r', 6},
    {"sltu", 'r', 7},
    {"srl", 'r', 8},
    {"addi", 'i', 0},
    {"lw", 'I', 1},
    {"slli", 'i', 2},
    {"lb", 'I', 3},
    {"lh", 'I', 4},
    {"lhu", 'I', 5},
    {"lbu", 'I', 6},
    {"slti", 'i', 7},
    {"sltiu", 'i', 8},
    {"xori", 'i', 9},
    {"ori", 'i', 10},
    {"andi", 'i', 11},
    {"srli", 'i', 12},
    {"lwu", 'I', 13},
    {"ld", 'I', 14},
    {"sw", 's', 0},
    {"sb", 's', 1},
    {"sh", 's', 2},
    {"sd", 's', 3},
    {"beq", 'b', 0},
    {"bne", 'b', 1},
    {"blt", 'b', 2},
    {"bge", 'b', 3},
    {"bltu", 'b', 4},
    {"bgeu", 'b', 5},
    {"lui", 'z', 0},
    {"auipc", 'z', 1},
    {"jal", 'z', 2}
};

label_list existing_label_list[20];
short int label_list_size = 0;
unsigned int current_offset = 0;

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

void print_b_type(B_type instruction, FILE* output_file)
{
    print_binary(instruction.imm13>>12, output_file, 1);
    print_binary(instruction.imm13>>5, output_file, 6);
    print_binary(instruction.rs2, output_file, 5);
    print_binary(instruction.rs1, output_file, 5);
    print_binary(instruction.funct3, output_file, 3);
    print_binary(instruction.imm13>>1, output_file, 4);
    print_binary(instruction.imm13>>11, output_file, 1);
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

int is_numeric(const char* token) {
    if (*token == '-' || *token == '+') token++; // Allow signed numbers
    while (*token) {
        if (!isdigit(*token)) return 0; // If non-digit found, return false
        token++;
    }
    return 1;
}

void encode_B_type(char* token, FILE* output_file, int list_index)
{
    B_type encoded_instruction = B_type_list[list_index];

    // Get the first token and set rs1 (source register 1)
    token = strtok(NULL, " ,\t");
    encoded_instruction.rs1 = atoi((token + 1)); // Convert "xN" to integer (skip 'x')

    // Get the next token and set rs2 (source register 2)
    token = strtok(NULL, " ,\t");
    encoded_instruction.rs2 = atoi((token + 1)); // Convert "xN" to integer (skip 'x')

    // Get the next token and set the immediate value (branch offset)
    token = strtok(NULL, " ,\t");

    if(is_numeric(token) == 1)
        encoded_instruction.imm13 = atoi(token); // Convert immediate string to integer
    else
        for(int i = 0 ; i < label_list_size ; i++)
        {
            if(strcmp(token, existing_label_list[i].name) == 0)
            {
                encoded_instruction.imm13 =  existing_label_list[i].offset - current_offset ;
                encoded_instruction.imm13 *= 4; // Convert 32 bit words into 8 bit bytes
                printf("%d = %d - %d", encoded_instruction.imm13, existing_label_list[i].offset, current_offset);
                break;
            }
        }

    print_b_type(encoded_instruction, output_file);

}

void encode_special_type(char* token, FILE* output_file)
{
    if(strcmp(token, "lui") == 0)
    {
        //Print and encode lui instruction
      
        token = strtok(NULL, " ,\t");
        unsigned int rd = atoi((token + 1));

        //Print immediate value
        token = strtok(NULL, " ,\t");
        print_binary(atoi(token), output_file, 20);

        //Print destination register
        print_binary(rd, output_file, 5);

        //Print opcode
        print_binary(55, output_file, 7);

        fprintf(output_file, "\n");
    }
    else if(strcmp(token, "auipc") == 0)
    {
        //Print and encode lui instruction
      
        token = strtok(NULL, " ,\t");
        unsigned int rd = atoi((token + 1));

        //Print immediate value
        token = strtok(NULL, " ,\t");
        print_binary(atoi(token), output_file, 20);

        //Print destination register
        print_binary(rd, output_file, 5);

        //Print opcode
        print_binary(23, output_file, 7);

        fprintf(output_file, "\n");
    }
    else if(strcmp(token, "jal") == 0)
    {
        
    }
}

void save_label(char* token, unsigned int offset)
{
    token[strlen(token) - 1] = '\0';
    label_list_size++;
    strcpy(existing_label_list[label_list_size-1].name, token);
    existing_label_list[label_list_size-1].offset = offset; 
    printf("label encoded = %s", token);
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
                    current_offset += 1;
                    break;
                case 'I':
                     encode_ILOAD_type(token, output_file, instruction_mnemonics_list[i].index);
                     current_offset += 1;
                    break;
                case 'i':
                    encode_I_type(token, output_file, instruction_mnemonics_list[i].index);
                    current_offset += 1;
                    break;
                case 's':
                    encode_S_type(token, output_file, instruction_mnemonics_list[i].index);
                    current_offset += 1;
                    break;
                case 'b':
                    printf("encoding b_type!");
                    current_offset += 1;
                    encode_B_type(token, output_file, instruction_mnemonics_list[i].index);
                    break;
                case 'z':
                    printf("encoding special type!");
                    current_offset += 1;
                    encode_special_type(token, output_file);
                    break;
            }

        }  
    }

}


