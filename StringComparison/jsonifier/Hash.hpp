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
#include <string>
#include <array>
#include <bit>

namespace jsonifier_internal {

	struct __m128x {
		uint64_t values[2]{};
	};

	constexpr __m128x set1Epi64(uint64_t value) {
		__m128x returnValue{};
		returnValue.values[0] = value;
		returnValue.values[1] = value;
		return returnValue;
	}

	constexpr __m128x setZero() {
		return {};
	}

	constexpr __m128x setREpi64(uint64_t value01, uint64_t value02) {
		__m128x returnValue{};
		returnValue.values[0] = value02;
		returnValue.values[1] = value01;
		return returnValue;
	}

	constexpr __m128x subEpi64(const __m128x& value01, const __m128x& value02) {
		__m128x returnValue{};
		returnValue.values[0] = value01.values[0] - value02.values[0];
		returnValue.values[1] = value01.values[1] - value02.values[1];
		return returnValue;
	}

	constexpr __m128x xorEpi64(const __m128x& value01, const __m128x& value02) {
		__m128x returnValue{};
		returnValue.values[0] = value01.values[0] ^ value02.values[0];
		returnValue.values[1] = value01.values[1] ^ value02.values[1];
		return returnValue;
	}

	constexpr __m128x addEpi64(const __m128x& value01, const __m128x& value02) {
		__m128x returnValue{};
		returnValue.values[0] = value01.values[0] + value02.values[0];
		returnValue.values[1] = value01.values[1] + value02.values[1];
		return returnValue;
	}

	constexpr __m128x srliEpi64(const __m128x& a, int imm8) {
		return __m128x{ a.values[0] >> imm8, a.values[1] >> imm8 };
	}

	constexpr uint64_t low_32_bits(uint64_t value) {
		return value & 0xFFFFFFFF;
	}

	constexpr __m128x mulEpu32(const __m128x& a, const __m128x& b) {
		return __m128x{ low_32_bits(a.values[0]) * low_32_bits(b.values[0]), low_32_bits(a.values[1]) * low_32_bits(b.values[1]) };
	}

	constexpr uint32_t extract32(uint64_t value, int index) {
		return static_cast<uint32_t>(value >> (index * 32));
	}

	constexpr __m128x shuffleEpi32(const __m128x& a, int mask) {
		uint32_t a0 = extract32(a.values[0], 0);
		uint32_t a1 = extract32(a.values[0], 1);
		uint32_t a2 = extract32(a.values[1], 0);
		uint32_t a3 = extract32(a.values[1], 1);
		uint32_t result[4]{};
		result[0] = (mask & 0x03) == 0 ? a0 : (mask & 0x03) == 1 ? a1 : (mask & 0x03) == 2 ? a2 : a3;
		result[1] = ((mask >> 2) & 0x03) == 0 ? a0 : ((mask >> 2) & 0x03) == 1 ? a1 : ((mask >> 2) & 0x03) == 2 ? a2 : a3;
		result[2] = ((mask >> 4) & 0x03) == 0 ? a0 : ((mask >> 4) & 0x03) == 1 ? a1 : ((mask >> 4) & 0x03) == 2 ? a2 : a3;
		result[3] = ((mask >> 6) & 0x03) == 0 ? a0 : ((mask >> 6) & 0x03) == 1 ? a1 : ((mask >> 6) & 0x03) == 2 ? a2 : a3;

		uint64_t shuffled_low  = static_cast<uint64_t>(result[0]) | (static_cast<uint64_t>(result[1]) << 32);
		uint64_t shuffled_high = static_cast<uint64_t>(result[2]) | (static_cast<uint64_t>(result[3]) << 32);

		return __m128x{ shuffled_low, shuffled_high };
	}

	constexpr __m128x blendvEpi8(const __m128x& a, const __m128x& b, const __m128x& mask) {
		__m128x result;
		for (int32_t i = 0; i < 2; ++i) {
			result.values[i] = 0;
			for (int32_t j = 0; j < 8; ++j) {
				uint8_t mask_byte	 = (mask.values[1 - i] >> (j * 8)) & 0xFF;
				uint8_t a_byte		 = (a.values[1 - i] >> (j * 8)) & 0xFF;
				uint8_t b_byte		 = (b.values[1 - i] >> (j * 8)) & 0xFF;
				uint8_t blended_byte = (mask_byte ? b_byte : a_byte);
				result.values[i] |= (static_cast<uint64_t>(blended_byte) << (j * 8));
			}
		}
		return result;
	}

