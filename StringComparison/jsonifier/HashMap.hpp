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

	struct naive_prng final {
		uint64_t x = 7185499250578500046;
		constexpr uint64_t operator()() noexcept {
			x ^= x >> 12;
			x ^= x << 25;
			x ^= x >> 27;
			return x * 0x2545F4914F6CDD1DULL;
		}
	};

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

	template<uint64_t length> struct fit_unsigned {
		using type = std::conditional_t<length <= std::numeric_limits<uint8_t>::max(), uint8_t, std::conditional_t<length <= std::numeric_limits<uint16_t>::max(), uint16_t, std::conditional_t<length <= std::numeric_limits<uint32_t>::max(), uint32_t, std::conditional_t<length<=std::numeric_limits<uint64_t>::max(),uint64_t, void>>>>;
	};

	template<const jsonifier::string_view& lhs> inline constexpr bool compareSv(const jsonifier::string_view rhs) noexcept {
		constexpr auto N = lhs.size();
		return (N == rhs.size()) && compare<N>(lhs.data(), rhs.data());
	}
	// compare_sv checks sizes
	inline constexpr bool compare_sv(const jsonifier::string_view lhs, const jsonifier::string_view rhs) noexcept {
		if (std::is_constant_evaluated()) {
			return lhs == rhs;
		} else {
			return (lhs.size() == rhs.size()) && compare(lhs.data(), rhs.data(), lhs.size());
		}
	}

	template<uint64_t length> using fit_unsigned_t = typename fit_unsigned<length>::type;

	constexpr size_t naive_map_max_size = 32;

	struct naive_map_desc {
		size_t N{};
		uint64_t seed{};
		size_t bucket_size{};
		bool use_hash_comparison = false;
		size_t min_length		 = (std::numeric_limits<size_t>::max)();
		size_t max_length{};
	};

	constexpr uint64_t to_uint64_n_below_8(const char* bytes, const size_t N) noexcept {
		static_assert(std::endian::native == std::endian::little);
		uint64_t res{};
		if (std::is_constant_evaluated()) {
			for (size_t i = 0; i < N; ++i) {
				res |= (uint64_t(uint8_t(bytes[i])) << (i << 3));
			}
		} else {
			switch (N) {
				case 1: {
					std::memcpy(&res, bytes, 1);
					break;
				}
				case 2: {
					std::memcpy(&res, bytes, 2);
					break;
				}
				case 3: {
					std::memcpy(&res, bytes, 3);
					break;
				}
				case 4: {
					std::memcpy(&res, bytes, 4);
					break;
				}
				case 5: {
					std::memcpy(&res, bytes, 5);
					break;
				}
				case 6: {
					std::memcpy(&res, bytes, 6);
					break;
				}
				case 7: {
					std::memcpy(&res, bytes, 7);
					break;
				}
				default: {
					// zero size case
					break;
				}
			}
		}
		return res;
	}

	template<size_t N = 8> constexpr uint64_t to_uint64(const char* bytes) noexcept {
		static_assert(N <= sizeof(uint64_t));
		static_assert(std::endian::native == std::endian::little);
		if (std::is_constant_evaluated()) {
			uint64_t res{};
			for (size_t i = 0; i < N; ++i) {
				res |= (uint64_t(uint8_t(bytes[i])) << (i << 3));
			}
			return res;
		} else if constexpr (N == 8) {
			uint64_t res;
			std::memcpy(&res, bytes, N);
			return res;
		} else {
			uint64_t res{};
			std::memcpy(&res, bytes, N);
			constexpr auto num_bytes = sizeof(uint64_t);
			constexpr auto shift	 = (uint64_t(num_bytes - N) << 3);
			if constexpr (shift == 0) {
				return res;
			} else {
				return (res << shift) >> shift;
			}
		}
	}

	// With perfect hash tables we can sacrifice quality of the hash function since
	// we keep generating seeds until its perfect. This allows for the usage of fast
	// but terible hashing algs.
	// This is one such terible hashing alg
	template<bool use_hash_comparison> struct naive_hash final {
		static inline constexpr uint64_t bitmix(uint64_t h) noexcept {
			if constexpr (use_hash_comparison) {
				h ^= (h >> 33);
				h *= 0xff51afd7ed558ccdL;
				h ^= (h >> 33);
				h *= 0xc4ceb9fe1a85ec53L;
				h ^= (h >> 33);
			} else {
				h *= 0x9FB21C651E98DF25L;
				h ^= std::rotr(h, 49);
			}
			return h;
		};

		constexpr uint64_t operator()(std::integral auto value, const uint64_t seed) noexcept {
			return bitmix(uint64_t(value) ^ seed);
		}

		template<uint64_t seed> constexpr uint64_t operator()(std::integral auto value) noexcept {
			return bitmix(uint64_t(value) ^ seed);
		}

		constexpr uint64_t operator()(const jsonifier::string_view value, const uint64_t seed) noexcept {
			uint64_t h		 = (0xcbf29ce484222325 ^ seed) * 1099511628211;
			const auto n	 = value.size();
			const char* data = value.data();

			if (n < 8) {
				return bitmix(h ^ to_uint64_n_below_8(data, n));
			}

			const char* end7 = data + n - 7;
			for (auto d0 = data; d0 < end7; d0 += 8) {
				h = bitmix(h ^ to_uint64(d0));
			}
			// Handle potential tail. We know we have at least 8
			return bitmix(h ^ to_uint64(data + n - 8));
		}

		template<naive_map_desc D> constexpr uint64_t operator()(const jsonifier::string_view value) noexcept {
			constexpr auto h_init = (0xcbf29ce484222325 ^ D.seed) * 1099511628211;
			if constexpr (D.max_length < 8) {
				const auto n = value.size();
				if (n > 7) {
					return D.seed;
				}
				return bitmix(h_init ^ to_uint64_n_below_8(value.data(), n));
			} else if constexpr (D.min_length > 7) {
				const auto n = value.size();

				if (n < 8) {
					return D.seed;
				}

				uint64_t h		 = h_init;
				const char* data = value.data();
				const char* end7 = data + n - 7;
				for (auto d0 = data; d0 < end7; d0 += 8) {
					h = bitmix(h ^ to_uint64(d0));
				}
				// Handle potential tail. We know we have at least 8
				return bitmix(h ^ to_uint64(data + n - 8));
			} else {
				uint64_t h		 = h_init;
				const auto n	 = value.size();
				const char* data = value.data();

				if (n < 8) {
					return bitmix(h ^ to_uint64_n_below_8(data, n));
				}

				const char* end7 = data + n - 7;
				for (auto d0 = data; d0 < end7; d0 += 8) {
					h = bitmix(h ^ to_uint64(d0));
				}
				// Handle potential tail. We know we have at least 8
				return bitmix(h ^ to_uint64(data + n - 8));
			}
		}
	};

	constexpr bool contains(auto&& data, auto&& val) noexcept {
		const auto n = data.size();
		for (size_t i = 0; i < n; ++i) {
			if (data[i] == val) {
				return true;
			}
		}
		return false;
	}

	template<bool use_hash_comparison, size_t N>
	constexpr naive_map_desc naive_map_hash(const std::array<jsonifier::string_view, N>& v) noexcept {
		constexpr auto invalid = (std::numeric_limits<uint64_t>::max)();

		naive_map_desc desc{ N };
		// std::bit_ceil(N * N) / 2 results in a max of around 62% collision chance (e.g. size 32).
		// This uses 512 bytes for 32 keys.
		// Keeping the bucket size a power of 2 probably makes the modulus more efficient.
		desc.bucket_size		 = (N == 1) ? 1 : std::bit_ceil(N * N) / 2;
		desc.use_hash_comparison = use_hash_comparison;
		auto& seed				 = desc.seed;

		for (size_t i = 0; i < N; ++i) {
			const auto n = v[i].size();
			if (n < desc.min_length) {
				desc.min_length = n;
			}
			if (n > desc.max_length) {
				desc.max_length = n;
			}
		}

		auto naive_perfect_hash = [&] {
			std::array<size_t, N> bucket_index{};

			naive_prng gen{};
			for (size_t i = 0; i < 1024; ++i) {
				seed		 = gen();
				size_t index = 0;
				for (const auto& key: v) {
					const auto hash = naive_hash<use_hash_comparison>{}(key, seed);
					if (hash == seed) {
						break;
					}
					const auto bucket = hash % desc.bucket_size;
					if (contains(std::span{ bucket_index.data(), index }, bucket)) {
						break;
					}
					bucket_index[index] = bucket;
					++index;
				}

				if (index == N) {
					// make sure the seed does not collide with any hashes
					const auto bucket = seed % desc.bucket_size;
					if (not contains(std::span{ bucket_index.data(), N }, bucket)) {
						return;// found working seed
					}
				}
			}

			seed = invalid;
		};

		naive_perfect_hash();
		if (seed == invalid) {
			// Failed to find perfect hash
			std::abort();
			return {};
		}

		return desc;
	}

	template<class Value, naive_map_desc D>
	struct simd_map {
		// Birthday paradox makes this unsuitable for large numbers of keys without
		// using a ton of memory.
		static constexpr auto N = D.N;
		using hash_alg			= naive_hash<D.use_hash_comparison>;
		std::array<std::pair<jsonifier::string_view, Value>, N> items{};
		std::array<uint64_t, N * D.use_hash_comparison> hashes{};
		std::array<uint8_t, D.bucket_size> table{};

		constexpr decltype(auto) begin() const noexcept {
			return items.begin();
		}
		constexpr decltype(auto) end() const noexcept {
			return items.end();
		}

		constexpr size_t size() const noexcept {
			return items.size();
		}

		constexpr decltype(auto) find(auto&& key) const noexcept {
			const auto hash = hash_alg{}.template operator()<D>(key);
			// constexpr bucket_size means the compiler can replace the modulos with
			// more efficient instructions So this is not as expensive as this looks
			const auto index = table[hash % D.bucket_size];
			if (hashes[index] != hash) [[unlikely]]
				return items.end();
			return items.begin() + index;
		}
	};

	template<class value_type, naive_map_desc D>
	constexpr auto make_naive_map(const std::array<std::pair<jsonifier::string_view, value_type>, D.N>& pairs) {
		simd_map<value_type, D> ht{ pairs };

		using hash_alg = naive_hash<D.use_hash_comparison>;

		for (size_t i = 0; i < D.N; ++i) {
			const auto hash = hash_alg{}.template operator()<D>(pairs[i].first);
			if constexpr (D.use_hash_comparison) {
				ht.hashes[i] = hash;
			}
			ht.table[hash % D.bucket_size] = uint8_t(i);
		}

		return ht;
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
			constexpr std::array<jsonifier::string_view, n> keys{ getKey<value_type, I>()... };

			constexpr auto naive_desc = naive_map_hash<true, n>(keys);
			return make_naive_map<value_t, naive_desc>(std::array{ keyValue<value_type, I>()... });
		}
	}

	template<typename value_type> constexpr auto makeMap() {
		constexpr auto indices = std::make_index_sequence<std::tuple_size_v<jsonifier::concepts::core_t<value_type>>>{};
		return makeMapImpl<jsonifier::concepts::decay_keep_volatile_t<value_type>>(indices);
	}
}