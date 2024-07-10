#pragma once

#ifndef builtin_multibit_hpp
#include "multibit.h"

#ifdef __cplusplus
#include <type_traits>

/// <summary>
///	is the template version of builtin/multibit.h in order to use in C++ easily.
/// </summary>
#define builtin_multibit_hpp builtin_multibit_h

using count_t = unsigned char;

/// <summary>
/// MACROS Classified
/// </summary>
namespace builtin {
	namespace bitVector {
		/// <summary>
		/// Interface 
		/// </summary>
		/// <typeparam name="vBit_t"></typeparam>
		/// <typeparam name="is">to make it valid or not</typeparam>
		template<typename vBit_t, bool is>
		class Template;

		/// <summary>
		/// None-valid Class
		/// </summary>
		/// <typeparam name="t"></typeparam>
		template<typename t>
		class Template<t, 0> {};

		/// <summary>
		/// Valid Class, generic MACRO included.
		/// Any integer type
		/// </summary>
		/// <typeparam name="vBit_t"></typeparam>
		template<typename vBit_t>
		class Template<vBit_t, 1> : protected Template<void, 0> {
		protected:
			vBit_t vBit;
		public:

			/// <summary>
			/// 
			/// </summary>
			/// <param name="a"></param>
			/// <returns></returns>
			inline static constexpr count_t counts(const vBit_t&& a) {
				return counts(&a);
			}

			/// <summary>
			/// An empty vector
			/// </summary>
			/// <returns> [vBit_t]:
			/// A zero vector
			/// </returns>
			constexpr static vBit_t empty = bit_zero;

			/// <summary>
			/// A vector filled with 1.
			/// </summary>
			/// <param name="a">The length of the vector.</param>
			inline constexpr static vBit_t filled(const vBitLen_t a = sizeof(vBit_t)) {
				return bit_filled(a);
			}

			/// <summary>
			/// Vector maker;
			/// A vector where the only [idx]th bit is on;
			/// </summary>
			/// <param name="a">[idx]</param>
			inline constexpr static vBit_t val(const vBitIdx_t a) {
				return bit_val(a);
			}

			/// <summary>
			/// Turns on the bit selected.
			/// </summary>
			/// <param name="a"> [idx]
			/// The number of bit to turn on.
			/// </param>
			inline constexpr vBit_t on(const vBitIdx_t a) const {
				return bit_on(vBit, a);
			}

			/// <summary>
			/// Turns off the bit selected.
			/// </summary>
			/// <param name="a"> [idx]
			/// A vector where the [idx]th bit is off
			/// </param>
			inline constexpr vBit_t off(const vBitIdx_t a) const {
				return bit_off(vBit, a);
			}

			/// <summary>
			/// Changes the value of the bit selected to [boolean].
			/// </summary>
			/// <param name="a">
			/// The index of bit to turn off.
			/// </param>
			/// <param name="b">
			/// The value to set
			/// </param>
			inline constexpr vBit_t set(vBitIdx_t a, bool b) const {
				return bit_set(vBit, a, b);
			}

			/// <summary>
			/// gets the vector from [bIdx] to [eIdx];
			/// certain macro works even if [bIdx] is greater than [eIdx];
			/// The trimmed vector;
			/// </summary>
			/// <param name="a"> [bIdx]:
			/// The number of bit to turn off.
			/// </param>
			/// <param name="b"> [eIdx]:
			/// The number of bit to turn off.
			/// </param>
			inline constexpr vBit_t chop(vBitIdx_t a, vBitIdx_t b) const {
				return bit_chop(vBit, a, b);
			}

			/// <summary>
			/// Operates NOT to the value of the bit selected to wanted value;
			/// The changed vector;
			/// </summary>
			/// <typeparam name="a">[idx]:
			/// The number of bit to operate.
			/// </typeparam>
			inline constexpr vBit_t flip(vBitIdx_t a) const {
				return bit_flip(vBit, a);
			}

			/// <summary>
			/// Seek the value on the index of [idx].
			/// </summary>
			/// <param name="a">[idx]:
			/// The index of bit to see.
			/// </param>
			inline constexpr bool check(vBitIdx_t a) const {
				return bit_check(vBit, a);
			}

			/// <summary>
			/// Returns the size of the Bit Vector as bytes
			/// </summary>
			/// <param name="a"> [vBit]:
			/// Bit Vector
			/// </param>
			inline constexpr vBitLen_t size_byte() const {
				return bit_size_byte(vBit);
			}


			inline constexpr Template(vBit_t& a) : vBit(a) {}
			inline constexpr Template(vBit_t&& a) : vBit(a) {}
			inline constexpr Template() : vBit(empty) {}

			inline constexpr operator vBit_t& () { return this->vBit; }
			inline constexpr operator const vBit_t& ()	const { return this->vBit; }


