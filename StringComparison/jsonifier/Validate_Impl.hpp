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

#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Validator.hpp"

namespace jsonifier_internal {

	template<typename iterator_type> JSONIFIER_INLINE uint64_t getRemainingLength(iterator_type&& iter, iterator_type&& end) {
		return end - iter;
	}

	template<typename derived_type> struct validate_impl<json_structural_type::Object_Start, derived_type> {
		template<const validate_options_internal<derived_type>& options, typename iterator_type> static bool impl(iterator_type&& iter, iterator_type&& end, uint64_t& depth) {
			if (!iter || *iter != '{') {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Object_Start>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}
			++depth;
			++iter;
			if (*iter == '}') {
				++iter;
				--depth;
				return true;
			}

			while (iter != end) {
				if (!validate_impl<json_structural_type::String, derived_type>::template impl<options>(iter, end)) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_String_Characters>(
						iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				}

				if (*iter != ':') {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Colon>(
						iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				}

				++iter;
				if (!validator<derived_type>::template impl<options>(iter, end, depth)) {
					return false;
				}

				if (*iter == ',') {
					++iter;
				} else if (*iter == '}') {
					++iter;
					if (iter < end && *iter != ',' && *iter != ']' && *iter != '}') {
						static constexpr auto sourceLocation{ std::source_location::current() };
						options.validatorPtr->getErrors().emplace_back(
							error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Comma_Or_Closing_Brace>(iter - iter.getRootPtr(),
								iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
						return false;
					}
					--depth;
					return true;
				} else {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(
						error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Comma_Or_Closing_Brace>(iter - iter.getRootPtr(),
							iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				}
			}
			if (*iter != ',' && *iter != '}') {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Comma_Or_Closing_Brace>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}
			--depth;
			return true;
		}
	};

	template<typename derived_type> struct validate_impl<json_structural_type::Array_Start, derived_type> {
		template<const validate_options_internal<derived_type>& options, typename iterator_type> static bool impl(iterator_type&& iter, iterator_type&& end, uint64_t& depth) {
			if (!iter || *iter != '[') {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Array_Start>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}
			++depth;
			++iter;

			if (iter.operator bool() && *iter == ']') {
				++iter;
				--depth;
				return true;
			}

			while (iter != end) {
				if (!validator<derived_type>::template impl<options>(iter, end, depth)) {
					return false;
				}
				if (*iter == ',') {
					++iter;
				} else if (*iter == ']') {
					++iter;
					if (iter < end && *iter != ',' && *iter != ']' && *iter != '}') {
						static constexpr auto sourceLocation{ std::source_location::current() };
						options.validatorPtr->getErrors().emplace_back(
							error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Comma_Or_Closing_Brace>(iter - iter.getRootPtr(),
								iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
						return false;
					}
					--depth;
					return true;
				} else {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(
						error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Comma_Or_Closing_Brace>(iter - iter.getRootPtr(),
							iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				}
			}
			if (*iter != ',' && *iter != ']') {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Missing_Comma_Or_Closing_Brace>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}
			--depth;
			return true;
		}
	};

	constexpr std::array<bool, 256> hexDigits{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true,
		true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

	template<typename derived_type> struct validate_impl<json_structural_type::String, derived_type> {
		template<const validate_options_internal<derived_type>& options, typename iterator_type> static bool impl(iterator_type&& iter, iterator_type&& end) {
			auto newPtr = iter.operator->();
			++iter;
			auto endPtr = iter.operator->();
			skipWs(newPtr);
			if (newPtr == endPtr || *newPtr != '"') {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_String_Characters>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}
			++newPtr;
			while (newPtr != endPtr && *newPtr != '"') {
				if (*newPtr == '\\') {
					++newPtr;

					if (*newPtr == '"' || *newPtr == '\\' || *newPtr == '/' || *newPtr == 'b' || *newPtr == 'f' || *newPtr == 'n' || *newPtr == 'r' || *newPtr == 't') {
						++newPtr;
					} else if (*newPtr == 'u') {
						++newPtr;
						for (int32_t i = 0; i < 4; ++i) {
							if (!hexDigits[*newPtr]) {
								static constexpr auto sourceLocation{ std::source_location::current() };
								options.validatorPtr->getErrors().emplace_back(
									error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Escape_Characters>(iter - iter.getRootPtr(),
										iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
								return false;
							}
							++newPtr;
						}
					} else {
						static constexpr auto sourceLocation{ std::source_location::current() };
						options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Escape_Characters>(
							iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
						return false;
					}
				} else if (*newPtr < 0x20u) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_String_Characters>(
						iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				} else {
					++newPtr;
				}
			}

			if (*newPtr != '"') {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_String_Characters>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}

			return iter.operator bool();
		}
	};

	template<typename derived_type> struct validate_impl<json_structural_type::Number, derived_type> {
		template<const validate_options_internal<derived_type>& options, typename iterator_type> static bool impl(iterator_type&& iter, iterator_type&& end) {
			auto newPtr = iter.operator->();
			++iter;
			auto endPtr = iter.operator->();
			skipWs(newPtr);
			auto newSize = endPtr - newPtr;
			if (newSize > 1 && *newPtr == '0' && numberTable[*(newPtr + 1)]) {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Number_Value>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}

			skipWs(newPtr);

			auto consumeChar = [&](char expected) {
				if (*newPtr == expected) {
					++newPtr;
					return true;
				}
				return false;
			};

			auto consumeDigits = [&](uint64_t min_count = 1) {
				uint64_t count = 0;
				while (digitTableBool[static_cast<uint64_t>(*newPtr)]) {
					++newPtr;
					++count;
				}
				return count >= min_count;
			};

			auto consumeSign = [&] {
				if (*newPtr == '-' || *newPtr == '+') {
					++newPtr;
					return true;
				}
				return false;
			};

			consumeSign();

			consumeDigits(1);

			if (consumeChar(0x2Eu)) {
				if (!consumeDigits(1)) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Number_Value>(
						iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				}
			}
			if (consumeChar(0x65u) || consumeChar(0x45u)) {
				bool didWeFail{ false };
				consumeSign();
				didWeFail = !consumeDigits(1);
				if (didWeFail) {
					static constexpr auto sourceLocation{ std::source_location::current() };
					options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Number_Value>(
						iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
					return false;
				}
			}
			skipWs(newPtr);
			if (newPtr != endPtr) {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Number_Value>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}

			return true;
		}
	};

	template<typename derived_type> struct validate_impl<json_structural_type::Bool, derived_type> {
		template<const validate_options_internal<derived_type>& options, typename iterator_type> static bool impl(iterator_type&& iter, iterator_type&& end) {
			auto newPtr = iter.operator->();
			++iter;
			static constexpr char falseStr[]{ "false" };
			static constexpr char trueStr[]{ "true" };
			skipWs(newPtr);
			if (getRemainingLength(iter, end) >= 4 && std ::memcmp(newPtr, trueStr, std::strlen(trueStr)) == 0) {
				newPtr += std::strlen(trueStr) - 1;
			} else if (getRemainingLength(iter, end) >= 5 && std::memcmp(newPtr, falseStr, std::strlen(falseStr)) == 0) {
				newPtr += std::strlen(falseStr) - 1;
			} else {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Bool_Value>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}
			return iter.operator bool();
		}
	};

	template<typename derived_type> struct validate_impl<json_structural_type::Null, derived_type> {
		template<const validate_options_internal<derived_type>& options, typename iterator_type> static bool impl(iterator_type&& iter, iterator_type&& end) {
			auto newPtr = iter.operator->();
			++iter;
			skipWs(newPtr);
			static constexpr char nullStr[]{ "null" };

			if (std::memcmp(newPtr, nullStr, std::strlen(nullStr)) == 0) {
				newPtr += std::size(nullStr) - 1;
			} else {
				static constexpr auto sourceLocation{ std::source_location::current() };
				options.validatorPtr->getErrors().emplace_back(error::constructError<sourceLocation, error_classes::Validating, validate_errors::Invalid_Null_Value>(
					iter - iter.getRootPtr(), iter.getEndPtr() - iter.getRootPtr(), iter.getRootPtr()));
				return false;
			}

			return iter.operator bool();
		}
	};

}