#include "cpu_functions.h"



int main(int argc, char* argv[])
{
    FILE* input_file = fopen("../assemblyParser/output.txt", "r");
    if (input_file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    //Instantiate code segment
    //Allocating 400 bytes of memory for the code segment
    unsigned int* code_segment = (unsigned int*)malloc(sizeof(int) * 400);
    //Allocating 2048 bytes of memory for the data segment
    uint8_t* data_segment = (uint8_t*)malloc(sizeof(uint8_t)*2048);
    unsigned int* pc = code_segment;
    //Populating the code segment
    populate_code_segment(code_segment, input_file);

    

    bool processor_running = true;
    unsigned int current_instruction;
    while(processor_running)
    {
        //Instruction fetch
        current_instruction = *pc;
        switch(get_opcode(current_instruction))
        {
            case R_OP:
                printf("Starting R instruction\n");
                R_instruction_execute(current_instruction);
                break;

            case IMM_OP:
                printf("Starting IMM instruction\n");
                IMM_instruction_execute(current_instruction);
                break;

            case ILOAD_OP:
                printf("Starting LOAD instruction\n");
                ILOAD_instruction_execute(current_instruction, data_segment);
                break;
            
            case S_OP:
                printf("Starting S instruction\n");
                S_instruction_execute(current_instruction, data_segment);
        }

        pc += 1;
    }

    return 0;
}