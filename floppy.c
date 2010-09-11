#include <k_defines.h>
#include <floppy.h>

floppy_drive_t floppy_drives[1];

void inti_floppy()
{
	int i;
	unsigned char c, a, b;

	outportb(0x70, 0x10);	// select floppy drive type byte in CMOS
	c = inportb(0x71);

	if(c == 0x00)
	{
		k_printf("No floppy drive(s) detected.\n");
		return;
	};

	a = c >> 4;
	b = c & 0x0F;

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
};

unsigned char calibrate_floppy(unsigned char drive)
{
	unsigned char calibrate_command[] = {FLOPPY_CALIBRATE_DRIVE, drive};

	if(floppy_drives[drive].motor_on != 1)
	{
		start_floppy_motor(drive, 1);
	};

	return send_floppy_command(drive, calibrate_command, NULL);
};

void start_floppy_motor(unsigned char drive, unsigned char wait)
{
	if(floppy_drives[drive].motor_on != 1)
	{
		outportb(0x03F2, 0x0C | drive | <1 << (4 + drive)));
		if(wait == 1)
		{
			sleep(500); // wait for the motor to spin up
		};
		floppy_drives[drive].motor_on = 1;
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
	c = c & 0xC0;
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

// used to send commands to the FDC(from Frank's floppy.c with a very few small modifications for easier reading by me)
int send_floppy_command(unsigned char drive, unsigned char *command_bytes, unsigned char *status_bytes)
{
	int i;
	unsigned long time;
	unsigned char opcode = command_bytes[0] & 0x1F;

	irq6_fired = 0;

	for(i=0; i<floppy_num_command_bytes[opcode]; i++)
	{
		while(is_fdc_ready != 1); // wait until the FDC is ready
		outportb(FLOPPY_DATA, command_bytes[i]);
	};

	if(opcode != FLOPPY_CHECK_INT_STATUS && opcode != FLOPPY_SPECIFY)
	{
		time = amount_of_ticks + 2048;
		while(irq6_fired != 1)
		{
			if(amout_of_ticks == time)
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

// returns 1 if the disk has been changed since the last command
// or 0 if it hasn't
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
void int6(regs_t *regs)
{
	int6_fired = 1;
};
