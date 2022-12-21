
# Sanjana Vellanki
# UID: 117990881
# Directory ID: svellank
# Last Updated: November 30, 2022

reverse_prefix_sum:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra
   
   # BODY
   lw $t0, ($a0)          # load r into t0 
   bne $t0, -1, rec       # check if end of array is reached
   li $v0, 0              # return 0 when end of array 
   j ret
   
rec:
   subu $sp, $sp, 8       
   sw 	$a0, 8($sp)      # store a0 on stack for later use
   sw    $t0, 4($sp)      # store t0 on stack for later use
   addu  $a0, $a0, 4      # goes to next index

   jal reverse_prefix_sum # call reverse_prefix_sum recursively


   lw $t0, -12($fp)        # load original t0 from stack
   lw $a0, -8($fp)         # load original a0 from stack

   addu $v0, $v0, $t0      # add array value to recursive sum
   sw $v0, ($a0)           # store v0 value

ret:
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel