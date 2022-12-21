
# Sanjana Vellanki
# UID: 117990881
# Directory ID: svellank
# Last Updated: November 30, 2022

isqrt:
     # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   # BODY
   bge $a0, 2, rec
   move $v0, $a0    
   j ret

   rec: 
    subu $sp, $sp, 4    
    sw $a0, 4($sp)      # store a0 (n) on stack

    srl $a0, $a0, 2     # shift two bits to the right

    jal isqrt           # recursive call
    sll $t0, $v0, 1     # $t0 = small = result shifted one bit to the left
    
    addu $t1, $t0, 1    # $t1 = large = small + 1
    mul $t2, $t1, $t1   # $t2 = large * large
    
    lw $t3, -8($fp)      # $t3 = n
    bgt $t2, $t3, small
    move $v0, $t1       # else returns large

    j ret

    small:
    move $v0, $t0       # returns small

    ret:
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel