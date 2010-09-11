#ifndef __MP_HEADER
#define __MP_HEADER

typedef struct	// MP floating pointer structure
{
	u_char sig[4];		// signature "_MP_"
	void *MPConfig_p;		// start address of MP configuration table, 0 if not present
	u_char length;		// size of structure in 16 byte paragraphs
	u_char specification;	// MP specification version
	u_char checksum;		// checksum
	u_char features[5];	// features
}  mp_floating_t;

u_char is_mp_present();

#endif
