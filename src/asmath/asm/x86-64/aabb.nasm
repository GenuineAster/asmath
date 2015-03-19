; aabb.nasm (c) 2015 Mischa "Aster" Alff
; x86-64 Assembly implementations of AABB intersection checks.
; The algorithm in use here is:
;   !(B.left>A.right || B.right<A.left || B.top>A.bottom || B.bottom<A.top)


section .data
section .bss
	struc aabb_i32
		.left: resd 1
		.top: resd 1
		.right: resd 1
		.bottom: resd 1
	endstruc

section .text
	global aabb_intersects_i32:function
	global aabb_intersects_conventional_i32:function

	; check_aabb: calculates intersection between two AABBs using SSE
	; Notes: This routine runs 2.66 times faster than `aabb_intersects_conventional_i32`
	;   using the yasm assembler on x86-64 (Intel Haswell i7-4770)
	; Takes two arguments: *AABB and *AABB, in rdi and rsi respectively.
	aabb_intersects_i32:
		; load the first AABB from memory into xmm0 as four 32-bit signed ints
		movdqa xmm0, [rdi]
		; load the second AABB from memory into xmm1 as four 32-bit signed ints
		movdqa xmm1, [rsi]
		; shuffle the second AABB so that top/bottom and left/right are inverted
		shufps xmm1, xmm1, 01001110b
		; compare each signed 32-bit integer in xmm1 to the ones in xmm0 and
		;   store the result in xmm1 as a bit mask
		;   (0xFFFFFFFF for true, 0x0 for false)
		pcmpgtd xmm1, xmm0
		; obtain that bitmask in a two-byte register (we use rax here because it
		;   zeroes out all the other bits)
		pmovmskb rax, xmm1
		; invert the greater-than for two members to make them
		;   less-than-or-equal-to (yes, this is not as intended, but since this 
		;   is a PoC and will probably be adapted for floating point, nobody
		;   cares whether it's > or >= for now.)
		xor eax, 0x00FF
		; Convert our bitmask to a boolean, and not it:
		;   If the result of the xor is zero, the zero flag is 1, and thus al
		;     is 1 (and thus rax is 1)
		;   If not, the ZF is 0, and thus al is 0, and rax is 0.
		setz al
		; Still not sure if this is required, but better be safe than sorry,
		;   I guess. (Yes, C++ treats >0 as true, but I want a true logical 0/1)
		and eax, 0x1
		ret

	; check_aabb_conventional : calculates intersection between two AABBs using
	;   conventional methods
	; Notes: This is a naïve port of the C version mentioned above
	; Takes two arguments: *AABB and *AABB, in rdi and rsi respectively.
	aabb_intersects_conventional_i32:
		; set ecx to 0, as we'll be using it for our OR accumulator
		xor ecx, ecx
		; load B.left into eax, A.right into ebx, and compare for greater-than
		mov eax, dword [rdi + (aabb_i32.left)]
		mov ebx, dword [rsi + (aabb_i32.right)]
		cmp eax, ebx
		; place the result of the comparison in cl
		setg cl
		; ... repeat
		mov eax, dword [rdi + (aabb_i32.right)]
		mov ebx, dword [rsi + (aabb_i32.left)]
		cmp eax, ebx
		; place the result of the comparison in ch
		setle ch
		; or cl and ch, repeat
		or cl, ch
		mov eax, dword [rdi + (aabb_i32.top)]
		mov ebx, dword [rsi + (aabb_i32.bottom)]
		cmp eax, ebx
		setg ch
		or cl, ch
		mov eax, dword [rdi + (aabb_i32.bottom)]
		mov ebx, dword [rsi + (aabb_i32.top)]
		cmp eax, ebx
		setle ch
		or cl, ch
		; convert to logical bool and NOT.
		not ecx
		and ecx, 1
		mov eax, ecx
		ret
