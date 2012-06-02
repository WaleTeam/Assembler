.origin 0x500

pbuffer = 0x60
poffset = 0x62

main:

; INITIALIZE REDBUS
jsr      set16bit

rep     #0x30                    ; 16-bit registers
.regmode (16, 16)

lda     #0x0300
mmu     #0x01                    ; set redbus offset to 0x0300

mmu     #0x02                    ; enable redbus

; SETUP DISPLAY
sep     #0x20                    ; 8-bit a register
.regmode (8)

lda      0x01
mmu     #0x00                    ; select display

ldx     #0x0300

stz      0x00, X                 ; row to 0
stz      0x01, X                 ; cursor x to 0
stz      0x02, X                 ; cursor y to 0
stz      0x03, X                 ; cursor mode to hidden

; SET UP POINTERS
rep     #0x20                    ; 16-bit a register
.regmode(16)

lda     #logo
sta      pbuffer                ; set pbuffer

lda     #0x0310 + 0xa
sta      poffset                ; set bus offset

sep     #0x20                    ; 8-bit a register
.regmode(8)

jsr     clear_screen

print_row:
jsr     read_pbuffer

.regmode(8)
cmp     #0x00
beq      inc_row


sta     (poffset)

inc      poffset
jsr      inc_pbuffer


jmp      print_row


inc_row:
lda     #0xAB
jsr      inc_pbuffer

jsr      read_pbuffer

.regmode(8)
cmp     #0x00
beq      end                    ; finish if character is \0

jsr      set8bit
inc      0x0300                  ; increment row

jsr      set16bit
rep     #0x10                    ; 16-bit x, y registers
.regmode(0, 16)

ldy     #0x0310 + 0xa
sty      poffset                ; reset bus offset

jmp     print_row


read_pbuffer:
rep     #0x20                    ; 16-bit a register
.regmode(16)

lda     (pbuffer)

sep     #0x20                    ; 8-bit a register
.regmode(8)

rts

end:
jmp      end                    ; loop forever

inc_pbuffer:
inc      pbuffer

lda      pbuffer
bne      skip_carry
inc      pbuffer + 1
skip_carry:
rts

clear_screen:
ldx     #0x0300

lda     #0x01
sta      0x07, X                 ; blit mode to 1

lda     #0x20
sta      0x08, X                 ; blit value to ' '

stz      0x0A, X                 ; blit x to 0
stz      0x0B, X                 ; blit y to 0

lda     #0x50
sta      0x0C, X                 ; blit width to 80

lda     #0x32
sta      0x0D, X                 ; blit height to 50

clear_screen_wait:
lda      0x07, X
bne      clear_screen_wait

rts

; SUBROUTINES
set8bit:
sec                             ; set carry flag to 1
xce                             ; set emulation flag to carry flag
rts

set16bit:
clc                             ; set carry flag to 0
xce                             ; set emulation flag to carry flag
rts


; DATA
logo:
" ", 0x0
" ", 0x0
"                         ...                    ", 0x00
"                        .......                 ", 0x00
"                  .,::,,,i:,,::i:iii,.          ", 0x00
"                ,,:,,,,.::i,,,,,,...:v.         ", 0x00
"              .:.,.,.,,,,,.,,:,..,iS8BZ         ", 0x00
"              i,,,,.....,,,,,,.,C6EZ96#i        ", 0x00
"             cYv;;;;ii::,:,,,.:bb9zU2AE9        ", 0x00
"             Y;i;i;i;i;i;i;::.ZZIzIzU2E0        ", 0x00
"             C;ii;iiiiiii;i;:C8IoIoUzUbQ        ", 0x00
"             7;iiiiiii;i;i;i:z8oIoIzU2b0        ", 0x00
"             7i;iii;i;iii;ii:66IoI2U268W        ", 0x00
"             Y;i;i;i;i;i;i;i:I6II2U2UIbW        ", 0x00
"             X;;i;i;i;i;i;;i:66UIAUAIAZ0x       ", 0x00
"             7;i;i;i;i;;;i;iiA9I80W89Ib0x       ", 0x00
"             C;;i;i;i;i;;;ii:6ZE0i.Zb9Z0x,  ... ", 0x00
"             X;i;i;i;i;;;;;ii6EW.   bb8QY,;7X2C ", 0x00
"             t;;i;i;;;i;;;;;iZWn .M..W6W;i;vvU  ", 0x00
"             Xci;;;;v;;;v;vii8#,  MM bb0vi;io;  ", 0x00
"             tv;;v;;i;;v;v;;ibW   MM.2ZEv;;;Zi  ", 0x00
"             Cv;;;;;;;v;;;;iibW 70MM,1bb7iv;0i  ", 0x00
"             C;;;;;v;;;v;c;;iEB 9MMM.I6bS;i7#:  ", 0x00
"             Cv:iii;;;v;v;v;iZ# :MMM.8Z6bzUb@.  ", 0x00
"             o#WCv:i:ii;i;i;i8BC MM:;EA668Zb@.  ", 0x00
"             S.CMM@EA1CY7YtYi9b#.  ,QA9U6A6Z@   ", 0x00
"             oi  ;UW0x@@@##Z7i89b0x1tWA9IAA6ABQ ", 0x00
"             n7Y;.         :v98IbWQU6U6I6I6MY   ", 0x00
"             2cYc777v;iiivYY;Z962AI6UAI6U60xM   ", 0x00
"             o7vcvYv7Y77CYY;v9bUAU6U6U6I9#@i    ", 0x00
"             zYY;YvYcYcYY7YY;b99U6I6UAzb@0x,    ", 0x00
"             tt;vvYvYvYcYcYvv68U6U6IAIQMb.      ", 0x00
"             .01;v;vvccYvYY7;996U6IAU0xMt       ", 0x00
"              ,BESCvv;v;cc7cvCEA6IA6@#;         ", 0x00
"               iE0xBWbUtXcc;c;;IbIAEME,         ", 0x00
"                 .vo00x#WE62CCc7A00xMn.         ", 0x00
"                     ,vI00x0x0x009E##;          ", 0x00
"                         .co0Q0AY.              ", 0x00
" ", 0x00
" ", 0x00
'888       888          888           .d88888b.   .d8888b.    ', 0x00
'888   o   888          888          d88P" "Y88b d88P  Y88b   ', 0x00
'888  d8b  888          888          888     888 Y88b.        ', 0x00
'888 d888b 888  8888b.  888  .d88b.  888     888  "Y888b.     ', 0x00
'888d88888b888     "88b 888 d8P  Y8b 888     888     "Y88b.   ', 0x00
'88888P Y88888 .d888888 888 88888888 888     888       "888   ', 0x00
'8888P   Y8888 888  888 888 Y8b.     Y88b. .d88P Y88b  d88P   ', 0x00
'888P     Y888 "Y888888 888  "Y8888   "Y88888P"   "Y8888P"    ', 0x00
" "
.word , 0x00


label1: "string"
label2:

.align(nop, 128)