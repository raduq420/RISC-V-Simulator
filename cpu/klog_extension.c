#include "klog_extension.h"
#include "cpu_functions.h"
#include <inttypes.h>

void print_klog(unsigned int instruction, char instruction_type, char* mnemonic, int64_t oldValue, int64_t newValue)
{
    FILE* log_file = fopen("log.txt", "a");
    int16_t immediate_12;
    uint32_t immediate_20;
    switch(instruction_type)
    {
        case 'r':
            fprintf(log_file, "%s x%d, x%d, x%d\n", mnemonic, get_rd(instruction), get_rs1(instruction), get_rs2(instruction));
            fprintf(log_file, "   X%d = %" PRId64 " / %" PRId64 "\n", get_rd(instruction), oldValue, newValue);
            break;
        case 'i':
            // Need to store the value to guarantee proper 2's complement
            immediate_12 = get_imm12_I(instruction);
            fprintf(log_file, "%s x%d, x%d, %d\n", mnemonic, get_rd(instruction), get_rs1(instruction), immediate_12);
            fprintf(log_file, "   X%d = %" PRId64 " / %" PRId64 "\n", get_rd(instruction), oldValue, newValue);
            break;
        case 'I':
            fprintf(log_file, "%s x%d, %d(x%d)\n", mnemonic, get_rd(instruction), get_imm12_I(instruction), get_rs1(instruction));
            fprintf(log_file, "   MR 0x%" PRIx64 ": %" PRId64 "\n", (uint64_t)oldValue, newValue);
            break;
        case 's':
            fprintf(log_file, "%s x%d, %d(x%d)\n", mnemonic, get_rd(instruction), get_imm12_S(instruction), get_rs1(instruction));
            fprintf(log_file, "   MW 0x%" PRIx64 ": %" PRId64 "\n", (uint64_t)oldValue, newValue);
            break;
        case 'b':
            immediate_12 = get_immediate_branch(instruction);
            fprintf(log_file, "%s x%d, x%d, %d\n", mnemonic, get_rs1(instruction), get_rs2(instruction), immediate_12);
            break;
        case 'z':
            immediate_20 = (instruction >> 12);
            fprintf(log_file, "%s, x%d, %d\n", mnemonic, get_rd(instruction), immediate_20);
            fprintf(log_file, "   X%d = %" PRId64 " / %" PRId64 "\n", get_rd(instruction), oldValue, newValue);
    }
    fclose(log_file);
}