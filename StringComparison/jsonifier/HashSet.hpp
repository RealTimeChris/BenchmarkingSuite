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
/// Feb 20, 2023
#pragma once

#include <jsonifier/StaticVector.hpp>
#include <jsonifier/StringView.hpp>
#include <jsonifier/Reflection.hpp>
#include <jsonifier/Derailleur.hpp>
#include <jsonifier/Config.hpp>
#include <jsonifier/Tuple.hpp>
#include <jsonifier/Error.hpp>
#include <algorithm>
#include <numeric>
#include <span>

namespace jsonifier_internal {

	struct xoshiro256 {
		std::array<size_t, 4> state{};

		constexpr xoshiro256() {
			auto x = 7185499250578500046ull >> 12ull;
			x ^= x << 25ull;
			x ^= x >> 27ull;
			size_t s = x * 0x2545F4914F6CDD1Dull;
			for (auto& y: state) {
				y = splitmix64(s);
			}
		}

		constexpr size_t operator()() {
			const size_t result = rotl(state[1] * 5ull, 7ull) * 9ull;

			const size_t t = state[1] << 17ull;

			state[2ull] ^= state[0ull];
			state[3ull] ^= state[1ull];
			state[1ull] ^= state[2ull];
			state[0ull] ^= state[3ull];

			state[2ull] ^= t;

			state[3ull] = rotl(state[3ull], 45ull);

			return result;
		}

	  protected:
		constexpr size_t rotl(const size_t x, size_t k) const {
			return (x << k) | (x >> (64ull - k));
		}

		constexpr size_t splitmix64(size_t& seed) const {
			size_t result = seed += 0x9E3779B97F4A7C15ull;
			result		  = (result ^ (result >> 30ull)) * 0xBF58476D1CE4E5B9ull;
			result		  = (result ^ (result >> 27ull)) * 0x94D049BB133111EBull;
			return result ^ (result >> 31ull);
		}
	};

	template<typename value_type01, typename value_type02> constexpr bool contains(const value_type01* hashData, value_type02 byteToCheckFor, size_t size) {
		for (size_t x = 0; x < size; ++x) {
			if (hashData[x] == byteToCheckFor) {
				return true;
			}
		}
		return false;
	}

	template<size_t size, size_t length> constexpr size_t getMaxSizeIndex(const size_t (&maxSizes)[length]) {
		for (size_t x = 0; x < std::size(maxSizes); ++x) {
			if (size <= maxSizes[x]) {
				return x;
			}
		}
		return std::size(maxSizes) - 1;
	}

	struct set_construction_values {
		size_t minStringLength{};
		size_t maxStringLength{};
		size_t maxSizeIndex{ 0 };
		size_t maxMixCount{ 5 };
		size_t stringLength{ std::numeric_limits<uint64_t>::max() };
		size_t mixCount{ 1 };
		bool success{};
		size_t seed{};

		static constexpr set_construction_values constructConstructionValues(uint64_t minStringLength, uint64_t maxStringLength) {
			set_construction_values returnValues{};
			returnValues.minStringLength = minStringLength;
			returnValues.stringLength	 = returnValues.minStringLength;
			returnValues.maxStringLength = maxStringLength;
			return returnValues;
		}

		constexpr set_construction_values incrementSizeIndex() const {
			set_construction_values returnValues{ *this };
			++returnValues.maxSizeIndex;
			returnValues.stringLength = minStringLength;
			returnValues.mixCount	  = 0;
			return returnValues;
		}

		constexpr set_construction_values incrementStringLength() const {
			set_construction_values returnValues{ *this };
			++returnValues.stringLength;
			return returnValues;
		}

		constexpr set_construction_values incrementMixCount() const {
			set_construction_values returnValues{ *this };
			++returnValues.mixCount;
			return returnValues;
		}
	};

	template<size_t length> struct set_simd {
		using type = std::conditional_t<length >= 64 && bytesPerStep >= 64, simd_int_512, std::conditional_t<length >= 32 && bytesPerStep >= 32, simd_int_256, simd_int_128>>;
	};

