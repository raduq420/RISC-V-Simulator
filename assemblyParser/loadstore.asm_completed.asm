#Stocheaza toate numerele fibonacci pana la al nr-lea, inclusiv
#la adresele 0x0, 0x4, 0x8, etc si compara la final valorile

.section .data

nr: .word 10

.section .text

#X10 - indicele fibonaci dorit, mai mare ca 0
addi x10, x0, nr

#X4 - valoarea anterioara
#X5 - valoarea curenta

#Caz indice 0
beq x10, x0, done

addi x4, x0, 1
addi x5, x0, 1
#X3 - adresa la care scriem
addi x3, x0, 0

#Stocam primele 2 valori
sw x5, 0(x3)
addi x3, x3, 4
sw x5, 0(x3)
addi x3, x3, 4

#Daca nr < 3 am terminat
addi x20, x0, 3
bge x20, X10, failed
addi x10, x10, -2

loop:
#Incepem stocarea
#X6 - valoarea anterioara a lui x5
add x6, x5, x0
#X5 = X4 + X5
add x5, x5, x4
#X4 = X5 anterior
add x4, x0, x6
#Stocam valoarea in memorie
sw x5, 0(x3)
addi x3, x3, 4
addi x10, x10, -1
bne x10, x0, loop

done:
#Verificam daca e corect
addi x4, x0, 1
addi x5, x0, 1
addi x3, x0, 0

#Verificam primele 2 elemente
lw x7, 0(x3)
bne x5, x7, failed
addi x3, x3, 4

lw x7, 0(x3)
bne x5, x7, failed
addi x3, x3, 4

#Resetam valoarea din x10 pentru a parcurge din nou sirul
addi x10, x0, nr
addi x10, x10, -2

loop2:
#Incepem stocarea
#X6 - valoarea anterioara a lui x5
add x6, x5, x0
#X5 = X4 + X5
add x5, x5, x4
#X4 = X5 anterior
add x4, x0, x6
#Verificam valoarea
lh x7, 0(x3)
addi x3, x3, 4
bne x7, x5, failed
addi x10, x10, -1
bne x10, x0, loop2



success:
addi x1, x1, 1
ebreak x1, x2
failed:
ori x1, x1, 0xFFFFFFFE
ebreak x1, x2