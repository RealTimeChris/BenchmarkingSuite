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
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Derailleur.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Error.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Tuple.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Base.hpp"
#include <algorithm>
#include <numeric>
#include <array>
#include <span>

namespace jsonifier_internal {

	struct naive_prng final {
		constexpr naive_prng() noexcept = default;

		uint64_t x = 7185499250578500046;
		constexpr uint64_t operator()() noexcept {
			x ^= x >> 12;
			x ^= x << 25;
			x ^= x >> 27;
			return x * 0x2545F4914F6CDD1DULL;
		}
	};

	template<uint64_t n> constexpr uint64_t nextPowerOf2() {
		if constexpr (n == 0) {
			return 1;
		}

		if constexpr ((n & (n - 1)) == 0) {
			return n;
		}

		uint64_t p = 1;
		while (p < n) {
			p <<= 1;
		}
		return p;
	}

	template<uint64_t length> struct set_simd {
		using type = std::conditional_t<bytesPerStep >= 64 && length >= 64, simd_int_512, std::conditional_t<bytesPerStep >= 32 && length >= 32, simd_int_256, simd_int_128>>;
	};

	template<uint64_t length> struct set_integer {
		using type = std::conditional_t<bytesPerStep >= 64 && length >= 64, uint64_t, std::conditional_t<bytesPerStep >= 32 && length >= 32, uint32_t, uint16_t>>;
	};

	template<uint64_t length> constexpr uint64_t setSimdWidth() {
		return bytesPerStep >= 64 && length >= 64 ? 64 : bytesPerStep >= 32 && length >= 32 ? 32 : 16;
	};

	template<uint64_t length> using set_simd_t = set_simd<length>::type;

	template<uint64_t length> using set_integer_t = set_integer<length>::type;

	template<typename value_type> constexpr value_type abs(value_type value) {
		return value < 0 ? -value : value;
	}

	constexpr std::array<double, 10> scalingFactorTable{ 0.10f, 0.20f, 0.30f, 0.40f, 0.50f, 0.60f, 0.70f, 0.80f, 0.90f, 1.0f };

	template<typename key_type, typename value_type, size_t bucketSizeNew, size_t N> struct simd_set : public fnv1a_hash {
		static constexpr uint64_t bucketSize  = bucketSizeNew;
		static constexpr uint64_t storageSize = N >= bucketSize ? N : bucketSize;
		static constexpr uint64_t numGroups	  = storageSize > bucketSize ? storageSize / bucketSize : 1;
		uint64_t stringScalingFactorIndex{};
		uint64_t seed{};
		using hasher	   = fnv1a_hash;
		using simd_type	   = set_simd_t<bucketSize>;
		using integer_type = set_integer_t<bucketSize>;
		JSONIFIER_ALIGN std::array<uint8_t, storageSize> controlBytes{};
		JSONIFIER_ALIGN std::array<value_type, storageSize> items{};
		JSONIFIER_ALIGN std::array<uint64_t, storageSize> hashes{};

		constexpr simd_set() noexcept = default;

		constexpr auto begin() const noexcept {
			return items.data();
		}

		constexpr auto end() const noexcept {
			return items.data() + storageSize;
		}

		constexpr auto size() const noexcept {
			return N;
		}

		template<typename key_type_new> constexpr auto find(key_type_new&& key) const noexcept {
			const auto hash =
				hasher::operator()(key.data(), static_cast<uint64_t>(static_cast<float>(key.size()) * static_cast<float>(scalingFactorTable[stringScalingFactorIndex])), seed);
			const auto resultIndex = ((hash >> 7) % numGroups) * bucketSize;
			if (std::is_constant_evaluated()) {
				auto adjustedIndex = constMatch(controlBytes.data() + resultIndex, static_cast<uint8_t>(hash)) % bucketSize + resultIndex;
				return (hashes[adjustedIndex] == hash) ? items.data() + adjustedIndex : end();
			} else {
				prefetchInternal(controlBytes.data() + resultIndex);
				prefetchInternal(hashes.data() + resultIndex);
				prefetchInternal(controlBytes.data() + resultIndex + bucketSize);
				prefetchInternal(hashes.data() + resultIndex + bucketSize);
				auto adjustedIndex = nonConstMatch(controlBytes.data() + resultIndex, static_cast<uint8_t>(hash)) % bucketSize + resultIndex;
				return (hashes[adjustedIndex] == hash) ? items.data() + adjustedIndex : end();
			}
		}

	  protected:
		constexpr integer_type tzcnt(integer_type value) const {
			integer_type count{};
			while ((value & 1) == 0 && value != 0) {
				value >>= 1;
				++count;
			}
			return count;
		}

		constexpr integer_type constMatch(const uint8_t* hashData, uint8_t hash) const {
			integer_type mask = 0;
			for (uint64_t i = 0; i < bucketSize; ++i) {
				if (hashData[i] == hash) {
					mask |= (1 << i);
				}
			}
			return tzcnt(mask);
		}

		constexpr integer_type nonConstMatch(const uint8_t* hashData, uint8_t hash) const {
			return simd_internal::tzcnt(simd_internal::opCmpEq(simd_internal::gatherValue<simd_type>(hash), simd_internal::gatherValues<simd_type>(hashData)));
		}
	};

