SNKDec:
	move.w #$2700,sr
	movem.l	d0/d1/d3-a0/a2-a6,-(sp)
    	;d2 = VRAM address
	;a1 = compressed art to write to VRAM
SNKDecToVRAM:
	lsl.l	#2,d2 ; Moves all bits around, putting the top ones in (word-swapped) place
	addq.w	#1,d2 ; Bit for write
	ror.w	#2,d2 ; Move bits in low word to (word-swapped) place
	swap d2       ; Put all bits in the proper place
	andi.w #3,d2  ; Remove junk that might have been at the top half of d2
	lea	(VDP_data_port).l,a2
	move.l d2,$4(a2) ;(VDP_control_port).l
SNKDecMain:	
	;16 words = 1 tile
	moveq #0,d0
	moveq #0,d1
	move.w	(a1)+,d1 
	lsl.l   #5,d1 ;number of uncompressed bytes
	lsr.l #1,d1 ;number of uncompressed words
	
	move.b	(a1)+,d3
	tst.b d0
	bne.s +
	move.b d3,d6
	lsl.w   #8,d6
	st d0
	bra.s ++
+
	move.b  d3,d6
	move.w  d6,(a2)
	clr.b d0
	dbf d1,+
	bra.s SNKDecEnd
+
-	move.b	(a1)+,d4
	cmp.b d3,d4
	bne.s SNKDecCont
	tst.b d0
	bne.s +
	move.b d3,d6
	lsl.w   #8,d6
	st d0
	bra.s ++
+
	move.b  d3,d6
	move.w  d6,(a2)
	clr.b d0
	dbf d1,+
	bra.s SNKDecEnd
+	
	move.b	(a1)+,d5
-
	tst.b d5
	beq.s --
	sub.b #1,d5
	tst.b d0
	bne.s +
	move.b d3,d6
	lsl.w   #8,d6
	st d0
	bra.s -
+
	move.b  d3,d6
	move.w  d6,(a2)
	clr.b d0
	dbf d1,-
	bra.s SNKDecEnd
	
SNKDecCont:
	move.b d4,d3
	tst.b d0
	bne.s +
	move.b d3,d6
	lsl.w   #8,d6
	st d0
	bra.s --
+
	move.b  d3,d6
	move.w  d6,(a2)
	clr.b d0
	dbf d1,--
	
SNKDecEnd:
	movem.l (sp)+,d0/d1/d3-a0/a2-a6
	move.w #$2000,sr
	rts
