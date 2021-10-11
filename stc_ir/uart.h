#include "IR.h"
#ifndef __UART__
#define __UART__
void init_uart();
void send_hex(unsigned char);
void send_str(unsigned char *);
void send_code(IR_CODE);
#endif