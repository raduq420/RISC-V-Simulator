#include "cpu_functions.h"
#include "klog_extension.h"


int main(int argc, char* argv[])
{
    FILE* input_file = fopen("../assemblyParser/output.txt", "r");
    if (input_file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    //Emptying LOG file
    FILE* log_file = fopen("log.txt", "w");
    fprintf(log_file, "\n");
    fclose(log_file);

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
    unsigned int instruction_counter = 0;

    while(processor_running)
    {
        //Instruction fetch
        current_instruction = *pc;
        instruction_counter++;

        FILE* log_file = fopen("log.txt", "a");
        fprintf(log_file, "%d: ", instruction_counter);
        fclose(log_file);

        uint32_t old_pc = (uint32_t)pc;

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

        log_file = fopen("log.txt", "a");
        fprintf(log_file, "   PC = 0x%x / 0x%x\n", old_pc, (int)pc);
        fclose(log_file);
    }

    return 0;
}