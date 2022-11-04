#pragma once
#include "CommonInterface.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <cassert>
#include <limits>
#include <functional>

namespace KHAS {

    template <typename TString>
    inline void CommonInterface::push(TString&& str) {
        out_buffer_ << std::forward<TString>(str);
        out_buffer_ << '\n';
    }

    template<typename ...TString>
    inline std::string CommonInterface::stringGeneration(char aggregate, TString&& ... str)
    {
        static_assert(sizeof...(str) > 0);
        std::initializer_list forward_str{ std::string(std::forward<TString>(str))... };

        std::size_t length_forward_str{ std::accumulate(
            forward_str.begin()
            , forward_str.end()
            , std::size_t{}
            , [](auto lhs, auto&& rhs) {
                return lhs + rhs.size();
            }) };

        assert(length_forward_str > 0);

        const std::size_t numerator{ table_width_ - length_forward_str };
        const std::size_t denominator{ sizeof...(str) + 1 };
        const std::size_t delimiter_size{ numerator / denominator + 1 };
        auto tmp{ std::string(delimiter_size, aggregate) };

        std::string result{ std::accumulate(
            forward_str.begin()
            , forward_str.end()
            , tmp
            , [&tmp](auto&& lhs, auto&& rhs) {
                auto result{ lhs + rhs + tmp };
                return result;
            }) };
        assert(result.size() <= table_width_ + denominator - numerator % denominator);
        result.resize(result.size() - denominator + numerator % denominator);
        result.shrink_to_fit();
        result.front() = result.back() = '#';
        return result;
    }

    template<typename TInput>
    inline std::pair<TInput, bool> CommonInterface::dataInput(ActionWithInputValue awiv) const noexcept
    {
        using T = std::decay_t<TInput>;

        T tmp;
        std::cin >> tmp;

        if (awiv == ActionWithInputValue::ErrorReturn) {

            bool is_good{ std::cin.good() };

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (is_good) return { tmp, true };
            return { T(), false};
        }

        while (awiv == ActionWithInputValue::LoopIsError && std::cin.fail()) {

            std::cout << "Повторите ввод!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> tmp;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return { tmp, true };        
    }

    template <typename TValue, typename TLower, typename THight>
    inline constexpr auto CommonInterface::clamp(TValue&& v, TLower&& lo, THight&& hi) {
        return clamp(std::forward<TValue>(v), std::forward<TLower>(lo), std::forward<THight>(hi), std::less{});
    }

    template <typename TValue, typename TLower, typename THight, typename Compare>
    inline constexpr auto CommonInterface::clamp(TValue&& v, TLower&& lo, THight&& hi, Compare comp) {
        auto value{ std::forward<TValue>(v) };
        auto lower{ std::forward<TLower>(lo) };
        auto hight{ std::forward<THight>(hi) };
        return comp(value, lower) ? lower : comp(hight, value) ? hight : value;
    }

    template <typename TValue, typename TLower, typename THight>
    inline constexpr bool CommonInterface::isClamp(TValue&& v, TLower&& lo, THight&& hi) const {
        auto value{ std::forward<TValue>(v) };
        auto lower{ std::forward<TLower>(lo) };
        auto hight{ std::forward<THight>(hi) };
        return value >= lower && value <= hight;
    }    
    
    
}