	template<typename value_type> constexpr bool contains(const value_type* hashData, value_type byteToCheckFor, uint64_t size) {
		for (uint64_t x = 0; x < size; ++x) {
			if (hashData[x] == byteToCheckFor) {
				return true;
			}
		}
		return false;
	}

	template<uint64_t bucketSize, typename key_type, typename value_type> using simd_set_variant =
		std::variant<simd_set<key_type, value_type, bucketSize, 16>, simd_set<key_type, value_type, bucketSize, 32>, simd_set<key_type, value_type, bucketSize, 64>,
			simd_set<key_type, value_type, bucketSize, 128>, simd_set<key_type, value_type, bucketSize, 256>, simd_set<key_type, value_type, bucketSize, 512>,
			simd_set<key_type, value_type, bucketSize, 1024>, simd_set<key_type, value_type, bucketSize, 2048>, simd_set<key_type, value_type, bucketSize, 4096>,
			simd_set<key_type, value_type, bucketSize, 8192>, simd_set<key_type, value_type, bucketSize, 16384>>;

	constexpr std::array<uint64_t, 10> maxSizes{ 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };

	template<uint64_t size> constexpr uint64_t getMaxSizeIndex() {
		for (uint64_t x = 0; x < maxSizes.size(); ++x) {
			if (size <= maxSizes[x]) {
				return x;
			}
		}
		return -1;
	}

	template<typename key_type, typename value_type, size_t N> constexpr auto constructSimdSet(const std::array<std::pair<key_type, value_type>, N>& pairsNew,
		naive_prng prng = naive_prng{}) -> simd_set_variant<setSimdWidth<N>(), key_type, value_type> {
		return constructSimdSetHelper<key_type, value_type, N, getMaxSizeIndex<N>()>(pairsNew, prng);
	}

	template<typename key_type, typename value_type, size_t N, uint64_t maxSizeIndex>
	constexpr auto constructStringScalingFactor(const std::array<std::pair<key_type, value_type>, N>& pairsNew, uint64_t seed) {
		return constructStringScalingFactorHelper<key_type, value_type, N, getMaxSizeIndex<N>(), 0>(pairsNew, seed);
	}

