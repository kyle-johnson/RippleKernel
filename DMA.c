#define FP_SEG(fp)        (((FARPTR) fp) >> 16)
#define FP_OFF(fp)        (((FARPTR) fp) & 0xffff)