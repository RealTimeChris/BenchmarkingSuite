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
#include <bit>

namespace jsonifier_internal {

	struct fnv1a_hash {
		static constexpr uint64_t fnv64Prime{ 0x00000100000001B3ULL };
		constexpr fnv1a_hash() noexcept = default;

		constexpr uint64_t hashKeyCt(string_view_ptr value, uint64_t length, uint64_t seed) const {
			while (length >= 8) {
				seed ^= readBitsCt<uint64_t>(value);
				seed *= fnv64Prime;
				value += 8;
				length -= 8;
			}
			if (length >= 4) {
				seed ^= static_cast<uint64_t>(readBitsCt<uint32_t>(value));
				seed *= fnv64Prime;
				value += 4;
				length -= 4;
			}
			for (uint64_t x = 0; x < length; ++x) {
				seed ^= static_cast<uint64_t>(value[x]);
				seed *= fnv64Prime;
			}
			return seed;
		}

		JSONIFIER_INLINE uint64_t hashKeyRt(string_view_ptr value, uint64_t length, uint64_t seed) const {
			while (length >= 8) {
				seed ^= readBitsRt<uint64_t>(value);
				seed *= fnv64Prime;
				value += 8;
				length -= 8;
			}
			if (length >= 4) {
				seed ^= static_cast<uint64_t>(readBitsCt<uint32_t>(value));
				seed *= fnv64Prime;
				value += 4;
				length -= 4;
			}
			for (uint64_t x = 0; x < length; ++x) {
				seed ^= static_cast<uint64_t>(value[x]);
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

		template<typename value_type> JSONIFIER_INLINE value_type readBitsRt(const char* ptr) const {
			JSONIFIER_ALIGN value_type returnValue{};
			std::memcpy(&returnValue, ptr, sizeof(value_type));
			return returnValue;
		}
	};
}