	.text
	.file	"ex6.ll"
	.globl	ex6
	.type	ex6,@function
.ex6:
	cmp r0, r1
	ble .if_else
.if_then:
    add r0, r0, r1
	bx lr
.if_else:
    sub r0, r0, r1
	bx lr
.Ltmp0:
	.size	ex6, .Ltmp0-ex6
