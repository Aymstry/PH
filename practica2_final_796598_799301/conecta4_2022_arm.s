;*******************************************************************
; Autores: Ayelen Nu�o con NIA 799301 y Loreto Matinero con NIA 796598
;*******************************************************************
	AREA datos, DATA, READWRITE
deltas_fila 	DCB 0x00, 0xFF, 0xFF, 0x01
deltas_columna 	DCB 0xFF, 0x00, 0xFF, 0xFF
	
	AREA codigo, CODE	
	EXPORT conecta4_buscar_alineamiento_arm
	EXPORT conecta4_hay_linea_arm_c
	EXPORT conecta4_hay_linea_arm_arm
	IMPORT conecta4_buscar_alineamiento_c
	PRESERVE8 {TRUE}
	ENTRY
	
	; Parámetros y funciones asignadas a los mismos 
	; r0 = @tablero 
	; r1 = fila 
	; r2 = columna 
	; r3 = color 
	; r4 = contador del bucle exterior (el for en c)
	; r5 = delta_columna 
	; r6 = delta_fila 
	; r7 = @deltas 
	; r8 = resultado temporal 
	; r9  y r10 = se utilizan para calcular la @ de la celda 
	; r11 = copia de fila 
	; r12 = copia de columna 

	; Función que comprueba que hay 4 fichas iguales en línea.
	; Devolverá 1 en r0 si se ha hecho un 4 en ralla, en caso contrario devolverá 0.

conecta4_hay_linea_arm_arm
	STMDB R13!, {R4-R12, R14}
	mov r4, #0				; contador i del bucle for 
	LDR r7, =deltas_fila 	; r7 = @deltas_fila	
