#ifndef builtin_multibit_h
/// <summary>
/// It is a builtin multibit header;
/// Provides a pre-definition of reading a single integer as a boolean vector;
/// </summary>
#define builtin_multibit_h

#pragma region Types

/// <summary>
/// 8-bit sized boolean vector
/// </summary>
typedef unsigned char				vBit8_t;

/// <summary> [signed]:
/// signed vBit8_t
/// </summary>
typedef char						vsBit8_t;

/// <summary>
/// Most possible bit count from single vector (0 ~ 64)
/// </summary>
typedef unsigned char				vBitLen_t;

/// <summary>
/// Most possible bit count from single vector (0 ~ 64)
/// </summary>
typedef vBitLen_t					vBitIdx_t;

/// <summary>
/// 16-bit sized boolean vector
/// </summary>
typedef unsigned short				vBit16_t;

/// <summary> [signed]:
/// 16-bit sized boolean vector
/// </summary>
typedef short						vsBit16_t;

/// <summary>
/// 32-bit sized boolean vector
/// </summary>
typedef unsigned long int			vBit32_t;

/// <summary> [signed]:
/// 32-bit sized boolean vector
/// </summary>
typedef long						vsBit32_t;

/// <summary>
/// 64-bit sized boolean vector
/// </summary>
typedef unsigned long long int		vBit64_t;

/// <summary> [signed]:
/// 64-bit sized boolean vector
/// </summary>
typedef unsigned long long int		vsBit64_t;
#pragma endregion

#pragma region Fun

/// <summary>
/// An empty vector
/// </summary>
/// <returns> [vBit_t]:
/// A zero vector
/// </returns>
#define bit_zero					0

/// <summary>
/// A vector filled with 1.
/// </summary>
/// <param name="a"> [len]:
/// The length of the vector.
/// </param>
/// <return> [vBit_t]:
/// A vector length of [len] filled with 1.
/// </return>
#define bit_filled(a)				(((vBit64_t)-1) >> (64 - (a)))

/// <summary>
/// Vector maker
/// </summary>
/// <param name="a"> [idx]:
/// </param>
/// <return> [vBit_t]:
/// A vector where the only [idx]th bit is on
/// </return>
#define bit_val(a)					(1 << (a))

/// <summary>
/// Turns on the bit selected.
/// </summary>
/// <param name="a"> [vBit]:
/// A vector where the [idx]th bit is on
/// </param>
/// <param name="b"> [idx]:
/// The number of bit to turn on.
/// </param>
/// <returns>The changed vector</returns>
#define bit_on(a, b)				((a) | bit_val(b))

/// <summary>
/// Turns off the bit selected.
/// </summary>
/// <param name="a"> [vBit]:
/// A vector where the [idx]th bit is off
/// </param>
/// <param name="b"> [idx]:
/// The number of bit to turn off.
/// </param>
/// <returns>The changed vector</returns>
#define bit_off(a, b)				((a) & (~bit_val(b)))

/// <summary>
/// Changes the value of the bit selected to [boolean].
/// </summary>
/// <param name="a"> [vBit]:
/// A vector in order to change
/// </param>
/// <param name="b"> [idx]:
/// The index of bit to turn off.
/// </param>
/// <param name="c"> [boolean]:
/// The value to set
/// </param>
/// <returns>The changed vector</returns>
#define bit_set(a, b, c)			(((c) ? bit_zero : bit_val(b)) ^ (a))

/// <summary>
/// gets the vector from [bIdx] to [eIdx].
/// </summary>
/// <param name="a"> [vBit]:
/// A vector in order to trim 
/// </param>
/// <param name="b"> [bIdx]:
/// The number of bit to turn off.
/// </param>
/// <param name="c"> [eIdx]:
/// The number of bit to turn off.
/// </param>
/// <returns>The trimmed vector</returns>
#define bit_chop_r(a, b, c)			(((a) >> (b)) & ((1 << ((c) - (b))) - 1))

/// <summary>
/// gets the vector from [bIdx] to [eIdx];
/// certain macro works even if [bIdx] is greater than [eIdx];
/// </summary>
/// <param name="a"> [vBit]:
/// A vector in order to trim
/// </param>
/// <param name="b"> [bIdx]:
/// The number of bit to turn off.
/// </param>
/// <param name="c"> [eIdx]:
/// The number of bit to turn off.
/// </param>
/// <returns>The trimmed vector</returns>
#define bit_chop(a, b, c)			((b) > (c) ? bit_chop_r(a, c, b) : bit_chop_r(a, b, c))

/// <summary>
/// Operates NOT to the value of the bit selected to wanted value..
/// </summary>
/// <param name="a"> [vBit]:
/// A vector in order to change
/// </param>
/// <param name="b"> [idx]:
/// The number of bit to operate.
/// </param>
/// <returns>The changed vector</returns>
#define bit_flip(a, b)				((a) ^ bit_val(b))