	template<size_t length> struct set_integer {
		using type = size_t;
	};

	template<size_t length> constexpr size_t setSimdWidth() {
		return length >= 64 && bytesPerStep >= 64 ? 64 : length >= 32 && bytesPerStep >= 32 ? 32 : 16;
	};

	template<size_t length> using set_simd_t = set_simd<length>::type;

	template<size_t length> using set_integer_t = set_integer<length>::type;

	template<uint64_t dimension1, uint64_t dimension2, uint64_t dimension3> using construction_values_array =
		static_vector<set_construction_values, dimension1 * dimension2 * dimension3>;

	template<uint64_t dimension1, uint64_t dimension2, uint64_t dimension3>
	constexpr set_construction_values collectOptimalConstructionValues(construction_values_array<dimension1, dimension2, dimension3> indexTable) {
		set_construction_values returnValues{};
		for (size_t x = 0; x < indexTable.size(); ++x) {
			if (indexTable[x].success && (returnValues.stringLength > indexTable[x].stringLength) ||
				(returnValues.stringLength >= indexTable[x].stringLength && returnValues.maxSizeIndex > indexTable[x].maxSizeIndex)) {
				returnValues = indexTable[x];
			}
		}
		return returnValues;
	}

	JSONIFIER_INLINE constexpr bool compareSvConst(const jsonifier::string_view& lhs, const jsonifier::string_view rhs) noexcept {
		return lhs == rhs;
	}

	JSONIFIER_INLINE constexpr bool compareSvNonConst(const jsonifier::string_view& lhs, const jsonifier::string_view rhs) noexcept {
		const auto N = lhs.size();
		return (N == rhs.size()) && compare(lhs.data(), rhs.data(), N);
	}

	JSONIFIER_INLINE constexpr size_t H1(size_t hash) {
		return hash >> 8;
	}

	JSONIFIER_INLINE constexpr uint8_t H2(size_t hash) {
		return hash & 0xFF;
	}

	constexpr size_t simdHashSetMaxSizes[]{ 16, 32, 64, 128, 256, 512, 1024 };

