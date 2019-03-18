.global reads, lba_packet

.extern stage_entry
.extern __stage_start, __stage_sectors

#include "boot.h"

#define off_drive 88
#define off_sectors 89
#define off_heads 90
#define off_cylinders 91

.section .boot, "awx", @progbits
.code16

// FAT BPB
    jmp start
.byte 0x90              // nop
.ascii "STARRIOS"       // OEM identifier (8byte)
.word 512               // bytes per sector
.byte 1                 // sectors per cluster
.word 1                 // reserved sectors
.byte 2                 // number of FATs
.word 224               // number of root directory entries
.word 2880              // number of total sectors
.byte 0xf0              // media descriptor type (0xf0=1.44MB/2.88MB floppy)
.word 9                 // sectors per FAT
.word 18                // sectors per track
.word 2                 // number of heads
.long 0                 // number of hidden sectors
.long 0                 // number of total sectors (32bit)
// FAT12/16 Extended
.byte 0x00              // drive number (floppy: 0x00, hard: 0x80)
.byte 0                 // reserved
.byte 0x29              // signature
.long 0xffffffff        // VolumeID Serial Number
.ascii "STARRIOS-FD"    // volume label string (11byte)
.ascii "FAT12   "       // file system type (8byte)
.fill 28, 1, 0          // FAT32 takes 28 bytes more

start:
    ljmp $0, $1f
1:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov $0x7000, %sp
    mov $lba_packet, %bp
    mov %dl, off_drive(%bp)
    sti

    // check LBA extension
    mov $0x41, %ah
    mov $0x55aa, %bx
    int $0x13
    jc nolba
    cmp $0xaa55, %bx
    jne nolba
    test $1, %cl
    jz nolba

    movw $0x00eb, reads
    jmp done

nolba:
    // get drive parameters
    // cl[5:0] -> max sector index
    // cl[6:7], ch -> max cylinder index
    // dh -> max head index
    mov $0x08, %ah
    mov off_drive(%bp), %dl
    xor %di, %di
    mov %di, %es
    int $0x13
    jc floppy

    inc %ch
    inc %dh
    xchg %ch, %dh
    mov %cx, off_sectors(%bp)
    mov %dh, off_cylinders(%bp)
    jmp done

floppy:
    // try to failback to floppy
    mov off_drive(%bp), %al
    test $0xfe, %al
    jnz panic
    movw $0x0212, off_sectors(%bp)
    movb $80, off_cylinders(%bp)

done:
    call reads
    jc panic
    mov %sp, %bp
    jmp stage_entry

panic:
    mov $2f, %si
    cld
0:
    lodsb
    test %al, %al
    jz 1f
    mov $0x07, %bx
    mov $0x0e, %ah
    int $0x10
    jmp 0b
1:
    hlt
    jmp 1b
2: .asciz "boot panic"

reads:
    jmp reads_chs

reads_lba:
    push %si
    mov $0x42, %ah
    mov $lba_packet, %si
    mov off_drive(%si), %dl
    mov $lba_packet, %si
    int $0x13
    pop %si
    ret

reads_chs:
    push %es
    pusha
    mov $lba_packet, %bp

    // int 0x13, ah=0x02
    // ax=count, ch=cyl[0:7], cl=cyl[8:9]+sec, dh=head, dl=drive, es:bx=address
    // convert lba_packet to chs
    mov lbapck_lba2(%bp), %eax
    test %eax, %eax
    jnz err

    mov lbapck_lba(%bp), %eax
    xor %edx, %edx
    xor %ebx, %ebx
    mov off_sectors(%bp), %bl
    mov %bx, %si
    div %ebx

    mov %dl, %cl
    inc %cl
    xor %dx, %dx
    mov off_heads(%bp), %bl
    div %ebx

    mov %dl, %dh
    mov %si, %bx
    shl $2, %bx
    mov off_cylinders(%bp), %bl
    cmp %ebx, %eax
    jae err
    mov %al, %ch
    shl $6, %ah
    or %ah, %cl

    mov lbapck_addr(%bp), %bx
    mov lbapck_add1(%bp), %ax
    test $0xfff0, %ax
    jnz err
    shl $12, %ax
    mov %ax, %es

    mov off_drive(%bp), %dl
    mov lbapck_count(%bp), %di
repeat:
    test %di, %di
    jz success

    mov $5, %si
retry:
    mov $0x0201, %ax
    int $0x13
    jc onefail

    mov %es, %ax
    add $0x20, %ax
    mov %ax, %es

    inc %cl
    movzx off_sectors(%bp), %ax
    mov %ax, %si
    mov %cl, %ah
    and $0x3f3f, %ax
    cmp %al, %ah
    jbe 1f

    and $0xc0, %ch
    mov $1, %cl
    inc %dh
    cmp off_heads(%bp), %dh
    jb 1f

    xor %dh, %dh
    add $1, %ch
    jnc 0f
    add $0x40, %cl
0:
    mov %si, %ax
    shl $2, %ax
    mov %al, off_cylinders(%bp)
    mov %ax, %si
    mov %ch, %al
    mov %cl, %ah
    shr $6, %ah
    cmp %si, %ax
    jae err
1:
    dec %di
    jmp repeat

onefail:
    dec %si
    jnz retry

err:
    stc
success:
    // Thanks to test operation before 'jz success', we don't need clear CF.
    pop %es
    popa
    ret

.org 424

lba_packet:
.word 16
.word __stage_sectors
.word __stage_start
.word 0x0000
.word 33
.word 0
.word 0
.word 0

.long 0    // disk signature
.word 0

.quad 0, 0  // partition #1
.quad 0, 0  // partition #2
.quad 0, 0  // partition #3
.quad 0, 0  // partition #4

.byte 0x55, 0xaa

.section .bootbss, "aw", @nobits

drive: .byte 0
chs_sectors: .byte 0
chs_heads: .byte 0
chs_cylinders: .byte 0
