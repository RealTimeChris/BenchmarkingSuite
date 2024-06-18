// BenchmarkSuite.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "../StringComparison/jsonifier/Index.hpp"
//#include "C:/users/chris/source/repos/benchmarkingsuite/stringcomparison/jsonifier/Index.hpp"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

namespace bnch_swt {

	namespace fs = std::filesystem;

	class file_loader {
	  public:
		file_loader(const std::string& filePathNew) {
			filePath = filePathNew;
			std::string directory{ filePathNew.substr(0, filePathNew.find_last_of("/") + 1) };
			if (!fs::exists(directory.operator std::basic_string_view<char, std::char_traits<char>>())) {
				std::filesystem::create_directories(directory.operator std::basic_string_view<char, std::char_traits<char>>());
			}

			if (!fs::exists(filePath.operator std::basic_string_view<char, std::char_traits<char>>())) {
				std::ofstream createFile(filePath.data());
				createFile.close();
			}

			std::ifstream theStream(filePath.data(), std::ios::binary | std::ios::in);
			std::stringstream inputStream{};
			inputStream << theStream.rdbuf();
			fileContents = inputStream.str();
			theStream.close();
		}

		void saveFile(const std::string& fileToSave) {
			std::ofstream theStream(filePath.data(), std::ios::binary | std::ios::out | std::ios::trunc);
			theStream.write(fileToSave.data(), fileToSave.size());
			theStream.close();
		}

		operator std::string&() {
			return fileContents;
		}

	  protected:
		std::string fileContents{};
		std::string filePath{};
	};

	inline thread_local jsonifier::jsonifier_core parser{};

	template<uint64_t sizeVal> struct string_literal {
		static constexpr uint64_t length{ sizeVal > 0 ? sizeVal - 1 : 0 };

		constexpr string_literal(const char (&str)[sizeVal]) {
			std::copy(str, str + sizeVal, values);
		}

		constexpr const char* data() const {
			return values;
		}

		constexpr auto size() const {
			return length;
		}

		constexpr operator jsonifier::string_view() const {
			return { values, sizeVal };
		}

		char values[sizeVal];
	};

	static void const volatile* volatile globalForceEscapePointer;

	void useCharPointer(char const volatile* const v) {
		globalForceEscapePointer = reinterpret_cast<void const volatile*>(v);
	}

	template<typename value_type> using unwrap_t = jsonifier::concepts::unwrap_t<value_type>;

	template<typename function_type, typename... arg_types> struct return_type_helper {
		using type = std::invoke_result_t<function_type, arg_types...>;
	};

	template<typename value_type, typename... arg_types>
	concept invocable = std::is_invocable_v<unwrap_t<value_type>, arg_types...>;

	template<typename value_type, typename... arg_types>
	concept not_invocable = !std::is_invocable_v<unwrap_t<value_type>, arg_types...>;

	template<typename value_type, typename... arg_types>
	concept invocable_void = invocable<value_type, arg_types...> && std::is_void_v<typename return_type_helper<value_type, arg_types...>::type>;

	template<typename value_type, typename... arg_types>
	concept invocable_not_void = invocable<value_type, arg_types...> && !std::is_void_v<typename return_type_helper<value_type, arg_types...>::type>;

#if defined(JSONIFIER_MSVC)
	#define doNotOptimize(value) \
		useCharPointer(&reinterpret_cast<char const volatile&>(value)); \
		_ReadWriteBarrier();
#elif defined(JSONIFIER_CLANG)
	#define doNotOptimize(value) asm volatile("" : "+r,m"(value) : : "memory");
#else
	#define doNotOptimize(value) asm volatile("" : "+m,r"(value) : : "memory");
#endif

	template<not_invocable value_type> JSONIFIER_INLINE void doNotOptimizeAway(value_type&& value) {
		const auto* valuePtr = &value;
		doNotOptimize(valuePtr)
	}

	template<invocable_void function_type, typename... arg_types> JSONIFIER_INLINE void doNotOptimizeAway(function_type&& value, arg_types&&... args) {
		std::forward<function_type>(value)(std::forward<arg_types>(args)...);
		doNotOptimize(value);
	}

