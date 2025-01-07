#include "klog_extension.h"
#include "cpu_functions.h"

void print_klog(unsigned int instruction, char instruction_type, char* mnemonic, uint32_t oldValue, uint32_t newValue)
{
    FILE* log_file = fopen("log.txt", "a");
    switch(instruction_type)
    {
        case 'r':
            fprintf(log_file, "%s x%d, x%d, x%d\n", mnemonic, get_rd(instruction), get_rs1(instruction), get_rs2(instruction));
            fprintf(log_file, "   X%d = %d / %d\n", get_rd(instruction), oldValue, newValue);
            break;
        case 'i':
            fprintf(log_file, "%s x%d, x%d, %d\n", mnemonic, get_rd(instruction), get_rs1(instruction), get_imm12_I(instruction));
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

    }
    fclose(log_file);

}