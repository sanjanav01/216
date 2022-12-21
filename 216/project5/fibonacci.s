
# Sanjana Vellanki
# UID: 117990881
# Directory ID: svellank
# Last Updated: November 30, 2022

fibonacci:

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   #BODY
   bgt	$a0, 1, rec         # if arg >= 1 go to rec
   move	$v0, $a0
   j ret

rec: 
   subu	$sp, $sp, 8	      
   sw 	$a0, 4($sp)         # save arg j on stack

   sub $a0, $a0, 1	       # $a0 = arg j-1
   jal  fibonacci 		    # call fibonacci
   sw   $v0, 8($sp)         # save fibonacci(n-1) on stack  
   lw $a0, 4($sp)          # restore orginial arg value 
   sub $a0, $a0, 2	       # $a0 = arg j-2
   jal  fibonacci 	       # call fibonacci

   lw $t0, 8($sp)          # t0 = fibonacci(n-1) value
   add $v0, $v0, $t0        # $v0 = fibonacci(n-1) + fibonacci(n-2)

ret:
   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel