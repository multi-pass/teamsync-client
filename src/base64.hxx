// http://www.cplusplus.com/forum/beginner/51572/#msg280295
#ifndef BASE64_H
#define BASE64_H

#include <string>

std::string base64_encode(unsigned char const *bytes_to_encode,
						  size_t in_len);
std::string base64_decode(std::string const& base64_string);

#endif
