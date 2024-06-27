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

#include <source_location>
#include <unordered_map>
#include <exception>
#include <utility>
#include <string>

namespace jsonifier_internal {

	JSONIFIER_INLINE void prefetchInternal(const void* ptr, size_t size) {
		constexpr size_t cacheLineSize = 64;
		const char* value			   = static_cast<const char*>(ptr);
		while (size >= 64) {
			PREFETCH(value + 64);
			size -= 64;
			value += 64;
		}
	}	

	constexpr uint64_t fnv64Prime = 0x00000100000001B3;

	struct _mm128x {
		uint64_t u64[2]{};
	};

	constexpr _mm128x mm_set1_epi64(uint64_t value) {
		_mm128x result;
		result.u64[0] = value;
		result.u64[1] = value;
		return result;
	}

	constexpr uint64_t charArrayToUint64(const char* ptr, size_t index) {
		return static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 0]) | (static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 1]) << 8) |
			(static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 2]) << 16) | (static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 3]) << 24) |
			(static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 4]) << 32) | (static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 5]) << 40) |
			(static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 6]) << 48) | (static_cast<uint64_t>(ptr[index * sizeof(uint64_t) + 7]) << 56);
	}

	constexpr _mm128x mm_loadu_si128(const char* ptr) {
		_mm128x result;
		result.u64[0] = charArrayToUint64(ptr, 0);
		result.u64[1] = charArrayToUint64(ptr, 1);
		return result;
	}

	constexpr _mm128x mm_xor_si128(const _mm128x& a, const _mm128x& b) {
		_mm128x result;
		result.u64[0] = a.u64[0] ^ b.u64[0];
		result.u64[1] = a.u64[1] ^ b.u64[1];
		return result;
	}

	constexpr _mm128x mm_srli_si128(const _mm128x& a, int shift) {
		_mm128x result = {};
		if (shift >= 16) {
			result.u64[0] = 0;
			result.u64[1] = 0;
		} else if (shift >= 8) {
			int bit_shift = (shift - 8) * 8;
			result.u64[0] = a.u64[1] >> bit_shift;
			result.u64[1] = 0;
		} else {
			int bit_shift = shift * 8;
			result.u64[0] = (a.u64[0] >> bit_shift) | (a.u64[1] << (64 - bit_shift));
			result.u64[1] = a.u64[1] >> bit_shift;
		}
		return result;
	}

	constexpr void my_memcpy(char* dest, const char* src, size_t count) {
		for (size_t i = 0; i < count; ++i) {
			dest[i] = src[i];
		}
	}

	constexpr uint64_t ctHash(const char* value, uint64_t storageSize, uint64_t seed) {
		constexpr size_t blockSize = 16;

		const char* ptr		= value;
		const size_t blocks = storageSize / blockSize;
		_mm128x hash		= mm_set1_epi64(static_cast<uint64_t>(seed));
		for (size_t i = 0; i < blocks; ++i) {
			_mm128x block = mm_loadu_si128(ptr + i * blockSize);
			hash		  = mm_xor_si128(hash, block);
		}
		size_t remainder = storageSize % blockSize;
		if (remainder != 0) {
			alignas(16) char last_block[blockSize] = { 0 };
			my_memcpy(last_block, ptr + blocks * blockSize, remainder);
			_mm128x last_block_m128 = mm_loadu_si128(last_block);
			hash					= mm_xor_si128(hash, last_block_m128);
		}
		hash = mm_xor_si128(hash, mm_srli_si128(hash, 8));
		hash = mm_xor_si128(hash, mm_srli_si128(hash, 4));
		hash = mm_xor_si128(hash, mm_srli_si128(hash, 2));
		hash = mm_xor_si128(hash, mm_srli_si128(hash, 1));

		return hash.u64[0];
	}

	uint64_t rtHash(const char* value, uint64_t storageSize, uint64_t seed) {
		const size_t blocks = storageSize / 16;

		__m128i hash = _mm_set1_epi64x(seed);

		for (size_t i = 0; i < blocks; ++i) {
			__m128i block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(value + i * 16));
			hash		  = _mm_xor_si128(hash, block);
		}

		size_t remainder = storageSize % 16;
		if (remainder != 0) {
			__m128i last_block{};
			memcpy(&last_block, value + blocks * 16, remainder);
			hash = _mm_xor_si128(hash, last_block);
		}
		hash = _mm_xor_si128(hash, _mm_srli_si128(hash, 8));
		hash = _mm_xor_si128(hash, _mm_srli_si128(hash, 4));
		hash = _mm_xor_si128(hash, _mm_srli_si128(hash, 2));
		hash = _mm_xor_si128(hash, _mm_srli_si128(hash, 1));

		return hash.m128i_u64[0];
	}

	struct simd_hash {
		constexpr uint64_t operator()(string_view_ptr value, uint64_t storageSize, uint64_t seed) const {
			if (std::is_constant_evaluated()) {
				return ctHash(value, storageSize, seed);
			} else {
				return rtHash(value, storageSize, seed);
			}
		}

		constexpr uint64_t hashKeySimple(string_view_ptr value, uint64_t storageSize, uint64_t seed) const {
			uint64_t hash = seed;

			for (size_t i = 0; i < storageSize; ++i) {
				hash ^= static_cast<uint64_t>(value[i]);
				hash *= 0x100000001b3;
			}

			hash ^= (hash >> 33);
			hash *= 0xff51afd7ed558ccdULL;
			hash ^= (hash >> 33);
			hash *= 0xc4ceb9fe1a85ec53ULL;
			hash ^= (hash >> 33);
			return hash;
		}

		template<const auto keyStats> constexpr uint64_t hashKey(string_view_ptr value, uint64_t length, uint64_t seed) const {
			if (std::is_constant_evaluated()) {
				return ctHash(value, length, seed);
			} else {
				if constexpr (keyStats.minLength >= 16) {
					return rtHash(value, length, seed);
				} else {
					return hashKeySimple(value, length, seed);
				}
			}
		}
	};

}