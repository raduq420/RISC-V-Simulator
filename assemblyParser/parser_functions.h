#ifndef HEADER_PARSER_H
#define HEADER_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned short int opcode: 7;
    unsigned short int rd: 5;
    unsigned short int funct3: 3;
    unsigned short int rs1: 5;
    unsigned short int rs2: 5;
    unsigned short int funct7: 7;
} R_type;

extern R_type R_type_list[];
typedef struct {
    unsigned short int opcode: 7;
    unsigned short int rd: 5;
    unsigned short int funct3: 3;
    unsigned short int rs1: 5;
    unsigned short int imm12: 12;
} I_type;

extern I_type I_type_list[];

typedef struct {
    unsigned short int opcode: 7;
    unsigned short int imm12: 12;
    unsigned short int funct3: 3;
    unsigned short int rs1: 5;
    unsigned short int rs2: 5;
} S_type;

extern S_type S_type_list[];

typedef struct {
    const char* mnemonic;
    const char format;
    const unsigned short int index;
} instruction_mnemonics;

#define mnemonic_list_size 3
extern instruction_mnemonics instruction_mnemonics_list[];
void parse_and_encode_instruction(char line[], FILE* output_file);

#endif