	constexpr __m128x loaduSi128(const uint8_t* ptr) {
		uint64_t low  = 0;
		uint64_t high = 0;

		for (int32_t i = 0; i < 8; ++i) {
			low |= static_cast<uint64_t>(ptr[i]) << (i * 8);
		}

		for (int32_t i = 0; i < 8; ++i) {
			high |= static_cast<uint64_t>(ptr[i + 8]) << (i * 8);
		}

		return __m128x{ low, high };
	}

	constexpr __m128x loaduSi128(const char* ptr) {
		uint64_t low  = 0;
		uint64_t high = 0;

		for (int32_t i = 0; i < 8; ++i) {
			low |= static_cast<uint64_t>(ptr[i]) << (i * 8);
		}

		for (int32_t i = 0; i < 8; ++i) {
			high |= static_cast<uint64_t>(ptr[i + 8]) << (i * 8);
		}

		return __m128x{ low, high };
	}

	constexpr __m128x loaduSi128(uint64_t* ptr) {
		uint64_t low  = ptr[0];
		uint64_t high = ptr[1];

		return __m128x{ low, high };
	}

	constexpr void storeuSi128(uint8_t* ptr, const __m128x& data) {
		for (int32_t i = 0; i < 8; ++i) {
			ptr[i] = static_cast<uint8_t>(data.values[0] >> (i * 8));
		}

		for (int32_t i = 0; i < 8; ++i) {
			ptr[i + 8] = static_cast<uint8_t>(data.values[1] >> (i * 8));
		}
	}

	constexpr void storeuSi128(uint64_t* ptr, const __m128x& data) {
		ptr[0] = static_cast<uint64_t>(data.values[0]);
		ptr[1] = static_cast<uint64_t>(data.values[1]);
	}

	constexpr uint64_t secretDefaultSize{ 192 };

	JSONIFIER_ALIGN constexpr uint8_t xxh3KSecret[secretDefaultSize]{ 0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c, 0xde, 0xd4,
		0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f, 0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78, 0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff,
		0x72, 0x21, 0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e, 0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c, 0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb, 0x88, 0xd0,
		0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3, 0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e, 0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8, 0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c,
		0x34, 0x3f, 0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d, 0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31, 0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64, 0xea, 0xc5,
		0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3, 0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb, 0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49, 0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4,
		0x68, 0x9e, 0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc, 0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce, 0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28, 0xaf, 0xd7,
		0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e };

	constexpr uint64_t primeMx1{ 0x165667919E3779F9ULL };

	constexpr uint64_t xxhPrime321{ 0x9E3779B1U };
	constexpr uint64_t xxhPrime322{ 0x85EBCA77U };
	constexpr uint64_t xxhPrime323{ 0xC2B2AE3DU };

	constexpr uint64_t xxhPrime641{ 0x9E3779B185EBCA87ULL };
	constexpr uint64_t xxhPrime642{ 0xC2B2AE3D27D4EB4FULL };
	constexpr uint64_t xxhPrime643{ 0x165667B19E3779F9ULL };
	constexpr uint64_t xxhPrime644{ 0x85EBCA77C2B2AE63ULL };
	constexpr uint64_t xxhPrime645{ 0x27D4EB2F165667C5ULL };

	constexpr uint64_t xxhPrefetchDist{ bytesPerStep };

	constexpr uint64_t xxhSecretConsumeRate{ bytesPerStep };
	constexpr uint64_t xxhSecretDefaultSize{ 192 };
	constexpr std::array<uint64_t, 8> xxhInitAcc{ xxhPrime323, xxhPrime641, xxhPrime642, xxhPrime643, xxhPrime644, xxhPrime322, xxhPrime645, xxhPrime321 };

	template<typename value_type> JSONIFIER_INLINE uint64_t XXH_readLE64Rt(const value_type* ptr) {
		uint64_t val;
		::memcpy(&val, ptr, sizeof(val));
		return val;
	}

	template<typename value_type> constexpr uint64_t XXH_readLE64Ct(const value_type* ptr) {
		JSONIFIER_ALIGN uint64_t returnValue{};
		for (uint64_t x = 0; x < sizeof(uint64_t); ++x) {
			returnValue |= static_cast<uint64_t>(ptr[x]) << (x * 8);
		}
		return returnValue;
	}

