	.file "file_code.s" 
	.text 
	.align   2 
	.global  main 
	.type    main, %function 
main: 
	adds	r2, r1, #3
	add	r0, #250
	add	r2, r1, r0
	add	r12, PC
	add	r1, SP, #256
	add 	SP, SP, #16
	add	r0, SP, r0
	add	SP, r12
	and	r1 ,r1
	asr	r0, r1, #31
	asr	r0, r0, r2
	bmi	64	//for condition list look to the page 99 of ARMv6-M
			//architecture reference.

	b	1024
	bl	512
	ldm	r0!, {r1, r2, r3}
	ldr	r0, [r1, #+16]
	ldr	r0, [SP, #+128]
	ldr	r0, [PC, #128]
	ldr	r0, [r1, r2]
	mov	r0, #128
	rev r0, r1
	str r0, [r1, #8]
	str r0, [SP, #128]
	str r0, [r2, r4]
