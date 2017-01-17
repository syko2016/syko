/* Author: Konrad Gotfryd */

	.file "file_code_1.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
main: 
	ldm	r0!, {r1, r2, r3}
