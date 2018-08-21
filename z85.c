//  --------------------------------------------------------------------------
//  Reference implementation for rfc.zeromq.org/spec:32/Z85
//
//  This implementation provides a Z85 codec as an easy-to-reuse C class 
//  designed to be easy to port into other languages.

//  --------------------------------------------------------------------------
//  Copyright (c) 2010-2013 iMatix Corporation and Contributors
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
//  DEALINGS IN THE SOFTWARE.
//  --------------------------------------------------------------------------

#include "z85.h"

#include <assert.h>
#include <stddef.h>

//  Maps base 256 to base 85
static const char encoder[] = {
    "0123456789" 
    "abcdefghij" 
    "klmnopqrst" 
    "uvwxyzABCD"
    "EFGHIJKLMN" 
    "OPQRSTUVWX" 
    "YZ.-:+=^!/" 
    "*?&<>()[]{" 
    "}@%$#"
};

//  --------------------------------------------------------------------------
//  Encode a byte array as a string

char *Z85_encode(void *data, uint32_t size, char *buf, uint32_t buf_sz)
{
    uint8_t *data_b = data;
    //  Accepts only byte arrays bounded to 4 bytes
    if (size % 4)
        return NULL;
    
    uint32_t encoded_size = size * 5 / 4;
    assert(buf_sz >= encoded_size + 1);
    char *encoded = buf;
    uint32_t char_nbr = 0;
    uint32_t byte_nbr = 0;
    uint32_t value = 0;
    while (byte_nbr < size) {
        //  Accumulate value in base 256 (binary)
        value = value * 256 + data_b[byte_nbr++];
        if (byte_nbr % 4 == 0) {
            //  Output value in base 85
            uint32_t divisor = 85 * 85 * 85 * 85;
            while (divisor) {
                encoded[char_nbr++] = encoder[value / divisor % 85];
                divisor /= 85;
            }
            value = 0;
        }
    }
    assert(char_nbr == encoded_size);
    encoded[char_nbr] = '\0';
    return encoded;
}
    