	template<invocable_not_void function_type, typename... arg_types> JSONIFIER_INLINE void doNotOptimizeAway(function_type&& value, arg_types&&... args) {
		auto resultVal = std::forward<function_type>(value)(std::forward<arg_types>(args)...);
		doNotOptimize(resultVal);
	}

	JSONIFIER_INLINE bool checkDoubleForValidLt(double valueToCheck, double valueToCheckAgainst) {
		return (valueToCheck != std::numeric_limits<double>::infinity() && valueToCheck != std::numeric_limits<double>::quiet_NaN() &&
				   valueToCheck != -std::numeric_limits<double>::infinity() && valueToCheck != -std::numeric_limits<double>::quiet_NaN()) &&
			valueToCheck < valueToCheckAgainst;
	}

	JSONIFIER_INLINE double calcMedian(jsonifier::vector<double>& data) {
		std::sort(data.begin(), data.end());
		auto midIdx = data.size() / 2U;
		if (1U == (data.size() & 1U)) {
			return data[midIdx];
		}
		return (data[midIdx - 1U] + data[midIdx]) / 2U;
	}

	JSONIFIER_INLINE double medianAbsolutePercentError(const jsonifier::vector<double>& data) {
		jsonifier::vector<double> dataNew{ data };
		if (dataNew.empty()) {
			return 0.0;
		}
		auto med = calcMedian(dataNew);
		for (double& x: dataNew) {
			x = (x - med) / x;
			if (x < 0.0f) {
				x = -x;
			}
		}
		return calcMedian(dataNew);
	}

	struct benchmark_results {
		double medianAbsolutePercentageError{};
		uint64_t currentIterationCount{};
		double resultTime{};
	};

	template<uint64_t maxIterationCount, uint64_t minIterationCount, typename function_type>
	inline benchmark_results collectMape(function_type&& lambda, uint64_t totalIterationCount = 0) {
		double medianAbsolutePercentageError{};
		jsonifier::vector<double> durations{};
		jsonifier::concepts::unwrap_t<function_type> newFunction{ lambda };
		jsonifier::vector<jsonifier::concepts::unwrap_t<function_type>> lambdasNew{};

		for (uint64_t x = 0; x < maxIterationCount; ++x) {
			lambdasNew.emplace_back(newFunction);
		}

		uint64_t currentIterationCount{};

		while (currentIterationCount < maxIterationCount) {
			auto startTime = std::chrono::high_resolution_clock::now();
			doNotOptimizeAway(lambdasNew[currentIterationCount]);
			auto endTime = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(endTime - startTime).count();
			durations.emplace_back(duration);

			if (currentIterationCount > minIterationCount) {
				medianAbsolutePercentageError = medianAbsolutePercentError(durations);
			}

			if (currentIterationCount > minIterationCount && checkDoubleForValidLt(medianAbsolutePercentageError, 0.05)) {
				totalIterationCount += currentIterationCount;
				return { medianAbsolutePercentageError, totalIterationCount, durations[currentIterationCount - 1] };
			}

			++currentIterationCount;
		}
		return { medianAbsolutePercentageError, totalIterationCount, durations[currentIterationCount - 1] };
	}

	template<uint64_t iterationCount, typename function_type> inline benchmark_results benchmark(function_type&& function) {
		static constexpr int64_t warmupCount	   = iterationCount;
		static constexpr int64_t minIterationCount = static_cast<int64_t>(static_cast<float>(iterationCount) * 0.10f);
		using function_type_final				   = std::decay_t<function_type>;
		function_type_final functionNew{ std::forward<function_type>(function) };

		jsonifier::vector<function_type_final> warmupLambdas{};
		for (uint64_t x = 0; x < warmupCount; ++x) {
			warmupLambdas.emplace_back(functionNew);
		}

		jsonifier::vector<function_type_final> lambdas{};
		for (uint64_t x = 0; x < iterationCount; ++x) {
			lambdas.emplace_back(functionNew);
		}

		int64_t currentIterationCount = 0;
		jsonifier::vector<double> durations{};

		while (currentIterationCount < warmupCount) {
			auto startTime = std::chrono::high_resolution_clock::now();
			doNotOptimizeAway(std::move(warmupLambdas[currentIterationCount]));
			auto endTime = std::chrono::high_resolution_clock::now();
			++currentIterationCount;
		}

		currentIterationCount = 0;
		return collectMape<iterationCount, minIterationCount>(functionNew);
	}

