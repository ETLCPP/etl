#include "etl/checksum.h"
#include "etl/crc.h"
#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cassert>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  FuzzedDataProvider fdp(data, size);
  const size_t kMaxBufferSize = 1024;
  // ---- Checksums ----
  std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(
      fdp.ConsumeIntegralInRange<size_t>(0, kMaxBufferSize));
  uint8_t sum = etl::checksum<uint8_t>(buffer.begin(), buffer.end());

  etl::checksum<uint8_t> checksum_calculator;

  for (size_t i = 0UL; i < buffer.size(); ++i) {
    checksum_calculator.add(buffer[i]);
  }

  uint8_t loop_sum = checksum_calculator;
  assert(sum == loop_sum);

  // ---- CRC ----
  // Get new data for new algorithm.
  buffer = fdp.ConsumeBytes<uint8_t>(
      fdp.ConsumeIntegralInRange<size_t>(0, kMaxBufferSize));
  
  uint8_t crc = etl::crc8_ccitt(buffer.begin(), buffer.end());
  etl::crc8_ccitt crc_calculator;
  
  for (size_t i = 0UL; i < buffer.size(); ++i) {
    crc_calculator.add(buffer[i]);
  }

  uint8_t loop_crc = crc_calculator;
  assert(crc == loop_crc);

  return 0;
}