	template<typename key_type, typename value_type, size_t N, size_t maxSizeIndex, uint64_t stringScalingFactorIndex>
	constexpr auto constructStringScalingFactorHelper(const std::array<std::pair<key_type, value_type>, N>& pairsNew, uint64_t seed) {
		std::array<std::pair<key_type, value_type>, N> pairs{ pairsNew };
		constexpr uint64_t bucketSize  = setSimdWidth<N>();
		constexpr uint64_t storageSize = maxSizes[maxSizeIndex];
		constexpr uint64_t numGroups   = storageSize > bucketSize ? storageSize / bucketSize : 1;

		simd_set<key_type, value_type, setSimdWidth<N>(), storageSize> setNew{};
		if constexpr (stringScalingFactorIndex < scalingFactorTable.size() - 1) {
			std::array<uint8_t, storageSize> controlBytes{};
			std::array<uint64_t, numGroups> bucketSizes{};
			std::array<uint64_t, storageSize> slots{};

			for (size_t i = 0; i < N; ++i) {
				const auto hash			 = fnv1a_hash{}.operator()(pairs[i].first.data(),
					 static_cast<uint64_t>(static_cast<float>(pairs[i].first.size()) * (static_cast<float>(scalingFactorTable[stringScalingFactorIndex]))), seed);
				const auto groupPos		 = (hash >> 7) % numGroups;
				const auto ctrlByte		 = static_cast<uint8_t>(hash);
				const auto bucketSizeNew = ++bucketSizes[groupPos];
				const auto slot			 = (groupPos * bucketSize) + bucketSizeNew;

				if (bucketSizeNew >= bucketSize || contains(slots.data() + groupPos * bucketSize, slot, bucketSize) ||
					contains(controlBytes.data() + groupPos * bucketSize, ctrlByte, bucketSize)) {
					return constructStringScalingFactorHelper<key_type, value_type, N, maxSizeIndex, stringScalingFactorIndex + 1>(pairs, seed);
				}

				controlBytes[slot] = ctrlByte;
				slots[i]		   = slot;
			}
			return stringScalingFactorIndex;
		} else {
			return stringScalingFactorIndex;
		}
	}

	template<typename key_type, typename value_type, size_t N, size_t maxSizeIndex>
	constexpr auto constructSimdSetHelper(const std::array<std::pair<key_type, value_type>, N>& pairsNew, naive_prng prng = naive_prng{})
		-> simd_set_variant<setSimdWidth<N>(), key_type, value_type> {
		std::array<std::pair<key_type, value_type>, N> pairs{ pairsNew };
		constexpr uint64_t bucketSize  = setSimdWidth<N>();
		constexpr uint64_t storageSize = maxSizes[maxSizeIndex];
		constexpr uint64_t numGroups   = storageSize > bucketSize ? storageSize / bucketSize : 1;

		simd_set<key_type, value_type, setSimdWidth<N>(), storageSize> setNew{};
		if constexpr (maxSizeIndex < maxSizes.size() - 1) {
			auto seed = prng();
			std::array<uint8_t, storageSize> controlBytes{};
			std::array<uint64_t, numGroups> bucketSizes{};
			std::array<value_type, storageSize> items{};
			std::array<uint64_t, storageSize> hashes{};
			std::array<uint64_t, storageSize> slots{};
			auto stringScalingFactorIndex = constructStringScalingFactorHelper<key_type, value_type, N, maxSizeIndex, 0>(pairs, seed);

			for (size_t i = 0; i < N; ++i) {
				const auto hash			 = fnv1a_hash{}.operator()(pairs[i].first.data(),
					 static_cast<uint64_t>(static_cast<float>(pairs[i].first.size()) * (static_cast<float>(scalingFactorTable[stringScalingFactorIndex]))), seed);
				const auto groupPos		 = (hash >> 7) % numGroups;
				const auto ctrlByte		 = static_cast<uint8_t>(hash);
				const auto bucketSizeNew = ++bucketSizes[groupPos];
				const auto slot			 = (groupPos * bucketSize) + bucketSizeNew;

				if (bucketSizeNew >= bucketSize || contains(slots.data() + groupPos * bucketSize, slot, bucketSize) ||
					contains(controlBytes.data() + groupPos * bucketSize, ctrlByte, bucketSize)) {
					return constructSimdSetHelper<key_type, value_type, N, maxSizeIndex + 1>(pairs, prng);
				}

				items[slot]		   = pairs[i].second;
				controlBytes[slot] = ctrlByte;
				slots[i]		   = slot;
				hashes[slot]	   = hash;
			}
			setNew.stringScalingFactorIndex = stringScalingFactorIndex;
			setNew.controlBytes				= controlBytes;
			setNew.hashes					= hashes;
			setNew.items					= items;
			setNew.seed						= seed;
			return simd_set_variant<setSimdWidth<N>(), key_type, value_type>{ setNew };
		} else {
			throw std::runtime_error{ "Sorry, but I failed to construct that hash-set!" };
			return simd_set_variant<setSimdWidth<N>(), key_type, value_type>{ setNew };
		}
	}

	template<const jsonifier::string_view& lhs> inline constexpr bool compareSv(const jsonifier::string_view rhs) noexcept {
		constexpr auto N = lhs.size();
		return (N == rhs.size()) && compare<N>(lhs.data(), rhs.data());
	}

