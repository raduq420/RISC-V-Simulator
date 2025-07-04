#ifndef HEADER_CPU_H
#define HEADER_CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// R-type 
#define R_OP 0b0110011
#define IMM_OP 0b0010011
#define ILOAD_OP 0b0000011
#define S_OP 0b0100011

// I-type
#define ADDI_OP 0b0010011
#define LW_OP 0b0000011

// S-type
#define SW_OP 0b0100011

// B-type
#define B_OP 0b1100011

// Special types
#define LUI_OP 0b0110111
#define AUIPC_OP 0b00010111
#define JAL_OP 0b1101111
#define EBREAK_OP 0b01110011


typedef struct {
    int32_t value: 21;
} immediate21;

void populate_code_segment(unsigned int* code_segment, FILE* input_file);
void R_instruction_execute(unsigned int current_instruction);
void IMM_instruction_execute(unsigned int instruction);
void ILOAD_instruction_execute(unsigned int instruction, uint8_t* data_segment);
void S_instruction_execute(unsigned int instruction, uint8_t* data_segment);
void B_instruction_execute(unsigned int instruction, unsigned int** pc, bool* increment_pc);
void LUI_instruction_execute(unsigned int instruction);
void AUIPC_instruction_execute(unsigned int instruction, unsigned int PC);
void JAL_instruction_execute(unsigned int instruction, unsigned int** pc, bool* increment_pc);
void EBREAK_instruction_execute(unsigned int instruction);


uint8_t get_opcode(unsigned int instruction);
uint8_t get_rd(unsigned int instruction);
uint8_t get_funct3(unsigned int instruction);
uint8_t get_rs1(unsigned int instruction);
uint8_t get_rs2(unsigned int instruction); 
int16_t get_imm12_I(unsigned int instruction);
uint8_t get_imm12_S(unsigned int instruction);
int16_t get_immediate_branch(unsigned int instruction);
immediate21 get_immediate_jal(unsigned instruction);


#endif