	template<typename return_type> struct benchmark_result {
		return_type returnValue{};
		uint64_t resultSize{};
		double resultTime{};
		inline bool operator<(const benchmark_result& other) const {
			return resultTime < other.resultTime;
		}
		inline benchmark_result operator+(const benchmark_result& other) const {
			return benchmark_result{ .returnValue = returnValue, .resultTime = resultTime + other.resultTime };
		}

		inline benchmark_result operator/(const uint64_t& other) const {
			return benchmark_result{ .returnValue = returnValue, .resultTime = resultTime / other };
		}
	};

	template<> struct benchmark_result<void> {
		uint64_t resultSize{};
		double resultTime{};
		inline bool operator<(const benchmark_result& other) const {
			return resultTime < other.resultTime;
		}
		inline benchmark_result operator+(const benchmark_result& other) const {
			return benchmark_result{ .resultTime = resultTime + other.resultTime };
		}

		inline benchmark_result operator/(const auto& other) const {
			return benchmark_result{ .resultTime = resultTime / other };
		}
	};

	struct benchmark_result_final {
		jsonifier::string_view benchmarkColor{};
		jsonifier::string_view benchmarkName{};
		double medianAbsolutePercentageError{};
		uint64_t iterationCount{};
		double resultTime{};

		inline benchmark_result_final() noexcept = default;

		inline benchmark_result_final& operator=(benchmark_result<void>&& other) {
			resultTime = other.resultTime;
			return *this;
		}

		inline benchmark_result_final(benchmark_result<void>&& other) {
			*this = std::move(other);
		}

		inline benchmark_result_final& operator=(const benchmark_result<void>& other) {
			resultTime = other.resultTime;
			return *this;
		}

		inline benchmark_result_final(const benchmark_result<void>& other) {
			*this = other;
		}

		template<typename value_type> inline benchmark_result_final& operator=(benchmark_result<value_type>&& other) {
			resultTime = other.resultTime;
			return *this;
		}

		template<typename value_type> inline benchmark_result_final(benchmark_result<value_type>&& other) {
			*this = std::move(other);
		}

		template<typename value_type> inline benchmark_result_final& operator=(const benchmark_result<value_type>& other) {
			resultTime = other.resultTime;
			return *this;
		}

		template<typename value_type> inline benchmark_result_final(const benchmark_result<value_type>& other) {
			*this = other;
		}

		inline bool operator<(const benchmark_result_final& other) const {
			return resultTime < other.resultTime;
		}

		inline benchmark_result_final operator+(const benchmark_result_final& other) const {
			benchmark_result_final returnValues{};
			returnValues.medianAbsolutePercentageError = other.medianAbsolutePercentageError;
			returnValues.benchmarkColor = other.benchmarkColor;
			returnValues.benchmarkName	= other.benchmarkName;
			returnValues.resultTime		= resultTime + other.resultTime;
			return returnValues;
		}

		inline benchmark_result_final operator/(const uint64_t& other) const {
			benchmark_result_final returnValues{};
			returnValues.medianAbsolutePercentageError = medianAbsolutePercentageError;
			returnValues.benchmarkColor = benchmarkColor;
			returnValues.benchmarkName	= benchmarkName;
			returnValues.resultTime		= resultTime / other;
			return returnValues;
		}
	};

	template<string_literal str> struct benchmark_suite_results_stored {
		constexpr benchmark_suite_results_stored() noexcept = default;
		mutable std::array<benchmark_result_final, 256> results{};
		mutable jsonifier::string_view benchmarkSuiteName{ str.operator jsonifier::string_view() };
		mutable uint64_t currentCount{};
	};

