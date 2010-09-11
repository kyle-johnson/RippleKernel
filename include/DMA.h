#ifndef DMA_HEADER
#define DMA_HEADER

#define FP_SEG(fp)	(((FARPTR) fp) >> 16)
#define FP_OFF(fp)	(((FARPTR) fp) & 0xffff)

#define LOW_BYTE(x)	(x & 0xFFFF)
#define HIGH_BYTE(x)	(x >> 8)

struct
{
    char page;
    unsigned int offset;
    unsigned int length;
} DMA_block;

unsigned char PagePort[8]  = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
unsigned char AddrPort[8]  = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
unsigned char CountPort[8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };
unsigned char ClearReg[8]  = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };

// transfer modes
#define DMA_VERIFY 0x00
#define DMA_WRITE 0x01
#define DMA_READ 0x02

// this could be useful...
#define NUM_DMA_CHANNELS 8

// DMA registers. This could be done via arrays,
// but this way it is smaller
#define DMA_STATUS1 0x08
#define DMA_COMMAND1 0x08
#define DMA_REQUEST1 0x09
#define DMA_CHANNEL_MASK1 0x0A
#define DMA_MODE1 0x0B
#define DMA_INTERMED1 0x0D
#define DMA_MASK1 0x0F
#define DMA_RST_FLIPFLOP1 0x0C
#define DMA_MASTER_CLR1 0x0D
#define DMA_CLR_MASKREG1 0x0E
// now for the 16bit
#define DMA_STATUS2 0xD0
#define DMA_COMMAND2 0xD0
#define DMA_REQUEST2 0xD2
#define DMA_CHANNEL_MASK2 0xD4
#define DMA_MODE2 0xD6
#define DMA_INTERMED2 0xDA
#define DMA_MASK2 0xDE
#define DMA_RST_FLIPFLOP2 0xD8
#define DMA_MASTER_CLR2 0xDA
#define DMA_CLR_MASKREG2 0xDC
///////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadPageAndOffset(DMA_block *blk, char *data);
void select_and_enable_DMA_channel(unsigned char channel, unsigned char trans_mode, unsigned char op_mode, DMA_block *blk);
void disable_dma_channel(unsigned char channel);

#endif
