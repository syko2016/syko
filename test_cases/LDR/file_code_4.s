/* Author: Konrad Gotfryd */

	.file "file_code.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 

main:
	ldr	r0, [r1, r2]
