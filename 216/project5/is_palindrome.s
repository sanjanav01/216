
# Sanjana Vellanki
# UID: 117990881
# Directory ID: svellank
# Last Updated: November 30, 2022

strlen:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   li, $t0, 0              # counter for string length

   whileloop:
   lb, $t1, 0($a0)       # gets the nth byte of the string
   beqz $t1, end         # end loop if arg is null character
   add $a0, $a0, 1       # move pointer to next character
   add $t0, $t0, 1       # incremember length counter
   j whileloop


   end:
   move $v0, $t0
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel

is_palindrome:

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   jal strlen              # call strlen function

   move $t0, $v0           # t0 = strlen


   move $t1, $a0           # $t1 = end of pointer
   sub $t2, $t1, 1         # end of the word
   sub $t1, $t1, $t0       # first letter
   
   div $t0, $t0, 2         # $t0 = strlen/2
   add $t0, $t0, 1         # in case its even 
   li $t3, 0               # counter at 0
   li $v0, 1               # default value (is palindrome)

   loop:
        beq $t3, $t0, endloop       # counter == strlen/2
        lb $t4,  0($t1)             # first char of string
        lb $t5, 0($t2)              # last char of string
        beq $t4, $t5, continue      # if equal, continue
        li $v0, 0                   # if not, set v0 to 0
        j endloop

        continue:
            add $t1, $t1, 1     # Increment pointer at beginning of string 
            sub $t2, $t2, 1     # decrement pointer at end of string
            add $t3, $t3, 1     # increment counter
            j loop              # return to loop
        
endloop:
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel

        

