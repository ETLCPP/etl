#include "etl/string.h"
#include "etl/to_arithmetic.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>
#include <string>

template <typename T> void fuzzed_convert_integral(FuzzedDataProvider *fdp) {
  assert(fdp != nullptr);
  std::string s = fdp->ConsumeRandomLengthString();
  etl::string_view etl_string(s.c_str(), s.size());

  etl::radix::value_type radix = fdp->PickValueInArray({
    etl::radix::binary,
    etl::radix::octal,
    etl::radix::decimal,
    etl::radix::hex
  });

  auto result = etl::to_arithmetic<T, char>(etl_string, radix);
  if (result.has_value()) {
    volatile T value = result.value();
    (void)value;
  }
}

template <typename T> void fuzzed_convert_float(FuzzedDataProvider *fdp) {
  assert(fdp != nullptr);
  std::string s = fdp->ConsumeRandomLengthString();
  etl::string_view etl_string(s.c_str(), s.size());

  auto result = etl::to_arithmetic<T, char>(etl_string);
  if (result.has_value()) {
    volatile T value = result.value();
    (void)value;
  }
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);
  // Unsigned.
  fuzzed_convert_integral<uint8_t>(&fdp);
  fuzzed_convert_integral<uint16_t>(&fdp);
  fuzzed_convert_integral<uint32_t>(&fdp);
  fuzzed_convert_integral<uint64_t>(&fdp);

  // Signed.
  fuzzed_convert_integral<int8_t>(&fdp);
  fuzzed_convert_integral<int16_t>(&fdp);
  fuzzed_convert_integral<int32_t>(&fdp);
  fuzzed_convert_integral<int64_t>(&fdp);

  // Floating.
  fuzzed_convert_float<float>(&fdp);
  fuzzed_convert_float<double>(&fdp);
  fuzzed_convert_float<long double>(&fdp);
  return 0;
}
