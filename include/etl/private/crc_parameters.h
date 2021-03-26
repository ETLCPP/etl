#pragma once

#include "../platform.h"
#include "../integral_limits.h"

namespace etl
{
  namespace private_crc
  {
    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    struct crc_parameters
    {
      typedef TAccumulator accumulator_type;
      static ETL_CONSTANT TAccumulator Polynomial       = Polynomial_;
      static ETL_CONSTANT TAccumulator Initial          = Initial_;
      static ETL_CONSTANT TAccumulator Xor_Out          = Xor_Out_;
      static ETL_CONSTANT bool         Reflect          = Reflect_;
      static ETL_CONSTANT size_t       Accumulator_Bits = etl::integral_limits<accumulator_type>::bits; 
    };

    // 8 bit.
    typedef crc_parameters<uint8_t, 0x07U, 0x00U, 0x00U, false> crc8_ccitt_parameters;
    typedef crc_parameters<uint8_t, 0x07U, 0xFFU, 0x00U, true>  crc8_rohc_parameters;
    typedef crc_parameters<uint8_t, 0x9BU, 0xFFU, 0x00U, false> crc8_cdma2000;
    typedef crc_parameters<uint8_t, 0x39U, 0x00U, 0x00U, true>  crc8_darc;
    typedef crc_parameters<uint8_t, 0xD5U, 0x00U, 0x00U, false> crc8_dvbs2;
    typedef crc_parameters<uint8_t, 0x1DU, 0xFFU, 0x00U, true>  crc8_ebu;
    typedef crc_parameters<uint8_t, 0x1DU, 0xFDU, 0x00U, false> crc8_icode;
    typedef crc_parameters<uint8_t, 0x07U, 0x00U, 0x55U, false> crc8_itu;
    typedef crc_parameters<uint8_t, 0x31U, 0x00U, 0x00U, true>  crc8_maxim;
    typedef crc_parameters<uint8_t, 0x9BU, 0x00U, 0x00U, true>  crc8_wcdma;

    // 16 bit.
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0x0000U, true>  crc16_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0x0000U, false> crc16_ccitt_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x1D0FU, 0x0000U, false> crc16_aug_ccitt_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0xFFFFU, false> crc16_genibus_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x0000U, 0x0000U, true>  crc16_kermit_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0xFFFFU, 0x0000U, true>  crc16_modbus_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0xFFFFU, 0xFFFFU, true>  crc16_usb_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0xFFFFU, true>  crc16_x25_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x0000U, 0x0000U, false> crc16_xmodem_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0x0000U, false> crc16_buypass;
    typedef crc_parameters<uint16_t, 0xc867U, 0xFFFFU, 0x0000U, false> crc16_cdma2000;
    typedef crc_parameters<uint16_t, 0x8005U, 0x800DU, 0x0000U, false> crc16_dds110;
    typedef crc_parameters<uint16_t, 0x0589U, 0x0000U, 0x0001U, false> crc16_dectr;
    typedef crc_parameters<uint16_t, 0x0589U, 0x0000U, 0x0000U, false> crc16_dectx;
    typedef crc_parameters<uint16_t, 0x3D65U, 0x0000U, 0xFFFFU, true>  crc16_dnp;
    typedef crc_parameters<uint16_t, 0x3D65U, 0x0000U, 0xFFFFU, false> crc16_en13757;
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0xFFFFU, true>  crc16_maxim;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0x0000U, true>  crc16_mcrf4xx;
    typedef crc_parameters<uint16_t, 0x1021U, 0xB2AAU, 0x0000U, true>  crc16_riello;
    typedef crc_parameters<uint16_t, 0x8BB7U, 0x0000U, 0x0000U, false> crc16_t10dif;
    typedef crc_parameters<uint16_t, 0xA097U, 0x0000U, 0x0000U, false> crc16_teledisk;
    typedef crc_parameters<uint16_t, 0x1021U, 0x89ECU, 0x0000U, true>  crc16_tms37157;
    typedef crc_parameters<uint16_t, 0x1021U, 0xC6C6U, 0x0000U, true>  crc16_a;

    // 32 bit.
    typedef crc_parameters<uint32_t, 0x04C11DB7U, 0xFFFFFFFFU, 0xFFFFFFFFU, true>  crc32_parameters;
    typedef crc_parameters<uint32_t, 0x1EDC6F41U, 0xFFFFFFFFU, 0xFFFFFFFFU, true>  crc32_c_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7U, 0xFFFFFFFFU, 0xFFFFFFFFU, false> crc32_bzip2_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7U, 0xFFFFFFFFU, 0x00000000U, false> crc32_mpeg2_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7U, 0x00000000U, 0xFFFFFFFFU, false> crc32_posix_parameters;
    typedef crc_parameters<uint32_t, 0xA833982BU, 0xFFFFFFFFU, 0xFFFFFFFFU, true>  crc32_d;
    typedef crc_parameters<uint32_t, 0x814141ABU, 0x00000000U, 0x00000000U, false> crc32_q;
    typedef crc_parameters<uint32_t, 0x04C11DB7U, 0xFFFFFFFFU, 0x00000000U, true>  crc32_jamcrc;
    typedef crc_parameters<uint32_t, 0x000000AFU, 0x00000000U, 0x00000000U, false> crc32_xfer;

    // 64 bit.
    typedef crc_parameters<uint64_t, 0x42F0E1EBA9EA3693U, 0x0000000000000000U, 0x0000000000000000U, false> crc64_ecma_parameters;
  }
}


