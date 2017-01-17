/* Author: Konrad Gotfryd */

	.file "file_code.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
 
main:	bmi test
	and r0, r1
test:	and r2, r3
