#ifndef HEADER_PARSER_H
#define HEADER_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    short int imm12: 12;
} I_type;

extern I_type I_type_list[];

typedef struct {
    unsigned short int opcode: 7;
    short int imm12: 12;
    unsigned short int funct3: 3;
    unsigned short int rs1: 5;
    unsigned short int rs2: 5;
} S_type;

extern S_type S_type_list[];

typedef struct {

    unsigned short int opcode: 7;
    unsigned short int funct3: 3;
    unsigned short int rs1: 5;
    unsigned short int rs2: 5;
    short int imm13: 13;

} B_type;

extern B_type B_type_list[];

typedef struct {
    const char* mnemonic;
    const char format;
    const unsigned short int index;
} instruction_mnemonics;

typedef struct {
    char name[20];
    unsigned int offset: 21;
} label_list;


#define mnemonic_list_size 17
extern instruction_mnemonics instruction_mnemonics_list[];
void parse_and_encode_instruction(char line[], FILE* output_file);
void save_label(char* token, unsigned int offset);

#endif