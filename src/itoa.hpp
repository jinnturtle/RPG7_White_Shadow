#ifndef SRC_ITOA_HPP_
#define SRC_ITOA_HPP_

/*******************************************************************************
 * Fast algorithms to convert integers to text (std::string or char*)
 ******************************************************************************/

#include <string.h>

#include <string>
#include <iostream>

std::string itostr(int val);
std::string itostr(unsigned int val);

/* Renders val into chars (does not terminate char string with NULL).
 * It is up to the caller to ensure the buffer is big enough.
 * Buf should point to the end of the buffer as it will be filled back-to-front.
 */
char* itoa(int val, char* buf);

#endif // SRC_ITOA_HPP_
