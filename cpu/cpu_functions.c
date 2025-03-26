#include "cpu_functions.h"
#include "klog_extension.h"

int64_t x[32];

uint8_t get_opcode(unsigned int instruction)
{
    uint32_t value;
    value = (uint8_t) instruction & 0x0000007f;
    return (uint8_t)value;
}

uint8_t get_rd(unsigned int instruction)
{
    uint32_t value;
    value =(instruction & 0b00000000000000000000111110000000) >> 7;
    return (uint8_t)value;
}

uint8_t get_funct3(unsigned int instruction)
{
    uint32_t value;
    value = (instruction & 0b00000000000000000111000000000000) >> 12;
    return (uint8_t)value;
}

uint8_t get_imm12_S(unsigned int instruction)
{
    uint32_t value;
    value = ( (instruction & 0b11111110000000000000000000000000) >> 20 ) + ( (instruction & 0b00000000000000000000111110000000) >> 7 );
    return (uint8_t)value;
}

uint8_t get_rs1(unsigned int instruction)
{
    uint32_t value;
    value = (instruction & 0b00000000000011111000000000000000) >> 15;
    return (uint8_t)value;
}

uint8_t get_rs2(unsigned int instruction)
{
    uint32_t value;
    value = (instruction & 0b00000001111100000000000000000000) >> 20;
    return (uint8_t)value;
}

int16_t get_imm12_I(unsigned int instruction) {
    // Extract the 12-bit immediate from bits [31:20]
    int16_t imm12 = (instruction & 0xFFF00000) >> 20;

    // Sign-extend the immediate to fit a signed 12-bit value
    if (imm12 & (1 << 11)) { // If the sign bit (bit 11) is set
        imm12 |= 0xF000; // Sign-extend the immediate (fill upper bits with 1s)
    }

    return imm12;
}

uint8_t get_funct7(unsigned int instruction)
{
    uint32_t value;
    value = (instruction & 0b11111110000000000000000000000000) >> 25;
    return (uint8_t) value;
}

int16_t get_immediate_branch(unsigned int instruction)
{
    int16_t imm = 0;

    // Extract individual bits and assemble the 13-bit signed immediate
    imm |= ((instruction >> 31) & 0x1) << 12; // imm[12]
    imm |= ((instruction >> 7)  & 0x1) << 11; // imm[11]
    imm |= ((instruction >> 25) & 0x3F) << 5; // imm[10:5]
    imm |= ((instruction >> 8)  & 0xF) << 1;  // imm[4:1]
    
    // Sign extend from bit 12 if needed
    if (imm & (1 << 12)) { 
        imm |= 0xE000; // Fill upper bits with 1s for negative values
    }

    return imm;
}

int64_t alu_unit(char* mnemonic, int64_t in1, int64_t in2, uint8_t funct3, uint8_t funct7)
{
    switch(funct3)
    {

        
        //ADD - ADDI - SUB
        case 0b000:
            if(funct7 != 0b1111111)
                switch(funct7)
                {
                    //ADD
                    case 0b0000000:
                        strcpy(mnemonic, "add");
                        return in1 + in2;
                    //SUB
                    case 0b0100000:
                        strcpy(mnemonic, "sub");
                        return in1 - in2;
                }
            else
            {
                //ADDI
                strcpy(mnemonic, "addi");
                return in1 + in2;
            }
            break;
        
        //SLL - SLLI
        case 0b001:
            strcpy(mnemonic, "sll");
            return (in1 << (in2 & 0x3F));
        
        //XOR
        case 0b100:
            strcpy(mnemonic, "xor");
            return in1 ^ in2;

        //SLT
        case 0b010:
            strcpy(mnemonic, "slt");
            return in1 < in2;

        //AND
        case 0b111:
            strcpy(mnemonic, "and");
            return in1 & in2;

        //OR
        case 0b110:
            strcpy(mnemonic, "or");
            return in1 | in2;
    }

    return 0;
}




void populate_code_segment(unsigned int* code_segment, FILE* input_file)
{
    char line[33];

    int parsing_counter = 0;

    while(fgets(line, 100, input_file))
    {
        unsigned int instruction = 0;
        unsigned int current_order = 1;

        for(int i = 31 ; i >= 0 ; i--)
        {
            instruction += current_order * (line[i] - 48);
            current_order *= 2;
        }

        *(code_segment + parsing_counter) = instruction;
        parsing_counter++;
    }   

}