			/// <summary>
			/// Count every 64 bits
			/// </summary>
			/// <param name="a"> [bIdx]:
			/// Begin Index of Vector to count
			/// </param>
			/// <param name="b"> [eIdx]:
			/// End Index of Vector to count
			/// </param>
			/// <returns>A number of 1</returns>
			inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 64) const {
				return bit_counts64(vBit, a, b);
			}
		};

		/// <summary>
		/// Filter for the invalid types;
		/// Provided [vBit_t] is not integer, class will be blank;
		/// </summary>
		/// <typeparam name="vBit_t">[vBit] An integer type, bit-vector like</typeparam>
		template<typename vBit_t>
		using f = Template<vBit_t, (std::is_unsigned<vBit_t>::value || std::is_signed<vBit_t>::value)>;

		/// <summary>
		/// Implementations
		/// </summary>
		namespace Class {
			/// <summary>
			/// [vBit8_t]: with MACROS
			/// </summary>
			class _u8c : public f<vBit8_t> {
			public:
				inline constexpr _u8c(vBit8_t& a) : f<vBit8_t>(a) {}
				inline constexpr _u8c(vBit8_t&& a) : f<vBit8_t>(a) {}


				/// <summary>
				/// Count every 8 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 8) const {
					return bit_counts8(vBit, a, b);
				}
			};

			/// <summary>
			/// [vsBit8_t]: with MACROS
			/// </summary>
			class _8c : public f<vsBit8_t> {
			public:
				inline constexpr _8c(vBit8_t& a) : f<vsBit8_t>(a) {}
				inline constexpr _8c(vBit8_t&& a) : f<vsBit8_t>(a) {}

				/// <summary>
				/// Count every 8 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 8) const {
					return bit_counts8(vBit, a, b);
				}
			};

			/// <summary>
			/// [vBit16_t]: with MACROS
			/// </summary>
			class _u16c : public f<vBit16_t> {
			public:
				inline constexpr _u16c(vBit16_t& a) : f<vBit16_t>(a) {}
				inline constexpr _u16c(vBit16_t&& a) : f<vBit16_t>(a) {}

				/// <summary>
				/// Count every 16 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 16) const {
					return bit_counts16(vBit, a, b);
				}
			};

			/// <summary>
			/// [vsBit16_t]: with MACROS
			/// </summary>
			class _16c : public f<vsBit16_t> {
			public:
				inline constexpr _16c(vBit16_t& a) : f<vsBit16_t>(a) {}
				inline constexpr _16c(vBit16_t&& a) : f<vsBit16_t>(a) {}

				/// <summary>
				/// Count every 16 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 16) const {
					return bit_counts16(vBit, a, b);
				}
			};

			/// <summary>
			/// [vBit32_t]: with MACROS
			/// </summary>
			class _u32c : public f<vBit32_t> {
			public:
				inline constexpr _u32c(vBit32_t& a) : f<vBit32_t>(a) {}
				inline constexpr _u32c(vBit32_t&& a) : f<vBit32_t>(a) {}

				/// <summary>
				/// Count every 32 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 32) const {
					return bit_counts32(vBit, a, b);
				}
			};

			/// <summary>
			/// [vsBit32_t]: with MACROS
			/// </summary>
			class _32c : public f<vsBit32_t> {
			public:
				inline constexpr _32c(vsBit32_t& a) : f<vsBit32_t>(a) {}
				inline constexpr _32c(vsBit32_t&& a) : f<vsBit32_t>(a) {}

				/// <summary>
				/// Count every 32 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 32) const {
					return bit_counts32(vBit, a, b);
				}
			};

			/// <summary>
			/// [vBit64_t]: with MACROS
			/// </summary>
			class _u64c : public f<vBit64_t> {
			public:
				inline constexpr _u64c(vBit64_t& a) : f<vBit64_t>(a) {}
				inline constexpr _u64c(vBit64_t&& a) : f<vBit64_t>(a) {}

				/// <summary>
				/// Count every 64 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 64) const {
					return bit_counts64(vBit, a, b);
				}
			};

			/// <summary>
			/// [vsBit64_t]: with MACROS
			/// </summary>
			class _64c : public f<vsBit64_t> {
			public:
				inline constexpr _64c(vsBit64_t& a) : f<vsBit64_t>(a) {}
				inline constexpr _64c(vsBit64_t&& a) : f<vsBit64_t>(a) {}

				/// <summary>
				/// Count every 64 bits
				/// </summary>
				/// <param name="a"> [bIdx]:
				/// Begin Index of Vector to count
				/// </param>
				/// <param name="b"> [eIdx]:
				/// End Index of Vector to count
				/// </param>
				/// <returns>A number of 1</returns>
				inline constexpr vBitLen_t count(vBitIdx_t a = 0, vBitIdx_t b = 64) const {
					return bit_counts64(vBit, a, b);
				}
			};
		}
	}
}
#endif // __cplusplus
#endif