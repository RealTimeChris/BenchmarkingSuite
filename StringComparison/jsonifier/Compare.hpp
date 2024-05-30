/*
	MIT License

	Copyright (c) 2023 RealTimeChris

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit
	persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or
	substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/
/// https://github.com/RealTimeChris/jsonifier
/// Feb 3, 2023
#pragma once

#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/ISADetection.hpp"

namespace jsonifier_internal {

	template<auto repeat, jsonifier::concepts::uint16_type return_type> constexpr return_type repeatByte() {
		return 0x0101ull * repeat;
	}

	template<auto repeat, jsonifier::concepts::uint32_type return_type> constexpr return_type repeatByte() {
		return 0x01010101ull * repeat;
	}

	template<auto repeat, jsonifier::concepts::uint64_type return_type> constexpr return_type repeatByte() {
		return 0x0101010101010101ull * repeat;
	}

	template<auto value> JSONIFIER_INLINE void memchar(string_view_ptr& data, size_t lengthNew) {
#if JSONIFIER_CHECK_FOR_AVX(JSONIFIER_AVX512)
		{
			using simd_type						 = typename jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 2>::type::type;
			static constexpr uint64_t vectorSize = jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 2>::type::bytesProcessed;
			simd_type search_value{ simd_internal::gatherValue<simd_type>(static_cast<uint8_t>(value)) }, chunk;
			while (lengthNew >= vectorSize) {
				chunk	  = simd_internal::gatherValuesU<simd_type>(data);
				auto mask = simd_internal::opCmpEq(chunk, search_value);
				if (mask != 0) {
					data += simd_internal::tzcnt(mask);
					return;
				};
				lengthNew -= vectorSize;
				data += vectorSize;
			}
		}
#endif

#if JSONIFIER_CHECK_FOR_AVX(JSONIFIER_AVX2)
		{
			using simd_type						 = typename jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 1>::type::type;
			static constexpr uint64_t vectorSize = jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 1>::type::bytesProcessed;
			simd_type search_value{ simd_internal::gatherValue<simd_type>(static_cast<uint8_t>(value)) }, chunk;
			while (lengthNew >= vectorSize) {
				chunk	  = simd_internal::gatherValuesU<simd_type>(data);
				auto mask = simd_internal::opCmpEq(chunk, search_value);
				if (mask != 0) {
					data += simd_internal::tzcnt(mask);
					return;
				};
				lengthNew -= vectorSize;
				data += vectorSize;
			}
		}
#endif

#if JSONIFIER_CHECK_FOR_AVX(JSONIFIER_AVX) || JSONIFIER_CHECK_FOR_INSTRUCTION(JSONIFIER_NEON)
		{
			using simd_type						 = typename jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 0>::type::type;
			static constexpr uint64_t vectorSize = jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 0>::type::bytesProcessed;
			simd_type search_value{ simd_internal::gatherValue<simd_type>(static_cast<uint8_t>(value)) }, chunk;
			while (lengthNew >= vectorSize) {
				chunk	  = simd_internal::gatherValuesU<simd_type>(data);
				auto mask = simd_internal::opCmpEq(chunk, search_value);
				if (mask != 0) {
					data += simd_internal::tzcnt(mask);
					return;
				};
				lengthNew -= vectorSize;
				data += vectorSize;
			}
		}
#endif
		{
			static constexpr uint64_t mask64  = repeatByte<0b01111111, uint64_t>();
			static constexpr uint64_t value64 = repeatByte<value, uint64_t>();
			uint64_t simdValue, newValue, next;
			while (lengthNew >= 8) {
				std::memcpy(&simdValue, data, sizeof(uint64_t));

				newValue					= simdValue & mask64;
				newValue					= (newValue ^ value64) + mask64;
				newValue					= ~(newValue | simdValue);
				static constexpr auto hiBit = repeatByte<0b10000000, uint64_t>();
				next						= simd_internal::tzcnt(static_cast<uint64_t>(newValue & hiBit)) >> 3u;

				if (next != 8) {
					data += next;
					return;
				} else {
					data += 8;
					lengthNew -= 8;
				}
			}
		}
		{
			static constexpr uint32_t mask32  = repeatByte<0b01111111, uint32_t>();
			static constexpr uint32_t value32 = repeatByte<value, uint32_t>();
			uint32_t simdValue, newValue, next;
			if (lengthNew >= 4) {
				std::memcpy(&simdValue, data, sizeof(uint32_t));

				newValue					= simdValue & mask32;
				newValue					= (newValue ^ value32) + mask32;
				newValue					= ~(newValue | simdValue);
				static constexpr auto hiBit = repeatByte<0b10000000, uint32_t>();
				next						= simd_internal::tzcnt(static_cast<uint32_t>(newValue & hiBit)) >> 3u;

				if (next != 4) {
					data += next;
					return;
				} else {
					data += 4;
					lengthNew -= 4;
				}
			}
		}
		{
			static constexpr uint16_t mask16  = repeatByte<0b01111111, uint16_t>();
			static constexpr uint16_t value16 = repeatByte<value, uint16_t>();
			uint16_t simdValue, newValue, next;
			if (lengthNew >= 2) {
				std::memcpy(&simdValue, data, sizeof(uint16_t));

				newValue					= simdValue & mask16;
				newValue					= (newValue ^ value16) + mask16;
				newValue					= ~(newValue | simdValue);
				static constexpr auto hiBit = repeatByte<0b10000000, uint16_t>();
				next						= simd_internal::tzcnt(static_cast<uint16_t>(newValue & hiBit)) >> 3u;

				if (next != 2) {
					data += next;
					return;
				} else {
					data += 2;
					lengthNew -= 2;
				}
			}
		}

		if (lengthNew > 0 && *data == value) {
			return;
		}

		data = nullptr;
	}

	JSONIFIER_INLINE bool compare(string_view_ptr lhs, string_view_ptr rhs, uint64_t lengthNew) {
#if JSONIFIER_CHECK_FOR_AVX(JSONIFIER_AVX512)
		{
			using simd_type						 = typename jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 2>::type::type;
			static constexpr uint64_t vectorSize = jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 2>::type::bytesProcessed;
			simd_type value01;
			simd_type value02;
			while (lengthNew >= vectorSize) {
				value01 = simd_internal::gatherValuesU<simd_type>(lhs);
				value02 = simd_internal::gatherValuesU<simd_type>(rhs);
				if (!simd_internal::opCmpEq(value01, value02)) {
					return false;
				};
				lengthNew -= vectorSize;
				lhs += vectorSize;
				rhs += vectorSize;
			}
		}
#endif

#if JSONIFIER_CHECK_FOR_AVX(JSONIFIER_AVX2)
		{
			using simd_type						 = typename jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 1>::type::type;
			static constexpr uint64_t vectorSize = jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 1>::type::bytesProcessed;
			simd_type value01;
			simd_type value02;
			while (lengthNew >= vectorSize) {
				value01 = simd_internal::gatherValuesU<simd_type>(lhs);
				value02 = simd_internal::gatherValuesU<simd_type>(rhs);
				if (!simd_internal::opCmpEq(value01, value02)) {
					return false;
				};
				lengthNew -= vectorSize;
				lhs += vectorSize;
				rhs += vectorSize;
			}
		}
#endif

#if JSONIFIER_CHECK_FOR_AVX(JSONIFIER_AVX) || JSONIFIER_CHECK_FOR_INSTRUCTION(JSONIFIER_NEON)
		{
			using simd_type						 = typename jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 0>::type::type;
			static constexpr uint64_t vectorSize = jsonifier::concepts::get_type_at_index<simd_internal::avx_list, 0>::type::bytesProcessed;
			simd_type value01;
			simd_type value02;
			while (lengthNew >= vectorSize) {
				value01 = simd_internal::gatherValuesU<simd_type>(lhs);
				value02 = simd_internal::gatherValuesU<simd_type>(rhs);
				if (!simd_internal::opCmpEq(value01, value02)) {
					return false;
				};
				lengthNew -= vectorSize;
				lhs += vectorSize;
				rhs += vectorSize;
			}
		}
#endif

		if (lengthNew > 7) {
			static constexpr uint64_t size{ sizeof(uint64_t) };
			uint64_t v[2];
			while (lengthNew > size) {
				std::memcpy(v, lhs, size);
				std::memcpy(v + 1, rhs, size);
				if (v[0] != v[1]) {
					return false;
				}
				lengthNew -= size;
				lhs += size;
				rhs += size;
			}

			auto shift = size - lengthNew;
			lhs -= shift;
			rhs -= shift;

			std::memcpy(v, lhs, size);
			std::memcpy(v + 1, rhs, size);
			return v[0] == v[1];
		}
		{
			static constexpr uint64_t size{ sizeof(uint32_t) };
			if (lengthNew >= size) {
				uint32_t v[2];
				std::memcpy(v, lhs, size);
				std::memcpy(v + 1, rhs, size);
				if (v[0] != v[1]) {
					return false;
				}
				lengthNew -= size;
				lhs += size;
				rhs += size;
			}
		}
		{
			static constexpr uint64_t size{ sizeof(uint16_t) };
			if (lengthNew >= size) {
				uint16_t v[2];
				std::memcpy(v, lhs, size);
				std::memcpy(v + 1, rhs, size);
				if (v[0] != v[1]) {
					return false;
				}
				lengthNew -= size;
				lhs += size;
				rhs += size;
			}
		}
		if (lengthNew && *lhs != *rhs) {
			return false;
		}
		return true;
	}

	template<uint64_t Count, typename char_type> inline bool compare(const char_type* lhs, const char_type* rhs) noexcept {
		if constexpr (Count > 8) {
			auto count{ Count };
			uint64_t v[2];
			while (count > 8) {
				std::memcpy(v, lhs, 8);
				std::memcpy(v + 1, rhs, 8);
				if (v[0] != v[1]) {
					return false;
				}
				count -= 8;
				lhs += 8;
				rhs += 8;
			}

			const auto shift = 8 - count;
			lhs -= shift;
			rhs -= shift;

			std::memcpy(v, lhs, 8);
			std::memcpy(v + 1, rhs, 8);
			return v[0] == v[1];
		} else if constexpr (Count == 8) {
			uint64_t v[2];
			std::memcpy(v, lhs, Count);
			std::memcpy(v + 1, rhs, Count);
			return v[0] == v[1];
		} else if constexpr (Count > 4) {
			uint64_t v[2]{};
			std::memcpy(v, lhs, Count);
			std::memcpy(v + 1, rhs, Count);
			return v[0] == v[1];
		} else if constexpr (Count == 4) {
			uint32_t v[2];
			std::memcpy(v, lhs, Count);
			std::memcpy(v + 1, rhs, Count);
			return v[0] == v[1];
		} else if constexpr (Count == 3) {
			uint32_t v[2]{};
			std::memcpy(v, lhs, Count);
			std::memcpy(v + 1, rhs, Count);
			return v[0] == v[1];
		} else if constexpr (Count == 2) {
			uint16_t v[2];
			std::memcpy(v, lhs, Count);
			std::memcpy(v + 1, rhs, Count);
			return v[0] == v[1];
		} else if constexpr (Count == 1) {
			return *lhs == *rhs;
		} else {
			return true;
		}
	}

}// namespace jsonifier_internal