void R_instruction_execute(unsigned int instruction)
{
    int old_value = x[get_rd(instruction)];
    char mnemonic[4];

    x[get_rd(instruction)] = alu_unit(mnemonic, x[get_rs1(instruction)], x[get_rs2(instruction)], get_funct3(instruction), get_funct7(instruction));

    print_klog(instruction, 'r', mnemonic, old_value, x[get_rd(instruction)]);

}

void IMM_instruction_execute(unsigned int instruction)
{
    int old_value = x[get_rd(instruction)];
    
    char mnemonic[6];

    x[get_rd(instruction)] = alu_unit(mnemonic, x[get_rs1(instruction)], get_imm12_I(instruction), get_funct3(instruction), 0b1111111);

    print_klog(instruction, 'i', mnemonic, old_value, x[get_rd(instruction)]);

}

void S_instruction_execute(unsigned int instruction, uint8_t* data_segment)
{

    char mnemonic[4];

    switch(get_funct3(instruction))
    {
        //SW
        case 0b010:
        {
            strcpy(mnemonic, "sw");
            uint32_t* data_address;
            data_address = (uint32_t*)(data_segment + x[get_rs1(instruction)] + get_imm12_S(instruction));
            *data_address = x[get_rs2(instruction)];
            break;
        }
    }

    print_klog(instruction, 's', mnemonic, x[get_rs1(instruction)] + get_imm12_S(instruction), x[get_rs2(instruction)]);

}

void ILOAD_instruction_execute(unsigned int instruction, uint8_t* data_segment)
{
    char mnemonic[4];

    switch(get_funct3(instruction))
    {
        //LW
        case 0b010:
        {
            strcpy(mnemonic, "lw");
            uint32_t* data_address;
            data_address = (uint32_t*)(data_segment + get_imm12_I(instruction) + x[get_rs1(instruction)]);
            x[get_rd(instruction)] = *data_address;

            break;
        }
    }

    print_klog(instruction, 'I', mnemonic, get_imm12_I(instruction) + x[get_rs1(instruction)], x[get_rd(instruction)]);
}

void B_instruction_execute(unsigned int instruction, unsigned int** pc, bool* increment_pc)
{

    char mnemonic[4];

    switch(get_funct3(instruction))
    {
        // BEQ
        case 0b000:
        {
            strcpy(mnemonic, "beq");
            if(x[get_rs1(instruction)] == x[get_rs2(instruction)])
            {
                *pc += (get_immediate_branch(instruction) / 4);
                *increment_pc = false;
            }
            break;
        }
        
        //BNE
        case 0b001:
        {
            strcpy(mnemonic, "bne");
            if(x[get_rs1(instruction)] != x[get_rs2(instruction)])
            {
                *pc += (get_immediate_branch(instruction) / 4);
                *increment_pc = false;
            }
            break;
        }

        //BLT
        case 0b100:
        {
            strcpy(mnemonic, "blt");
            if(x[get_rs1(instruction)] < x[get_rs2(instruction)])
            {
                *pc += (get_immediate_branch(instruction) / 4);
                *increment_pc = false;
            }
            break;
        }

        //BGE
        case 0b101:
        {
            strcpy(mnemonic, "bge");
            if(x[get_rs1(instruction)] > x[get_rs2(instruction)])
            {
                *pc += (get_immediate_branch(instruction) / 4);
                *increment_pc = false;
            }
            break;
        }

        //BLTU
        case 0b110:
        {
            strcpy(mnemonic, "bltu");
            if(((uint64_t)x[get_rs1(instruction)]) < ((uint64_t)x[get_rs2(instruction)]))
            {
                *pc += (get_immediate_branch(instruction) / 4);
                *increment_pc = false;
            }
            break;
        }

        //BGEU
        case 0b111:
        {
            strcpy(mnemonic, "bgeu");
            if(((uint64_t)x[get_rs1(instruction)]) >= ((uint64_t)x[get_rs2(instruction)]))
            {
                *pc += (get_immediate_branch(instruction) / 4);
                *increment_pc = false;
            }
            break;
        }
    }

    print_klog(instruction, 'b', mnemonic, 0, 0);



}
