/*0
01	MIT License

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
/// Note: Most of the code in this header was sampled from Glaze library: https://github.com/stephenberry/glaze
/// https://github.com/RealTimeChris/jsonifier
/// Feb 20, 2023
#pragma once

#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/StringView.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/StaticVector.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Reflection.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Error.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Tuple.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Base.hpp"
#include <algorithm>
#include <numeric>
#include <span>

namespace jsonifier_internal {

	template<uint64_t length> struct set_simd {
		using type =
			std::conditional_t<(length >= 64) && (BytesPerStep == 64), simd_int_512, std::conditional_t<(length >= 32) && (BytesPerStep == 32), simd_int_256, simd_int_128>>;
	};

	template<uint64_t length> struct set_integer {
		using type = std::conditional_t<(length >= 64) && (BytesPerStep == 64), uint64_t, std::conditional_t<(length >= 32) && (BytesPerStep == 32), uint32_t, uint16_t>>;
	};

	template<uint64_t length> using set_simd_t = set_simd<length>::type;

	template<uint64_t length> using set_integer_t = set_integer<length>::type;

	template<uint64_t length> constexpr uint64_t setSimdLength() {
		return (length >= 64) && (BytesPerStep == 64) ? 64 : (length >= 32) && (BytesPerStep == 32) ? 32 : 16;
	};

	template<typename key_type, typename value_type, size_t N> struct simd_map : public jsonifier_internal::fnv1a_hash {
		static constexpr uint64_t storageSize = roundUpToMultiple<setSimdLength<N>(), uint64_t>(N);
		static constexpr uint64_t bucketSize  = ((N >= 16) ? 16 : N);
		static constexpr uint64_t numGroups	  = storageSize > bucketSize ? storageSize / bucketSize : 1;
		uint64_t seed{};
		using hasher	   = jsonifier_internal::fnv1a_hash;
		using simd_type	   = set_simd_t<storageSize>;
		using integer_type = set_integer_t<storageSize>;
		JSONIFIER_ALIGN std::pair<key_type, value_type> items[storageSize]{};
		JSONIFIER_ALIGN uint8_t controlBytes[storageSize]{};
		JSONIFIER_ALIGN uint64_t hashes[storageSize]{};

		constexpr auto begin() const noexcept {
			return items;
		}

		constexpr auto end() const noexcept {
			return items + storageSize;
		}

		constexpr auto size() const noexcept {
			return N;
		}

		template<typename key_type_new> constexpr auto find(key_type_new&& key) const noexcept {
			const auto hash		   = hasher::operator()(key.data(), key.size(), seed);
			const auto hashNew	   = hash >> 7;
			const size_t groupPos  = hashNew % numGroups;
			const auto resultIndex = match(controlBytes + groupPos * bucketSize, static_cast<uint8_t>(hash));

			return (hashes[groupPos * bucketSize + resultIndex] >> 7) == hashNew ? items + groupPos * bucketSize + resultIndex : end();
		}

		constexpr simd_map(const std::array<std::pair<key_type, value_type>, N>& pairs) : items{}, hashes{} {
			if constexpr (N == 0) {
				return;
			}

			size_t bucketSizes[numGroups]{};
			seed = 1;
			bool failed{};

			do {
				std::fill_n(items, storageSize, std::pair<key_type, value_type>{});
				std::fill_n(controlBytes, storageSize, 0);
				std::fill_n(bucketSizes, numGroups, 0);
				std::fill_n(hashes, storageSize, 0);

				failed = false;
				for (size_t i = 0; i < N; ++i) {
					const auto hash			 = hasher::operator()(pairs[i].first.data(), pairs[i].first.size(), seed);
					const auto groupPos		 = (hash >> 7) % numGroups;
					const auto bucketSizeNew = bucketSizes[groupPos]++;
					const auto ctrlByte		 = static_cast<uint8_t>(hash);

					if (bucketSizeNew >= bucketSize || doesItContainIt(controlBytes + groupPos * bucketSize, ctrlByte)) {
						failed				  = true;
						bucketSizes[groupPos] = 0;
						++seed;
						break;
					}
					controlBytes[groupPos * bucketSize + bucketSizeNew] = ctrlByte;
					hashes[groupPos * bucketSize + bucketSizeNew]		= hash;
					items[groupPos * bucketSize + bucketSizeNew]		= pairs[i];
				}
			} while (failed);
		}

	  protected:
		constexpr bool doesItContainIt(const uint8_t* hashData, uint8_t byteToCheckFor) const {
			for (uint64_t x = 0; x < bucketSize; ++x) {
				if (hashData[x] == byteToCheckFor) {
					return true;
				}
			}
			return false;
		}

		constexpr uint8_t tzcnt(integer_type value) const {
			uint8_t count{};
			while ((value & 1) == 0 && value != 0) {
				value >>= 1;
				++count;
			}
			return count;
		}

		constexpr uint8_t constMatch(const uint8_t* hashData, uint8_t hash) const {
			uint32_t mask = 0;
			for (int32_t i = 0; i < bucketSize; ++i) {
				if (hashData[i] == hash) {
					mask |= (1 << i);
				}
			}
			return tzcnt(mask);
		}

		constexpr uint8_t nonConstMatch(const uint8_t* hashData, uint8_t hash) const {
			return simd_internal::tzcnt(simd_internal::opCmpEq(simd_internal::gatherValue<simd_type>(hash), simd_internal::gatherValues<simd_type>(hashData)));
		}

		constexpr uint8_t match(const uint8_t* hashData, uint8_t hash) const {
			return std::is_constant_evaluated() ? constMatch(hashData, hash) : nonConstMatch(hashData, hash);
		}
	};

	template<const jsonifier::string_view& lhs> inline constexpr bool compareSv(const jsonifier::string_view rhs) noexcept {
		constexpr auto N = lhs.size();
		return (N == rhs.size()) && compare<N>(lhs.data(), rhs.data());
	}

	template<const jsonifier::string_view& S, bool CheckSize = true> inline constexpr bool cxStringCmp(const jsonifier::string_view key) noexcept {
		if (std::is_constant_evaluated()) {
			return key == S;
		} else {
			if constexpr (CheckSize) {
				return compareSv<S>(key);
			} else {
				return compare<S.size()>(key.data(), S.data());
			}
		}
	}

	template<typename value_type, const jsonifier::string_view& S> struct micro_map1 {
		std::array<std::pair<jsonifier::string_view, value_type>, 1> items{};

		constexpr decltype(auto) begin() const noexcept {
			return items.begin();
		}
		constexpr decltype(auto) end() const noexcept {
			return items.end();
		}

		constexpr decltype(auto) find(auto&& key) const noexcept {
			if (S == key) [[likely]] {
				return items.begin();
			} else [[unlikely]] {
				return items.end();
			}
		}
	};

	template<typename value_type, const jsonifier::string_view& S0, const jsonifier::string_view& S1> struct micro_map2 {
		std::array<std::pair<jsonifier::string_view, value_type>, 2> items{};

		static constexpr bool same_size	 = S0.size() == S1.size();
		static constexpr bool check_size = !same_size;

		constexpr decltype(auto) begin() const noexcept {
			return items.begin();
		}
		constexpr decltype(auto) end() const noexcept {
			return items.end();
		}

		constexpr decltype(auto) find(auto&& key) const noexcept {
			if constexpr (same_size) {
				constexpr auto n = S0.size();
				if (key.size() != n) {
					return items.end();
				}
			}

			if (cxStringCmp<S0, check_size>(key)) {
				return items.begin();
			} else if (cxStringCmp<S1, check_size>(key)) {
				return items.begin() + 1;
			} else [[unlikely]] {
				return items.end();
			}
		}
	};

	template<typename value_type, typename... Ts> struct unique {
		using type = value_type;
	};

	template<template<typename...> class value_type, typename... Ts, typename U, typename... Us> struct unique<value_type<Ts...>, U, Us...>
		: std::conditional_t<(std::is_same_v<U, Ts> || ...), unique<value_type<Ts...>, Us...>, unique<value_type<Ts..., U>, Us...>> {};

	template<typename value_type> struct tuple_variant;

	template<typename... Ts> struct tuple_variant<std::tuple<Ts...>> : unique<std::variant<>, Ts...> {};

	template<typename value_type> struct tuple_ptr_variant;

	template<typename... Ts> struct tuple_ptr_variant<std::tuple<Ts...>> : unique<std::variant<>, std::add_pointer_t<Ts>...> {};

	template<typename... Ts> struct tuple_ptr_variant<std::pair<Ts...>> : unique<std::variant<>, std::add_pointer_t<Ts>...> {};

	template<typename Tuple, typename = std::make_index_sequence<std::tuple_size<Tuple>::value>> struct value_tuple_variant;

	template<typename Tuple, size_t I> struct member_type {
		using T0   = std::tuple_element_t<0, std::tuple_element_t<I, Tuple>>;
		using type = std::tuple_element_t<std::is_member_pointer_v<T0> ? 0 : 1, std::tuple_element_t<I, Tuple>>;
	};

	template<typename Tuple, size_t... I> struct value_tuple_variant<Tuple, std::index_sequence<I...>> {
		using type = typename tuple_variant<decltype(std::tuple_cat(std::declval<std::tuple<typename member_type<Tuple, I>::type>>()...))>::type;
	};

	template<typename Tuple> using value_tuple_variant_t = typename value_tuple_variant<Tuple>::type;

	template<typename value_type, size_t I> constexpr jsonifier::string_view getKey() noexcept {
		constexpr auto& first = get<0>(get<I>(jsonifier::concepts::core_v<value_type>));
		using T0			  = std::decay_t<decltype(first)>;
		if constexpr (std::is_member_pointer_v<T0>) {
			return getName<first>();
		} else {
			return { first };
		}
	}

	template<typename value_type, size_t I> struct core_sv {
		static constexpr jsonifier::string_view value = getKey<value_type, I>();
	};

	template<typename value_type, size_t I> constexpr auto keyValue() noexcept {
		using value_t		  = value_tuple_variant_t<jsonifier::concepts::core_t<value_type>>;
		constexpr auto& first = get<0>(get<I>(jsonifier::concepts::core_v<value_type>));
		using T0			  = std::decay_t<decltype(first)>;
		if constexpr (std::is_member_pointer_v<T0>) {
			return std::pair<jsonifier::string_view, value_t>{ getName<first>(), first };
		} else {
			return std::pair<jsonifier::string_view, value_t>{ jsonifier::string_view(first), get<1>(get<I>(jsonifier::concepts::core_v<value_type>)) };
		}
	}

	template<typename value_type, size_t... I> constexpr auto makeMapImpl(std::index_sequence<I...>) {
		using value_t	 = value_tuple_variant_t<jsonifier::concepts::core_t<value_type>>;
		constexpr auto n = std::tuple_size_v<jsonifier::concepts::core_t<value_type>>;

		if constexpr (n == 0) {
			return nullptr;
		} else if constexpr (n == 1) {
			return micro_map1<value_t, core_sv<value_type, I>::value...>{ keyValue<value_type, I>()... };
		} else if constexpr (n == 2) {
			return micro_map2<value_t, core_sv<value_type, I>::value...>{ keyValue<value_type, I>()... };
		} else {
			return simd_map<jsonifier::string_view, value_t, n>({ keyValue<value_type, I>()... });
		}
	}

	template<typename value_type> constexpr auto makeMap() {
		constexpr auto indices = std::make_index_sequence<std::tuple_size_v<jsonifier::concepts::core_t<value_type>>>{};
		return makeMapImpl<jsonifier::concepts::decay_keep_volatile_t<value_type>>(indices);
	}
}