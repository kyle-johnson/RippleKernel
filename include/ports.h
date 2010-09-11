#ifndef __PORTS_HEADER
#define __PORTS_HEADER

inline void outportb(unsigned int port,unsigned char value);
inline void outportw(unsigned int port,unsigned int value);
inline unsigned char inportb(unsigned short port);

#endif
