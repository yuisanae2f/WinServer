#pragma once
#ifndef GEN_8_H
// Handling a string is a big deal.
// Iterating the full string is quite heavy calculation to do for CPU.
// 
// In order to prevent, these built-in MACRO convert string as integer on compile time.
// 
// So in here what does GEN_8_H do?
// is a builder for unsigned long long from 8 chars;
#define GEN_8_H

/// <summary>
/// 8-byte Integer Builder from 8-chars
/// </summary>
/// <typeparam name="a">Number A</typeparam>
/// <typeparam name="b">Number B</typeparam>
/// <typeparam name="c">Number C</typeparam>
/// <typeparam name="d">Number D</typeparam>
/// <typeparam name="e">Number E</typeparam>
/// <typeparam name="f">Number F</typeparam>
/// <typeparam name="g">Number G</typeparam>
/// <typeparam name="h">Number H</typeparam>
template<
	const unsigned char a = 0, const unsigned char b = 0,
	const unsigned char c = 0, const unsigned char d = 0,
	const unsigned char e = 0, const unsigned char f = 0,
	const unsigned char g = 0, const unsigned char h = 0
>
struct NumGen8 {
	/// <summary>
	/// Integer Merged
	/// </summary>
	static const constexpr unsigned long long val =
		a | ((unsigned short)b << 8) | ((unsigned long)c << 16) | ((unsigned long)d << 24) |
		((unsigned long long)e << 32) | ((unsigned long long)f << 40) | ((unsigned long long)g << 48) | ((unsigned long long)h << 56);
};

/// <summary>
/// 8-byte Integer Builder from 8-chars (safe-function)
/// </summary>
/// <param name="a">The string</param>
/// <param name="size">The Limit (could not be upper 8)</param>
/// <returns>The merged integer</returns>
constexpr unsigned long long Gen8(const char* a, unsigned size = 8) {
	/// lol
	union Rtn {
		unsigned long long r;
		unsigned char a[8];
	} rtn{ 0ull };
	unsigned char i = 0;

	for (; i < 8 && i < size && a[i]; i++) {
		rtn.a[i] = a[i];
	}
	return rtn.r;
}

#endif // !GEN_8_H