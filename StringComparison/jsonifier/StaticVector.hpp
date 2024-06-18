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

#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Iterator.hpp"
#include <iterator>

namespace jsonifier_internal {

	template<typename value_type_new, uint64_t rows> class static_vector {
	  public:
		using value_type	  = value_type_new;
		using reference		  = value_type&;
		using pointer		  = value_type*;
		using const_pointer	  = const value_type*;
		using size_type		  = uint64_t;
		using const_reference = const value_type&;
		using iterator		  = pointer;
		using const_iterator  = const_pointer;

		template<typename value_type_newer, size_type rowsNew> friend class static_vector;

		constexpr static_vector& operator=(static_vector&& other) = default;
		constexpr static_vector(static_vector&& other)			  = default;

		constexpr static_vector& operator=(const static_vector& other) = default;
		constexpr static_vector(const static_vector& other)			   = default;

		constexpr static_vector() = default;

		constexpr static_vector& operator=(std::vector<value_type>&& values) noexcept {
			if (rows < values.size()) {
				throw std::out_of_range{ "Sorry, but there is not the correct number of rows in the vector!" };
			}

			sizeVal = values.size();
			std::move(values.data(), values.data() + sizeVal, dataVal);
			return *this;
		}

		constexpr static_vector(std::vector<value_type>&& values) noexcept {
			*this = values;
		}

		constexpr static_vector& operator=(const std::vector<value_type>& values) {
			if (rows < values.size()) {
				throw std::out_of_range{ "Sorry, but there is not the correct number of rows in the vector!" };
			}

			sizeVal = values.size();
			std::copy(values.data(), values.data() + sizeVal, dataVal);
			return *this;
		}

		constexpr static_vector(const std::vector<value_type>& values) {
			*this = values;
		}

		template<typename... value_type_newer> constexpr reference emplace_back(value_type_newer&&... newValue) {
			std::construct_at(&dataVal[sizeVal], std::forward<value_type_newer>(newValue)...);
			++sizeVal;
			return dataVal[sizeVal - 1];
		}

		constexpr void pop_back() {
			if (sizeVal > 0) {
				--sizeVal;
				std::destroy_at(&dataVal[sizeVal]);
			} else {
				throw std::out_of_range{ "Vector is empty, cannot pop_back!" };
			}
		}

		constexpr iterator erase(iterator pos) {
			if (pos >= data() && pos < data() + sizeVal) {
				std::copy(pos + 1, data() + sizeVal, pos);
				--sizeVal;
			}
			return pos;
		}

		constexpr iterator begin() noexcept {
			return iterator{ dataVal };
		}

		constexpr iterator end() noexcept {
			return iterator{ dataVal + sizeVal };
		}

		constexpr const_reference start() const noexcept {
			return dataVal;
		}

		constexpr const_reference back() const noexcept {
			return *(dataVal + sizeVal - 1);
		}

		constexpr reference start() noexcept {
			return dataVal;
		}

		constexpr reference back() noexcept {
			return *(dataVal + sizeVal - 1);
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator{ dataVal };
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator{ dataVal + sizeVal };
		}

		constexpr const_pointer data() const {
			return static_cast<const_pointer>(dataVal);
		}

		constexpr pointer data() {
			return static_cast<pointer>(dataVal);
		}

		constexpr size_type size() const {
			return sizeVal;
		}

		constexpr void clear() {
			sizeVal = 0;
		}

		constexpr size_type capacity() const {
			return rows;
		}

		constexpr bool empty() const {
			return sizeVal == 0;
		}

		constexpr void resize(size_type newSize) {
			if (rows < newSize) {
				throw std::out_of_range{ "Sorry, but there is not the correct number of rows in this vector!" };
			}
			sizeVal = newSize;
		}

		constexpr const_reference operator[](size_type index) const {
			return dataVal[index];
		}

		constexpr reference operator[](size_type index) {
			return dataVal[index];
		}

		constexpr void print() const {
			for (size_type x = 0; x < rows; ++x) {
				std::cout << dataVal[x] << 0x20;
				std::cout << '\n';
			}
		}

		value_type dataVal[rows]{};
		size_type sizeVal{};
	};


}
