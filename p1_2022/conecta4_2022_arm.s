;*******************************************************************
; Autores: Ayelen Nu�o con NIA 799301 y Loreto Matinero con NIA 796598
;*******************************************************************

; Parámetros que nos pasan a la función 
    ; r0 = tablero  
    ; r1 = fila 
    ; r2 = columna 
	; r3 = color 

	; Faltan las deltas que se pasan siemre a través de la pila
	; delta de la columna es sp 
	; delta de la fila es fp +4    

	AREA codigo, CODE
	EXPORT conecta4_buscar_alineamiento_arm
	

conecta4_buscar_alineamiento_arm
	STMDB R13!, {R4-R10,R14}
	mov r4, r0 			; r4 = tablero 
	mov r5, r1			; r5 = fila
	mov r6, r2 			; r6 = columna 
	mov r7, r3			; r7 = color
	; comprobamos si son correctos los valores de la celda que nos proporcionan
	; estan dentro del tablero 
	cmp r5, #1
	movlt r0, #0
	blt termina 			; salta si r5 >= 1 
	cmp r5, #6
	movgt r0, #0
	bgt termina 			; salta si r4 > NUM_FILAS
	cmp r6, #1
	movlt r0, #0
	blt termina 			; salta si r6 >= 1 
	cmp r6, #7			 	; salta si r6 <= NUM_COLUMNAS 
	movgt r0, #0
	bgt termina 		
	; comprobamos que la celda no sea vacia, y sea del mismo color 
	add r10, r4, r5, LSL #3			; r10 = @tablero + 8*fila 
	ldrb r9, [r10, r6] 				; r9 = dato de la celda = r10 + columna 
	tst r9, #0x4					; and lógico que actualiza los flags 
	moveq r0, #0
	beq termina 					; salta si flag z = 1 pq la celda estará vacia 
	and r10, r9, #0x03				; and logico para encontrar color de la celda
	cmp r10, r7						; comparacion del color obtenido con el guardado en r7
	movne r0, #0
	bne termina						; salta si no son iguales
	; obtenemos el valor de delta y lo avanzamos 
	ldr r9, [sp, #32]		 		; r9 = deltaFila = sp + 12
	add r1, r5, r9					; r5 = nueva_fila = fila + delta_fila
	ldr r10, [sp, #36]				; r10 = deltaColumna = sp + 8
	add r2, r6, r10					; r6 = nueva_columna = columna + delta_columna
	;llamamos a la función recursiva
	mov r8, #1						; apilamos en resultado un 1
	STMDB R13!, {r8}
	STMDB R13!, {r9, r10}		; apilamos los parámetros y el resultado para la siguiente iteración 
	bl conecta4_buscar_alineamiento_arm
	ldr r1, [sp, #8]				; leemos resultado (1)
	add r0, r0, r1					; r0 = resultado final = resultado anterior (r0) + resultado de esta invocacion de la subrutina(r1)
	add sp, sp, #12					; liberamo el esacio de los parámetros apilados 
termina ; salimos de la subrutina 
	LDMIA R13!, {R4-R10, R14}
	mov pc, r14
	END