for 
	mov r8, #0		;contador resultado para alineamiento	
	; funcion alineamiento
	; actualizamos los deltas 
	ldrsb r6, [r7]			; r4 = valor deltas_fila
	ldrsb r5, [r7, #4]		; r5 = valor deltas_columna; r8 = r8 +1
	; comprobamos si son correctos los valores de la celda que nos proporcionan
	; estan dentro del tablero 
	mov r11, r1
	mov r12, r2
buc	cmp r1, #1
	blt termina 					; salta si r1 < 1 
	cmp r1, #6
	bgt termina 					; salta si r5 > NUM_FILAS
	cmp r2, #1
	blt termina 					; salta si r6 < 1 
	cmp r2, #7			 	
	bgt termina 					; salta si r6 > NUM_COLUMNAS 
	; comprobamos que la celda no sea vacia, y sea del mismo color 
	add r10, r0, r1, LSL #3			; r10 = @tablero + 8*fila 
	ldrb r9, [r10, r2] 				; r9 = dato de la celda = r10 + columna 
	tst r9, #0x4					; and lógico que actualiza los flags 
	beq termina 					; salta si flag z = 1 pq la celda estará vacia 
	and r10, r9, #0x03				; and logico para encontrar color de la celda
	cmp r10, r3						; comparacion del color obtenido con el guardado en r7
	bne termina	
	add r1, r1, r6					; nueva_fila = fila + delta_fila
	add r2, r2, r5					; nueva_columna = columna + delta_columna
	add r8, r8, #1					; incremento resultado
	b buc
termina
	cmp r8, #4						; salta si r4 >= 4 
	bge continua 
	mov r1, r11
	mov r2, r12 	
	; preparamos los parametros para la siguiente invocación 	
	ldrsb r6, [r7]			; r4 = valor deltas_fila
	ldrsb r5, [r7, #4]		; r5 = valor deltas_columna; r8 = r8 +1	

	mov r10, #-1			; r10 = -1 para actualizar los deltas
	sub r1, r1, r6			; fila - delta_fila
	sub r2, r2, r5			; columna - delta_columna
	mul r6, r10, r6			; delta_fila
	mul r5, r10, r5			; delta_columna
	; comprobamos si son correctos los valores de la celda que nos proporcionan
	; estan dentro del tablero 
buc1
	cmp r1, #1
	blt term	 					; salta si r1 < 1 
	cmp r1, #6
	bgt term 						; salta si r5 > NUM_FILAS
	cmp r2, #1
	blt term 						; salta si r6 < 1 
	cmp r2, #7			 	
	bgt term 						; salta si r6 > NUM_COLUMNAS 
	; comprobamos que la celda no sea vacia, y sea del mismo color 
	add r10, r0, r1, LSL #3			; r10 = @tablero + 8*fila 
	ldrb r9, [r10, r2] 				; r9 = dato de la celda = r10 + columna 
	tst r9, #0x4					; and lógico que actualiza los flags 
	beq term 						; salta si flag z = 1 pq la celda estará vacia 
	and r10, r9, #0x03				; and logico para encontrar color de la celda
	cmp r10, r3						; comparacion del color obtenido con el guardado en r7
	bne term	
	add r1, r1, r6					; nueva_fila = fila + delta_fila
	add r2, r2, r5					; nueva_columna = columna + delta_columna
	add r8, r8, #1					; incremento resultado
	b buc1
term	
	cmp r8, #4						; salta si r4 >= 4 
	bge continua  
	add r7, r7, #1					; r7 = r7 +1
	add r4, r4, #1 					; incrementamos contador 
	cmp r4, #4						; salta si r4 < 4 
	mov r1, r11
	mov r2, r12 
	blt for 	
	; salimos del bucle y terminamos la subrutina 	
continua
	cmp r8, #4 			; Guardamos el resultado en r0 para devolverlo 
	movge r0, #1
	movlt r0, #0
	LDMIA R13!, {R4-R12,PC}
	
	;-------------------------------------------------------------------------------------------------------------------------------------
	; Parámetros y funciones asignadas a los mismos 
	; r0 = @tablero y resultados
	; r1 = fila 
	; r2 = columna 
	; r3 = color 
	; r4 = contador i para el bucle for 
	; r9 = valor delta_fila 
	; r10 = valor delta_columna 
	; r11 = -1 (valor para actualizar los deltas), y resultados temporales
	; Estos cuatro registros actúan como variables auxiliares dado que su valor se modifica en las invocaciones a función.
	; r5 = fila 
	; r6 = @tablero  
	; r7 = @deltas 
	; r8 = columna 
	
	; Función que comprueba que hay 4 fichas iguales en línea a partir del resultado obtenido en conecta4_buscar_alineamiento_c
	; Devolverá 1 en r0 si se ha hecho un 4 en ralla, en caso contrario devolverá 0.
	
conecta4_hay_linea_arm_c

	STMDB R13!, {R4-R12, R14}
	mov r4, #0				; contador i del bucle for 
	mov r6, r0				; valor auxiliar del tablero
	mov r5, r1 				; r5 = valor de la fila
	mov r8, r2				; r8 = valor de la columna 
	mov r12, r3				; r12 = color
	LDR r7, =deltas_fila 	; r7 = @deltas_fila
	
for1 
	; actualizamos los deltas 
	ldrsb r9, [r7]			; r9 = valor deltas_fila
	ldrsb r10, [r7, #4]		; r10 = valor deltas_columna; r8 = r8 +1
	add r7, r7, #1			; r7 = r7 +1
	STMDB R13!, {r9,r10}	; apilamos los deltas
	; invocamos a la función para buscar en un sentido 
	bl conecta4_buscar_alineamiento_c
	add sp, sp, #8			; liberamos los parámetros 
	cmp r0, #4				; salta si r4 >= 4 
	movge r11, r0
	bge continua1  
	; preparamos los parametros para la siguiente invocación 
	mov r11, #-1			; r12 = -1 para actualizar los deltas
	mov r1, r5				; devolvemos el valor a r1 (fila)
	mov r2, r8 				; devolvemos el valor a r2 (columna)
	sub r1, r1, r9			; fila = fila - delta_fila 
	sub r2, r2, r10			; columna = columna - delta_columna 
	mul r9, r11, r9
	mul r10, r11, r10	
	; devolvemos los parámetros a sus registros correspondientes
	mov r11, r0				; resultado temporal en r11 
	mov r0, r6				; devolvemos el valor a r0 (cuadricula)
	mov r3, r12				; devolvemos el valor a r3 (color)
	STMDB R13!, {r9,r10}	; apilamos los deltas
	bl conecta4_buscar_alineamiento_c
	add r11, r11, r0 		; guardamos en r11 el valor del resultado actualizado
	add sp, sp, #8			; liberamos los parámetros 
	cmp r11, #4				; salta si r4 >= 4 
	bge continua1  
	; devolvemos a la normalidad los registros para la siguiente iteracion  
	mov r0, r6				; devolvemos el valor a r0 (cuadricula)
	mov r1, r5				; devolvemos el valor a r1 (fila)
	mov r2, r8 				; devolvemos el valor a r2 (columna)
	mov r3, r12				; devolvemos el valor a r3 (color)
	; comprobamos si volvemos a saltar al bucle 
	add r4, r4, #1 			; incrementamos contador 
	cmp r4, #4				; salta si r4 < 4 
	blt for1 	
	; salimos del bucle y terminamos la subrutina 	
continua1
	cmp r11, #4 			; Guardamos el resultado en r0 para devolverlo 
	movge r0, #1
	movlt r0, #0
	LDMIA R13!, {R4-R12,PC}

	;-------------------------------------------------------------------------------------------------------------------------------------
	; Parámetros y funciones asignadas a los mismos 
	; r0 = r4 = @ de tablero y paso de resultados a la función invocadora
	; r1 = r5 =  fila
	; r2 = r6 = columna 
	; r3 = r7 = color de la ficha 
	; r8 = se utilizará para guardar el valor #1 para apilarlo como resultado en caso de que sea necesario. 
	; r9 = deltaFila
	; r10 = deltaColumna

	; Función que cuenta el número de fichas iguales en una misma linea dado un sentido.
	; Devolverá el número de fichas iguales en línea por r0.

conecta4_buscar_alineamiento_arm
	STMDB R13!, {R4-R10,R14}
	mov r4, r0 						; r4 = tablero 
	mov r5, r1						; r5 = fila
	mov r6, r2 						; r6 = columna 
	mov r7, r3						; r7 = color
	; comprobamos si son correctos los valores de la celda que nos proporcionan
	; estan dentro del tablero 
	cmp r5, #1
	movlt r0, #0
	blt termina1 					; salta si r5 < 1 
	cmp r5, #6
	movgt r0, #0
	bgt termina1 					; salta si r5 > NUM_FILAS
	cmp r6, #1
	movlt r0, #0
	blt termina1 					; salta si r6 < 1 
	cmp r6, #7			 	
	movgt r0, #0
	bgt termina1 					; salta si r6 > NUM_COLUMNAS 
	; comprobamos que la celda no sea vacia, y sea del mismo color 
	add r10, r4, r5, LSL #3			; r10 = @tablero + 8*fila 
	ldrb r9, [r10, r6] 				; r9 = dato de la celda = r10 + columna 
	tst r9, #0x4					; and lógico que actualiza los flags 
	moveq r0, #0
	beq termina1 					; salta si flag z = 1 pq la celda estará vacia 
	and r10, r9, #0x03				; and logico para encontrar color de la celda
	cmp r10, r7						; comparacion del color obtenido con el guardado en r7
	movne r0, #0
	bne termina1						; salta si no son iguales
	; obtenemos el valor de delta y lo avanzamos 
	ldr r9, [sp, #32]		 		; r9 = deltaFila = sp + 12
	add r1, r5, r9					; r1 = nueva_fila = fila + delta_fila
	ldr r10, [sp, #36]				; r10 = deltaColumna = sp + 8
	add r2, r6, r10					; r2 = nueva_columna = columna + delta_columna
	;llamamos a la función recursiva
	mov r8, #1						; apilamos en resultado un 1
	STMDB R13!, {r8}
	STMDB R13!, {r9, r10}			; apilamos los parámetros y el resultado para la siguiente iteración 
	bl conecta4_buscar_alineamiento_arm
	ldr r1, [sp, #8]				; leemos resultado (1)
	add r0, r0, r1					; r0 = resultado final = resultado anterior (r0) + resultado de esta invocacion de la subrutina(r1)
	add sp, sp, #12					; liberamo el esacio de los parámetros apilados 
termina1 ; salimos de la subrutina 
	LDMIA R13!, {R4-R10, PC}
	END