/// <summary>
/// Seek the value on the index of [idx]
/// </summary>
/// <param name="a"> [vBit]:
/// A bit Vector
/// </param>
/// <param name="b"> [idx]:
/// The index of bit to see.
/// </param>
/// <param name="c"> [bool]:
/// Whether to check or ignore.
/// </param>
/// <returns>A bit</returns>
#define bit_check_r(a, b, c)		(((a) >> (b)) & (c))

/// <summary>
/// Seek the value on the index of [idx]
/// </summary>
/// <param name="a"> [vBit]:
/// A bit Vector
/// </param>
/// <param name="b"> [idx]:
/// The index of bit to see.
/// </param>
/// <returns>[boolean]: A bit</returns>
#define bit_check(a, b)				bit_check_r(a, b, 1)

/// <summary>
/// Checks if [b] is between [a] and [c], sorted
/// </summary>
#define __within_r(a, b, c)			((a) <= (b) && (b) < (c))

/// <summary>
/// Checks if [b] is between [a] and [c], sorted
/// </summary>
#define __within(a, b, c)			((a) < (c) ? __within_r(a, b, c) : __within_r(c, b, a))

/// <summary>
/// Returns the size of the Bit Vector as bytes
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
#define bit_size_byte(a)			((a) > ((vBit32_t)-1) ? 8 : (a) > ((vBit16_t)-1) ? 4 : (a) > ((vBit8_t)-1) ? 2 : 1)

/// <summary>
/// Count every 8 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <returns>A number of 1</returns>
#define bit_count8(a)				(bit_check(a, 0) + bit_check(a, 1) + bit_check(a, 2) + bit_check(a, 3) + bit_check(a, 4) + bit_check(a, 5) + bit_check(a, 6) + bit_check(a, 7))

/// <summary>
/// Count every 8 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts8_r(a, b, c)		(bit_check_r(a, 0, __within(b, 0, c)) + bit_check_r(a, 1, __within(b, 1, c)) + bit_check_r(a, 2, __within(b, 2, c)) + bit_check_r(a, 3, __within(b, 3, c)) + bit_check_r(a, 4, __within(b, 4, c)) + bit_check_r(a, 5, __within(b, 5, c)) + bit_check_r(a, 6, __within(b, 6, c)) + bit_check_r(a, 7, __within(b, 7, c)))

/// <summary>
/// Count every 8 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts8(a, b, c)		((b) > (c) ? bit_counts8_r(a, c, b) : bit_counts8_r(a, b, c))

/// <summary>
/// Count every 16 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <returns>A number of 1</returns>
#define bit_count16(a)				(bit_count8(a) + bit_count8((a) >> 8))

/// <summary>
/// Count every 16 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts16_r(a, b, c)		(((b) < 8 ? bit_counts8(a, b, (c) < 8 ? (c) : 8) : 0) + (c) < 8 ? 0 : bit_counts8((a) >> 8, 0, c - 8))

/// <summary>
/// Count every 16 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts16(a, b, c)		((b) < (c) ? bit_counts16_r(a, b, c) : bit_counts16_r(a, c, b))

/// <summary>
/// Count every 32 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <returns>A number of 1</returns>
#define bit_count32(a)				(bit_count16(a) + bit_count16((a) >> 16))

/// <summary>
/// Count every 32 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts32_r(a, b, c)		(((b) < 16 ? bit_counts16(a, b, (c) < 16 ? (c) : 16) : 0) + (c) < 16 ? 0 : bit_counts16((a) >> 16, 0, c - 16))

/// <summary>
/// Count every 32 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts32(a, b, c)		((b) < (c) ? bit_counts32_r(a, b, c) : bit_counts32_r(a, c, b))

/// <summary>
/// Count every 64 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <returns>A number of 1</returns>
#define bit_count64(a)				(bit_count32(a) + bit_count32((a) >> 32))

/// <summary>
/// Count every 64 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts64_r(a, b, c)		(((b) < 32 ? bit_counts32(a, b, (c) < 32 ? (c) : 32) : 0) + (c) < 32 ? 0 : bit_counts32((a) >> 32, 0, c - 32))

/// <summary>
/// Count every 64 bits
/// </summary>
/// <param name="a"> [vBit]:
/// Bit Vector
/// </param>
/// <param name="b"> [bIdx]:
/// Begin Index of Vector to count
/// </param>
/// <param name="c"> [eIdx]:
/// End Index of Vector to count
/// </param>
/// <returns>A number of 1</returns>
#define bit_counts64(a, b, c)		((b) < (c) ? bit_counts64_r(a, b, c) : bit_counts64_r(a, c, b))

union vBit_u {
	const vBit8_t* u8;
	const vsBit8_t* s8;

	const vBit16_t* u16;
	const vsBit16_t* s16;

	const vBit32_t* u32;
	const vsBit32_t* s32;

	const vBit64_t* u64;
	const vsBit64_t* s64;
};
#pragma endregion

#endif // !builtin_multibit_h