# strlen(str)

strlen:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   li, $t0, 0              # counter for string length

   loop:
   lb, $t1, 0($a0)       # gets the nth byte of the string
   beqz $t1, end         # end loop if $t1 is null character
   add $a0, $a0, 1       # move pointer to next character
   add $t0, $t0, 1       # incremember length counter
   j loop


   end:
   move $v0, $t0
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
