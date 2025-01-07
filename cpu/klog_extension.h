#ifndef HEADER_KLOG_H
#define HEADER_KLOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

void print_klog(unsigned int instruction, char instruction_type, char* mnemonic, uint32_t oldValue, uint32_t newValue);

#endif