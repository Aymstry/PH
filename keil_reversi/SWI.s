;/*****************************************************************************/
;/* SWI.S: SWI Handler                                                        */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/

T_Bit           EQU     0x20
I_Bit           EQU     0x80            ; when I bit is set, IRQ is disabled 1000 0000
F_Bit           EQU     0x40            ; when F bit is set, FIQ is disabled 0100 0000 

                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  SWI_Handler
SWI_Handler   

                STMFD   SP!, {R12, LR}         ; Store R12, LR
                MRS     R12, SPSR              ; Get SPSR ----- Guardar el estado (Usuario / Supervisor)
                STMFD   SP!, {R8, R12}         ; Store R8, SPSR
                TST     R12, #T_Bit            ; Check Thumb Bit ---- 23-27 chequea algo de bits thumb
                LDRNEH  R12, [LR,#-2]          ; Thumb: Load Halfword
                BICNE   R12, R12, #0xFF00      ; Extract SWI Number
                LDREQ   R12, [LR,#-4]          ; ARM:   Load Word
                BICEQ   R12, R12, #0xFF000000  ; Extract SWI Number -- Se obtiene el numero de la llamda al sistema (en este caso decrementar)

; add code to enable/disable the global IRQ flag
                CMP     R12,#0xFF              
                BEQ     __enable_irq
                CMP     R12,#0xFE
                BEQ     __disable_irq                  
                CMP     R12,#0xFD
                BEQ     __enable_irq_fiq
                CMP     R12,#0xFC
                BEQ     __disable_irq_fiq
                
                LDR     R8, SWI_Count
                CMP     R12, R8
                BHS     SWI_Dead               ; Overflow
                ADR     R8, SWI_Table
                LDR     R12, [R8,R12,LSL #2]   ; Load SWI Function Address -- saltamos a la tabla
                MOV     LR, PC                 ; Return Address
                BX      R12                    ; Call SWI Function 

                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR ----- Recupera el estado (Usuario / Supervisor)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

SWI_Dead        B       SWI_Dead               ; None Existing SWI

SWI_Cnt         EQU    (SWI_End-SWI_Table)/4
SWI_Count       DCD     SWI_Cnt

                IMPORT  __SWI_0                 ; importa las funciones añadidas en modo swi 
                IMPORT  __SWI_1
;               IMPORT  __SWI_2                 ; read_irq
;               IMPORT  __SWI_3                 ; read_fiq
SWI_Table
                DCD     __SWI_0                ; SWI 0 Function Entry crea la tabla con las direcciones de las funciones
                DCD     __SWI_1                ; SWI 1 Function Entry
                DCD     __read_IRQ_bit         ; SWI 2 Function Entry
                DCD     __read_FIQ_bit         ; SWI 3 Function Entry

;               ...
SWI_End

                EXTERN shared_var [DATA,SIZE=4]

__read_IRQ_bit
                MRS     R12, cpsr 
                LSL     R12, R12, #24 
                LSR     R12, R12, #31          ; Desplazamiento, nos quedamos con el bit 7 del registro de estados
                MOV     R12, R0                ; Devolvemos el valor a través de R0  
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR ------ LINEA DUDOSA
                MSR     SPSR_cxsf, R12         ; Set SPSR --- Devolver spsr al sistema para volver al modo en el que estabamos (usuario)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return     

__read_FIQ_bit
                MRS     R12, cpsr 
                LSL     R12, R12, #25 
                LSR     R12, R12, #31 
                MOV     R12, R0                ; Devovlemos valor por r0 
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR ------ LINEA DUDOSA
                MSR     SPSR_cxsf, R12         ; Set SPSR --- Devolver spsr al sistema para volver al modo en el que estabamos (usuario)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return 

__enable_irq
; activamos las interrupciones irq   __enable_irq // solo se ha cambiado el nombre el resto falta :X LDR R8, =shared_var
		MRS     R12, cpsr              ; leemos el registro cpsr  
                AND     R12, R12, #0XFFFFFF7F
                MSR     CPSR_c, R12
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR --- Devolver spsr al sistema para volver al modo en el que estabamos (usuario)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

__disable_irq   
                MRS     R12, cpsr              ; leemos el registro cpsr  
                ORR     R12, R12, #I_Bit 
                MSR     CPSR_c, R12 
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR ------ LINEA DUDOSA
                MSR     SPSR_cxsf, R12         ; Set SPSR --- Devolver spsr al sistema para volver al modo en el que estabamos (usuario)
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return             
                
__disable_irq_fiq
                MRS     R12, cpsr 
                ORR     R12, R12, #F_Bit
                MSR     CPSR_c, R12 
                B       __disable_irq 


__enable_irq_fiq
                MRS     R12, cpsr 
                AND     R12, R12, #0XFFFFFFBF
                MSR     CPSR_c, R12 
                B       __enable_irq  
                




; void             __swi(0xFF)             enable_irq(void);
; si irq desactivar irq, pero aceptar fiq y si fiq no tepuede llegar irq que ya se autogestiona pero ... pensar palabras del señor 


		END



