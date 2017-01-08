	.file "file_code.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
main: 
	bl test3
	and r0, r1
test3:
	and r2, r3
