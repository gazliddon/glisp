
(define test-fn 
  (if println a b ))

(asm 
  lda #10
  sta $d020
  lda twat:x
  lda @twat 
  lda [twat]
  )


