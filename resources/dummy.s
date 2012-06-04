.origin $500

pbuffer = $60
poffset = $62

main:

; INITIALIZE REDBUS
jsr      set16bit

rep     #$30                    ; 16-bit registers
.regmode (16, 16)

lda     #$0300
mmu     #$01                    ; set redbus offset to $0300

mmu     #$02                    ; enable redbus

; SETUP DISPLAY
sep     #$20                    ; 8-bit a register
.regmode (8)

lda      $01
mmu     #$00                    ; select display

ldx     #$0300

stz      $00, X                 ; row to 0
stz      $01, X                 ; cursor x to 0
stz      $02, X                 ; cursor y to 0
stz      $03, X                 ; cursor mode to hidden

; SET UP POINTERS
rep     #$20                    ; 16-bit a register
.regmode(16)

lda     #logo
sta      pbuffer                ; set pbuffer

lda     #$0310 + $a
sta      poffset                ; set bus offset

sep     #$20                    ; 8-bit a register
.regmode(8)

jsr     clear_screen

print_row:
jsr     read_pbuffer

.regmode(8)
cmp     #$00
beq      inc_row


sta     (poffset)

inc      poffset
jsr      inc_pbuffer


jmp      print_row


inc_row:
lda     #$AB
jsr      inc_pbuffer

jsr      read_pbuffer

.regmode(8)
cmp     #$00
beq      end                    ; finish if character is \0

jsr      set8bit
inc      $0300                  ; increment row

jsr      set16bit
rep     #$10                    ; 16-bit x, y registers
.regmode(0, 16)

ldy     #$0310 + $a
sty      poffset                ; reset bus offset

jmp     print_row


read_pbuffer:
rep     #$20                    ; 16-bit a register
.regmode(16)

lda     (pbuffer)

sep     #$20                    ; 8-bit a register
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
ldx     #$0300

lda     #$01
sta      $07, X                 ; blit mode to 1

lda     #$20
sta      $08, X                 ; blit value to ' '

stz      $0A, X                 ; blit x to 0
stz      $0B, X                 ; blit y to 0

lda     #$50
sta      $0C, X                 ; blit width to 80

lda     #$32
sta      $0D, X                 ; blit height to 50

clear_screen_wait:
lda      $07, X
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
" ", $0
" ", $0
"                         ...                    ", $00
"                        .......                 ", $00
"                  .,::,,,i:,,::i:iii,.          ", $00
"                ,,:,,,,.::i,,,,,,...:v.         ", $00
"              .:.,.,.,,,,,.,,:,..,iS8BZ         ", $00
"              i,,,,.....,,,,,,.,C6EZ96#i        ", $00
"             cYv;;;;ii::,:,,,.:bb9zU2AE9        ", $00
"             Y;i;i;i;i;i;i;::.ZZIzIzU2E0        ", $00
"             C;ii;iiiiiii;i;:C8IoIoUzUbQ        ", $00
"             7;iiiiiii;i;i;i:z8oIoIzU2b0        ", $00
"             7i;iii;i;iii;ii:66IoI2U268W        ", $00
"             Y;i;i;i;i;i;i;i:I6II2U2UIbW        ", $00
"             X;;i;i;i;i;i;;i:66UIAUAIAZ$       ", $00
"             7;i;i;i;i;;;i;iiA9I80W89Ib$       ", $00
"             C;;i;i;i;i;;;ii:6ZE0i.Zb9Z$,  ... ", $00
"             X;i;i;i;i;;;;;ii6EW.   bb8QY,;7X2C ", $00
"             t;;i;i;;;i;;;;;iZWn .M..W6W;i;vvU  ", $00
"             Xci;;;;v;;;v;vii8#,  MM bb0vi;io;  ", $00
"             tv;;v;;i;;v;v;;ibW   MM.2ZEv;;;Zi  ", $00
"             Cv;;;;;;;v;;;;iibW 70MM,1bb7iv;0i  ", $00
"             C;;;;;v;;;v;c;;iEB 9MMM.I6bS;i7#:  ", $00
"             Cv:iii;;;v;v;v;iZ# :MMM.8Z6bzUb@.  ", $00
"             o#WCv:i:ii;i;i;i8BC MM:;EA668Zb@.  ", $00
"             S.CMM@EA1CY7YtYi9b#.  ,QA9U6A6Z@   ", $00
"             oi  ;UW$@@@##Z7i89b$1tWA9IAA6ABQ ", $00
"             n7Y;.         :v98IbWQU6U6I6I6MY   ", $00
"             2cYc777v;iiivYY;Z962AI6UAI6U6$M   ", $00
"             o7vcvYv7Y77CYY;v9bUAU6U6U6I9#@i    ", $00
"             zYY;YvYcYcYY7YY;b99U6I6UAzb@$,    ", $00
"             tt;vvYvYvYcYcYvv68U6U6IAIQMb.      ", $00
"             .01;v;vvccYvYY7;996U6IAU$Mt       ", $00
"              ,BESCvv;v;cc7cvCEA6IA6@#;         ", $00
"               iE$BWbUtXcc;c;;IbIAEME,         ", $00
"                 .vo0$#WE62CCc7A0$Mn.         ", $00
"                     ,vI0$$$009E##;          ", $00
"                         .co0Q0AY.              ", $00
" ", $00
" ", $00
'888       888          888           .d88888b.   .d8888b.    ', $00
'888   o   888          888          d88P" "Y88b d88P  Y88b   ', $00
'888  d8b  888          888          888     888 Y88b.        ', $00
'888 d888b 888  8888b.  888  .d88b.  888     888  "Y888b.     ', $00
'888d88888b888     "88b 888 d8P  Y8b 888     888     "Y88b.   ', $00
'88888P Y88888 .d888888 888 88888888 888     888       "888   ', $00
'8888P   Y8888 888  888 888 Y8b.     Y88b. .d88P Y88b  d88P   ', $00
'888P     Y888 "Y888888 888  "Y8888   "Y88888P"   "Y8888P"    ', $00
" "

os_config:
'''
{
	"os": {
		"version": "0.1",
		"copyright": "waleos team"
	}
}
'''

.word , $00

label1: "string"
label2:

.align(nop, 128)