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

		template<typename value_type> JSONIFIER_INLINE constexpr value_type readBits(const char* ptr) const {
			JSONIFIER_ALIGN value_type returnValue{};
			if (std::is_constant_evaluated()) {
				for (uint64_t x = 0; x < sizeof(value_type); ++x) {
					returnValue |= static_cast<value_type>(static_cast<unsigned char>(ptr[x])) << (x * 8);
				}
			} else {
				std::memcpy(&returnValue, ptr, sizeof(value_type));
			}
			return returnValue;
		}

		constexpr uint64_t operator()(string_view_ptr value, uint64_t storageSize, uint64_t seed) const {
			while (storageSize >= 8) {
				seed ^= readBits<uint64_t>(value);
				seed *= fnv64Prime;
				value += 8;
				storageSize -= 8;
			}
			if (storageSize >= 4) {
				seed ^= readBits<uint32_t>(value);
				seed *= fnv64Prime;
				value += 4;
				storageSize -= 4;
			}
			if (storageSize >= 2) {
				seed ^= readBits<uint16_t>(value);
				seed *= fnv64Prime;
				value += 2;
				storageSize -= 2;
			}
			if (storageSize > 0) {
				seed ^= readBits<uint8_t>(value);
				seed *= fnv64Prime;
			}
			return seed;
		}
	};
}