	struct benchmark_suite_results {
		inline benchmark_suite_results() noexcept = default;
		inline benchmark_suite_results(auto& values) {
			for (uint64_t x = 0; x < values.currentCount; ++x) {
				results.emplace_back(values.results[x]);
			}
			benchmarkSuiteName = values.benchmarkSuiteName;
		}
		jsonifier::vector<benchmark_result_final> results{};
		jsonifier::string_view benchmarkSuiteName{};
	};

	template<typename value_type> std::ostream& operator<<(std::ostream& os, const jsonifier::vector<value_type>& vector) {
		os << '[';
		for (uint64_t x = 0; x < vector.size(); ++x) {
			os << vector[x];
			if (x < vector.size() - 1) {
				os << ',';
			}
		}
		os << ']' << std::endl;
		return os;
	}

	template<string_literal benchmarkSuite> struct benchmark_suite {
		constexpr benchmark_suite() noexcept {};
		static constexpr benchmark_suite_results_stored<benchmarkSuite> results{};
		inline static void printResults() {
			benchmark_suite_results newValues{ results };
			for (auto& value: newValues.results) {
				std::cout << "Benchmark Name: " << value.benchmarkName << ", MAPE: " << value.medianAbsolutePercentageError << ", Result Time: " << value.resultTime << std::endl;
				//auto stringToWrite = parser.serializeJson(value);
				//std::cout << "STRING TO WRITE: " << stringToWrite << std::endl;
			}
			//auto stringToWrite = parser.serializeJson(newValues);
			//file_loader fileLoader{ filePath };
			//fileLoader.saveFile(static_cast<std::string>(stringToWrite));
			//std::cout << "STRING TO WRITE: " << stringToWrite << std::endl;
			return ;
		}
		inline static std::string writeJsonData(const std::string& filePath) {
			benchmark_suite_results newValues{ results };
			for (auto& value: newValues.results) {
				//auto stringToWrite = parser.serializeJson(value);
				//std::cout << "STRING TO WRITE: " << stringToWrite << std::endl;
			}
			//auto stringToWrite = parser.serializeJson(newValues);
			//file_loader fileLoader{ filePath };
			//fileLoader.saveFile(static_cast<std::string>(stringToWrite));
			//std::cout << "STRING TO WRITE: " << stringToWrite << std::endl;
			return {};
		}

		static bool checkDoubleForValidLt(double valueToCheck, double valueToCheckAgainst) {
			return (valueToCheck != std::numeric_limits<double>::infinity() && valueToCheck != std::numeric_limits<double>::quiet_NaN() &&
					   valueToCheck != -std::numeric_limits<double>::infinity() && valueToCheck != -std::numeric_limits<double>::quiet_NaN()) &&
				valueToCheck < valueToCheckAgainst;
		}

		static double calcMedian(jsonifier::vector<double>& data) {
			std::sort(data.begin(), data.end());
			auto midIdx = data.size() / 2U;
			if (1U == (data.size() & 1U)) {
				return data[midIdx];
			}
			return (data[midIdx - 1U] + data[midIdx]) / 2U;
		}

		static double medianAbsolutePercentError(const jsonifier::vector<double>& data) {
			jsonifier::vector<double> dataNew{ data };
			if (dataNew.empty()) {
				return 0.0;
			}
			auto med = calcMedian(dataNew);
			for (double& x: dataNew) {
				x = (x - med) / x;
				if (x < 0.0f) {
					x = -x;
				}
			}
			return calcMedian(dataNew);
		}

		static inline bool checkDoubleValueForLt(double valueToCheck, double valueToCheckAgainst) {
			return (valueToCheck != std::numeric_limits<double>::infinity() && valueToCheck != std::numeric_limits<double>::quiet_NaN() &&
					   valueToCheck != -std::numeric_limits<double>::infinity() && valueToCheck != -std::numeric_limits<double>::quiet_NaN()) &&
				valueToCheck < valueToCheckAgainst;
		}

		template<string_literal benchmarkName, string_literal benchmarkColor, int64_t maxIterationCount, invocable_void function_type, typename... arg_types>
		static inline auto benchmark(function_type&& function, arg_types&&... args) {
			static constexpr int64_t warmupCount	   = maxIterationCount;
			static constexpr int64_t minIterationCount =
				static_cast<int64_t>(static_cast<float>(maxIterationCount) * 0.10f) > 0 ? static_cast<int64_t>(static_cast<float>(maxIterationCount) * 0.10f) : 1;
			using function_type_final				   = jsonifier::concepts::unwrap_t<function_type>;
			auto functionNew						   = [=] {
				  return function(args...);
			};

			jsonifier::vector<decltype(functionNew)> warmupLambdas{};
			for (uint64_t x = 0; x < warmupCount; ++x) {
				warmupLambdas.emplace_back(functionNew);
			}

			jsonifier::vector<decltype(functionNew)> lambdas{};
			for (uint64_t x = 0; x < maxIterationCount; ++x) {
				lambdas.emplace_back(functionNew);
			}

			benchmark_results results = collectMape<maxIterationCount, minIterationCount>(lambdas[0]);

			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].medianAbsolutePercentageError =
				results.medianAbsolutePercentageError;
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].resultTime	  = results.resultTime;
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].benchmarkName = benchmarkName.operator jsonifier::string_view();
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].benchmarkColor =
				benchmarkColor.operator jsonifier::string_view();
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].iterationCount = results.currentIterationCount;
			++benchmark_suite<benchmarkSuite>::results.currentCount;
			return results.resultTime;
		}

		template<string_literal benchmarkName, string_literal benchmarkColor, int64_t maxIterationCount, invocable_not_void function_type, typename... arg_types>
		static inline auto benchmark(function_type&& function, arg_types&&... args) {
			static constexpr int64_t warmupCount	   = maxIterationCount;
			static constexpr int64_t minIterationCount =
				static_cast<int64_t>(static_cast<float>(maxIterationCount) * 0.10f) > 0 ? static_cast<int64_t>(static_cast<float>(maxIterationCount) * 0.10f) : 1;
			using function_type_final				   = jsonifier::concepts::unwrap_t<function_type>;
			auto functionNew						   = [=] {
				  return function(args...);
			};

			jsonifier::vector<decltype(functionNew)> warmupLambdas{};
			for (uint64_t x = 0; x < warmupCount; ++x) {
				warmupLambdas.emplace_back(functionNew);
			}

			jsonifier::vector<decltype(functionNew)> lambdas{};
			for (uint64_t x = 0; x < maxIterationCount; ++x) {
				lambdas.emplace_back(functionNew);
			}

			benchmark_results results = collectMape<maxIterationCount, minIterationCount>(lambdas[0]);

			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].medianAbsolutePercentageError =
				results.medianAbsolutePercentageError;
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].resultTime	  = results.resultTime;
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].benchmarkName = benchmarkName.operator jsonifier::string_view();
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].benchmarkColor =
				benchmarkColor.operator jsonifier::string_view();
			benchmark_suite<benchmarkSuite>::results.results[benchmark_suite<benchmarkSuite>::results.currentCount].iterationCount = results.currentIterationCount;
			++benchmark_suite<benchmarkSuite>::results.currentCount;
			return results.resultTime;
		}
	};

}

namespace jsonifier {

	template<> struct core<bnch_swt::benchmark_result_final> {
		using value_type = bnch_swt::benchmark_result_final;
		static constexpr auto parseValue = createValue<&value_type::benchmarkName, &value_type::medianAbsolutePercentageError, &value_type::resultTime, &value_type::benchmarkColor,
			&value_type::iterationCount>();
	};

	template<> struct core<bnch_swt::benchmark_suite_results> {
		using value_type				 = bnch_swt::benchmark_suite_results;
		static constexpr auto parseValue = createValue<&value_type::results, &value_type::benchmarkSuiteName>();
	};
}