*
*	F line trap handler for trace
*
*	most of code is from human.sys
*
*	K.Abe


	.xref	_display1
	.xref	_display2
	.xref	_Old_handler
	.xdef	_New_handler

sprcs_line1111_emulator_E45A		=$e45a
DOS_callVectorTable_1800		=$1800
WK_DOS_call_counter_1C08		=$1c08
DOS_call_stack_save_1C5C		=$1c5c
WK_DOS_function_No_1C0A			=$1c0a
WK_cur_FCB_adrs_1C98			=$1c98
L008176					=$8176
WK_DOS_want_call_prcs_switcher_1C14	=$1c14
L00E168					=$e168
L008242					=$8242
L00823E					=$823e




FLINE_TRAP_VECTOR_ADDRESS=11*4


_New_handler:
	movem.l	a5-a6,-(sp)
	move.l	d0,-(sp)
	lea.l	$E(sp),a6
	movea.l	(a6),a5				* a5 = (pc)
	move.w	(a5)+,d0			* get traped instruction
	cmpi.w	#$FFF5,d0
	bcc	not_doscall_8072
	cmpi.w	#$FF00,d0
	bcs	not_doscall_8072
	move.l	a5,(a6)+			* next pc is 2byte after
	btst.b	#$05,12(sp)
	bne.s	L007FFE
	move.l	usp,a6
L007FFE:
	addq.l	#4,sp
	movem.l	d1-d7/a0-a4,-(sp)

* our stack may be very poor.

	move.l	sp,stack_buffer
	lea.l	mystack,sp
	move.l	d0,-(sp)
	move.l	_Old_handler,FLINE_TRAP_VECTOR_ADDRESS
	move.l	a6,-(sp)
	move.l	d0,-(sp)
	jsr	_display1		* send DOS CALL NUMBER and ARGUMENT
	addq.l	#8,sp
	move.l	#_New_handler,FLINE_TRAP_VECTOR_ADDRESS
	move.l	(sp)+,d0
	move.l	stack_buffer,sp

	clr.l	d1
	move.b	d0,d1
	move.l	d1,d0
	lea.l	DOS_callVectorTable_1800,a0
	add.w	d1,d1
	add.w	d1,d1
	adda.l	d1,a0
	movea.l	(a0),a0
	tst.w	WK_DOS_call_counter_1C08
	bne.s	L00802C
	move.l	sp,DOS_call_stack_save_1C5C
	move.b	d0,WK_DOS_function_No_1C0A
L00802C:
	addq.w	#1,WK_DOS_call_counter_1C08
	clr.l	WK_cur_FCB_adrs_1C98
	jsr	(a0)
	clr.l	WK_cur_FCB_adrs_1C98
	subq.w	#1,WK_DOS_call_counter_1C08
	bne.s	L00804E
	clr.b	WK_DOS_function_No_1C0A
L00804E:
	move.l	d0,-(sp)
	jsr	L008176
	move.l	(sp)+,d0

	movem.l	(sp)+,d1-d7/a0-a4
	movem.l	(sp)+,a5-a6

* our stack may be very poor.
	move.l	sp,stack_buffer
	lea.l	mystack,sp
	movem.l	d0-d7/a0-a6,-(sp)
	move.l	_Old_handler,FLINE_TRAP_VECTOR_ADDRESS
	clr.l	-(sp)
	move.l	d0,-(sp)
	jsr	_display2		* send RESULT CODE
	addq.l	#8,sp
	move.l	#_New_handler,FLINE_TRAP_VECTOR_ADDRESS
	movem.l	(sp)+,d0-d7/a0-a6
	move.l	stack_buffer,sp

	tst.b	WK_DOS_want_call_prcs_switcher_1C14
	beq	skip2
	jmp	L00E168
  skip2:
L008064:
	btst.b	#$07,(sp)
	bne.s	L00806C
	rte

L00806C:
	ori.w	#$8000,sr
	rte

not_doscall_8072:
*	move.l	_Old_handler,FLINE_TRAP_VECTOR_ADDRESS
	move.l	(sp)+,d0
	movem.l	(sp)+,a5-a6
	move.l	_Old_handler,-(sp)
	rts

	.bss
stack_buffer:
	ds.l	1

	ds.b	4096
mystack:

	.end
