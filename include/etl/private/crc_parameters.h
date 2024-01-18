///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_CRC_PARAMETERS_INCLUDED
#define ETL_CRC_PARAMETERS_INCLUDED

#include "../platform.h"
#include "../integral_limits.h"

//*****************************************************************************
// The parameters for these CRCs were created from https://crccalc.com/
//*****************************************************************************

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

    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    ETL_CONSTANT TAccumulator crc_parameters<TAccumulator, Polynomial_, Initial_, Xor_Out_, Reflect_>::Polynomial;

    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    ETL_CONSTANT TAccumulator crc_parameters<TAccumulator, Polynomial_, Initial_, Xor_Out_, Reflect_>::Initial;

    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    ETL_CONSTANT TAccumulator crc_parameters<TAccumulator, Polynomial_, Initial_, Xor_Out_, Reflect_>::Xor_Out;

    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    ETL_CONSTANT bool crc_parameters<TAccumulator, Polynomial_, Initial_, Xor_Out_, Reflect_>::Reflect;

    template <typename TAccumulator, TAccumulator Polynomial_, TAccumulator Initial_, TAccumulator Xor_Out_, bool Reflect_>
    ETL_CONSTANT size_t crc_parameters<TAccumulator, Polynomial_, Initial_, Xor_Out_, Reflect_>::Accumulator_Bits;

    // 8 bit.
    typedef crc_parameters<uint8_t, 0x07U, 0x00U, 0x00U, false> crc8_ccitt_parameters;
    typedef crc_parameters<uint8_t, 0x07U, 0xFFU, 0x00U, true>  crc8_rohc_parameters;
    typedef crc_parameters<uint8_t, 0x9BU, 0xFFU, 0x00U, false> crc8_cdma2000_parameters;
    typedef crc_parameters<uint8_t, 0x39U, 0x00U, 0x00U, true>  crc8_darc_parameters;
    typedef crc_parameters<uint8_t, 0xD5U, 0x00U, 0x00U, false> crc8_dvbs2_parameters;
    typedef crc_parameters<uint8_t, 0x1DU, 0xFFU, 0x00U, true>  crc8_ebu_parameters;
    typedef crc_parameters<uint8_t, 0x1DU, 0xFDU, 0x00U, false> crc8_icode_parameters;
    typedef crc_parameters<uint8_t, 0x07U, 0x00U, 0x55U, false> crc8_itu_parameters;
    typedef crc_parameters<uint8_t, 0x31U, 0x00U, 0x00U, true>  crc8_maxim_parameters;
    typedef crc_parameters<uint8_t, 0x9BU, 0x00U, 0x00U, true>  crc8_wcdma_parameters;
    typedef crc_parameters<uint8_t, 0x1DU, 0xFFU, 0xFFU, false> crc8_j1850_parameters;
    typedef crc_parameters<uint8_t, 0x1DU, 0x00U, 0x00U, false> crc8_j1850_zero_parameters;

    // 16 bit.
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0x0000U, true>  crc16_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0x0000U, false> crc16_ccitt_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x1D0FU, 0x0000U, false> crc16_aug_ccitt_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0x0000U, false> crc16_buypass_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0xFFFFU, false> crc16_genibus_parameters;
    typedef crc_parameters<uint16_t, 0x1DCFU, 0xFFFFU, 0xFFFFU, false> crc16_profibus_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x0000U, 0x0000U, true>  crc16_kermit_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0xFFFFU, 0x0000U, true>  crc16_modbus_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0xFFFFU, 0xFFFFU, true>  crc16_usb_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0xFFFFU, true>  crc16_x25_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x0000U, 0x0000U, false> crc16_xmodem_parameters;
    typedef crc_parameters<uint16_t, 0xc867U, 0xFFFFU, 0x0000U, false> crc16_cdma2000_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0x800DU, 0x0000U, false> crc16_dds110_parameters;
    typedef crc_parameters<uint16_t, 0x0589U, 0x0000U, 0x0001U, false> crc16_dect_r_parameters;
    typedef crc_parameters<uint16_t, 0x0589U, 0x0000U, 0x0000U, false> crc16_dect_x_parameters;
    typedef crc_parameters<uint16_t, 0x3D65U, 0x0000U, 0xFFFFU, true>  crc16_dnp_parameters;
    typedef crc_parameters<uint16_t, 0x3D65U, 0x0000U, 0xFFFFU, false> crc16_en13757_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0xFFFFU, true>  crc16_maxim_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xFFFFU, 0x0000U, true>  crc16_mcrf4xx_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xB2AAU, 0x0000U, true>  crc16_riello_parameters;
    typedef crc_parameters<uint16_t, 0x8BB7U, 0x0000U, 0x0000U, false> crc16_t10dif_parameters;
    typedef crc_parameters<uint16_t, 0xA097U, 0x0000U, 0x0000U, false> crc16_teledisk_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0x89ECU, 0x0000U, true>  crc16_tms37157_parameters;
    typedef crc_parameters<uint16_t, 0x1021U, 0xC6C6U, 0x0000U, true>  crc16_a_parameters;
    typedef crc_parameters<uint16_t, 0x8005U, 0x0000U, 0x0000U, true>  crc16_arc_parameters;
    typedef crc_parameters<uint16_t, 0x5935U, 0xFFFFU, 0x0000U, false> crc16_m17_parameters;

    // 32 bit.
    typedef crc_parameters<uint32_t, 0x04C11DB7UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, true>  crc32_parameters;
    typedef crc_parameters<uint32_t, 0x1EDC6F41UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, true>  crc32_c_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, false> crc32_bzip2_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7UL, 0xFFFFFFFFUL, 0x00000000UL, false> crc32_mpeg2_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7UL, 0x00000000UL, 0xFFFFFFFFUL, false> crc32_posix_parameters;
    typedef crc_parameters<uint32_t, 0xA833982BUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, true>  crc32_d_parameters;
    typedef crc_parameters<uint32_t, 0x814141ABUL, 0x00000000UL, 0x00000000UL, false> crc32_q_parameters;
    typedef crc_parameters<uint32_t, 0x04C11DB7UL, 0xFFFFFFFFUL, 0x00000000UL, true>  crc32_jamcrc_parameters;
    typedef crc_parameters<uint32_t, 0x000000AFUL, 0x00000000UL, 0x00000000UL, false> crc32_xfer_parameters;

    // 64 bit.
    typedef crc_parameters<uint64_t, 0x42F0E1EBA9EA3693ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, false> crc64_ecma_parameters;
  }
}

#endif
