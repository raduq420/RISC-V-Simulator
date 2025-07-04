cd assemblyParser
gcc -c -Wall parser_functions.c parser.c
gcc parser.o parser_functions.o -o parser
parser testcode.asm
cd ..
cd cpu
gcc -c -Wall cpu.c cpu_functions.c .\klog_extension.c
gcc cpu.o cpu_functions.o klog_extension.o -o cpu
cpu
cd ..