	template<typename key_type, typename value_type, size_t actualCount, size_t storageSize> struct simd_hash_set : public key_hasher {
		static constexpr size_t bucketSize = setSimdWidth<actualCount>();
		static constexpr size_t numGroups  = storageSize > bucketSize ? storageSize / bucketSize : 1;
		using simd_type					   = set_simd_t<bucketSize>;
		using integer_type				   = set_integer_t<bucketSize>;
		JSONIFIER_ALIGN std::pair<jsonifier::string_view, value_type> items[storageSize]{};
		JSONIFIER_ALIGN uint8_t controlBytes[storageSize]{};
		JSONIFIER_ALIGN uint64_t stringLength{};

		constexpr simd_hash_set() noexcept = default;

		JSONIFIER_INLINE constexpr decltype(auto) begin() const noexcept {
			return &items->second;
		}

		JSONIFIER_INLINE constexpr decltype(auto) end() const noexcept {
			return &(items + storageSize)->second;
		}

		JSONIFIER_INLINE constexpr auto size() const noexcept {
			return actualCount;
		}

		template<typename key_type_new> JSONIFIER_INLINE constexpr decltype(auto) find(key_type_new&& key) const noexcept {
			if (!std::is_constant_evaluated()) {
				JSONIFIER_ALIGN const auto hash = key_hasher::hashKeyRt(key.data(), stringLength);
				JSONIFIER_ALIGN size_t group	= (hash >> 8) % numGroups;
				JSONIFIER_ALIGN auto matchVal	= match(static_cast<uint8_t>(hash), controlBytes + group * bucketSize);
				JSONIFIER_ALIGN auto finalIndex = ((group * bucketSize) + matchVal) % storageSize;
				return compareSvNonConst((items + finalIndex)->first, key) ? &(items + finalIndex)->second : end();
			} else {
				JSONIFIER_ALIGN const auto hash = key_hasher::hashKeyCt(key.data(), stringLength);
				JSONIFIER_ALIGN size_t group	= (hash >> 8) % numGroups;
				JSONIFIER_ALIGN auto matchVal	= constMatch(static_cast<uint8_t>(hash), controlBytes + group * bucketSize);
				JSONIFIER_ALIGN auto finalIndex = ((group * bucketSize) + matchVal) % storageSize;
				return compareSvConst((items + finalIndex)->first, key) ? &(items + finalIndex)->second : end();
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

		uint32_t match(uint8_t hash, const uint8_t* ctrl) const {
			return simd_internal::tzcnt(simd_internal::opCmpEq(simd_internal::gatherValues<simd_type>(ctrl), simd_internal::gatherValue<simd_type>(hash)));
		}

		constexpr integer_type constMatch(uint8_t hash, const uint8_t* hashData) const {
			integer_type mask = 0;
			for (size_t x = 0; x < bucketSize; ++x) {
				if (hashData[x] == hash) {
					mask |= (1ull << x);
				}
			}
			return tzcnt(mask);
		}
	};

	template<size_t startingValue, size_t actualCount, typename key_type, typename value_type, template<typename, typename, size_t, size_t> typename set_type> using set_variant =
		std::variant<set_type<key_type, value_type, actualCount, startingValue>, set_type<key_type, value_type, actualCount, startingValue * 2ull>,
			set_type<key_type, value_type, actualCount, startingValue * 4ull>, set_type<key_type, value_type, actualCount, startingValue * 8ull>,
			set_type<key_type, value_type, actualCount, startingValue * 16ull>, set_type<key_type, value_type, actualCount, startingValue * 32ull>,
			set_type<key_type, value_type, actualCount, startingValue * 64ull>>;

	template<typename key_type, typename value_type, size_t actualCount, size_t storageSize>
	constexpr auto constructSimdHashSetFinal(const std::array<std::pair<key_type, value_type>, actualCount>& pairsNew, set_construction_values constructioinValues)
		-> set_variant<16, actualCount, key_type, value_type, simd_hash_set> {
		constexpr size_t bucketSize = setSimdWidth<actualCount>();
		constexpr size_t numGroups	= storageSize >= bucketSize ? storageSize / bucketSize : 1;
		size_t storageSizeNew{ storageSize };
		simd_hash_set<key_type, value_type, actualCount, storageSize> simdHashSetNew{};
		simdHashSetNew.setSeed(constructioinValues.seed);
		simdHashSetNew.stringLength = constructioinValues.stringLength;
		std::array<size_t, numGroups> bucketSizes{};
		for (size_t x = 0; x < actualCount; ++x) {
			const auto hash					  = simdHashSetNew.hashKeyCt(pairsNew[x].first.data(),
				  constructioinValues.stringLength > pairsNew[x].first.size() ? pairsNew[x].first.size() : constructioinValues.stringLength);
			const auto groupPos				  = H1(hash) % numGroups;
			const auto ctrlByte				  = H2(hash);
			const auto bucketSizeNew		  = bucketSizes[groupPos]++;
			const auto slot					  = ((groupPos * bucketSize) + bucketSizeNew) % storageSize;
			simdHashSetNew.items[slot]		  = pairsNew[x];
			simdHashSetNew.controlBytes[slot] = ctrlByte;
		}

		return set_variant<16, actualCount, key_type, value_type, simd_hash_set>{ simd_hash_set<key_type, value_type, actualCount, storageSize>(simdHashSetNew) };
	}

	template<typename key_type, typename value_type, size_t storageSize, size_t actualCount> using construct_simd_hash_set_function_ptr =
		decltype(&constructSimdHashSetFinal<key_type, value_type, storageSize, 16ull>);

	template<typename key_type, typename value_type, size_t actualCount>
	constexpr construct_simd_hash_set_function_ptr<key_type, value_type, actualCount, 16ull> constructSimdHashSetFinalPtrs[7] = {
		&constructSimdHashSetFinal<key_type, value_type, actualCount, 16ull>, &constructSimdHashSetFinal<key_type, value_type, actualCount, 32ull>,
		&constructSimdHashSetFinal<key_type, value_type, actualCount, 64ull>, &constructSimdHashSetFinal<key_type, value_type, actualCount, 128ull>,
		&constructSimdHashSetFinal<key_type, value_type, actualCount, 256ull>, &constructSimdHashSetFinal<key_type, value_type, actualCount, 512ull>,
		&constructSimdHashSetFinal<key_type, value_type, actualCount, 1024ull>
	};

	template<set_construction_values constructionValuesNew, typename key_type, typename value_type, size_t actualCount>
	constexpr auto constructSimdHashSet(const std::array<std::pair<key_type, value_type>, actualCount>& pairsNew, xoshiro256 prng = xoshiro256{},
		construction_values_array<constructionValuesNew.maxStringLength - constructionValuesNew.minStringLength, std::size(simdHashSetMaxSizes) + 1,
			constructionValuesNew.maxMixCount + 1>
			constructionValues = construction_values_array<constructionValuesNew.maxStringLength - constructionValuesNew.minStringLength, std::size(simdHashSetMaxSizes) + 1,
				constructionValuesNew.maxMixCount + 1>{}) -> set_variant<16, actualCount, key_type, value_type, simd_hash_set> {
		constexpr size_t bucketSize	 = setSimdWidth<actualCount>();
		constexpr size_t storageSize = simdHashSetMaxSizes[constructionValuesNew.maxSizeIndex];
		constexpr size_t numGroups	 = storageSize / bucketSize;
		auto seed					 = prng();
		std::array<uint8_t, storageSize> controlBytes{};
		std::array<size_t, numGroups> bucketSizes{};
		std::array<size_t, storageSize> slots{};
		key_hasher hasherNew{ seed };
		for (size_t x = 0; x < actualCount; ++x) {
			const auto hash			 = hasherNew.hashKeyCt(pairsNew[x].first.data(),
				 constructionValuesNew.stringLength > pairsNew[x].first.size() ? pairsNew[x].first.size() : constructionValuesNew.stringLength);
			const auto groupPos		 = H1(hash) % numGroups;
			const auto ctrlByte		 = H2(hash);
			const auto bucketSizeNew = ++bucketSizes[groupPos];
			const auto slot			 = ((groupPos * bucketSize) + bucketSizeNew) % storageSize;
			if (bucketSizeNew >= bucketSize || contains(controlBytes.data() + groupPos * bucketSize, ctrlByte, bucketSize) ||
				contains(slots.data() + groupPos * bucketSize, slot, bucketSize)) {
				if constexpr (constructionValuesNew.mixCount < constructionValuesNew.maxMixCount - 1 && constructionValuesNew.maxSizeIndex < std::size(simdHashSetMaxSizes) - 1) {
					constexpr auto newConstructionValues = constructionValuesNew.incrementMixCount();
					return constructSimdHashSet<newConstructionValues, key_type, value_type, actualCount>(pairsNew, prng, constructionValues);
				} else if constexpr (constructionValuesNew.stringLength < constructionValuesNew.maxStringLength - 1 &&
					constructionValuesNew.maxSizeIndex < std::size(simdHashSetMaxSizes) - 1) {
					constexpr auto newConstructionValues = constructionValuesNew.incrementStringLength();
					return constructSimdHashSet<newConstructionValues, key_type, value_type, actualCount>(pairsNew, prng, constructionValues);
				} else if constexpr (constructionValuesNew.maxSizeIndex < std::size(simdHashSetMaxSizes) - 1) {
					constexpr auto newConstructionValues = constructionValuesNew.incrementSizeIndex();
					return constructSimdHashSet<newConstructionValues, key_type, value_type, actualCount>(pairsNew, prng, constructionValues);
				} else {
					auto resultValues = collectOptimalConstructionValues<constructionValuesNew.maxStringLength - constructionValuesNew.minStringLength,
						std::size(simdHashSetMaxSizes) + 1, constructionValuesNew.maxMixCount + 1>(constructionValues);
					auto newSet		  = constructSimdHashSetFinalPtrs<key_type, value_type, actualCount>[resultValues.maxSizeIndex](pairsNew, resultValues);
					return { newSet };
				}
			}
			controlBytes[slot] = ctrlByte;
			slots[x]		   = slot;
		}
		constructionValues.emplace_back(set_construction_values{ constructionValuesNew.minStringLength, constructionValuesNew.maxStringLength, constructionValuesNew.maxSizeIndex,
			constructionValuesNew.maxMixCount, constructionValuesNew.stringLength, constructionValuesNew.mixCount, true, seed });
		if constexpr (constructionValuesNew.mixCount < constructionValuesNew.maxMixCount - 1 && constructionValuesNew.maxSizeIndex < std::size(simdHashSetMaxSizes) - 1) {
			constexpr auto newConstructionValues = constructionValuesNew.incrementMixCount();
			return constructSimdHashSet<newConstructionValues, key_type, value_type, actualCount>(pairsNew, prng, constructionValues);
		} else if constexpr (constructionValuesNew.stringLength < constructionValuesNew.maxStringLength - 1 &&
			constructionValuesNew.maxSizeIndex < std::size(simdHashSetMaxSizes) - 1) {
			constexpr auto newConstructionValues = constructionValuesNew.incrementStringLength();
			return constructSimdHashSet<newConstructionValues, key_type, value_type, actualCount>(pairsNew, prng, constructionValues);
		} else if constexpr (constructionValuesNew.maxSizeIndex < std::size(simdHashSetMaxSizes) - 1) {
			constexpr auto newConstructionValues = constructionValuesNew.incrementSizeIndex();
			return constructSimdHashSet<newConstructionValues, key_type, value_type, actualCount>(pairsNew, prng, constructionValues);
		} else {
			auto resultValues = collectOptimalConstructionValues<constructionValuesNew.maxStringLength - constructionValuesNew.minStringLength, std::size(simdHashSetMaxSizes) + 1,
				constructionValuesNew.maxMixCount + 1>(constructionValues);
			auto newSet		  = constructSimdHashSetFinalPtrs<key_type, value_type, actualCount>[resultValues.maxSizeIndex](pairsNew, resultValues);
			return { newSet };
		}
	}

	template<const jsonifier::string_view& S, bool CheckSize = true> JSONIFIER_INLINE constexpr bool cxStringCmp(const jsonifier::string_view key) noexcept {
		if (std::is_constant_evaluated()) {
			return key == S;
		} else {
			constexpr auto size = S.size();
			if constexpr (CheckSize) {
				return (size == key.size()) && compare<size>(S.data(), key.data());
			} else {
				return compare<size>(S.data(), key.data());
			}
		}
	}	

	template<const jsonifier::string_view& lhs> JSONIFIER_INLINE constexpr bool compareSv(const jsonifier::string_view rhs) noexcept {
		if (std::is_constant_evaluated()) {
			return lhs == rhs;
		} else {
			constexpr auto N = lhs.size();
			return (N == rhs.size()) && compare<N>(lhs.data(), rhs.data());
		}
	}

	template<typename value_type, const jsonifier::string_view& S> struct micro_set1 {
		std::pair<jsonifier::string_view, value_type> items[1]{};

		JSONIFIER_INLINE constexpr decltype(auto) begin() const noexcept {
			return &items->second;
		}

		JSONIFIER_INLINE constexpr decltype(auto) end() const noexcept {
			return &(items + 1)->second;
		}

		template<typename key_type> JSONIFIER_INLINE constexpr decltype(auto) find(key_type&& key) const noexcept {
			if (compareSv<S>(key)) [[likely]] {
				return begin();
			} else [[unlikely]] {
				return end();
			}
		}
	};

	template<typename value_type, const jsonifier::string_view& S0, const jsonifier::string_view& S1> struct micro_set2 {
		std::pair<jsonifier::string_view, value_type> items[2]{};

		static constexpr bool sameSize	 = S0.size() == S1.size();
		static constexpr bool checkSize = !sameSize;

		JSONIFIER_INLINE constexpr decltype(auto) begin() const noexcept {
			return &items->second;
		}

		JSONIFIER_INLINE constexpr decltype(auto) end() const noexcept {
			return &(items + 2)->second;
		}

		template<typename key_type> JSONIFIER_INLINE constexpr decltype(auto) find(key_type&& key) const noexcept {
			if constexpr (sameSize) {
				constexpr auto n = S0.size();
				if (key.size() != n) {
					return end();
				}
			}

			if (cxStringCmp<S0, checkSize>(key)) {
				return begin();
			} else if (cxStringCmp<S1, checkSize>(key)) {
				return begin() + 1;
			} else [[unlikely]] {
				return end();
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

	template<typename value_type, size_t I> struct core_sv {
		static constexpr jsonifier::string_view value = getKey<value_type, I>();
	};

	template<typename value_type, size_t I> constexpr auto keyValue() noexcept {
		using value_t		  = value_tuple_variant_t<jsonifier::concepts::core_t<value_type>>;
		constexpr auto& first = std::get<0>(std::get<I>(jsonifier::concepts::core_v<value_type>));
		using T0			  = jsonifier::concepts::unwrap_t<decltype(first)>;
		if constexpr (std::is_member_pointer_v<T0>) {
			return std::pair<jsonifier::string_view, value_t>{ getName<first>(), first };
		} else {
			return std::pair<jsonifier::string_view, value_t>{ jsonifier::string_view(first), std::get<1>(std::get<I>(jsonifier::concepts::core_v<value_type>)) };
		}
	}

	template<typename value_type, size_t I> constexpr auto getValue() noexcept {
		constexpr auto& first = std::get<0>(std::get<I>(jsonifier::concepts::core_v<value_type>));
		using T0			  = jsonifier::concepts::unwrap_t<decltype(first)>;
		if constexpr (std::is_member_pointer_v<T0>) {
			return first;
		} else {
			return std::get<1>(std::get<I>(jsonifier::concepts::core_v<value_type>));
		}
	}

	template<typename value_type, size_t... I> constexpr auto makeSetImpl(std::index_sequence<I...>) {
		using value_t	 = value_tuple_variant_t<jsonifier::concepts::core_t<value_type>>;
		constexpr auto n = std::tuple_size_v<jsonifier::concepts::core_t<value_type>>;
		constexpr auto keyStatsVal = keyStats<value_type>();
		if constexpr (n == 0) {
			return nullptr;
		} else if constexpr (n == 1) {
			return micro_set1<value_t, core_sv<value_type, I>::value...>{ keyValue<value_type, I>()... };
		} else if constexpr (n == 2) {
			return micro_set2<value_t, core_sv<value_type, I>::value...>{ keyValue<value_type, I>()... };
		} else {
			constexpr auto constructionVals = set_construction_values::constructConstructionValues(keyStatsVal.minLength / 2, keyStatsVal.maxLength);
			constexpr auto setNew{ constructSimdHashSet<constructionVals, jsonifier::string_view, value_t, n>({ keyValue<value_type, I>()... }) };
			constexpr auto newIndex = setNew.index();
			return std::get<newIndex>(setNew);
		}
	}

	template<typename value_type> constexpr auto makeSet() {
		constexpr auto indices = std::make_index_sequence<std::tuple_size_v<jsonifier::concepts::core_t<value_type>>>{};
		return makeSetImpl<jsonifier::concepts::decay_keep_volatile_t<value_type>>(indices);
	}
}