	JSONIFIER_INLINE __m128x XXH_mult64to128Rt(uint64_t lhs, uint64_t rhs) {
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__wasm__) && defined(__SIZEOF_INT128__) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 128)
		__uint128_t const product = ( __uint128_t )lhs * ( __uint128_t )rhs;
		__m128x r128;
		r128.values[0] = ( uint64_t )(product);
		r128.values[1] = ( uint64_t )(product >> 64);
		return r128;
#elif (defined(_M_X64) || defined(_M_IA64)) && !defined(_M_ARM64EC)

	#ifndef _MSC_VER
		#pragma intrinsic(_umul128)
	#endif
		uint64_t product_high;
		uint64_t const product_low = _umul128(lhs, rhs, &product_high);
		__m128x r128;
		r128.values[0] = product_low;
		r128.values[1] = product_high;
		return r128;
#elif defined(_M_ARM64) || defined(_M_ARM64EC)

	#ifndef _MSC_VER
		#pragma intrinsic(__umulh)
	#endif
		__m128x r128;
		r128.values[0] = lhs * rhs;
		r128.values[1] = __umulh(lhs, rhs);
		return r128;

#else
		uint64_t const lo_lo = XXH_mult32to64(lhs & 0xFFFFFFFF, rhs & 0xFFFFFFFF);
		uint64_t const hi_lo = XXH_mult32to64(lhs >> 32, rhs & 0xFFFFFFFF);
		uint64_t const lo_hi = XXH_mult32to64(lhs & 0xFFFFFFFF, rhs >> 32);
		uint64_t const hi_hi = XXH_mult32to64(lhs >> 32, rhs >> 32);
		uint64_t const cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
		uint64_t const upper = (hi_lo >> 32) + (cross >> 32) + hi_hi;
		uint64_t const lower = (cross << 32) | (lo_lo & 0xFFFFFFFF);

		__m128x r128;
		r128.values[0] = lower;
		r128.values[1] = upper;
		return r128;
