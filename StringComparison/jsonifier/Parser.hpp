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

#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/JsonStructuralIterator.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Validator.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/HashMap.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/String.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Error.hpp"
#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Simd.hpp"

namespace jsonifier {

	struct parse_options {
		bool refreshString{ true };
		bool minified{ false };
	};

}

namespace jsonifier_internal {

	template<typename derived_type> class parser;

	template<typename derived_type> struct parse_options_internal {
		mutable parser<derived_type>* parserPtr{};
		jsonifier::parse_options optionsReal{};
		mutable string_view_ptr rootIter{};
	};

	JSONIFIER_INLINE std::pair<string_view_ptr, string_view_ptr> collectIterPair(string_view_ptr startPtr, string_view_ptr endPtr) {
		while (whitespaceTable[static_cast<uint64_t>(*startPtr)]) {
			++startPtr;
		}
		auto startPtrNew = startPtr;

		while (whitespaceTable[static_cast<uint64_t>(*endPtr)]) {
			--endPtr;
		}
		auto endPtrNew = endPtr;

		return std::make_pair(startPtrNew, endPtrNew);
	}

	template<typename derived_type, typename value_type> struct parse_impl;

	template<typename derived_type> class parser {
	  public:
		template<typename derived_type_new, typename value_type> friend struct parse_impl;

		JSONIFIER_INLINE parser& operator=(const parser& other) = delete;
		JSONIFIER_INLINE parser(const parser& other)			= delete;

		template<jsonifier::parse_options options = jsonifier::parse_options{}, typename value_type, jsonifier::concepts::string_t buffer_type>
		JSONIFIER_INLINE bool parseJson(value_type&& object, buffer_type&& in) {
			static_assert(jsonifier::concepts::printErrorFunction<jsonifier::concepts::unwrap_t<value_type>>(),
				"No specialization of core exists for the type named above - please specialize it!");
			derivedRef.errors.clear();
			static constexpr parse_options_internal<derived_type> optionsReal{ .optionsReal = options };
			optionsReal.parserPtr = this;
			optionsReal.rootIter  = reinterpret_cast<string_view_ptr>(in.data());
			if constexpr (!options.minified) {
				derivedRef.section.reset(in.data(), in.size());
				json_structural_iterator iter{ derivedRef.section.begin(), derivedRef.section.end(), derivedRef.errors };
				json_structural_iterator end{ derivedRef.section.end(), derivedRef.section.end(), derivedRef.errors };
				if (!iter || (*iter != '{' && *iter != '[')) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(iter - optionsReal.rootIter,
						end - optionsReal.rootIter, optionsReal.rootIter));
					return false;
				}
				parse_impl<derived_type, value_type>::template impl<optionsReal>(std::forward<value_type>(object), iter, end);
				if constexpr (!options.minified) {
					if (readIters.first != readIters.second) {
						static constexpr auto sourceLocation{ std::source_location::current() };
						getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(iter - optionsReal.rootIter,
							end - optionsReal.rootIter, optionsReal.rootIter));
						return false;
					}
				}
			} else {
				auto readIters = collectIterPair(reinterpret_cast<string_view_ptr>(in.data()), reinterpret_cast<string_view_ptr>(in.data() + in.size()));
				if (!readIters.first || (*readIters.first != '{' && *readIters.first != '[')) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(readIters.first - optionsReal.rootIter,
						readIters.second - optionsReal.rootIter, optionsReal.rootIter));
					return false;
				}
				parse_impl<derived_type, value_type>::template impl<optionsReal>(std::forward<value_type>(object), readIters.first, readIters.second);
				if constexpr (!options.minified) {
					if (readIters.first != readIters.second) {
						static constexpr auto sourceLocation{ std::source_location::current() };
						getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(readIters.first - optionsReal.rootIter,
							readIters.second - optionsReal.rootIter, optionsReal.rootIter));
						return false;
					}
				}
			}
			return true;
		}

		template<typename value_type, jsonifier::parse_options options = jsonifier::parse_options{}, jsonifier::concepts::string_t buffer_type>
		JSONIFIER_INLINE value_type parseJson(buffer_type&& in) {
			static_assert(jsonifier::concepts::printErrorFunction<jsonifier::concepts::unwrap_t<value_type>>(),
				"No specialization of core exists for the type named above - please specialize it!");
			derivedRef.errors.clear();
			static constexpr parse_options_internal<derived_type> optionsReal{ .optionsReal = options };
			optionsReal.parserPtr = this;
			optionsReal.rootIter  = reinterpret_cast<string_view_ptr>(in.data());
			jsonifier::concepts::unwrap_t<value_type> object{};
			if constexpr (!options.minified) {
				derivedRef.section.reset(in.data(), in.size());
				json_structural_iterator iter{ derivedRef.section.begin(), derivedRef.section.end(), derivedRef.errors };
				json_structural_iterator end{ derivedRef.section.end(), derivedRef.section.end(), derivedRef.errors };
				if (!iter || (*iter != '{' && *iter != '[')) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					getErrors().emplace_back(
						createError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(iter - optionsReal.rootIter, end - optionsReal.rootIter, optionsReal.rootIter));
					return object;
				}

				parse_impl<derived_type, value_type>::template impl<optionsReal>(std::forward<value_type>(object), iter, end);
				if constexpr (!options.minified) {
					if (readIters.first != readIters.second) {
						static constexpr auto sourceLocation{ std::source_location::current() };
						getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(iter - optionsReal.rootIter,
							end - optionsReal.rootIter, optionsReal.rootIter));
						return object;
					}
				}
			} else {
				auto readIters = collectIterPair(reinterpret_cast<string_view_ptr>(in.data()), reinterpret_cast<string_view_ptr>(in.data() + in.size()));
				if (!readIters.first || (*readIters.first != '{' && *readIters.first != '[')) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(readIters.first - optionsReal.rootIter,
						readIters.second - optionsReal.rootIter, optionsReal.rootIter));
					return object;
				}
				parse_impl<derived_type, value_type>::template impl<optionsReal>(std::forward<value_type>(object), readIters.first, readIters.second);
				if constexpr (!options.minified) {
					if (readIters.first != readIters.second) {
						static constexpr auto sourceLocation{ std::source_location::current() };
						getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Parsing, parse_errors::No_Input>(readIters.first - optionsReal.rootIter,
							readIters.second - optionsReal.rootIter, optionsReal.rootIter));
						return object;
					}
				}
			}
			return object;
		}

	  protected:
		derived_type& derivedRef{ initializeSelfRef() };
		std::pair<string_view_ptr, string_view_ptr> readIters{};

		JSONIFIER_INLINE parser() noexcept : derivedRef{ initializeSelfRef() } {};

		JSONIFIER_INLINE derived_type& initializeSelfRef() {
			return *static_cast<derived_type*>(this);
		}

		JSONIFIER_INLINE jsonifier::vector<error>& getErrors() {
			return derivedRef.errors;
		}

		JSONIFIER_INLINE ~parser() noexcept = default;
	};
};