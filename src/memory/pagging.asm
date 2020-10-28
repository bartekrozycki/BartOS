global init_boot_pagging

extern KERNEL_BOOT_VMA
extern KERNEL_HIGH_VMA
extern _kernel_end
extern _kernel_start
extern permahalt

extern page_directory
extern page_entry_kernel
extern page_entry_low_memory
extern page_entry

init_boot_pagging:
    ;save registers that will be used 
    push eax
    push ebx
    push ecx
    push edx

    ; EAX pd EBX low_pde(set present)
    mov eax, page_directory              
    sub eax, KERNEL_HIGH_VMA    
    mov ebx, page_entry_low_memory          
    sub ebx, KERNEL_HIGH_VMA 
    or ebx, 0x1                     
    
    ;set low_pde
    mov [eax], ebx                  

    ; << page_directory
    push eax                   

    ; get PD offset for high kernel // EDX high offset
    mov eax, KERNEL_HIGH_VMA
    shr eax, 22                 ; shift right 22 bytes to get page index in pd :D
    mov ecx, 4                  
    mul ecx
    mov edx, eax
    
    ; EAX page_directory
    pop eax                     
    push eax               

    ; EAX PDE[offset to 3.5GiB] = high_pde
    add  eax, edx                
    mov ebx, page_entry_kernel
    sub ebx, KERNEL_HIGH_VMA
    or  ebx, 0x1
    mov [eax], ebx             

    ; EBX page_directory
    pop ebx

    mov ecx, 0x100000

_first_mb_loop:
    mov edx, ecx
    call _map_page ; Do the mapping
    sub ecx,  0x1000
    jnz _first_mb_loop ; not zero

    ; ECX kernel_start EAX kernel_end
    mov ecx, KERNEL_BOOT_VMA
    mov eax, _kernel_end
    sub eax, KERNEL_HIGH_VMA

_map_kernel_loop:
    mov edx, ecx
    call _map_page
    
    push ecx
    add ecx, KERNEL_HIGH_VMA
    call _map_page
    pop ecx
    
    add ecx, 0x1000
    cmp ecx, eax 

    jle _map_kernel_loop

    ;;;;; Enabling Paging :D !!!

    mov cr3, ebx ; PD
    mov eax, cr0 ; current state 
    or eax, 0x80000000 ; eax = 1<<31 ;; enable paging :O
    mov cr0, eax

    pop edx
    pop ecx
    pop ebx
    pop eax
ret

; ebx: physical addr of kernel PD
; ecx: the virtual address to map
; edx: the physical address to map to
_map_page:
    push eax
    push ebx
    push ecx
    push edx

    push edx ; PA 
    push ecx ; VA


    ; Find entry in PD 
    mov eax, ecx
    shr eax, 22 ; get first 10 bits
    mov ecx, 4  ; sizeof(int)
    mul ecx     
    add ebx, eax; PD address + offset
    mov eax, [ebx] ; eax = page table entry

    ;; check table has present flag
    mov ecx, eax 
    and ecx, 0x1
    cmp ecx, 0x0
    je permahalt ; zero ? permahalt

    and eax, 0xFFFFF000 

    pop edx ; VA
    push eax ; table entry to stack


    mov eax, edx ; VA to EAX
    shr eax, 12 ; shift right to discard non-significant bits.
    and eax, 0x03FF ; get 10 bits from the right (PT index)
    mov ecx, 4;sizeof(int)
    mul ecx
    mov ebx, eax; ebx=ecx
    
    pop eax     ; table entry to eax
    pop edx     ; PA

    add eax, ebx ; table entry address + offeset
    or edx, 0x1 ; PA - set as present
    

    mov [eax], edx

    pop edx
    pop ecx
    pop ebx
    pop eax
    ret