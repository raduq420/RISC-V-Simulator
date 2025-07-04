#Calculeaza al nr-lea numar fibonacci si il compara cu valoare din value

.section .data

nr: .word 10
value: .word 55

.section .text

#X10 - indicele fibonaci dorit, mai mare ca 0
addi x10, x0, nr
#X11 - valoarea acelui indice
addi x11, x11, value

#X4 - valoarea anterioara
#X5 - valoarea curenta

#Caz indice 0
beq x10, x0, done

addi x4, x0, 1
addi x5, x0, 1

#Caz indice 1 sau 2
addi x12, x0, 3
#Daca nr e mai mic decat 3 ne oprim aici
bge x12, x10, done
addi x10, x10, -2

loop:

addi x10, x10, -1
#Salvam valoarea lui X5
add x6, x5, x0
#X5 devin X5+X4
add x5, x5, x4
#x4 devine X5-ul anterior
add x4, x6, x0
bne x10, x0, loop

done:

#prepare to print registers 4 and 5
addi x2, x0, 9
slli x2, x2, 1
addi x1, x0, 11
slli x1, x1, 8
or x1, x1, x2
bne x5, x11, failed
