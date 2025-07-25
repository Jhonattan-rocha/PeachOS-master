[BITS 32]

section .asm

global print:function
global echo:function
global ls:function
global lss:function
global cat:function
global mkdir:function
global clear:function
global peachos_getkey:function
global peachos_malloc:function
global peachos_free:function
global peachos_putchar:function
global peachos_process_load_start:function
global peachos_process_get_arguments:function 
global peachos_system:function
global peachos_exit:function

; void cat(const char* path)
cat:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 14 ; Command cat
    int 0x80
    add esp, 4
    pop ebp
    ret

; void clear()
clear:
    push ebp
    mov ebp, esp
    mov eax, 13 ; Command clear
    int 0x80
    pop ebp
    ret

; void mkdir(const char* path)
mkdir:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 12 ; Command mkdir
    int 0x80
    add esp, 4
    pop ebp
    ret

; void ls(const char* path)
ls:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 11 ; Command ls
    int 0x80
    add esp, 4
    pop ebp
    ret

; int ls(const char* path, char output[][PEACHOS_MAX_PATH], int max_items)
lss:
    push ebp
    mov ebp, esp
    push dword[ebp+16] ; max_items
    push dword[ebp+12] ; output buffer
    push dword[ebp+8]  ; path
    mov eax, 15        ; novo syscall id para ls_v2
    int 0x80
    add esp, 12
    pop ebp
    ret

; void echo(const char* str)
echo:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 10 ; Meu comando para teste
    int 0x80
    add esp, 4
    pop ebp
    ret

; void print(const char* filename)
print:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 1 ; Command print
    int 0x80
    add esp, 4
    pop ebp
    ret

; int peachos_getkey()
peachos_getkey:
    push ebp
    mov ebp, esp
    mov eax, 2 ; Command getkey
    int 0x80
    pop ebp
    ret

; void peachos_putchar(char c)
peachos_putchar:
    push ebp
    mov ebp, esp
    mov eax, 3 ; Command putchar
    push dword [ebp+8] ; Variable "c"
    int 0x80
    add esp, 4
    pop ebp
    ret

; void* peachos_malloc(size_t size)
peachos_malloc:
    push ebp
    mov ebp, esp
    mov eax, 4 ; Command malloc (Allocates memory for the process)
    push dword[ebp+8] ; Variable "size"
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_free(void* ptr)
peachos_free:
    push ebp
    mov ebp, esp
    mov eax, 5 ; Command 5 free (Frees the allocated memory for this process)
    push dword[ebp+8] ; Variable "ptr"
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_process_load_start(const char* filename)
peachos_process_load_start:
    push ebp
    mov ebp, esp
    mov eax, 6 ; Command 6 process load start ( stars a process )
    push dword[ebp+8] ; Variable "filename"
    int 0x80
    add esp, 4
    pop ebp
    ret

; int peachos_system(struct command_argument* arguments)
peachos_system:
    push ebp
    mov ebp, esp
    mov eax, 7 ; Command 7 process_system ( runs a system command based on the arguments)
    push dword[ebp+8] ; Variable "arguments"
    int 0x80
    add esp, 4
    pop ebp
    ret


; void peachos_process_get_arguments(struct process_arguments* arguments)
peachos_process_get_arguments:
    push ebp
    mov ebp, esp
    mov eax, 8 ; Command 8 Gets the process arguments
    push dword[ebp+8] ; Variable arguments
    int 0x80
    add esp, 4
    pop ebp
    ret

; void peachos_exit()
peachos_exit:
    push ebp
    mov ebp, esp
    mov eax, 9 ; Command 9 process exit
    int 0x80
    pop ebp
    ret