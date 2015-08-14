.data
	input: .space 407	# storage for input
.data
	shuntStack: .space 4 	# storage for stack that we will use in shunting-yard algorithm
.data
	evalStack: .space 101 # storage for stack that we will use while evaluating expression
.data
	output: .space 200	# storage for shunting yard algorithm output
.data
	letter: .space 53	# storage for letters in input
.data
	value: .space 53	# storage for values associated with letters in the previous input
.data
	message: .asciiz "Result: "

.text
main:
	evaluate:
		li $v0 8
		la $a0 input
		li $a1 407
		syscall
		la $s0 input
		la $s1 shuntStack
		la $s2 output
		la $s3 letter
		la $s4 value
		la $s5 evalStack

		li $t0 46	# .
		li $t1 42	# *
		li $t2 47	# /
		li $t3 35	# #

		ShuntCheckLoop:
			lb $t4 ($s0)	# look for the current character
			beq $t3 $t4 ShuntEraseStack	# if we are at the end of expression - erase stack
			beq $t0 $t4 ShuntPoint	# if we are reading '.' operator - goto '.' operator processing
			beq $t1 $t4 ShuntMult	# if we are reading '*' operator - goto '*' operator processing
			beq $t2 $t4 ShuntDiv		# if we are reading '/' operator - goto '/' operator processing
			j ShuntNumber	# else it is a value

		ShuntNumber:	# if we are reading value
			sb $t4 ($s2)	# push to the output
			addi $s2 $s2 1	# increment counter of output
			addi $s0 $s0 1	# increment counter of input
			j ShuntCheckLoop	# rinse & repeat

		# / > * > .

		ShuntDiv:	# if we are reading '/' operator
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero ShuntEmpPush	# if stack is empty - push
			beq $t5 $t2 ShuntDivPopLoop # if top of stack = '/' - pop loop
			j ShuntPush	# else push

		ShuntMult:	# if we are reading '*' operator
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero ShuntEmpPush	# if stack is empty - push
			beq $t5 $t1 ShuntMultPopLoop	# if top of stack = '*' - pop loop
			beq $t5 $t2 ShuntMultPopLoop	# if top of stack = '/' - pop loop
			j ShuntPush	# else push

		ShuntPoint:	# if we are reading '.' operator
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero ShuntEmpPush	# if stack is empty - push
			j ShuntPointPopLoop	# else - pop loop

		ShuntEmpPush:	# pushing operator to empty stack
			sb $t4 ($s1)	# push to top of the stack
			addi $s0 $s0 1	# increment the counter of input array
			j ShuntCheckLoop	# rinse & repeat

		ShuntPush:	# pushing operator to nonempty stack
			addi $s1 $s1 1
			sb $t4 ($s1)	# push to top of the stack
			addi $s0 $s0 1	# increment the counter of input array
			j ShuntCheckLoop	# rinse & repeat

		ShuntDivPopLoop:	# popping items from stack loop if we are pushing '/' operator
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero ShuntEmpPush	# if stack is empty - push
			beq $t5 $t0 ShuntPush	# if top of stack = '.' - push
			beq $t5 $t1 ShuntPush	# if top of stack = '*' - push
			sb $t5 ($s2)	# push operator to output
			addi $s1 $s1 -1	# pop operator from stack
			addi $s2 $s2 1	# increment the counter of output array
			j ShuntDivPopLoop	# rinse & repeat


		ShuntMultPopLoop:	# popping items from stack loop if we are pushing '*' operator
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero ShuntEmpPush	# if stack is empty - push
			beq $t5 $t0 ShuntPush	# if top of stack = '.' - push
			sb $t5 ($s2)	# push operator to output
			addi $s1 $s1 -1	# pop operator from stack
			addi $s2 $s2 1	# increment the counter of output array
			j ShuntMultPopLoop	# rinse & repeat


		ShuntPointPopLoop:	# popping items from stack loop if we are pushing '.' operator
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero ShuntEmpPush
			sb $t5 ($s2)	# push operator to output
			addi $s1 $s1 -1	# pop operator from stack
			addi $s2 $s2 1	# increment the counter of output array
			j ShuntPointPopLoop	# rinse & repeat


		ShuntEraseStack:	# erasing stack
			lb $t5 ($s1)	# read top of stack
			beq $t5 $zero Store	# if stack is empty - proceed to values
			sb $t5 ($s2)	# push operator to output
			addi $s1 $s1 -1	# decrement the counter of stack array
			addi $s2 $s2 1	# increment the counter of output array
			j ShuntEraseStack	# rinse & repeat


	#-----------------------------------------------------------------------------#


		Store:	# prepairing for storing values of letters
			addi $s0 $s0 1	# since we were reading '#'
			j StoreLettLoop	# going to loop


		StoreLettLoop:	# storing letter in an array
			lb $t4 ($s0)	# storing letter...
			sb $t4 ($s3)	# ... in a letter array
			addi $s3 $s3 1  # incrementing counter to letter array
			addi $s0 $s0 2  # now looking for a value
			j StoreValLoop

		StoreValLoop:	# storing value of associated letter in an array
			lb $t4 ($s0)	# storing value...
			sb $t4 ($s4)	# ... in a value array
			addi $s4 $s4 1	# incrementing counter to value array
			addi $s0 $s0 1	# checking...
			lb $t4 ($s0)	# ... if we are...
			beq $t4 $t3 Convert	# ...at the end of input (if we are, then we are going to convert all those letters in out postfix expression to proper values)
			addi $s0 $s0 1	# we aren't done, so...
			j StoreLettLoop	# rinse & repeat

	#------------------------------------------------------------------------------#

		Convert:	# preparing to convert expression
			la $s2 output	# going back to the beginning of expression array
			la $s3 letter	# going back to the beginning of letters array
			la $s4 value	# going back to the beginning of values array
			j ConvertCheckLoop	# going to loop


		ConvertCheckLoop:	# converting expression
			lb $t4 ($s2)	# checking current character
			beq $zero $t4 Eval	# checking if the expression is done (if so, we are proceeding to evaluate)
			beq $t0 $t4 ConvertIncrement	# if it is an operator...
			beq $t1 $t4 ConvertIncrement	# ... we won't...
			beq $t2 $t4 ConvertIncrement	# ... do anything
			li $t8 0	# $t8 - counter for searching
			j SearchLoop 	# going to search

		ConvertIncrement:	# incrementing counter in expression array
			sb $t4 ($s2)	# storing the value (or just replicating if it is an operator)
			addi $s2 $s2 1
			j ConvertCheckLoop	# rinse & repeat


		SearchLoop:	# searching index of a letter in a letter array
			lb $t5 ($s3)
			beq $t4 $t5 SearchLoopAfter	# if we found our letter - search for value
			addi $t8 $t8 1	# increment counter
			addi $s3 $s3 1	# going to next element
			j SearchLoop

		SearchLoopAfter:	# searching for value
			add $s4 $s4 $t8	# just using our counter...
			lb $t4 ($s4)	# ... to find a value
			la $s3 letter	# going to the beginning of letters array for further usage
			la $s4 value	# going to the beginning of values array for further usage
			j ConvertIncrement





	#------------------------------------------------------------------------------#

		Eval:	# prepairing to evaluate
			la $s2 output	# reloading expression array
			j EvalCheckLoop	# going to evaluate

		EvalCheckLoop:	# evaluating loop
			lb $t4 ($s2)	# checking current character
			beq $zero $t4 PrintResult	# if the expression has finished - print the result
			beq $t0 $t4 EvalPoint	# if it is '.' operator - evaluate it
			beq $t1 $t4 EvalMult	# if it is '*' operator - evaluate it
			beq $t2 $t4 EvalDiv		# if it is '/' operator - evaluate it
			j EvalPush	# if it is not an operator, then push value to the stack


		EvalPush:	# pushing value to the stack
			lb $t5 ($s5)	# looking for top of the stack
			beq $t5 $zero EvalEmpPush	# if stack is empty
			addi $s5 $s5 1
			sb $t4 ($s5)
			addi $s2 $s2 1
			j EvalCheckLoop	# rinse & repeat

		EvalEmpPush:	# pushing value to the stack if it is empty
			sb $t4 ($s5)
			addi $s2 $s2 1
			j EvalCheckLoop

		EvalPoint:	# evaluating with '.' operator
			lb $t4 ($s5)	# taking first number
			sb $zero ($s5)	# popping item from stack
			addi $s5 $s5 -1	# going back in the stack
			lb $t5 ($s5)	# taking second number
			addi $t4 $t4 -48	# converting character to number
			addi $t5 $t5 -48	# converting character to number
			and $t6 $t4 $t5	# performing "." operation
			addi $t6 $t6 48	# converting result to character
			sb $t6 ($s5)	# pushing it to the stack (actually, swapping it with the second number we have taken since it wasn't popped)
			addi $s2 $s2 1	# proceeding
			j EvalCheckLoop	# rinse & repeat


		EvalMult:	# evaluating with '*' operator
			lb $t4 ($s5)	# taking first number
			sb $zero ($s5)	# popping item from stack
			addi $s5 $s5 -1	# going back in the stack
			lb $t5 ($s5)	# taking second number
			addi $t4 $t4 -48	# converting character to number
			addi $t5 $t5 -48	# converting character to number
			or $t6 $t4 $t5	# performing "*" operation
			addi $t6 $t6 48	# converting result to character
			sb $t6 ($s5)	# pushing it to the stack (actually, swapping it with the second number we have taken since it wasn't popped)
			addi $s2 $s2 1	# proceeding
			j EvalCheckLoop	# rinse & repeat

		EvalDiv:	# evaluating with '/' operator
			lb $t4 ($s5)	# taking first number
			sb $zero ($s5)	# popping item from stack
			addi $s5 $s5 -1	# going back in the stack
			lb $t5 ($s5)	# taking second number
			addi $t4 $t4 -48	# converting character to number
			addi $t5 $t5 -48	# converting character to number
			xor $t6 $t4 $t5	# performing "/" operation
			addi $t6 $t6 48	# converting result to character
			sb $t6 ($s5)	# pushing it to the stack (actually, swapping it with the second number we have taken since it wasn't popped)
			addi $s2 $s2 1	# proceeding
			j EvalCheckLoop	# rinse & repeat


	#------------------------------------------------------------------------------#

		PrintResult:
			li $v0 4
			la $a0 message
			syscall
			lb $t4 ($s5)
			addi $t4 $t4 -48
			li $v0 1
			move $a0 $t4
			syscall
			j Exit
			
		Exit:
			jr $ra
