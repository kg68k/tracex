*	F line trap handler for trace
*
*	most of code is from human.sys
*
*	K.Abe

;Copyright (C) 2025 TcbnErik
;
;This file is part of tracex.
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with this program.  If not, see <https://www.gnu.org/licenses/>.

.include doscall.mac
.include iocscall.mac

;display.c
.xref _display1
.xref _display2

;LIBC
.xref __iob
.xref _exit
.xref _fputs


;Human68k ver 3.02 以外で使用する場合はこれを変更すること。
;2.02,2.03(の一種類),3.01,3.02 以外は対応していない。
.ifndef HUMAN_VERSION
  HUMAN_VERSION: .equ 0x302
.endif

STACK_SIZE: .equ 8192
.fail (STACK_SIZE.lt.4096).or.(STACK_SIZE.and.%11)


EXIT_FAILURE: .equ 1
stderr: .reg __iob+36*2

;例外処理ベクタ
PRIV_VEC:  .equ 8
FLINE_VEC: .equ 11

;IOCSワーク
MPU_TYPE: .equ $cbc

;バージョン依存
.if HUMAN_VERSION.eq.0x202
OScode_abort_check:    .equ $8176  ;アボート処理(trap #10/11)
OScode_change_process: .equ $e168  ;プロセス切り換え
.elseif HUMAN_VERSION.eq.0x203
OScode_abort_check:    .equ $80b8
OScode_change_process: .equ $df88
.elseif HUMAN_VERSION.eq.0x301
OScode_abort_check:    .equ $869e
OScode_change_process: .equ $df3a
.else HUMAN_VERSION.eq.0x302
OScode_abort_check:    .equ $8740
OScode_change_process: .equ $e050
.else
  .fail 1
.endif

;OSワーク
DOS_callVectorTable_1800: .equ $1800
WK_DOS_call_counter_1C08: .equ $1c08
WK_DOS_function_No_1C0A:  .equ $1c0a
WK_DOS_want_call_prcs_switcher_1C14: .equ $1c14
DOS_call_stack_save_1C5C:  .equ $1c5c
WK_cur_FCB_adrs_1C98:      .equ $1c98


.cpu 68000

.text
.even

_Old_priv:  .dc.l 0
_Old_fline: .dc.l 0

_Fefunc_id:
	.dc.l	0			;'FEfn'
_New_handler:
	movem.l	d0/a5-a6,-(sp)		;L008598@human3.02
	lea	(4*3+2,sp),a6
	movea.l	(a6),a5			;a5 = (pc)
	move	(a5)+,d0		;get traped instruction

	cmpi	#$ff00,d0
	bcs	not_doscall
	cmpi	#$fff5,d0
	bcc	not_doscall

	move.l	a5,(a6)+		;next pc is 2byte after
	tst.b	(MPU_TYPE)
	beq	@f
	addq.l	#2,a6			;68010以降
@@:
	addq.l	#4,sp			;破壊が確定したd0.lの分
	movem.l	d1-d7/a0-a4,-(sp)

	btst	#5,(4*(7+5+2),sp)	;元からsuperか?
	bne	@f
	move.l	usp,a6
@@:
;ここから
	move.l	sp,(stack_buffer)
	lea	(mystack),sp
	move.l	(_Old_fline,pc),(FLINE_VEC*4)
	move.l	a6,-(sp)
	move.l	d0,-(sp)
	bsr	_display1		;send DOS CALL NUMBER and ARGUMENT
	move.l	(sp)+,d0
	addq.l	#4,sp
	move.l	#_New_handler,(FLINE_VEC*4)
	movea.l	(sp),sp
;ここまで
	moveq	#0,d1
	move.b	d0,d1
	move.l	d1,d0
	lea	(DOS_callVectorTable_1800),a0
	add	d1,d1
	add	d1,d1
	movea.l	(a0,d1.w),a0

	tst	(WK_DOS_call_counter_1C08)
	bne	@f

	move.l	sp,(DOS_call_stack_save_1C5C)	;分岐直前のssp
	move.b	d0,(WK_DOS_function_No_1C0A)	;実行中のDOSコール番号
@@:
	addq	#1,(WK_DOS_call_counter_1C08)	;inDOSフラグ
	clr.l	(WK_cur_FCB_adrs_1C98)
	jsr	(a0)
	clr.l	(WK_cur_FCB_adrs_1C98)
	subq	#1,(WK_DOS_call_counter_1C08)
	bne	@f

	clr.b	(WK_DOS_function_No_1C0A)	;DOSコールの実行終了
@@:
	move.l	d0,-(sp)
	jsr	(OScode_abort_check)
;ここから
	move.l	sp,(stack_buffer)
	lea	(mystack),sp
	move.l	(_Old_fline,pc),(FLINE_VEC*4)
	move.l	d0,-(sp)
	bsr	_display2		;send RESULT CODE
	addq.l	#4,sp
	move.l	#_New_handler,(FLINE_VEC*4)
	movea.l	(sp),sp
;ここまで
	movem.l	(sp)+,d0-d7/a0-a6

	tst.b	(WK_DOS_want_call_prcs_switcher_1C14)
	beq	@f			;プロセス切り換えスイッチ
	jmp	(OScode_change_process)
@@:
	tst	(sp)			;btst #15-8,(sp)
	bpl	@f
	ori	#$8000,sr
@@:	rte

not_doscall:
	movem.l	(sp)+,d0/a5-a6
	move.l	(_Old_fline,pc),-(sp)
	rts

;trace出力中で使用されるDOSコールまでtraceしないように、
;F-lineを一時的に元のベクタに戻す。
;LIBCでは特権違反となるDOSコールは使用せずに移動後の番号を
;使っているので、特権違反の方はフックしたままで良い。


;extern void check_human_version( void );
_check_human_version::
	DOS	_VERNUM
	cmpi	#HUMAN_VERSION,d0
	beq	human_version_ok

	pea	(version_error_message,pc)
	bra	_error_exit

;extern void error_exit( const char *format );
_error_exit::
	move.l	#stderr,(4,sp)
	bsr	_fputs
	pea	(EXIT_FAILURE)
	bsr	_exit
*	DOS	_EXIT

;extern void setup_doscall_handler( void );
_setup_doscall_handler::
	move	#FLINE_VEC,-(sp)
	DOS	_INTVCG
	addq.l	#2,sp
	movea.l	d0,a1
	subq.l	#4,a1
	IOCS	_B_LPEEK
	lea	(_Fefunc_id,pc),a1
	move.l	d0,(a1)+
	lea	(_Old_priv,pc),a0
**	lea	(_New_handler,pc),a1
	bsr	intvcs_priv
	move.l	d0,(a0)+		;_Old_priv
	bsr	intvcs_fline
	move.l	d0,(a0)+		;_Old_fline
human_version_ok:
	rts

;extern void restore_doscall_handler( void );
_restore_doscall_handler::
	lea	(_Old_priv,pc),a0
	movea.l	(a0)+,a1		;_Old_priv
	bsr	intvcs_priv
	movea.l	(a0)+,a1		;_Old_fline
*	bsr	intvcs_fline
intvcs_fline:
	move	#FLINE_VEC,d1
	IOCS	_B_INTVCS
	rts
intvcs_priv:
	move	#PRIV_VEC,d1
	IOCS	_B_INTVCS
	rts


.data

version_error_message:
	.dc.b	'trace: this version of human68k is not supported.',13,10,0
	.even


.bss

.even
	.ds.b	STACK_SIZE
mystack:
stack_buffer:	.ds.l	1		;mystack==stack_buffer であること


.end