#endif
	}

	constexpr uint64_t XXH_mult32to64(uint64_t x, uint64_t y) {
		return (x & 0xFFFFFFFF) * (y & 0xFFFFFFFF);
	}

	constexpr __m128x XXH_mult64to128Ct(uint64_t lhs, uint64_t rhs) {
		uint64_t const lo_lo = XXH_mult32to64(lhs & 0xFFFFFFFF, rhs & 0xFFFFFFFF);
		uint64_t const hi_lo = XXH_mult32to64(lhs >> 32, rhs & 0xFFFFFFFF);
		uint64_t const lo_hi = XXH_mult32to64(lhs & 0xFFFFFFFF, rhs >> 32);
		uint64_t const hi_hi = XXH_mult32to64(lhs >> 32, rhs >> 32);
		uint64_t const cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
		uint64_t const upper = (hi_lo >> 32) + (cross >> 32) + hi_hi;
		uint64_t const lower = (cross << 32) | (lo_lo & 0xFFFFFFFF);

		__m128x r128;
		r128.values[0] = lower;
		r128.values[1] = upper;
		return r128;
	}

	JSONIFIER_INLINE void XXH3_accumulateInternalRt(uint64_t* acc, const char* input, const uint8_t* secret) {
		__m128i* xacc = ( __m128i* )acc;
		{
			const __m128i data_vec	  = _mm_loadu_si128(reinterpret_cast<const __m128i*>(input));
			const __m128i key_vec	  = _mm_loadu_si128(reinterpret_cast<const __m128i*>(secret));
			const __m128i data_key	  = _mm_xor_si128(data_vec, key_vec);
			const __m128i data_key_lo = _mm_srli_epi64(data_key, 32);
			const __m128i product	  = _mm_mul_epu32(data_key, data_key_lo);
			const __m128i data_swap	  = _mm_shuffle_epi32(data_vec, ( _MM_PERM_ENUM )_MM_SHUFFLE(1, 0, 3, 2));
			const __m128i sum		  = _mm_add_epi64(*xacc, data_swap);
			*xacc					  = _mm_add_epi64(product, sum);
		}
	}

	constexpr void XXH3_accumulateInternalCt(uint64_t* acc, const char* input, const uint8_t* secret) {
		__m128x const xacc = loaduSi128(acc);
		{
			const __m128x data_vec	  = loaduSi128(input);
			const __m128x key_vec	  = loaduSi128(secret);
			const __m128x data_key	  = xorEpi64(data_vec, key_vec);
			const __m128x data_key_lo = srliEpi64(data_key, 32);
			const __m128x product	  = mulEpu32(data_key, data_key_lo);
			const __m128x data_swap	  = shuffleEpi32(data_vec, ( _MM_PERM_ENUM )_MM_SHUFFLE(1, 0, 3, 2));
			const __m128x sum		  = addEpi64(xacc, data_swap);
			storeuSi128(acc, addEpi64(product, sum));
		}
	}

	JSONIFIER_INLINE void XXH3_accumulateRt(uint64_t* acc, const char* input, const uint8_t* secret, size_t nbStripes) {
		size_t n{};
		for (n = 0; n < nbStripes; n++) {
			const char* const in = input + n * bytesPerStep;
			prefetchInternal(in + xxhPrefetchDist);
			XXH3_accumulateInternalRt(acc, in, secret + n * xxhSecretConsumeRate);
		}
	}

	constexpr void XXH3_accumulateCt(uint64_t* acc, const char* input, const uint8_t* secret, size_t nbStripes) {
		size_t n{};
		for (n = 0; n < nbStripes; n++) {
			const char* const in = input + n * bytesPerStep;
			XXH3_accumulateInternalCt(acc, in, secret + n * xxhSecretConsumeRate);
		}
	}

	JSONIFIER_INLINE uint64_t XXH3_mul128_fold64Rt(uint64_t lhs, uint64_t rhs) {
		__m128x product = XXH_mult64to128Rt(lhs, rhs);
		return product.values[0] ^ product.values[1];
	}

	constexpr uint64_t XXH3_mul128_fold64Ct(uint64_t lhs, uint64_t rhs) {
		__m128x product = XXH_mult64to128Ct(lhs, rhs);
		return product.values[0] ^ product.values[1];
	}

	JSONIFIER_INLINE uint64_t XXH3_mix2AccsRt(uint64_t* acc, const uint8_t* secret) {
		return XXH3_mul128_fold64Rt(acc[0] ^ XXH_readLE64Rt(secret), acc[1] ^ XXH_readLE64Rt(secret + 8));
	}

	constexpr uint64_t XXH3_mix2AccsCt(uint64_t* acc, const uint8_t* secret) {
		return XXH3_mul128_fold64Ct(acc[0] ^ XXH_readLE64Ct(secret), acc[1] ^ XXH_readLE64Ct(secret + 8));
	}

	template<uint64_t secretSize> JSONIFIER_INLINE void XXH3_hashLong_internal_loopRt(uint64_t* acc, const char* input, size_t len, const uint8_t* secret) {
		static constexpr size_t nbStripesPerBlock = (secretSize - bytesPerStep) / xxhSecretConsumeRate;
		static constexpr size_t block_len		  = bytesPerStep * nbStripesPerBlock;
		len										  = len > 0 ? len : 1;
		size_t const nb_blocks					  = (len + 1) / block_len;

		{
			size_t const nbStripes = ((len - 1) - (block_len * nb_blocks)) / bytesPerStep;
			XXH3_accumulateRt(acc, input + nb_blocks * block_len, secret, nbStripes);

			{
				const char* const p = input + len - bytesPerStep;
#define XXH_SECRET_LASTACC_START 7
				XXH3_accumulateInternalRt(acc, p, secret + secretSize - bytesPerStep - XXH_SECRET_LASTACC_START);
			}
		}
	}

	template<uint64_t secretSize> constexpr void XXH3_hashLong_internal_loopCt(uint64_t* acc, const char* input, size_t len, const uint8_t* secret) {
		constexpr size_t nbStripesPerBlock = (secretSize - bytesPerStep) / xxhSecretConsumeRate;
		constexpr size_t block_len		   = bytesPerStep * nbStripesPerBlock;
		len								   = len > 0 ? len : 1;
		size_t const nb_blocks			   = (len + 1) / block_len;

		{
			size_t const nbStripes = ((len - 1) - (block_len * nb_blocks)) / bytesPerStep;
			XXH3_accumulateCt(acc, input + nb_blocks * block_len, secret, nbStripes);

			{
				const char* const p = input + len - bytesPerStep;
#define XXH_SECRET_LASTACC_START 7
				XXH3_accumulateInternalCt(acc, p, secret + secretSize - bytesPerStep - XXH_SECRET_LASTACC_START);
			}
		}
	}

	constexpr uint64_t XXH_xorshift64(uint64_t v64, int32_t shift) {
		return v64 ^ (v64 >> shift);
	}

	constexpr uint64_t XXH3_avalanche(uint64_t h64) {
		h64 = XXH_xorshift64(h64, 37);
		h64 *= primeMx1;
		h64 = XXH_xorshift64(h64, 32);
		return h64;
	}

	JSONIFIER_INLINE uint64_t XXH3_mergeAccsRt(uint64_t* acc, const uint8_t* secret, uint64_t start) {
		uint64_t result64 = start;
		size_t i		  = 0;

		for (i = 0; i < 4; i++) {
			result64 += XXH3_mix2AccsRt(acc + 2 * i, secret + 16 * i);
		}

		return XXH3_avalanche(result64);
	}

	constexpr uint64_t XXH3_mergeAccsCt(uint64_t* acc, const uint8_t* secret, uint64_t start) {
		uint64_t result64 = start;
		size_t i		  = 0;

		for (i = 0; i < 4; i++) {
			result64 += XXH3_mix2AccsCt(acc + 2 * i, secret + 16 * i);
		}

		return XXH3_avalanche(result64);
	}

	JSONIFIER_INLINE void XXH3_initCustomSecretRt(uint8_t* customSecret, uint64_t seed64) {
		{
			static constexpr auto nbRounds = xxhSecretDefaultSize / sizeof(__m128i);
			const __m128i seed_pos		   = _mm_set1_epi64x(( int64_t )seed64);

			__m128i mask = _mm_setr_epi64x(0, -1);

			__m128i zeros = _mm_setzero_si128();

			__m128i neg_seed_pos = _mm_sub_epi64(zeros, seed_pos);
			__m128i seed		 = _mm_blendv_epi8(seed_pos, neg_seed_pos, mask);

			const __m128i* const src = ( const __m128i* )xxh3KSecret;
			__m128i* const dest		 = ( __m128i* )customSecret;
			int32_t i;
			for (i = 0; i < nbRounds; ++i) {
				dest[i] = _mm_add_epi64(_mm_load_si128(src + i), seed);
			}
		}
	}

	constexpr void XXH3_initCustomSecretCt(uint8_t* customSecret, uint64_t seed64) {
		{
			constexpr auto nbRounds = xxhSecretDefaultSize / sizeof(__m128i);
			const auto seed_pos		= set1Epi64(( int64_t )seed64);

			const auto mask = setREpi64(0, -1);

			const auto zeros = setZero();

			const auto neg_seed_pos = subEpi64(zeros, seed_pos);
			const auto seed			= blendvEpi8(seed_pos, neg_seed_pos, mask);
			for (int32_t i = 0; i < nbRounds; ++i) {
				auto newSource = loaduSi128(xxh3KSecret + (sizeof(__m128x) * i));
				auto newValue  = addEpi64(newSource, seed);
				storeuSi128(customSecret + (sizeof(__m128x) * i), newValue);
			}
		}
	}

	template<uint64_t secretSize> JSONIFIER_INLINE uint64_t XXH3_hashLong_64b_internalRt(const char* input, size_t len, const uint8_t* secret) {
		std::array<uint64_t, 8> XXH3_INIT_ACCNew{ xxhInitAcc };
		XXH3_hashLong_internal_loopRt<secretSize>(XXH3_INIT_ACCNew.data(), input, len, secret);
		return XXH3_mergeAccsRt(XXH3_INIT_ACCNew.data(), secret, len);
	}

	template<uint64_t secretSize> constexpr uint64_t XXH3_hashLong_64b_internalCt(const char* input, size_t len, const uint8_t* secret) {
		std::array<uint64_t, 8> XXH3_INIT_ACCNew{ xxhInitAcc };
		XXH3_hashLong_internal_loopCt<secretSize>(XXH3_INIT_ACCNew.data(), input, len, secret);
		return XXH3_mergeAccsCt(XXH3_INIT_ACCNew.data(), secret, len);
	}

	JSONIFIER_INLINE uint64_t XXH3_hashLong_64b_withSeed_internalRt(const char* input, size_t len, uint64_t seed, const uint8_t* secret) {
		return XXH3_hashLong_64b_internalRt<xxhSecretDefaultSize>(input, len, secret);
	}

	constexpr uint64_t XXH3_hashLong_64b_withSeed_internalCt(const char* input, size_t len, uint64_t seed, const uint8_t* secret) {
		return XXH3_hashLong_64b_internalCt<xxhSecretDefaultSize>(input, len, secret);
	}

	JSONIFIER_INLINE uint64_t XXH3_hashLong_64b_withSeedRt(const char* input, size_t len, uint64_t seed, const uint8_t* secret, size_t secretLen) {
		return XXH3_hashLong_64b_withSeed_internalRt(input, len, seed, secret);
	}

	constexpr uint64_t XXH3_hashLong_64b_withSeedCt(const char* input, size_t len, uint64_t seed, const uint8_t* secret, size_t secretLen) {
		return XXH3_hashLong_64b_withSeed_internalCt(input, len, seed, secret);
	}

	JSONIFIER_INLINE uint64_t XXH3_64bits_withSeedRt(const char* input, size_t length, uint64_t seed, const uint8_t* secret) {
		return XXH3_hashLong_64b_withSeedRt(input, length, seed, secret, sizeof(secret));
	}

	constexpr uint64_t XXH3_64bits_withSeedCt(const char* input, size_t length, uint64_t seed, const uint8_t* secret) {
		return XXH3_hashLong_64b_withSeedCt(input, length, seed, secret, sizeof(secret));
	}

	constexpr uint64_t fnv64Prime{ 0x00000100000001B3ULL };

	struct key_hasher {
		constexpr key_hasher() noexcept = default;

		constexpr key_hasher& operator=(uint64_t seedNew) {
			XXH3_initCustomSecretCt(secret, seedNew);
			seed = seedNew;
			return *this;
		}

		constexpr key_hasher(uint64_t seedNew) {
			*this = seedNew;
		}

		constexpr operator uint64_t() const {
			return seed;
		}

		constexpr uint64_t fnv1aHashCt(string_view_ptr value, uint64_t length, uint64_t seed) const {
			while (length >= 8) {
				seed ^= readBitsCt<uint64_t>(value);
				seed *= fnv64Prime;
				value += 8;
				length -= 8;
			}
			if (length >= 4) {
				seed ^= readBitsCt<uint32_t>(value);
				seed *= fnv64Prime;
				value += 4;
				length -= 4;
			}
			if (length >= 2) {
				seed ^= readBitsCt<uint16_t>(value);
				seed *= fnv64Prime;
				value += 2;
				length -= 2;
			}
			if (length > 0) {
				seed ^= readBitsCt<uint8_t>(value);
				seed *= fnv64Prime;
			}
			return seed;
		}

		JSONIFIER_INLINE uint64_t fnv1aHashRt(string_view_ptr value, uint64_t length, uint64_t seed) const {
			while (length >= 8) {
				seed ^= readBitsRt<uint64_t>(value);
				seed *= fnv64Prime;
				value += 8;
				length -= 8;
			}
			if (length >= 4) {
				seed ^= readBitsRt<uint32_t>(value);
				seed *= fnv64Prime;
				value += 4;
				length -= 4;
			}
			if (length >= 2) {
				seed ^= readBitsRt<uint16_t>(value);
				seed *= fnv64Prime;
				value += 2;
				length -= 2;
			}
			if (length > 0) {
				seed ^= readBitsRt<uint8_t>(value);
				seed *= fnv64Prime;
			}
			return seed;
		}

		template<typename value_type> constexpr value_type readBitsCt(const char* ptr) const {
			JSONIFIER_ALIGN value_type returnValue{};
			for (uint64_t x = 0; x < sizeof(value_type); ++x) {
				returnValue |= static_cast<value_type>(static_cast<char>(ptr[x])) << (x * 8);
			}
			return returnValue;
		}

		template<typename value_type> constexpr value_type readBitsRt(const char* ptr) const {
			JSONIFIER_ALIGN value_type returnValue{};
			std::memcpy(&returnValue, ptr, sizeof(value_type));
			return returnValue;
		}

		constexpr uint64_t hashKeyCt(string_view_ptr value, uint64_t length, uint64_t seed) const {
			if (length >= 16) {
				return XXH3_64bits_withSeedCt(value, length, seed, secret);
			} else {
				return fnv1aHashCt(value, length, seed);
			}
		}

		constexpr uint64_t hashKeyRt(string_view_ptr value, uint64_t length, uint64_t seed) const {
			if (length >= 16) {
				return XXH3_64bits_withSeedRt(value, length, seed, secret);
			} else {
				return fnv1aHashRt(value, length, seed);
			}
		}

	  protected:
		JSONIFIER_ALIGN uint8_t secret[xxhSecretDefaultSize]{};
		uint64_t seed{};
	};
}