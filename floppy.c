/***************************************************************
  Most of the information for these functions came from
  Frank Millea's floppy.c file. Most of these functions he
  wrote, and I have just done a tad here 'n' there to make
  them more readable for me.
***************************************************************/
#include <k_defines.h>
#include <data_types.h>
#include <real_time_clock.h>
#include <floppy.h>

floppy_drive_t floppy_drives[1];

unsigned char floppy_num_command_bytes[] = {0, 0, 9, 3, 2, 9, 9, 2, 1, 9, 2, 0, 9, 6, 0, 3};
unsigned char floppy_num_status_bytes[] =  {0, 0, 7, 0, 1, 7, 7, 0, 2, 7, 7, 0, 7, 7, 0, 0};
char *floppy_errors[] =
{
	"Unit check", "Not ready", "End of cylinder", "Data error",
	"Timeout", "No data", "Write protect", "No address mark",
	"CRC error", "Wrong cylinder", "Seek error", "Bad cylinder",
	"No IRQ6", "Unknown error", "Disk changed"
};

void inti_floppy()
{
	int i;
	unsigned char c, a, b;

	// these are the internal names... they can change, since it really doesn't matter what the floppy drives are called :)
	floppy_drives[0].name = "fda";
	floppy_drives[1].name = "fdb";

	outportb(0x70, 0x10);	// select floppy drive type byte in CMOS
	c = inportb(0x71);

	if(c == 0x00)
	{
		k_printf("No floppy drive(s) detected.\n");
		return;
	};

	a = c >> 4;
	b = c & 0x0F;

	floppy_drives[0].type = a;
	floppy_drives[1].type = b;

	switch(a) {
		case 0x04:
			k_printf("Floppy drive A found(1.44 MB 3.5 Drive).\n");
			break;
		case 0x00:
			k_printf("No A floppy drive found.\n");
			break;
		case 0x05:
			k_printf("Floppy drive A found(2.88 MB 3.5 drive).\n");
			break;
		case 0x01:
			k_printf("Floppy drive A found(360 KB 5.25 Drive).\n");
			break;
		case 0x02:
			k_printf("Floppy drive A found(1.2 MB 5.25 Drive).\n");
			break;
		case 0x03:
			k_printf("Floppy drive A found(720 KB 3.5 Drive).\n");
			break;
		default:
			k_printf("Unknown A floppy drive type.\n");
			break;
	};

	switch(b) {
		case 0x04:
			k_printf("Floppy drive B found(1.44 MB 3.5 Drive).\n");
			break;
		case 0x00:
			k_printf("No B floppy drive found.\n");
			break;
		case 0x01:
			k_printf("Floppy drive B found(360 KB 5.25 Drive).\n");
			break;
		case 0x02:
			k_printf("Floppy drive B found(1.2 MB 5.25 Drive).\n");
			break;
		case 0x03:
			k_printf("Floppy drive B found(720 KB 3.5 Drive).\n");
			break;
		case 0x05:
			k_printf("Floppy drive B found(2.88 MB 3.5 drive).\n");
			break;
		default:
			k_printf("Unknown B floppy drive type.\n");
			break;
	};

	if(floppy_drives[0].type != 1 && floppy_drives[0].type != 2) // most common
	{
		floppy_drives[0].gap3 = 27;
	}
	else
	{
		floppy_drives[0].gap3 = 42;
	};
	if(floppy_drives[1].type != 1 && floppy_drives[1].type != 2) // most common
	{
		floppy_drives[1].gap3 = 27;
	}
	else
	{
		floppy_drives[1].gap3 = 42;
	};

};

unsigned char calibrate_floppy(unsigned char drive)
{
	unsigned char calibrate_command[] = {FLOPPY_CALIBRATE_DRIVE, drive};

	if(floppy_drives[drive].motor_on != 1)
	{
		start_floppy_motor(drive, 1);
	};

	k_printf("Sending calibrate command to floppy drive %d.\n", drive);
	return send_floppy_command(drive, calibrate_command, NULL);
};

void start_floppy_motor(unsigned char drive, unsigned char wait)
{
	if(floppy_drives[drive].motor_on != 1)
	{
		outportb(0x03F2, 0x0C | drive | (1 << (4 + drive)));

		if(wait == 1)
		{
			k_printf("Waiting for floppy drive %d to spin up...\n", drive);
			sleep(500); // wait for the motor to spin up
		};
		floppy_drives[drive].motor_on = 1;

		k_printf("Motor for floppy drive %d is on.\n", drive);
	};
};

void stop_floppy_motor(unsigned char drive)
{
	if(floppy_drives[drive].motor_on == 1)
	{
		outportb(0x03F2, 0x0C | drive);
		floppy_drives[drive].motor_on = 0;
	};
};

void reset_fdc()
{
	outportb(0x03F2, 0x0);
};

unsigned char is_fdc_ready()
{
	unsigned char c;

	c = inportb(0x03F4);
	c = c & 0x80;
	if(c != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	};

	return 0;
};

// used to send commands to the FDC(from Frank's floppy.c with a very few small modifications for easier reading by me)
int send_floppy_command(unsigned char drive, unsigned char *command_bytes, unsigned char *status_bytes)
{
	int i;
	unsigned long time;
	unsigned char opcode = command_bytes[0] & 0x1F;

	irq6_fired = 0;

	for(i=0; i<floppy_num_command_bytes[opcode]; i++)
	{
		while(is_fdc_ready() != 1); // wait until the FDC is ready
		k_printf("The FDC is now ready and the actual command bytes are being sent to the FDC.\n");
		outportb(FLOPPY_DATA, command_bytes[i]);
	};

	if(opcode != FLOPPY_CHECK_INT_STATUS && opcode != FLOPPY_SPECIFY)
	{
		time = amount_of_ticks + 2048;
		while(irq6_fired != 1)
		{
			if(amount_of_ticks == time)
			{
				k_printf("send_floppy_command() - IRQ6 has timed out(2 seconds).\n");
				return FLOPPY_E_NO_IRQ6;
			};
		};
	};

	for(i=0; i<floppy_num_status_bytes[opcode]; i++)
	{
		while(is_fdc_ready != 1); // wait until the FDC is ready
		status_bytes[i] = inportb(FLOPPY_DATA);
	};

	return 0;
};

// THIS FUNCTION NEEDS TO BE COMPLETED
unsigned char disk_changed()
{
	unsigned char c;

	c = inportb(0x03F7);
	c = c & 0x80;
	if(c == 0x80)
	{
		return 1;
	}
	else
	{
		return 0;
	};

	return 0;
};

// int 0x6 is used by the FDC to tell us whether or not a command was completed
void irq6(regs_t *regs)
{
	irq6_fired = 1;
};
