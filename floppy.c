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
	if(floppy_drives[drive].motor_on != 1)
	{
		start_floppy_motor(drive);
	};
};

void start_floppy_motor(unsigned char drive)
{
	if(floppy_drives[drive].motor_on != 1)
	{
		outportb(0x03F2, 0x0C | drive | <1 << (4 + drive)));
		sleep(500); // wait for the motor to spin up
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
