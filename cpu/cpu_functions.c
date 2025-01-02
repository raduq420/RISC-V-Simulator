#include "cpu_functions.h"

uint64_t x[32];

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

uint8_t get_imm12_I(unsigned int instruction)
{
    uint32_t value;
    value = (instruction & 0b11111111111100000000000000000000) >> 20;
    return (uint8_t)value;
}
uint8_t get_funct7(unsigned int instruction)
{
    uint32_t value;
    value = (instruction & 0b11111110000000000000000000000000) >> 25;
    return (uint8_t) value;
}

uint64_t alu_unit(int64_t in1, int64_t in2, uint8_t funct3, uint8_t funct7)
{
    switch(funct3)
    {

        
        //ADD - ADDI
        case 0b000:
            if(funct7 != 0b1111111)
                switch(funct7)
                {
                    //ADD
                    case 0b0000000:
                        return in1 + in2;
                    //SUB
                    case 0b0100000:
                        return in1 - in2;
                }
            else
                //ADDI
                return in1 + in2;
            
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
    x[get_rd(instruction)] = alu_unit(x[get_rs1(instruction)], x[get_rs2(instruction)], get_funct3(instruction), get_funct7(instruction));
    printf("Executed instruction %u, result x%d = %d + %d \n", instruction, get_rd(instruction), (int)x[get_rs1(instruction)], (int)x[get_rs2(instruction)]);
}

void IMM_instruction_execute(unsigned int instruction)
{
    x[get_rd(instruction)] = alu_unit(x[get_rs1(instruction)], get_imm12_I(instruction), get_funct3(instruction), 0b1111111);
    printf("Executed instruction %u, result x%d = %d + %d \n", instruction, get_rd(instruction), (int)x[get_rs1(instruction)], get_imm12_I(instruction));
}

void S_instruction_execute(unsigned int instruction, uint8_t* data_segment)
{
    switch(get_funct3(instruction))
    {
        //SW
        case 0b010:
        {
            uint32_t* data_address;
            data_address = (uint32_t*)(data_segment + x[get_rs1(instruction)] + get_imm12_S(instruction));
           // printf("address = %d ", (int)(data_segment + x[get_rs1(instruction)] + get_imm12_S(instruction)));
           // printf("address = %d = %d + %d + %d ", (int)(data_segment + get_imm12_I(instruction) + get_rs1(instruction)), (int)data_segment,
          //  (get_imm12_I(instruction)), x[get_rs1(instruction)]);
            *data_address = x[get_rs2(instruction)];
            printf("Executed instruction %u, at adress x%d + %d it stored the value %d from x%d\n", instruction, get_rs1(instruction), get_imm12_S(instruction)
            , *data_address, get_rs2(instruction));
            break;
        }
    }
}

void ILOAD_instruction_execute(unsigned int instruction, uint8_t* data_segment)
{
    switch(get_funct3(instruction))
    {
        //LW
        case 0b010:
        {
            uint32_t* data_address;
            data_address = (uint32_t*)(data_segment + get_imm12_I(instruction) + x[get_rs1(instruction)]);
            x[get_rd(instruction)] = *data_address;
         //   printf("address = %d = %d + %d + %d ", (int)(data_segment + get_imm12_I(instruction) + get_rs1(instruction)), (int)data_segment,
          //  (get_imm12_I(instruction)), x[get_rs1(instruction)]);
            printf("Executed instruction %u, from address x%d + %d loaded the value %d in x%d\n", instruction, get_rs1(instruction),
            get_imm12_I(instruction), *data_address, get_rd(instruction));
            break;
        }
    }
}


