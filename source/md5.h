#ifndef MD5_H
#define MD5_H

//#include <stdlib.h>

extern uint32_t h0, h1, h2, h3;

//void md5(const uint8_t *initial_msg, size_t initial_len);
void md5(const uint8_t *initial_msg, size_t initial_len);

void md5_2(const uint8_t *initial_msg, size_t initial_len, uint8_t *result);

#endif // MD5_H
