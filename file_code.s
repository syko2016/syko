	.file "file_code.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
main: 
	add	r2, r1, #3
	add	r0, #250
	add	r2, r1, r0
	add	r12, PC
	add	r1, SP, #256
	add 	SP, SP, #16
	add	r0, SP, r0
	add	SP, r12
	and	r1 ,r1	//it is compiled to ands by default!
	asr	r0, r1, #31
	asr	r0, r0, r2
