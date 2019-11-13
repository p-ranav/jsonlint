#pragma once
// The UTF-8 encoding is a variable width encoding
// in which each Unicode code point can be encoded
// as a sequence of 1 to 4 octects. Each octect is
// composed by a heading byte and trailing bytes.
// Since the encoding is a variable width one, we
// need a way to tell where a sequence starts and
// where it ends. Thatrmation is stored in the
// head byte.
//
// Heading Byte:
//   The head byte can take one of these forms:
//   0xxxxxxx: Single byte
//   110xxxxx: Head of a sequence of two bytes
//   1110xxxx: Head of a sequence of three bytes
//   11110xxx: Head of a sequence of four bytes
//
// There's a pattern!
// The number of 1s in a heading byte tells
// us the length of the sequence
//
// Trailing Byte:
//   Trailing bytes in a multibyte UTF-8
//   sequence always have the following form:
//   10xxxxxx
//
// Encoding:
// The 0s and 1s in the heading and trailing bytes
// format are called control bits. In the UTF-8
// encoding, the concatenation of the non control
// bits is the scalar value of the encoded Unicode
// code point. Because of the structure of these
// forms, the number of required bytes to encode a
// specific Unicode code point with UTF-8 depends on
// the Unicode range where it falls (the ranges are
// given both hexadecimal and binary forms:
// [U+000, U+007F] [00000000, 01111111]: one byte
// [U+0080, U+07FF] [10000000, 111 11111111]: two bytes
// [U+0800, U+FFFF] [1000 00000000, 11111111 11111111]: three bytes
// [U+10000, U+10FFFF] [1 00000000 00000000, 10000 00000000 00000000]:
//   four bytes
static bool IsUTF8(char c) { return (((c)&0xC0) != 0x80); }

static const char TrailingBytesForUTF8[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};

// Returns the length of UTF-8 sequence
static int GetUTF8SequenceLength(char *c) {
  return TrailingBytesForUTF8[(unsigned int)(unsigned char)c[0]] + 1;
}
