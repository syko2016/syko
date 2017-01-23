	.file "file_code_1.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
main: 
	str	r0, [r1, #+16]
	str 	r2, [SP, #+32]
	str	r3, [r4, r5]
