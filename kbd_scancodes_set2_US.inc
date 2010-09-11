; The following special keys are processed by the Keyboard Task and handled
; as follows:
; NUMLOCK  - Lights NumLock LED and processes keys accordingly
; SHIFT    - Sets shift flag and processes keys accordingly
; CTRL	   - Sets Ctrl flag
; CAPSLOCK - Lights CapsLock LED and processes keys accordingly
; ALT	   - Sets Alt flag.
; SCRLLOCK - Lights ScrollLock LED and flag
;
; This table is used to translate all active editing keys from
; the raw value provided by the hardware.
;
;								SHIFT
;								Value
KbdTable DB 0;   		00
DB	01Bh	;  Esc	 	01
DB	031h	;  1	 	02		21h  !
DB	032h	;  2	 	03		40h  @
DB	033h	;  3	 	04		23h  #
DB	034h	;  4	 	05		24h  $
DB	035h	;  5	 	06		25h  %
DB	036h	;  6	 	07		5Eh  ^
DB	037h	;  7	 	08		26h  &
DB	038h	;  8	 	09		2Ah  *
DB	039h	;  9	 	0A		28h  (
DB	030h	;  0	 	0B		29h  )
DB	02Dh	;  -	 	0C		5Fh  _
DB	03Dh	;  =	 	0D		2Bh  +
DB	008h	;  BkSpc	0E
DB	009h	;  TAB	 	0F
DB	071h	;  q	 	10		51h
DB	077h	;  w	 	11		57h
DB	065h	;  e	 	12		45h
DB	072h	;  r	 	13		52h
DB	074h	;  t	 	14		54h
DB	079h	;  y	 	15		59h
DB	075h	;  u	 	16		55h
DB	069h	;  i	 	17		49h
DB	06Fh	;  o	 	18		4Fh
DB	070h	;  p	 	19		50h
DB	05Bh	;  [	 	1A		7Bh
DB	05Dh	;  ]	 	1B		7Dh
DB	00Dh	;  CR	 	1C
DB	0h	;  LCtrl 	1D    Special handling
DB	061h	;  a	 	1E		41h
DB	073h	;  s	 	1F		53h
DB	064h	;  d	 	20		44h
DB	066h	;  f	 	21		46h
DB	067h	;  g	 	22		47h
DB	068h	;  h	 	23		48h
DB	06Ah	;  j	 	24		4Ah
DB	06Bh	;  k	 	25		4Bh
DB	06Ch	;  l (L) 	26		4Ch
DB	03Bh	;  ;	 	27		3Ah
DB	027h	;  '    	28		22h
DB	060h	;  `	 	29		7Eh
DB	0h	;  LfShf 	2A    Special handling
DB	05Ch	;  \ 	 	2B		7Ch
DB	07Ah	;  z	 	2C		5Ah
DB	078h	;  x	 	2D		58h
DB	063h	;  c	 	2E		43h
DB	076h	;  v	 	2F		56h
DB	062h	;  b	 	30		42h
DB	06Eh	;  n	 	31		4Eh
DB	06Dh	;  m	 	32		4Dh
DB	02Ch	;  ,	 	33		3Ch
DB	02Eh	;  .	 	34		3Eh
DB	02Fh	;  /	 	35		3Fh
DB	0h	;  RtShf 	36    Special handling
DB	02Ah	;  Num * 	37		Num pad
DB	0h	;  LAlt  	38    Special handling
DB	020h	;  Space 	39
DB	0h	;  CpsLk 	3A    Special handling
DB	00Fh	;  F1	 	3B
DB	010h	;  F2	 	3C
DB	011h	;  F3	 	3D
DB	012h	;  F4	 	3E
DB	013h	;  F5	 	3F
DB	014h	;  F6	 	40
DB	015h	;  F7	 	41
DB	016h	;  F8	 	42
DB	017h	;  F9	 	43
DB	018h	;  F10	 	44
DB	0h	;  NumLk 	45   Special handling
DB	0h	;  ScrLk 	46   Special handling
DB	086h	;  Num 7 	47		37h		Num Home
DB	081h	;  Num 8 	48		38h		Num Up
DB	085h	;  Num 9 	49		39h		Num Pg Up
DB	0ADh	;  Num - 	4A				Num Pad
DB	083h	;  Num 4 	4B		34h		Num Left
DB	09Fh	;  Num 5 	4C		35h		Num (Extra code)
DB	084h	;  Num 6 	4D		36h		Num Right
DB	0ABh	;  Num + 	4E				Num Pad
DB	08Bh	;  Num 1 	4F		31h		Num End
DB	082h	;  Num 2 	50		32h		Num Down
DB	08Ch	;  Num 3 	51		33h		Num Pg Dn
DB	08Eh	;  Num 0 	52		30h		Num Insert
DB	0FFh	;  Num . 	53		2Eh		Num Del
DB	01Ch	;  Pr Scr 	54  			SYS REQUEST
DB	000h	; 	     	55
DB	000h	;	     	56
DB	019h	;  F11	 	57
DB	01Ah	;  F12	 	58
DB	000h	;	 	 	59
DB	000h	;	 		5A
DB	000h	;	 	 	5B
DB	000h	;	 	 	5C
DB	000h	;	 	 	5D
DB	000h	;	 	 	5E
DB	000h	;	 	 	5F   ;The following chars are subs from table2
DB	00Eh	;  Ins	 	60	Cursor pad
DB	00Bh	;  End	 	61	Cursor pad
DB	002h	;  Down	62	Cursor pad
DB	00Ch	;  PgDn	63	Cursor pad
DB	003h	;  Left	64	Cursor pad
DB	000h	;	     	65
DB	004h	;  Right	66	Cursor pad
DB	006h	;  Home	67	Cursor pad
DB	001h	;  Up		68	Cursor pad
DB	005h	;  PgUp	69	Cursor pad
DB	07Fh	;  Delete  6A	Cursor pad
DB	0AFh	;  /	 	6B  Num Pad
DB	08Dh	;  ENTER	6C  Num Pad
DB	0h	;			6D
DB  0h	 	;	     	6E
DB  0h		;	 		6F
DB  0h	 	;	 		70
DB  0h		;	 		71
DB  0h		;	 		72
DB  0h		;	 		73
DB  0h		;	 		74
DB  0h		;	 		75
DB  0h		;	 		76
DB  0h		;	 		77
DB  0h		;	 		78
DB  0h		;	 		79
DB  0h		;	 		7A
DB  0h		;	 		7B
DB  0h		;	 		7C
DB  0h		;	 		7D
DB  0h		;	 		7E
DB  0h		;	 		7F
;;This table does an initial character translation from the characters
;provided by the keyboard.  The Kbd translates incoming keystrokes
;from the original scan set 1 for the IBM PC.  All PCs are are set to this
;by default.  Keys on the 101 keyboard that were common to the numeric
;keypad use a two character escape sequence begining with E0 hex.
;If we see an E0 hex we scan this table and provide the translation
;to another unique character which is looked up in the primary
;table above.  This gives us unique single characters for every key!
;
nKbdTable2 EQU 10
;
;
KbdTable2 DB 052h,  060h    ;Insert
		  DB 04Fh,  061h    ;End
		  DB 050h,  062h    ;Down
		  DB 051h,  063h    ;Pg Down
		  DB 04Bh,  064h    ;Left
		  DB 04Dh,  066h    ;Rite
		  DB 047h,  067h    ;Home
		  DB 048h,  068h    ;Up
		  DB 049h,  069h    ;Pg Up
		  DB 053h,  06Ah    ;Delete
		  DB 037h,  06Bh    ;Num /
		  DB 01Ch,  06Ch    ;Num ENTER
		  DB 038h,  070h    ;Right ALT DOWN	    These are special cause we
		  DB 01Dh,  071h    ;Right Ctrl DOWN	track UP & DOWN!!!
		  DB 038h,  0F0h    ;Right ALT UP
		  DB 01Dh,  0F1h    ;Right Ctrl UP

;This table provides shift level values for codes from the primary KbdTable.
;In Shift-ON state ALL keycodes are translated through this table.
;In CAPS LOCK state codes 61h to 7Ah are translated through this table
;In NUM LOCK state only codes with High Bit set are translated
;

KbdTableS DB 0;	00
DB	38h		; 	01  Up   8  Numeric pad
DB	32h		; 	02  Dn   2  Numeric pad
DB	34h		; 	03  Left 4  Numeric pad
DB	36h		; 	04  Rite 6  Numeric pad
DB	39h		; 	05  PgUp 9  Numeric pad
DB	37h		; 	06  Home 7  Numeric pad
DB	07h		; 	07
DB	08h		; 	08
DB	09h		; 	09
DB	0Ah		; 	0A
DB	31h		; 	0B  End  1  Numeric Pad
DB	33h		; 	0C  PgDn 3  Numeric pad
DB	0Dh		; 	0D
DB	30h		; 	0E  Ins  0  Numeric pad
DB	0Fh		; 	0F
DB	10h		; 	10
DB	11h		; 	11
DB	12h		; 	12
DB	13h		; 	13
DB	14h		; 	14
DB	15h		; 	15
DB	16h		; 	16
DB	17h		; 	17
DB	18h		; 	18
DB	18h		; 	19
DB	1Ah		; 	1A
DB	1Bh		; 	1B
DB	1Ch		; 	1C
DB	1Dh		; 	1D
DB	1Eh		; 	1E
DB	35h		; 	1F	Blnk 5  Numeric pad
DB	20h		; 	20
DB	21h		; 	21
DB	22h		; 	22
DB	23h		; 	23
DB	24h		; 	24
DB	25h		; 	25
DB	26h		; 	26
DB	22h		; 	27  '  "
DB	28h		; 	28
DB	29h		; 	29
DB	2Ah		; 	2A
DB	2Bh		; 	2B
DB	3Ch		; 	2C  ,  <
DB	5Fh		; 	2D  -  _
DB	3Eh		; 	2E  .  >
DB	3Fh		; 	2F  /  ?
DB	29h		; 	30  0  )
DB	21h		; 	31  1  !
DB	40h	 	; 	32  2  @
DB	23h	 	; 	33  3  #
DB	24h		; 	34  4  $
DB	25h	 	; 	35  5  %
DB	5Eh	 	; 	36  6  ^
DB	26h	 	; 	37  7  &
DB	2Ah	 	; 	38  8  *
DB	28h	 	; 	39  9  (
DB	3Ah	 	; 	3A
DB	3Ah	 	; 	3B  ;  :
DB	3Ch	 	; 	3C
DB	2Bh	 	; 	3D  =  +
DB	3Eh	 	; 	3E
DB	3Fh	 	; 	3F
DB	40h	 	; 	40
DB	41h	 	; 	41
DB	42h	 	; 	42
DB	43h	 	; 	43
DB	44h	 	; 	44
DB	45h	 	; 	45
DB	46h	 	; 	46
DB	47h	 	; 	47
DB	48h	 	; 	48
DB	49h	 	; 	49
DB	4Ah	 	; 	4A
DB	4Bh	 	; 	4B
DB	4Ch	 	; 	4C
DB	4Dh	 	; 	4D
DB	4Eh	 	; 	4E
DB	4Fh	 	; 	4F
DB	50h	 	; 	50
DB	51h	 	; 	51
DB	52h	 	; 	52
DB	53h	 	; 	53
DB	54h	 	; 	54
DB	55h	 	; 	55
DB	56h	 	; 	56
DB	57h	 	; 	57
DB	58h	 	; 	58
DB	59h	 	; 	59
DB	5Ah	 	; 	5A
DB	7Bh	 	; 	5B  [  {
DB	7Ch	 	; 	5C  \  |
DB	7Dh	 	; 	5D  ]  }
DB	5Eh	 	; 	5E
DB	5Fh	 	; 	5F
DB	7Eh	 	; 	60  `  ~
DB	41h	 	; 	61  a  A
DB	42h	 	; 	62	b  B
DB	43h	 	; 	63	c  C
DB	44h	 	; 	64	d  D
DB	45h	 	; 	65	e  E
DB	46h	 	; 	66	f  F
DB	47h	 	; 	67	g  G
DB	48h	 	; 	68	h  H
DB	49h	 	; 	69	i  I
DB	4Ah	 	; 	6A	j  J
DB	4Bh	 	; 	6B	k  K
DB	4Ch	 	; 	6C	l  L
DB	4Dh	 	; 	6D	m  M
DB	4Eh	 	; 	6E	n  N
DB	4Fh	 	; 	6F	o  O
DB	50h	 	; 	70	p  P
DB	51h	 	; 	71	q  Q
DB	52h	 	; 	72	r  R
DB	53h	 	; 	73	s  S
DB	54h	 	; 	74	t  T
DB	55h	 	; 	75	u  U
DB	56h	 	; 	76	v  V
DB	57h	 	; 	77  w  W
DB	58h	 	; 	78  x  X
DB	59h	 	; 	79  y  Y
DB	5Ah	 	; 	7A  z  Z
DB	7Bh	 	; 	7B
DB	7Ch	 	; 	7C
DB	7Dh	 	; 	7D
DB	7Eh	 	; 	7E
DB	2Eh	 	; 	7F	Del  .  Numeric Pad