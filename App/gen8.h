#pragma once
#ifndef GEN_8_H

/// <summary>
/// ���� 8�ڸ��� �޾� long long���� ��ȯ
/// </summary>
#define GEN_8_H

/// <summary>
/// ���� 8�ڸ��� �ޱ����� ���ø�
/// </summary>
/// <typeparam name="word1"></typeparam>
/// <typeparam name="word2"></typeparam>
/// <typeparam name="word3"></typeparam>
/// <typeparam name="word4"></typeparam>
/// <typeparam name="word5"></typeparam>
/// <typeparam name="word6"></typeparam>
/// <typeparam name="word7"></typeparam>
/// <typeparam name="word8"></typeparam>
template<
	const unsigned char word1 = 0, const unsigned char word2 = 0,
	const unsigned char word3 = 0, const unsigned char word4 = 0,
	const unsigned char word5 = 0, const unsigned char word6 = 0,
	const unsigned char word7 = 0, const unsigned char word8 = 0
>
struct NumGen8 {
	/// <summary>
	/// �� ���ڸ� Integer�� ��ȯ
	/// </summary>
	static const constexpr unsigned long long val =
	                                word1 | ((unsigned short)word2 << 8) |
		     ((unsigned long)word3 << 16) | ((unsigned long)word4 << 24) |
	    ((unsigned long long)word5 << 32) | ((unsigned long long)word6 << 40) |
		((unsigned long long)word7 << 48) | ((unsigned long long)word8 << 56);
};

/// <summary>
/// �� ���ڸ� Integer�� ��ȯ (����ȭ)
/// </summary>
/// <param name="targetString"></param>
/// <param name="size">���ڿ� ���� (���ڿ� ���̴� �ݵ�� 8 �̸�)</param>
/// <returns>The merged integer</returns>
constexpr unsigned long long Gen8(const char* targetString, unsigned long size = 8) {

	union UResult {
		unsigned long long convertedNumber;
		unsigned char targetString[8];
	} uResult { 0ull };
	
	for (unsigned char i = 0; (i < 8) && (i < size && targetString[i]); i++) {
		uResult.targetString[i] = targetString[i];
	}

	return uResult.convertedNumber;
}

#endif