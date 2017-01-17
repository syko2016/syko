	.file "file_code_2.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
main: 
	ldm	r0, {r0, r4, r5, r6}
