#ifndef FLOPPY_HEADER
#define FLOPPY_HEADER

unsigned char irq6_fired;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// from Frank's floppy.h
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// primary addresses
#define FLOPPY_DOR	0x3F2	// digital output register (write only)
#define FLOPPY_TDR	0x3F3	// tape drive register (read/write) - I don't use this
#define FLOPPY_MSR	0x3F4	// main status register (read only)
#define FLOPPY_DRS	0x3F4	// data rate select register (write only) - I don't use this
#define FLOPPY_DATA 0x3F5	// data register (read/write)
#define FLOPPY_DIR	0x3F7	// digital input register (read only)
#define FLOPPY_CCR	0x3F7	// config. control register (write only)

// floppy command opcodes
#define FLOPPY_READ_TRACK			0x02
#define FLOPPY_SPECIFY				0x03
#define FLOPPY_CHECK_DRIVE_STATUS	0x04
#define FLOPPY_WRITE_SECTOR			0x05
#define FLOPPY_READ_SECTOR			0x06
#define FLOPPY_CALIBRATE_DRIVE		0x07
#define FLOPPY_CHECK_INT_STATUS		0x08
#define FLOPPY_WRITE_DEL_SECT		0x09
#define FLOPPY_READ_SECT_ID			0x0A
#define FLOPPY_READ_DEL_SECT		0x0C
#define FLOPPY_FORMAT_TRACK			0x0D
#define FLOPPY_SEEK					0x0F

// used to select between the various data rates in the configuration control register (and the data rate select register)
#define FLOPPY_500_KBPS		0x00
#define FLOPPY_300_KBPS		0x01
#define FLOPPY_250_KBPS		0x02
#define FLOPPY_1000_KBPS	0x03

typedef struct
{
	int cylinder;
	int head;
	int sector;
} floppy_chs_t;

typedef struct
{
	unsigned char st0;
	unsigned char st1;
	unsigned char st2;
	unsigned char cylinder;
	unsigned char head;
	unsigned char sector;
	unsigned char sect_size;
} __attribute__((packed)) floppy_data_status_t;

typedef struct
{
	char *name;
	int cylinders;
	int heads;
	int sectors_per_track;
	int step_rate;
} floppy_disk_t;

typedef struct
{
	char *name;
	char type;	// CMOS byte...
	unsigned short did;	// I/O device handle on the system
	floppy_disk_t *floppy_disk_p;	// points to type of floppy disk inserted
	char motor_on;	// set if motor on
	char disk_inserted;	// set if the floppy disk is inserted
	char data_rate;	// the currently selected data rate (i.e. 500 KBPS)
	char gap3;	// length of GAP3
} floppy_drive_t;

// floppy drive error codes
#define FLOPPY_E_UNIT_CHECK		(-1000+0)
#define FLOPPY_E_NOT_READY		(-1000+1)
#define FLOPPY_E_END_OF_CYL		(-1000+2)
#define FLOPPY_E_DATA_ERROR		(-1000+3)
#define FLOPPY_E_TIMEOUT		(-1000+4)
#define FLOPPY_E_NO_DATA		(-1000+5)
#define FLOPPY_E_WRITE_PROTECT	(-1000+6)
#define FLOPPY_E_NO_ADDR_MARK	(-1000+7)
#define FLOPPY_E_CRC_ERROR		(-1000+8)
#define FLOPPY_E_WRONG_CYL		(-1000+9)
#define FLOPPY_E_SEEK_ERROR		(-1000+10)
#define FLOPPY_E_BAD_CYL		(-1000+11)
#define FLOPPY_E_NO_IRQ6		(-1000+12)
#define FLOPPY_E_UNKNOWN_ERROR	(-1000+13)
#define FLOPPY_E_DISK_CHANGED	(-1000+14)
#define FLOPPY_E_UNKNOWN_FORMAT	(-1000+15)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void inti_floppy();
void start_floppy_motor(unsigned char drive, unsigned char wait);
void stop_floppy_motor(unsigned char drive);
unsigned char calibrate_floppy(unsigned char drive);
int send_floppy_command(unsigned char drive, unsigned char *command_bytes, unsigned char *status_bytes);
void irq6(regs_t *regs);
void reset_fdc();
unsigned char is_fdc_ready();
unsigned char disk_changed();


#endif
