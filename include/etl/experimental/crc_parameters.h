#pragma once

#include "../platform.h"

namespace etl
{
  namespace private_crc
  {
    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    struct crc_parameters
    {
      typedef TAccumulator accumulator_type;
      static ETL_CONSTANT TAccumulator Polynomial = Polynomial_;
      static ETL_CONSTANT TAccumulator Initial    = Initial_;
      static ETL_CONSTANT TAccumulator Xor_Out    = Xor_Out_;
      static ETL_CONSTANT bool         Reflect    = Reflect_;
    };

    // 8 bit.
    typedef crc_parameters<uint8_t, 0x07U, 0x00U, 0x00U, false> crc8_ccitt_parameters;
    typedef crc_parameters<uint8_t, 0x07U, 0xFFU, 0x00U, true>  crc8_rohc_parameters;

    // 16 bit.
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0x0000U, true>  crc16_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0x0000U, false> crc16_ccitt_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x1D0FU, 0x0000U, false> crc16_aug_ccitt_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0xFFFFU, false> crc16_genibus_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x0000U, 0x0000U, true>  crc16_kermit_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0xFFFFU, 0x0000U, true>  crc16_modbus_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0xFFFFU, 0xFFFFU, true>  crc16_usb_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0xFFFFU, true>  crc16_x25_parameters;
    typedef crc_parameters<uint16_t, 0x1021, 0x0000U, 0x0000U, false>  crc16_xmodem_parameters;

    // 32 bit.
    typedef crc_parameters<uint32_t, 0X04C11DB7U, 0xFFFFFFFFU, 0xFFFFFFFFU, true>  crc32_parameters;

    // 64 bit.
    typedef crc_parameters<uint64_t, 0x42F0E1EBA9EA3693U, 0x0000000000000000U, 0x0000000000000000U, false>  crc64_ecma_parameters;
  }
}


