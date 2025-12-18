///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_FORMAT_INCLUDED
#define ETL_FORMAT_INCLUDED

#include "platform.h"

#include "algorithm.h"
#include "array.h"
#include "array_view.h"
#include "error_handler.h"
#include "limits.h"
#include "math.h"
#include "optional.h"
#include "span.h"
#include "string.h"
#include "string_view.h"
#include "type_traits.h"
#include "utility.h"
#include "variant.h"
#include "visitor.h"

#include <cmath>

#if ETL_USING_CPP11

namespace etl
{
  class format_exception : public etl::exception
  {
  public:

    format_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  class bad_format_string_exception : public etl::format_exception
  {
  public:

    bad_format_string_exception(string_type file_name_, numeric_type line_number_)
      : etl::format_exception(ETL_ERROR_TEXT("format:bad", ETL_FORMAT_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  template<class... Args>
  ETL_CONSTEXPR14 bool check_f(const char* fmt)
  {
    // to be implemented later
    //return fmt[0] == 0; // actual check

    (void)fmt;
    return true;
  }

  inline void please_note_this_is_error_message_1() noexcept {}

  template<class... Args>
  struct basic_format_string
  {
    inline ETL_CONSTEVAL basic_format_string(const char* fmt)
    : _sv(fmt)
    {
      bool format_string_ok = check_f(fmt);

      if (!format_string_ok)
      {
        //if (etl::is_constant_evaluated()) // compile time error path
        //{
        //  // calling a non-constexpr function in a consteval context to trigger a compile error
        //  please_note_this_is_error_message_1();
        //}
        //else // run time error path
        //{
          ETL_ASSERT_FAIL_AND_RETURN(ETL_ERROR(bad_format_string_exception));
        //}
      }
    }

    ETL_CONSTEXPR basic_format_string(const basic_format_string& other) = default;
    ETL_CONSTEXPR14 basic_format_string& operator=(const basic_format_string& other) = default;

    ETL_CONSTEXPR string_view get() const
    {
      return _sv;
    }

  private:
    string_view _sv;
  };

  template<class... Args>
  using format_string = basic_format_string<type_identity_t<Args>...>;

  // supported types to format
  using supported_format_types = etl::variant<
    etl::monostate,
    bool,
    char,
    int,
    unsigned int,
    long long int,
    unsigned long long int,
    float,
    double,
    long double,
    const char*,
    etl::string_view,
    const void*
    // basic_format_arg::handle,
  >;

  template<class CharT>
  class basic_format_parse_context
  {
  public:

    using iterator = string_view::const_iterator;
    using const_iterator = string_view::const_iterator;
    using char_type = CharT;

    basic_format_parse_context(etl::string_view fmt, size_t n_args = 0)
    : range(fmt), num_args(n_args), current(0), automatic_mode(false), manual_mode(false)
    {
    }

    basic_format_parse_context<CharT>& operator=(const basic_format_parse_context&) = delete;

    iterator begin() const noexcept
    {
      return range.begin();
    }

    iterator end() const noexcept
    {
      return range.end();
    }

    ETL_CONSTEXPR14 void advance_to(iterator pos)
    {
      range = etl::string_view(pos, range.end());
    }

    ETL_CONSTEXPR14 size_t next_arg_id()
    {
      // automatic number generation only allowed if not already in manual mode
      ETL_ASSERT(manual_mode == false, ETL_ERROR(bad_format_string_exception));
      automatic_mode = true;
      // TODO: compile time check
      ETL_ASSERT(current < num_args, ETL_ERROR(bad_format_string_exception)/* not enough arguments for generated index */);
      return current++;
    }

    ETL_CONSTEXPR14 void check_arg_id(size_t id)
    {
      // manual index specification only allowed if not already in automatic mode
      ETL_ASSERT(automatic_mode == false, ETL_ERROR(bad_format_string_exception));
      manual_mode = true;
      ETL_ASSERT(id < num_args, ETL_ERROR(bad_format_string_exception)/* index out of range */);
    }

  private:
    etl::string_view range;
    size_t num_args;
    size_t current;
    bool automatic_mode;
    bool manual_mode;

    template<class, class>
    friend struct formatter;
  };

  using format_parse_context = basic_format_parse_context<char>;

  template<class Context>
  class basic_format_arg
  {
  public:

    class handle
    {
    public:
      void format(etl::basic_format_parse_context<char>& /* parse_ctx */,
             Context& /*format_ctx*/)
      {
        //typename Context::template formatter_type<TD> f;
        //parse_ctx.advance_to(f.parse(parse_ctx));
        //format_ctx.advance_to(f.format(const_cast<TQ&>(static_cast<const TD&>(ref)), format_ctx));
      }

    private:
      const void* obj;
      typedef void(*function_type)(etl::basic_format_parse_context<char>&, Context&, const void*);
      function_type func;
    };

    basic_format_arg()
    {
    }

    basic_format_arg(const bool v)
    : data(v)
    {
    }

    basic_format_arg(const int v)
    : data(v)
    {
    }

    basic_format_arg(const unsigned int v)
    : data(v)
    {
    }

    basic_format_arg(const long long int v)
    : data(v)
    {
    }

    basic_format_arg(const unsigned long long int v)
    : data(v)
    {
    }

    basic_format_arg(const char* v)
    : data(v)
    {
    }

    basic_format_arg(char v)
    : data(v)
    {
    }

    basic_format_arg(const float v)
    : data(v)
    {
    }

    basic_format_arg(const double v)
    : data(v)
    {
    }

    basic_format_arg(const long double v)
    : data(v)
    {
    }

    basic_format_arg(const etl::string_view v)
    : data(v)
    {
    }

    basic_format_arg(const basic_format_arg& other): data(other.data)
    {
    }

    basic_format_arg(const void* v)
    : data(v)
    {
    }

    basic_format_arg& operator=(const basic_format_arg& other)
    {
      data = other.data;
      return *this;
    }

    explicit operator bool() const
    {
      return !etl::holds_alternative<etl::monostate>(data);
    }

    template<class R, class Visitor>
    R visit(Visitor&& vis)
    {
      return etl::visit(etl::forward<Visitor>(vis), data);
    }

  private:
    supported_format_types data;
  };

  template<class Context, class... Args>
  class format_arg_store
  {
  public:
    format_arg_store(Args&... args): _args{args...} {}

    basic_format_arg<Context> get(size_t i) const
    {
      return _args.get(i);
    }

    etl::array_view<basic_format_arg<Context>> get()
    {
      return _args;
    }

  private:
    etl::array<basic_format_arg<Context>, sizeof...(Args)> _args;
  };

  template<class Context>
  class basic_format_args
  {
  public:
    template<class... Args>
    basic_format_args(format_arg_store<Context, Args...>& store): _args(store.get())
    {
    }

    basic_format_args(const basic_format_args<Context>& other): _args(other._args)
    {
    }

    basic_format_args& operator=(const basic_format_args<Context>& other)
    {
      _args = other._args;
      return *this;
    }

    basic_format_arg<Context> get(size_t i) const
    {
      return _args[i];
    }

    // non-standard
    size_t size()
    {
      return _args.size();
    }

  private:
    etl::array_view<basic_format_arg<Context>> _args;
  };

  namespace private_format
  {
    using char_type = char;

    enum class spec_align_t
    {
      NONE, // default
      START,
      END,
      CENTER
    };

    enum class spec_sign_t
    {
      MINUS, // default
      PLUS,
      SPACE
    };

    struct format_spec_t
    {
      etl::optional<size_t> index{etl::nullopt_t()};
      spec_align_t align{spec_align_t::NONE}; // '<' / '>' / '^' / none (default)
      char_type fill{' '}; // fill character (' ' is default)
      spec_sign_t sign{spec_sign_t::MINUS}; // '+' / '-' (default) / ' '
      bool hash{false}; // #
      bool zero{false}; // 0
      etl::optional<size_t> width{etl::nullopt_t()}; // the arg index if width_nested_replacement == true
      bool width_nested_replacement{false}; // {}
      etl::optional<size_t> precision{etl::nullopt_t()}; // the arg index if precision_nested_replacement == true
      bool precision_nested_replacement{false}; // {}
      bool locale_specific{false}; // 'L'
      etl::optional<char> type{etl::nullopt_t()}; // literal 's', 'b', 'd', ...
    };
  }

  template<class OutputIt, class CharT>
  class basic_format_context
  {
  public:

    using iterator = OutputIt;
    using char_type = CharT;

    basic_format_context(const basic_format_context& other): _it(other._it), _format_args(other._format_args)
    {
    }

    basic_format_context(OutputIt it, basic_format_args<basic_format_context>& fmt_args): _it(it), _format_args(fmt_args)
    {
    }

    basic_format_context& operator= (const basic_format_context&) = delete;

    basic_format_arg<basic_format_context> arg(size_t id) const
    {
      return _format_args.get(id);
    }

    iterator out()
    {
      return _it;
    }

    void advance_to(iterator it)
    {
      _it = it;
    }

    private_format::format_spec_t format_spec;

  private:
    iterator _it;
    basic_format_args<basic_format_context>& _format_args;
  };

  template<class OutputIt>
  using format_context = basic_format_context<OutputIt, char>;

  template<class OutputIt>
  using format_args = basic_format_args<format_context<OutputIt>>;

  template<class OutputIt>
  using format_arg = basic_format_arg<format_context<OutputIt>>;

  template<class OutputIt, class Context = format_context<OutputIt>, class... Args>
  format_arg_store<Context, Args...> make_format_args(Args&... args)
  {
    return format_arg_store<Context, Args...>(args...);
  }

  namespace private_format
  {
    inline bool is_digit(const char c)
    {
      return c >= '0' && c <= '9';
    }

    inline void advance(format_parse_context& parse_ctx)
    {
      parse_ctx.advance_to(parse_ctx.begin() + 1);
    }

    inline etl::optional<size_t> parse_num(format_parse_context& parse_ctx)
    {
      etl::optional<size_t> result;
      auto fmt_it = parse_ctx.begin();
      while (fmt_it != parse_ctx.end())
      {
        const char c = *fmt_it;
        if (is_digit(c))
        {
          size_t old_value = result.value_or(0);
          size_t new_value = old_value * 10 + static_cast<size_t>(c - '0');
          if (new_value < old_value) {
            // Overflow detected
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
          result = new_value;
        }
        else
        {
          break;
        }
        ++fmt_it;
      }
      if (result.has_value())
      {
        parse_ctx.advance_to(fmt_it);
      }
      return result;
    }

    inline etl::optional<char> parse_any_of(format_parse_context& parse_ctx, etl::string_view chars)
    {
      etl::optional<char> result;
      auto fmt_it = parse_ctx.begin();
      if (fmt_it != parse_ctx.end())
      {
        const char c = *fmt_it;
        auto it = etl::find(chars.cbegin(), chars.cend(), c);
        if (it != chars.cend())
        {
          result = *it;
          ++fmt_it;
          parse_ctx.advance_to(fmt_it);
        }
      }
      return result;
    }

    inline bool parse_char(format_parse_context& parse_ctx, char c)
    {
      auto fmt_it = parse_ctx.begin();
      if (fmt_it != parse_ctx.end())
      {
        char value = *fmt_it;
        if (value == c) {
          ++fmt_it;
          parse_ctx.advance_to(fmt_it);
          return true;
        }
      }
      return false;
    }

    inline bool parse_sequence(format_parse_context& parse_ctx, etl::string_view sequence)
    {
      auto fmt_it = parse_ctx.begin();
      if (etl::equal(sequence.cbegin(), sequence.cend(), fmt_it))
      {
        fmt_it += sequence.size();
        parse_ctx.advance_to(fmt_it);
        return true;
      }
      return false;
    }

    inline bool is_align_character(char c)
    {
      return c == '<' || c == '>' || c == '^';
    }

    inline spec_align_t align_from_char(char c)
    {
      spec_align_t result = spec_align_t::NONE;
      switch (c)
      {
        case '<': result = spec_align_t::START; break;
        case '>': result = spec_align_t::END; break;
        case '^': result = spec_align_t::CENTER; break;
        default:
          // invalid alignment specification
          ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
      }
      return result;
    }

    inline spec_align_t parse_fill_and_align(format_parse_context& parse_ctx, char_type& fill)
    {
      spec_align_t result = spec_align_t::NONE;
      fill = ' '; // default

      auto fmt_it = parse_ctx.begin();
      if (fmt_it != parse_ctx.end())
      {
        const char c = *fmt_it;
        ++fmt_it;

        if (is_align_character(c))
        {
          result = align_from_char(c);
          parse_ctx.advance_to(fmt_it);
        }
        else if (fmt_it != parse_ctx.end())
        {
          const char c2 = *fmt_it;
          ++fmt_it;
          if (is_align_character(c2))
          {
            result = align_from_char(c2);
            ETL_ASSERT(c != '{' && c != '}', ETL_ERROR(bad_format_string_exception)); // no { or } allowed as fill character
            fill = c;
            parse_ctx.advance_to(fmt_it);
          }
        }
        else
        {
          // no align and fill spec (valid)
        }
      }
      return result;
    }

    inline spec_sign_t sign_from_char(const char c)
    {
      spec_sign_t result = spec_sign_t::MINUS;
      switch (c)
      {
        case '-': result = spec_sign_t::MINUS; break;
        case '+': result = spec_sign_t::PLUS; break;
        case ' ': result = spec_sign_t::SPACE; break;
        default:
          // invalid sign character c
          ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
      }
      return result;
    }

    inline bool parse_nested_replacement(format_parse_context& parse_ctx, etl::optional<size_t>& index)
    {
      bool start = parse_char(parse_ctx, '{');
      if (start)
      {
        auto num = parse_num(parse_ctx);
        if (num)
        {
          // manual mode
          index = num;
          parse_ctx.check_arg_id(*index);
          bool end = parse_char(parse_ctx, '}');
          if (end)
          {
            return true;
          }
          else
          {
            // bad nested replacement index spec
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
        else
        {
          bool end = parse_char(parse_ctx, '}');
          if (end)
          {
            index = parse_ctx.next_arg_id();
            return true;
          }
          else
          {
            // bad nested replacement spec
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }
      return false;
    }

    template<class OutputIt>
    void parse_format_spec(format_parse_context& parse_ctx, format_context<OutputIt>& fmt_context)
    {
      auto& format_spec = fmt_context.format_spec;

      format_spec = format_spec_t(); // reset format_spec to defaults

      format_spec.index = parse_num(parse_ctx); // optional

      bool colon = parse_char(parse_ctx, ':');
      if (colon)
      {
        format_spec.align = parse_fill_and_align(parse_ctx, format_spec.fill);

        etl::optional<char> sign = parse_any_of(parse_ctx, "+- ");
        if (sign) {
          format_spec.sign = sign_from_char(*sign);
        }

        format_spec.hash = parse_char(parse_ctx, '#');
        format_spec.zero = parse_char(parse_ctx, '0');

        format_spec.width = parse_num(parse_ctx);
        if (!format_spec.width)
        {
          // possibly with via nested replacement
          format_spec.width_nested_replacement = parse_nested_replacement(parse_ctx, format_spec.width);
        }

        if (parse_char(parse_ctx, '.'))
        {
          format_spec.precision = parse_num(parse_ctx);
          if (!format_spec.precision)
          {
            // possibly with via nested replacement
            format_spec.precision_nested_replacement = parse_nested_replacement(parse_ctx, format_spec.precision);
          }
        }

        format_spec.locale_specific = parse_char(parse_ctx, 'L');

        format_spec.type = parse_any_of(parse_ctx, "s?bBcdoxXaAeEfFgGpP");
      }
    }
  }

  template<class T, class CharT = char>
  struct formatter
  {
    using char_type = CharT;
  };

  template<>
  struct formatter<etl::monostate>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      return parse_ctx.end();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(etl::monostate arg, format_context<OutputIt>& fmt_ctx)
    {
      (void)arg;
      return fmt_ctx.out();
    }
  };

  namespace private_format
  {
    // for 4321, return 1000
    template<typename UnsignedT, typename = etl::enable_if_t<etl::is_unsigned<UnsignedT>::value>>
    UnsignedT get_highest_digit(UnsignedT value, size_t base = 10)
    {
      ETL_ASSERT(base > 1, ETL_ERROR(bad_format_string_exception));
      UnsignedT result = 1;
      value /= base;
      while (result <= value)
      {
        result *= base;
      }
      return result;
    }

    template<typename T>
    T int_pow(T base, T exp)
    {
      T result = 1;
      while (exp > 0) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
      }
      return result;
    }

    template<typename OutputIt, typename T>
    void format_sign(OutputIt& it, T value, const format_spec_t& spec)
    {
      char c = '\0';
      if (value < 0)
      {
        c = '-';
      }
      else
      {
        switch (spec.sign)
        {
          case spec_sign_t::MINUS:
            // c already set above if negative
            break;
          case spec_sign_t::PLUS:
            c = '+';
            break;
          case spec_sign_t::SPACE:
            c = ' ';
            break;
          default:
            // invalid sign
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }

      if (c != '\0')
      {
        *it = c;
        ++it;
      }
    }

    template<typename OutputIt>
    void format_sequence(OutputIt& out_it, etl::string_view value)
    {
      auto it = value.cbegin();
      while (it != value.cend())
      {
        *out_it = *it;
        ++it;
        ++out_it;
      }
    }

    template<typename OutputIt, typename T>
    void format_alternate_form(OutputIt& it, const format_spec_t& spec)
    {
      if (spec.hash && spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 'b':
            format_sequence(it, "0b");
            break;
          case 'B':
            format_sequence(it, "0B");
            break;
          case 'o':
            format_sequence(it, "0");
            break;
          case 'x':
            format_sequence(it, "0x");
            break;
          case 'X':
            format_sequence(it, "0X");
            break;
          // default: no prefix
        }
      }
    }

    template<typename OutputIt>
    void format_plain_char(OutputIt& it, char_type c)
    {
      *it = c;
      ++it;
    }

    template<typename OutputIt>
    void format_escaped_char(OutputIt& it, char_type c)
    {
      switch (c)
      {
        case '\t':
          format_sequence(it, "\\t");
          break;
        case '\n':
          format_sequence(it, "\\n");
          break;
        case '\r':
          format_sequence(it, "\\r");
          break;
        case '"':
          format_sequence(it, "\\\"");
          break;
        case '\'':
          format_sequence(it, "\\'");
          break;
        case '\\':
          format_sequence(it, "\\\\");
          break;
        default:
          *it = c;
          ++it;
      }
    }

    template<typename OutputIt>
    void fill(OutputIt& it, size_t size, char_type c)
    {
      while (size > 0)
      {
        *it = c;
        ++it;
        --size;
      }
    }

    template<size_t default_base = 10>
    inline size_t base_from_spec(const format_spec_t& spec)
    {
      size_t base = default_base;
      if (spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 'a':
          case 'A':
            base = 16;
            break;
          case 'b':
          case 'B':
            base = 2;
            break;
          case 'o':
            base = 8;
            break;
          case 'p':
          case 'P':
          case 'x':
          case 'X':
            base = 16;
            break;
          // default: no prefix
        }
      }
      return base;
    }

    inline bool is_uppercase(const char c)
    {
      return c >= 'A' && c <= 'Z';
    }

    template<typename OutputIt, typename T>
    void format_digit_char(OutputIt& it, T value, const format_spec_t& spec)
    {
      if (value <= 9)
      {
        *it = static_cast<char_type>('0' + static_cast<typename etl::make_unsigned<T>::type>(value));
      }
      else
      {
        if (spec.type.has_value() && is_uppercase(spec.type.value()))
        {
          *it = static_cast<char_type>('A' + static_cast<typename etl::make_unsigned<T>::type>(value - 10));
        }
        else
        {
          *it = static_cast<char_type>('a' + static_cast<typename etl::make_unsigned<T>::type>(value - 10));
        }
      }
      ++it;
    }

    inline void adjust_width_from_spec(const format_spec_t& spec, size_t& width)
    {
      if (spec.zero && spec.width.has_value())
      {
        width = etl::max(width, spec.width.value());
      }
    }

    inline void check_precision(const format_spec_t& spec)
    {
      if (spec.precision.has_value())
      {
        // precision not allowed for integer numbers
        ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
      }
    }

    // used for both integers and float parts
    // skip_last_zeros helps in case of printing after-the-decimal zeros which are
    // redundant then
    template<typename OutputIt, typename T, T default_base = 10, bool skip_last_zeros = false>
    void format_plain_num(OutputIt& it, T value, const format_spec_t& spec, size_t width = 0)
    {
      using UnsignedT = typename etl::make_unsigned<T>::type;

      UnsignedT unsigned_value = etl::absolute_unsigned(value);

      size_t base = base_from_spec<default_base>(spec);
      UnsignedT highest_digit = get_highest_digit<UnsignedT>(unsigned_value, base);
      if (width > 0)
      {
        UnsignedT align_highest_digit = int_pow<UnsignedT>(base, width - 1);
        highest_digit = etl::max<UnsignedT>(align_highest_digit, highest_digit);
      }

      // this loop is iterated at least once, to print a number
      while (highest_digit > 0)
      {
        UnsignedT digit = unsigned_value / highest_digit;
        unsigned_value %= highest_digit;
        format_digit_char(it, digit, spec);

        if ETL_CONSTEXPR17 (skip_last_zeros)
        {
          if (unsigned_value == 0)
          {
            break;
          }
        }

        highest_digit /= base;
      }
    }

    // for integers
    template<typename OutputIt, typename T, bool skip_last_zeros = false>
    void format_num(OutputIt& it, T value, const format_spec_t& spec)
    {
      size_t width = 0;
      format_sign<OutputIt, T>(it, value, spec);
      format_alternate_form<OutputIt, T>(it, spec);
      adjust_width_from_spec(spec, width);
      check_precision(spec);
      format_plain_num(it, value, spec, width);
    }

    template<typename OutputIt, typename T>
    void format_floating_default(OutputIt& it, T value, const format_spec_t& spec)
    {
      const size_t fractional_decimals = 6; // default

      T integral;
      T fractional = modf(value, &integral);
      bool sign;
      unsigned long long int fractional_int;
      unsigned long long int integral_int;
      if (integral < 0.0)
      {
        sign = true;
        fractional_int = static_cast<unsigned long long int>(-fractional * pow(10., fractional_decimals));
        integral_int = static_cast<unsigned long long int>(-integral);
      }
      else
      {
        sign = false;
        fractional_int = static_cast<unsigned long long int>(fractional * pow(10., fractional_decimals));
        integral_int = static_cast<unsigned long long int>(integral);
      }

      private_format::format_sign<OutputIt, int>(it, sign ? -1 : 0, spec);
      private_format::format_plain_num<OutputIt, unsigned long long int>(it, integral_int, spec);
      private_format::format_sequence<OutputIt>(it, ".");
      private_format::format_plain_num<OutputIt, unsigned long long int, 10, true>(it, fractional_int, spec, fractional_decimals);
    }

    // floating point in hex notation
    template<typename OutputIt, typename T>
    void format_floating_a(OutputIt& it, T value, const format_spec_t& spec)
    {
      static const size_t fractional_decimals = 10; // default
      static const size_t exponent_decimals = 1;
      long long int exponent_int = 0;

      bool sign;
      unsigned long long int fractional_int;
      unsigned long long int integral_int;

      T integral;
      T fractional = modf(value, &integral);

      while (value >= 0x10 || value <= -0x10)
      {
        ++exponent_int;
        value /= 0x10;
        fractional = modf(value, &integral);
      }

      while ((value > 0.0000000000001 && value < 1) || (value < -0.0000000000001 && value > -1))
      {
        --exponent_int;
        value *= 0x10;
        fractional = modf(value, &integral);
      }

      if (integral < 0.0)
      {
        sign = true;
        fractional_int = static_cast<unsigned long long int>(-fractional * pow(static_cast<T>(0x10), fractional_decimals));
        integral_int = static_cast<unsigned long long int>(-integral);
      }
      else
      {
        sign = false;
        fractional_int = static_cast<unsigned long long int>(fractional * pow(static_cast<T>(0x10), fractional_decimals));
        integral_int = static_cast<unsigned long long int>(integral);
      }

      private_format::format_sign<OutputIt, int>(it, sign ? -1 : 0, spec);
      private_format::format_plain_char<OutputIt>(it, '0');
      char hex_letter = 'x';
      if (is_uppercase(spec.type.value()))
      {
        hex_letter = 'X';
      }
      private_format::format_plain_char<OutputIt>(it, hex_letter);
      private_format::format_plain_num<OutputIt, unsigned long long int, 16>(it, integral_int, spec);
      private_format::format_plain_char<OutputIt>(it, '.');
      private_format::format_plain_num<OutputIt, unsigned long long int, 16, true>(it, fractional_int, spec, fractional_decimals);
      char letter = 'p';
      if (is_uppercase(spec.type.value()))
      {
        letter = 'P';
      }
      private_format::format_plain_char<OutputIt>(it, letter);
      private_format::format_plain_char<OutputIt>(it, (exponent_int < 0) ? '-' : '+');
      private_format::format_plain_num<OutputIt, long long int, 16>(it, exponent_int, spec, exponent_decimals);
    }

    template<typename OutputIt, typename T>
    void format_floating_e(OutputIt& it, T value, const format_spec_t& spec)
    {
      static const size_t fractional_decimals = 6; // default
      static const size_t exponent_decimals = 2;
      long long int exponent_int = 0;

      bool sign;
      unsigned long long int fractional_int;
      unsigned long long int integral_int;

      T integral;
      T fractional = modf(value, &integral);

      while (value >= 10 || value <= -10)
      {
        ++exponent_int;
        value /= 10;
        fractional = modf(value, &integral);
      }

      while ((value > 0.0000000000001 && value < 1) || (value < -0.0000000000001 && value > -1))
      {
        --exponent_int;
        value *= 10;
        fractional = modf(value, &integral);
      }

      if (integral < 0.0)
      {
        sign = true;
        fractional_int = static_cast<unsigned long long int>(-fractional * pow(10., fractional_decimals));
        integral_int = static_cast<unsigned long long int>(-integral);
      }
      else
      {
        sign = false;
        fractional_int = static_cast<unsigned long long int>(fractional * pow(10., fractional_decimals));
        integral_int = static_cast<unsigned long long int>(integral);
      }

      private_format::format_sign<OutputIt, int>(it, sign ? -1 : 0, spec);
      private_format::format_plain_num<OutputIt, unsigned long long int>(it, integral_int, spec);
      private_format::format_sequence<OutputIt>(it, ".");
      private_format::format_plain_num<OutputIt, unsigned long long int>(it, fractional_int, spec, fractional_decimals);
      char letter = 'e';
      if (is_uppercase(spec.type.value()))
      {
        letter = 'E';
      }
      private_format::format_plain_char<OutputIt>(it, letter);
      private_format::format_plain_char<OutputIt>(it, (exponent_int < 0) ? '-' : '+');
      private_format::format_plain_num<OutputIt, long long int>(it, exponent_int, spec, exponent_decimals);
    }

    template<typename OutputIt, typename T>
    void format_floating_f(OutputIt& it, T value, const format_spec_t& spec)
    {
      const size_t fractional_decimals = 6; // default

      T integral;
      T fractional = modf(value, &integral);
      bool sign;
      unsigned long long int fractional_int;
      unsigned long long int integral_int;
      if (integral < 0.0)
      {
        sign = true;
        fractional_int = static_cast<unsigned long long int>(-fractional * pow(10., fractional_decimals));
        integral_int = static_cast<unsigned long long int>(-integral);
      }
      else
      {
        sign = false;
        fractional_int = static_cast<unsigned long long int>(fractional * pow(10., fractional_decimals));
        integral_int = static_cast<unsigned long long int>(integral);
      }

      private_format::format_sign<OutputIt, int>(it, sign ? -1 : 0, spec);
      private_format::format_plain_num<OutputIt, unsigned long long int>(it, integral_int, spec);
      private_format::format_sequence<OutputIt>(it, ".");
      private_format::format_plain_num<OutputIt, unsigned long long int>(it, fractional_int, spec, fractional_decimals);
    }

    class dummy_assign_to
    {
    public:
      dummy_assign_to& operator=(char_type)
      {
        return *this;
      }
    };

    template<class OutputIt>
    class limit_assign_to
    {
    public:
      limit_assign_to(OutputIt o, bool is_active)
      : out(o)
      , active(is_active)
      {
      }

      limit_assign_to& operator=(char_type c)
      {
        if (active)
        {
          *out = c;
        }
        return *this;
      }

    private:
      OutputIt out;
      bool active;
    };

    template<class OutputIt>
    class limit_iterator
    {
    public:
      limit_iterator(OutputIt& it, size_t n)
      : out(it)
      , limit(n)
      {
      }

      limit_iterator(const limit_iterator& other) = default;
      limit_iterator(limit_iterator&& other) = default;
      limit_iterator& operator=(const limit_iterator& other) = default;
      limit_iterator& operator=(limit_iterator&& other) = default;

      limit_assign_to<OutputIt> operator*()
      {
        return limit_assign_to<OutputIt>(out, (limit > 0));
      }

      limit_iterator& operator++()
      {
        if (limit > 0)
        {
          --limit;
          ++out;
        }
        return *this;
      }

      limit_iterator operator++(int)
      {
        limit_iterator temp = *this;
        if (limit > 0)
        {
          --limit;
          out++;
        }
        return temp;
      }

      OutputIt get()
      {
        return out;
      }

    private:
      OutputIt out;
      size_t limit;
    };

    class counter_iterator
    {
    public:
      counter_iterator(): count(0)
      {
      }

      counter_iterator(const counter_iterator& other) = default;
      counter_iterator& operator=(const counter_iterator& other) = default;

      dummy_assign_to operator*()
      {
        return dummy_assign_to();
      }

      counter_iterator& operator++()
      {
        ++count;
        return *this;
      }

      counter_iterator operator++(int)
      {
        counter_iterator temp = *this;
        count++;
        return temp;
      }

      size_t value()
      {
        return count;
      }

    private:
      size_t count;
    };

    template<typename OutputIt, typename T>
    void format_floating_g(OutputIt& it, T value, const format_spec_t& spec)
    {
      private_format::counter_iterator counter_e, counter_f;

      format_floating_e(counter_e, value, spec);
      format_floating_f(counter_f, value, spec);

      if (counter_e.value() < counter_f.value())
      {
        format_floating_e(it, value, spec);
      }
      else
      {
        format_floating_f(it, value, spec);
      }
    }

    template<typename OutputIt, typename T>
    void format_floating(OutputIt& it, T value, const format_spec_t& spec)
    {
      if (isnan(value))
      {
        if (spec.type.has_value() && (is_uppercase(spec.type.value())))
        {
          format_sequence(it, "NAN");
        }
        else
        {
          format_sequence(it, "nan");
        }
      }
      else if (isinf(value))
      {
        if (spec.type.has_value() && (is_uppercase(spec.type.value())))
        {
          format_sequence(it, "INF");
        }
        else
        {
          format_sequence(it, "inf");
        }
      }
      else if (!spec.type.has_value())
      {
        format_floating_default(it, value, spec);
      }
      else
      {
        switch (spec.type.value())
        {
          case 'a':
          case 'A':
            format_floating_a(it, value, spec);
            break;
          case 'e':
          case 'E':
            format_floating_e(it, value, spec);
            break;
          case 'f':
          case 'F':
            format_floating_f(it, value, spec);
            break;
          case 'g':
          case 'G':
            format_floating_g(it, value, spec);
            break;
          default:
            // unknown presentation type
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }
    }

    template<class OutputIt>
    struct format_visitor
    {
      using output_iterator = OutputIt;

      format_visitor(format_parse_context& parse_context, format_context<OutputIt>& f_ctx)
      : parse_ctx(parse_context)
      , fmt_ctx(f_ctx)
      {
      }

      // for all types in supported_format_types
      template<typename T>
      void operator()(T value)
      {
        formatter<T> f;
        format_parse_context::iterator it = f.parse(parse_ctx);
        parse_ctx.advance_to(it);
        OutputIt fit = f.format(value, fmt_ctx);
        fmt_ctx.advance_to(fit);
      }

      format_parse_context& parse_ctx;
      format_context<OutputIt>& fmt_ctx;
    };

    template<class OutputIt>
    void output(format_context<OutputIt>& fmt_context, char c)
    {
      *fmt_context.out() = c;
      OutputIt tmp = fmt_context.out();
      tmp++;
      fmt_context.advance_to(tmp);
    }

    template<typename OutputIt, typename Int>
    typename format_context<OutputIt>::iterator format_aligned_int(Int arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_num<private_format::counter_iterator, Int>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::NONE: // default
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_num<OutputIt, Int>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }

    template<typename OutputIt, typename Float>
    typename format_context<OutputIt>::iterator format_aligned_floating(Float arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_floating<private_format::counter_iterator, Float>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::NONE: // default
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_floating<OutputIt, Float>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }

    template<typename OutputIt>
    void format_string_view(OutputIt& it, etl::string_view arg, const format_spec_t& spec)
    {
      bool escaped = false;
      if (spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 's':
            // default output
            break;
          case '?':
            // escaped string
            escaped = true;
            break;
          default:
            // invalid type for string
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }
      size_t limit = etl::numeric_limits<size_t>::max();
      if (spec.precision.has_value())
      {
        limit = spec.precision.value();
      }

      if (escaped)
      {
        format_plain_char(it, '"');
      }
      etl::string_view::const_iterator arg_it = arg.begin();
      while (arg_it != arg.cend() && limit > 0)
      {
        if (escaped)
        {
          format_escaped_char(it, *arg_it);
        }
        else
        {
          format_plain_char(it, *arg_it);
        }
        ++arg_it;
        --limit;
      }
      if (escaped)
      {
        format_plain_char(it, '"');
      }
    }

    template<typename OutputIt>
    typename format_context<OutputIt>::iterator format_aligned_string_view(etl::string_view arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_string_view<private_format::counter_iterator>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::NONE: // default
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_string_view<OutputIt>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }

    template<typename OutputIt>
    void format_chars(OutputIt& it, const char* arg, const format_spec_t& spec)
    {
      bool escaped = false;
      if (spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 's':
            // default output
            break;
          case '?':
            // escaped string
            escaped = true;
            break;
          default:
            // invalid type for string
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }
      size_t limit = etl::numeric_limits<size_t>::max();
      if (spec.precision.has_value())
      {
        limit = spec.precision.value();
      }

      if (escaped)
      {
        format_plain_char(it, '"');
      }
      const char_type* arg_it = arg;
      while (*arg_it != '\0' && limit > 0)
      {
        if (escaped)
        {
          format_escaped_char(it, *arg_it);
        }
        else
        {
          format_plain_char(it, *arg_it);
        }
        ++arg_it;
        --limit;
      }
      if (escaped)
      {
        format_plain_char(it, '"');
      }
    }

    template<typename OutputIt>
    typename format_context<OutputIt>::iterator format_aligned_chars(const char* arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_chars<private_format::counter_iterator>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::NONE: // default
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_chars<OutputIt>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }

    inline void check_char_spec(const format_spec_t& spec)
    {
      if ((!spec.type.has_value() || spec.type.value() == 'c' || spec.type.value() == '?') &&
          (spec.sign != spec_sign_t::MINUS ||
           spec.zero ||
           spec.hash ||
           spec.precision)
      )
      {
        ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
      }
    }

    template<typename OutputIt>
    void format_char(OutputIt& it, char_type c, const format_spec_t& spec)
    {
      check_char_spec(spec);
      if (spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 'c':
            // default output
            format_plain_char(it, c);
            break;
          case '?':
            // escaped string
            format_plain_char(it, '\'');
            format_escaped_char(it, c);
            format_plain_char(it, '\'');
            break;
          case 'b':
          case 'B':
          case 'd':
          case 'o':
          case 'x':
          case 'X':
            private_format::format_num<OutputIt, unsigned int>(it, static_cast<unsigned int>(static_cast<unsigned char>(c)), spec);
            break;
          default:
            // invalid type for string
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }
      else
      {
        format_plain_char(it, c);
      }
    }

    template<typename OutputIt>
    typename format_context<OutputIt>::iterator format_aligned_char(char_type arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_char<private_format::counter_iterator>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::NONE: // default
              if (!fmt_ctx.format_spec.type.has_value() || fmt_ctx.format_spec.type.value() == 'c' || fmt_ctx.format_spec.type.value() == '?')
              {
                prefix_size = 0;
                suffix_size = pad;
              }
              else
              {
                prefix_size = pad;
                suffix_size = 0;
              }
              break;
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_char<OutputIt>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }

    template<typename OutputIt>
    void format_bool(OutputIt& it, bool value, const format_spec_t& spec)
    {
      if (spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 's':
            // default output
            format_sequence(it, value ? "true" : "false");
            break;
          case 'b':
          case 'B':
          case 'd':
          case 'o':
          case 'x':
          case 'X':
            private_format::format_num<OutputIt, unsigned int>(it, static_cast<unsigned int>(static_cast<unsigned char>(value)), spec);
            break;
          default:
            // invalid type for string
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }
      else
      {
        format_sequence(it, value ? "true" : "false");
      }
    }

    template<typename OutputIt>
    typename format_context<OutputIt>::iterator format_aligned_bool(bool arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_bool<private_format::counter_iterator>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::NONE: // default
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_bool<OutputIt>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }

    template<typename OutputIt>
    void format_pointer(OutputIt& it, const void* value, const format_spec_t& spec)
    {
      if (spec.type.has_value())
      {
        switch (spec.type.value())
        {
          case 'p':
          case 'P':
            format_sequence(it, spec.type.value() == 'p' ? "0x" : "0X");
            format_plain_num<OutputIt, uintptr_t>(it, reinterpret_cast<uintptr_t>(value), spec);
            break;
          default:
            // invalid type for string
            ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
        }
      }
      else
      {
        format_sequence(it, "0x");
        format_plain_num<OutputIt, uintptr_t>(it, reinterpret_cast<uintptr_t>(value), spec);
      }
    }

    template<typename OutputIt>
    typename format_context<OutputIt>::iterator format_aligned_pointer(const void* arg, format_context<OutputIt>& fmt_ctx)
    {
      size_t prefix_size = 0;
      size_t suffix_size = 0;

      if (fmt_ctx.format_spec.width)
      {
        // calculate size
        private_format::counter_iterator counter;
        private_format::format_pointer<private_format::counter_iterator>(counter, arg, fmt_ctx.format_spec);

        if (counter.value() < fmt_ctx.format_spec.width.value())
        {
          size_t pad = fmt_ctx.format_spec.width.value() - counter.value();
          switch (fmt_ctx.format_spec.align)
          {
            case private_format::spec_align_t::START:
              prefix_size = 0;
              suffix_size = pad;
              break;
            case private_format::spec_align_t::CENTER:
              prefix_size = pad / 2;
              suffix_size = pad - prefix_size;
              break;
            case private_format::spec_align_t::NONE: // default
            case private_format::spec_align_t::END:
              prefix_size = pad;
              suffix_size = 0;
              break;
            default:
              // invalid alignment specification
              ETL_ASSERT_FAIL(ETL_ERROR(bad_format_string_exception));
          }
        }
      }

      // actual output
      OutputIt it = fmt_ctx.out();
      private_format::fill<OutputIt>(it, prefix_size, fmt_ctx.format_spec.fill);
      private_format::format_pointer<OutputIt>(it, arg, fmt_ctx.format_spec);
      private_format::fill<OutputIt>(it, suffix_size, fmt_ctx.format_spec.fill);
      return it;
    }
  } // namespace private_format

  template<>
  struct formatter<int>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(int arg, format_context<OutputIt>& fmt_ctx)
    {
      if (fmt_ctx.format_spec.type.has_value() && fmt_ctx.format_spec.type.value() == 'c')
      {
        return private_format::format_aligned_char<OutputIt>(static_cast<private_format::char_type>(arg), fmt_ctx);
      }
      return private_format::format_aligned_int<OutputIt, int>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<unsigned int>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(unsigned int arg, format_context<OutputIt>& fmt_ctx)
    {
      if (fmt_ctx.format_spec.type.has_value() && fmt_ctx.format_spec.type.value() == 'c')
      {
        return private_format::format_aligned_char<OutputIt>(static_cast<private_format::char_type>(arg), fmt_ctx);
      }
      return private_format::format_aligned_int<OutputIt, unsigned int>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<long long int>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(long long int arg, format_context<OutputIt>& fmt_ctx)
    {
      if (fmt_ctx.format_spec.type.has_value() && fmt_ctx.format_spec.type.value() == 'c')
      {
        return private_format::format_aligned_char<OutputIt>(static_cast<private_format::char_type>(arg), fmt_ctx);
      }
      return private_format::format_aligned_int<OutputIt, long long int>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<unsigned long long int>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(unsigned long long int arg, format_context<OutputIt>& fmt_ctx)
    {
      if (fmt_ctx.format_spec.type.has_value() && fmt_ctx.format_spec.type.value() == 'c')
      {
        return private_format::format_aligned_char<OutputIt>(static_cast<private_format::char_type>(arg), fmt_ctx);
      }
      return private_format::format_aligned_int<OutputIt, unsigned long long int>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<char>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(private_format::char_type arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_char<OutputIt>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<float>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(float arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_floating<OutputIt, float>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<double>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(double arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_floating<OutputIt, double>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<long double>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(long double arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_floating<OutputIt, long double>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<etl::string_view>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(etl::string_view arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_string_view<OutputIt>(arg, fmt_ctx);
    }
  };

  // string formatter
  template<>
  struct formatter<const char*>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(const char* arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_chars<OutputIt>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<bool>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(bool arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_bool<OutputIt>(arg, fmt_ctx);
    }
  };

  template<>
  struct formatter<const void*>
  {
    format_parse_context::iterator parse(format_parse_context& parse_ctx)
    {
      // unified parsing is done already in vformat_to()
      return parse_ctx.begin();
    }

    template<class OutputIt>
    typename format_context<OutputIt>::iterator format(const void* arg, format_context<OutputIt>& fmt_ctx)
    {
      return private_format::format_aligned_pointer<OutputIt>(arg, fmt_ctx);
    }
  };

  template<class OutputIt>
  OutputIt vformat_to(OutputIt out, etl::string_view fmt, format_args<OutputIt> args)
  {
    format_parse_context parse_context(fmt, args.size());
    format_context<OutputIt> fmt_context(out, args);
    private_format::format_visitor<OutputIt> v(parse_context, fmt_context);

    while (parse_context.begin() != parse_context.end())
    {
      const char c = *parse_context.begin();
      private_format::advance(parse_context);
      if (c == '{')
      {

        if (*parse_context.begin() == '{')
        {
          // escape sequence for literal '{'
          private_format::output<OutputIt>(fmt_context, c);
          private_format::advance(parse_context);
        }
        else
        {
          private_format::parse_format_spec<OutputIt>(parse_context, fmt_context);
          etl::optional<size_t> index = fmt_context.format_spec.index;
          if (index.has_value())
          {
            parse_context.check_arg_id(*index);
          }
          else
          {
            index = parse_context.next_arg_id();
          }
          format_arg<OutputIt> arg = args.get(*index);
          arg.template visit<void>(v);

          ETL_ASSERT(*parse_context.begin() == '}', ETL_ERROR(bad_format_string_exception)/*"Closing brace missing"*/);
          if (parse_context.begin() != parse_context.end())
          {
            private_format::advance(parse_context);
          }
        }
      }
      else if (c == '}') // only matches here if } without { is found
      {
        ETL_ASSERT(*parse_context.begin() == '}', ETL_ERROR(bad_format_string_exception)/*"2nd closing brace missing on escaped closing brace"*/);
        // escape sequence for literal '}'
        private_format::output<OutputIt>(fmt_context, c);
        private_format::advance(parse_context);
      }
      else
      {
        private_format::output<OutputIt>(fmt_context, c);
      }
    }

    return fmt_context.out();
  }

  template<typename OutputIt,
           typename = etl::enable_if_t<!etl::is_base_of<etl::remove_reference<etl::istring>::type, OutputIt>::value>,
           class... Args>
  OutputIt format_to(OutputIt out, format_string<Args...> fmt, Args&&... args)
  {
    auto the_args{make_format_args<OutputIt>(args...)};
    return vformat_to(etl::move(out), fmt.get(), format_args<OutputIt>(the_args));
  }

  template<typename OutputIt, class WrapperIt = private_format::limit_iterator<OutputIt>, class... Args>
  OutputIt format_to_n(OutputIt out, size_t n, format_string<Args...> fmt, Args&&... args)
  {
    auto the_args{make_format_args<WrapperIt>(args...)};
    return vformat_to(WrapperIt(out, n),
                      fmt.get(),
                      format_args<WrapperIt>(the_args)).get();
  }

  // non std in the following, specific to etl
  template<class... Args>
  etl::istring::iterator format_to(etl::istring& out, format_string<Args...> fmt, Args&&... args)
  {
    etl::istring::iterator result = format_to_n(out.begin(), out.max_size(), fmt, etl::forward<Args>(args)...);
    out.uninitialized_resize(result - out.begin());
    return result;
  }

  template<class... Args>
  size_t formatted_size(format_string<Args...> fmt, Args&&... args)
  {
    private_format::counter_iterator it;
    it = format_to(it, fmt, etl::forward<Args>(args)...);
    return it.value();
  }
}

#endif

#endif
