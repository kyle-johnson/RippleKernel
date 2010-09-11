#include <data_types.h>
#include <ports.h>
#include <dma.h>

// this needs work... it's been made for Real Mode
void LoadPageAndOffset(DMA_block *blk, char *data)
{
    unsigned int temp, segment, offset;
    unsigned long foo;

    segment = FP_SEG(data);
    offset  = FP_OFF(data);

    blk->page = (segment & 0xF000) >> 12;
    temp = (segment & 0x0FFF) << 4;
    foo = offset + temp;
    if (foo > 0xFFFF)
        blk->page++;
    blk->offset = (unsigned int)foo;
};

void select_and_enable_DMA_channel(unsigned char channel, unsigned char trans_mode, unsigned char op_mode, DMA_block *blk)
{
	if(channel >= 4)
	{
		blk->length >> 1; // for 16 bit DMA, we use words, not bytes
	};
	blk->length--; // 0=1, 1=2, etc this way

	outportb(channel < 4 ? DMA_COMMAND1 : DMA_COMMAND2, 0x14); // disable the whole DMA controller(is this neccessary?)
	outportb(channel < 4 ? DMA_MODE1 : DMA_MODE2, (op_mode << 6) | (trans_mode << 2) | (channel & 0x03)); // increment address, no autoinitialization
	outportb(ClearReg[channel], 0x00); // clear any data transfers that are currently executing
	outportb(AddrPort[channel], LOW_BYTE(blk->offset));
	outportb(AddrPort[channel], HI_BYTE(blk->offset));
	outportb(PagePort[channel], blk->page); // send the physical page that the data lies on
	outportb(ClearReg[channel], 0x00); // clear any data transfers that are currently executing again
	outportb(CountPort[channel], LOW_BYTE(blk->length));
	outportb(CountPort[channel], HI_BYTE(blk->length));
	// new we need to enable the channel by clearing the channel's mask
	outportb(MaskReg[channel], channel & 0x03);
	outportb(channel < 4 ? DMA_COMMAND1 : DMA_COMMAND2, 0x10); // enable the DMA controller
};

void disable_dma_channel(unsigned char channel)
{
	outportb(channel < 4 ? DMA_COMMAND1 : DMA_COMMAND2, 0x14); // disable DMA controller
	outportb(channel < 4 ? DMA_CHANNEL_MASK1 : DMA_CHANNEL_MASK2,0x04 | (channel & 0x03));
	outportb(channel < 4 ? DMA_COMMAND1 : DMA_COMMAND2, 0x10); // enable the DMA controller
};
