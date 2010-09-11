#ifndef __FLOPPY_HEADER
#define __FLOPPY_HEADER

unsigned char irq6_fired;

// primary addresses
#define FDC_DOR		0x3F2	// digital output register(w)
#define FDC_MSR		0x3F4	// main status register(r)
#define FDC_DRS		0x3F4	// data rate select register(w)
#define FDC_DATA 	0x3F5	// data register(r/w)
#define FDC_DIR		0x3F7	// digital input register(r)
#define FDC_CCR		0x3F7	// config. control register(w)

// floppy command opcodes
#define FLOPPY_READ_TRACK			0x02
#define FLOPPY_SPECIFY				0x03
#define FLOPPY_CHECK_DRIVE_STATUS		0x04
#define FLOPPY_WRITE_SECTOR			0x05
#define FLOPPY_READ_SECTOR			0x06
#define FLOPPY_CALIBRATE_DRIVE			0x07
#define FLOPPY_CHECK_INT_STATUS			0x08
#define FLOPPY_WRITE_DEL_SECT			0x09
#define FLOPPY_READ_SECT_ID			0x0A
#define FLOPPY_READ_DEL_SECT			0x0C
#define FLOPPY_FORMAT_TRACK			0x0D
#define FLOPPY_SEEK				0x0F

// used to select between the various data rates in the configuration control register (and the data rate select register)
#define FLOPPY_DR_500_KBPS		0x00
#define FLOPPY_DR_300_KBPS		0x01
#define FLOPPY_DR_250_KBPS		0x02
#define FLOPPY_DR_1000_KBPS		0x03

typedef struct
{
	int cylinder;
	int head;
	int sector;
} floppy_chs_t;

typedef struct
{
	u_char st0;
	u_char st1;
	u_char st2;
	u_char cylinder;
	u_char head;
	u_char sector;
	u_char sector_size;
} __attribute__((packed)) floppy_data_status_t;

typedef struct
{
	u_char *name;
	u_short cylinders;
	u_short heads;
	u_short sectors_per_track;
	u_short step_rate;
} floppy_disk_t;

typedef struct
{
	u_char *name;		// whatever we want :)
	u_char type;		// what the CMOS says it is
	floppy_disk_t *floppy_disk_p;	// points to type of floppy disk inserted
	u_char motor_on;		// motor on = 1, motor off =0
	u_char disk_inserted;	// floppy inserted =1, no floppy = 0
	u_char data_rate;		// the current data rate
	u_char GAP3;		// GAP3 length
} floppy_drive_t;

void inti_floppy();
void start_floppy_motor(unsigned char drive, unsigned char wait);
void stop_floppy_motor(unsigned char drive);
unsigned char calibrate_floppy(unsigned char drive);
int send_floppy_command(unsigned char drive, unsigned char *command_bytes, unsigned char *status_bytes);
void irq6();
void reset_fdc();
unsigned char is_fdc_ready();
unsigned char disk_changed();


#endif
