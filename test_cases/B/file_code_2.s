	.file "file_code.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
 
main:	b test
	and r0, r1
test:	and r2, r3