	inline constexpr bool compareSv(const jsonifier::string_view lhs, const jsonifier::string_view rhs) noexcept {
		if (std::is_constant_evaluated()) {
			return lhs == rhs;
		} else {
			return (lhs.size() == rhs.size()) && compare(lhs.data(), rhs.data(), lhs.size());
		}
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

	template<typename value_type, const jsonifier::string_view& S> struct micro_set1 {
		std::array<value_type, 1> items{};

		constexpr decltype(auto) begin() const noexcept {
			return items.data();
		}
		constexpr decltype(auto) end() const noexcept {
			return items.data() + items.size();
		}

		constexpr decltype(auto) find(auto&& key) const noexcept {
			if (S == key) [[likely]] {
				return items.data();
			} else [[unlikely]] {
				return items.data() + items.size();
			}
		}
	};

	template<typename value_type, const jsonifier::string_view& S0, const jsonifier::string_view& S1> struct micro_set2 {
		std::array<value_type, 2> items{};

		static constexpr bool same_size	 = S0.size() == S1.size();
		static constexpr bool check_size = !same_size;

		constexpr decltype(auto) begin() const noexcept {
			return items.data();
		}
		constexpr decltype(auto) end() const noexcept {
			return items.data() + items.size();
		}

		constexpr decltype(auto) find(auto&& key) const noexcept {
			if constexpr (same_size) {
				constexpr auto n = S0.size();
				if (key.size() != n) {
					return items.data() + items.size();
				}
			}

			if (cxStringCmp<S0, check_size>(key)) {
				return items.data();
			} else if (cxStringCmp<S1, check_size>(key)) {
				return items.data() + 1;
			} else [[unlikely]] {
				return items.data() + items.size();
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
		constexpr auto& first = std::get<0>(std::get<I>(jsonifier::concepts::core_v<value_type>));
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
		constexpr auto& first = std::get<0>(std::get<I>(jsonifier::concepts::core_v<value_type>));
		using T0			  = std::decay_t<decltype(first)>;
		if constexpr (std::is_member_pointer_v<T0>) {
			return std::pair<jsonifier::string_view, value_t>{ getName<first>(), first };
		} else {
			return std::pair<jsonifier::string_view, value_t>{ jsonifier::string_view(first), std::get<1>(std::get<I>(jsonifier::concepts::core_v<value_type>)) };
		}
	}

	template<typename value_type, size_t I> constexpr auto getValue() noexcept {
		using value_t		  = value_tuple_variant_t<jsonifier::concepts::core_t<value_type>>;
		constexpr auto& first = std::get<0>(std::get<I>(jsonifier::concepts::core_v<value_type>));
		using T0			  = std::decay_t<decltype(first)>;
		if constexpr (std::is_member_pointer_v<T0>) {
			return first;
		} else {
			return std::get<1>(std::get<I>(jsonifier::concepts::core_v<value_type>));
		}
	}

	template<typename value_type, size_t... I> constexpr auto makeSetImpl(std::index_sequence<I...>) {
		using value_t	 = value_tuple_variant_t<jsonifier::concepts::core_t<value_type>>;
		constexpr auto n = std::tuple_size_v<jsonifier::concepts::core_t<value_type>>;

		if constexpr (n == 0) {
			return nullptr;
		} else if constexpr (n == 1) {
			return micro_set1<value_t, core_sv<value_type, I>::value...>{ getValue<value_type, I>()... };
		} else if constexpr (n == 2) {
			return micro_set2<value_t, core_sv<value_type, I>::value...>{ getValue<value_type, I>()... };
		} else {
			constexpr auto setNew{ constructSimdSet<jsonifier::string_view, value_t, n>({ keyValue<value_type, I>()... }) };
			constexpr auto newIndex = setNew.index();
			return std::get<newIndex>(setNew);
		}
	}

	template<typename value_type> constexpr auto makeSet() {
		constexpr auto indices = std::make_index_sequence<std::tuple_size_v<jsonifier::concepts::core_t<value_type>>>{};
		return makeSetImpl<jsonifier::concepts::decay_keep_volatile_t<value_type>>(indices);
	}
}