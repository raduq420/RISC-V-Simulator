#include "klog_extension.h"
#include "cpu_functions.h"

void print_klog(unsigned int instruction, char instruction_type, char* mnemonic, int32_t oldValue, int32_t newValue)
{
    FILE* log_file = fopen("log.txt", "a");
    int16_t immediate_12;
    switch(instruction_type)
    {
        case 'r':
            fprintf(log_file, "%s x%d, x%d, x%d\n", mnemonic, get_rd(instruction), get_rs1(instruction), get_rs2(instruction));
            fprintf(log_file, "   X%d = %d / %d\n", get_rd(instruction), oldValue, newValue);
            break;
        case 'i':
            // Need to store the value to guarrantee proper 2's complement
            immediate_12 = get_imm12_I(instruction);
            fprintf(log_file, "%s x%d, x%d, %d\n", mnemonic, get_rd(instruction), get_rs1(instruction), immediate_12);
            fprintf(log_file, "   X%d = %d / %d\n", get_rd(instruction), oldValue, newValue);
            break;
        case 'I':
            fprintf(log_file, "%s x%d, %d(x%d)\n", mnemonic, get_rd(instruction), get_imm12_I(instruction), get_rs1(instruction));
            fprintf(log_file, "   MR 0x%x: %d\n", oldValue, newValue);
            break;
        case 's':
            fprintf(log_file, "%s x%d, %d(x%d)\n", mnemonic, get_rd(instruction), get_imm12_S(instruction), get_rs1(instruction));
            fprintf(log_file, "   MW 0x%x: %d\n", oldValue, newValue);
            break;
        case 'b':
            immediate_12 = get_immediate_branch(instruction);
            printf("imm= %d", immediate_12);
            fprintf(log_file, "%s x%d, x%d, %d\n", mnemonic, get_rs1(instruction), get_rs2(instruction), immediate_12);
            
    }
    fclose(log_file);

}