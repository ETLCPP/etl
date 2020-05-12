///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#ifndef ETL_CRC16_TABLE_POLY_0X1EDC6F41_INCLUDED
#define ETL_CRC16_TABLE_POLY_0X1EDC6F41_INCLUDED

#include <stdint.h>

#include "../platform.h"
#include "../frame_check_sequence.h"
#include "../binary.h"
#include "../iterator.h"

#if defined(ETL_COMPILER_KEIL)
#pragma diag_suppress 1300
#endif

///\defgroup crc32_poly 0x1EDC6F41 32 bit CRC Poly 0x1EDC6F41 calculation
///\ingroup crc

namespace etl
{
  //***************************************************************************
  /// Table and add value.
  //***************************************************************************
  class crc32_table_poly_0x1edc6f41
  {
    public:

    //*************************************************************************
    uint32_t add(uint32_t crc, uint8_t value) const
    {
      static const uint32_t table[256] =
      {
        0x00000000, 0xC0C1D64F, 0xEF0EDC87, 0x2FCF0AC8, 0xF8E959E3, 0x38288FAC, 0x17E78564, 0xD726532B, 0xF31A9B51, 0x33DB4D1E, 0x1C1447D6, 0xDCD59199, 0x0BF3C2B2, 0xCB3214FD, 0xE4FD1E35, 0x243CC87A,
        0xF6E37A08, 0x3622AC47, 0x19EDA68F, 0xD92C70C0, 0x0E0A23EB, 0xCECBF5A4, 0xE104FF6C, 0x21C52923, 0x05F9E159, 0xC5383716, 0xEAF73DDE, 0x2A36EB91, 0xFD10B8BA, 0x3DD16EF5, 0x121E643D, 0xD2DFB272,
        0x7B71BD04, 0xBBB06B4B, 0x947F6183, 0x54BEB7CC, 0x8398E4E7, 0x435932A8, 0x6C963860, 0xAC57EE2F, 0x886B2655, 0x48AAF01A, 0x6765FAD2, 0xA7A42C9D, 0x70827FB6, 0xB043A9F9, 0x9F8CA331, 0x5F4D757E,
        0x8D92C70C, 0x4D531143, 0x629C1B8B, 0xA25DCDC4, 0x757B9EEF, 0xB5BA48A0, 0x9A754268, 0x5AB49427, 0x7E885C5D, 0xBE498A12, 0x918680DA, 0x51475695, 0x866105BE, 0x46A0D3F1, 0x696FD939, 0xA9AE0F76,
        0x3DB8DE82, 0xFD7908CD, 0xD2B60205, 0x1277D44A, 0xC5518761, 0x0590512E, 0x2A5F5BE6, 0xEA9E8DA9, 0xCEA245D3, 0x0E63939C, 0x21AC9954, 0xE16D4F1B, 0x364B1C30, 0xF68ACA7F, 0xD945C0B7, 0x198416F8,
        0xCB5BA48A, 0x0B9A72C5, 0x2455780D, 0xE494AE42, 0x33B2FD69, 0xF3732B26, 0xDCBC21EE, 0x1C7DF7A1, 0x38413FDB, 0xF880E994, 0xD74FE35C, 0x178E3513, 0xC0A86638, 0x0069B077, 0x2FA6BABF, 0xEF676CF0,
        0x46C96386, 0x8608B5C9, 0xA9C7BF01, 0x6906694E, 0xBE203A65, 0x7EE1EC2A, 0x512EE6E2, 0x91EF30AD, 0xB5D3F8D7, 0x75122E98, 0x5ADD2450, 0x9A1CF21F, 0x4D3AA134, 0x8DFB777B, 0xA2347DB3, 0x62F5ABFC,
        0xB02A198E, 0x70EBCFC1, 0x5F24C509, 0x9FE51346, 0x48C3406D, 0x88029622, 0xA7CD9CEA, 0x670C4AA5, 0x433082DF, 0x83F15490, 0xAC3E5E58, 0x6CFF8817, 0xBBD9DB3C, 0x7B180D73, 0x54D707BB, 0x9416D1F4,
        0x1EDC6F41, 0xDE1DB90E, 0xF1D2B3C6, 0x31136589, 0xE63536A2, 0x26F4E0ED, 0x093BEA25, 0xC9FA3C6A, 0xEDC6F410, 0x2D07225F, 0x02C82897, 0xC209FED8, 0x152FADF3, 0xD5EE7BBC, 0xFA217174, 0x3AE0A73B,
        0xE83F1549, 0x28FEC306, 0x0731C9CE, 0xC7F01F81, 0x10D64CAA, 0xD0179AE5, 0xFFD8902D, 0x3F194662, 0x1B258E18, 0xDBE45857, 0xF42B529F, 0x34EA84D0, 0xE3CCD7FB, 0x230D01B4, 0x0CC20B7C, 0xCC03DD33,
        0x65ADD245, 0xA56C040A, 0x8AA30EC2, 0x4A62D88D, 0x9D448BA6, 0x5D855DE9, 0x724A5721, 0xB28B816E, 0x96B74914, 0x56769F5B, 0x79B99593, 0xB97843DC, 0x6E5E10F7, 0xAE9FC6B8, 0x8150CC70, 0x41911A3F,
        0x934EA84D, 0x538F7E02, 0x7C4074CA, 0xBC81A285, 0x6BA7F1AE, 0xAB6627E1, 0x84A92D29, 0x4468FB66, 0x6054331C, 0xA095E553, 0x8F5AEF9B, 0x4F9B39D4, 0x98BD6AFF, 0x587CBCB0, 0x77B3B678, 0xB7726037,
        0x2364B1C3, 0xE3A5678C, 0xCC6A6D44, 0x0CABBB0B, 0xDB8DE820, 0x1B4C3E6F, 0x348334A7, 0xF442E2E8, 0xD07E2A92, 0x10BFFCDD, 0x3F70F615, 0xFFB1205A, 0x28977371, 0xE856A53E, 0xC799AFF6, 0x075879B9,
        0xD587CBCB, 0x15461D84, 0x3A89174C, 0xFA48C103, 0x2D6E9228, 0xEDAF4467, 0xC2604EAF, 0x02A198E0, 0x269D509A, 0xE65C86D5, 0xC9938C1D, 0x09525A52, 0xDE740979, 0x1EB5DF36, 0x317AD5FE, 0xF1BB03B1,
        0x58150CC7, 0x98D4DA88, 0xB71BD040, 0x77DA060F, 0xA0FC5524, 0x603D836B, 0x4FF289A3, 0x8F335FEC, 0xAB0F9796, 0x6BCE41D9, 0x44014B11, 0x84C09D5E, 0x53E6CE75, 0x9327183A, 0xBCE812F2, 0x7C29C4BD,
        0xAEF676CF, 0x6E37A080, 0x41F8AA48, 0x81397C07, 0x561F2F2C, 0x96DEF963, 0xB911F3AB, 0x79D025E4, 0x5DECED9E, 0x9D2D3BD1, 0xB2E23119, 0x7223E756, 0xA505B47D, 0x65C46232, 0x4A0B68FA, 0x8ACABEB5
      };

      return (crc << 8) ^ table[((crc >> 24) ^ value) & 0xFF];
    }
  };

  //***************************************************************************
  /// Table and add value.
  //***************************************************************************
  class crc32_table_poly_0x1edc6f41_reflected
  {
  public:

    //*************************************************************************
    uint32_t add(uint32_t crc, uint8_t value) const
    {
      static const uint32_t table[256] =
      {
        0x00000000, 0xF26B8303, 0xE13B70F7, 0x1350F3F4, 0xC79A971F, 0x35F1141C, 0x26A1E7E8, 0xD4CA64EB, 0x8AD958CF, 0x78B2DBCC, 0x6BE22838, 0x9989AB3B, 0x4D43CFD0, 0xBF284CD3, 0xAC78BF27, 0x5E133C24,
        0x105EC76F, 0xE235446C, 0xF165B798, 0x030E349B, 0xD7C45070, 0x25AFD373, 0x36FF2087, 0xC494A384, 0x9A879FA0, 0x68EC1CA3, 0x7BBCEF57, 0x89D76C54, 0x5D1D08BF, 0xAF768BBC, 0xBC267848, 0x4E4DFB4B,
        0x20BD8EDE, 0xD2D60DDD, 0xC186FE29, 0x33ED7D2A, 0xE72719C1, 0x154C9AC2, 0x061C6936, 0xF477EA35, 0xAA64D611, 0x580F5512, 0x4B5FA6E6, 0xB93425E5, 0x6DFE410E, 0x9F95C20D, 0x8CC531F9, 0x7EAEB2FA,
        0x30E349B1, 0xC288CAB2, 0xD1D83946, 0x23B3BA45, 0xF779DEAE, 0x05125DAD, 0x1642AE59, 0xE4292D5A, 0xBA3A117E, 0x4851927D, 0x5B016189, 0xA96AE28A, 0x7DA08661, 0x8FCB0562, 0x9C9BF696, 0x6EF07595,
        0x417B1DBC, 0xB3109EBF, 0xA0406D4B, 0x522BEE48, 0x86E18AA3, 0x748A09A0, 0x67DAFA54, 0x95B17957, 0xCBA24573, 0x39C9C670, 0x2A993584, 0xD8F2B687, 0x0C38D26C, 0xFE53516F, 0xED03A29B, 0x1F682198,
        0x5125DAD3, 0xA34E59D0, 0xB01EAA24, 0x42752927, 0x96BF4DCC, 0x64D4CECF, 0x77843D3B, 0x85EFBE38, 0xDBFC821C, 0x2997011F, 0x3AC7F2EB, 0xC8AC71E8, 0x1C661503, 0xEE0D9600, 0xFD5D65F4, 0x0F36E6F7,
        0x61C69362, 0x93AD1061, 0x80FDE395, 0x72966096, 0xA65C047D, 0x5437877E, 0x4767748A, 0xB50CF789, 0xEB1FCBAD, 0x197448AE, 0x0A24BB5A, 0xF84F3859, 0x2C855CB2, 0xDEEEDFB1, 0xCDBE2C45, 0x3FD5AF46,
        0x7198540D, 0x83F3D70E, 0x90A324FA, 0x62C8A7F9, 0xB602C312, 0x44694011, 0x5739B3E5, 0xA55230E6, 0xFB410CC2, 0x092A8FC1, 0x1A7A7C35, 0xE811FF36, 0x3CDB9BDD, 0xCEB018DE, 0xDDE0EB2A, 0x2F8B6829,
        0x82F63B78, 0x709DB87B, 0x63CD4B8F, 0x91A6C88C, 0x456CAC67, 0xB7072F64, 0xA457DC90, 0x563C5F93, 0x082F63B7, 0xFA44E0B4, 0xE9141340, 0x1B7F9043, 0xCFB5F4A8, 0x3DDE77AB, 0x2E8E845F, 0xDCE5075C,
        0x92A8FC17, 0x60C37F14, 0x73938CE0, 0x81F80FE3, 0x55326B08, 0xA759E80B, 0xB4091BFF, 0x466298FC, 0x1871A4D8, 0xEA1A27DB, 0xF94AD42F, 0x0B21572C, 0xDFEB33C7, 0x2D80B0C4, 0x3ED04330, 0xCCBBC033,
        0xA24BB5A6, 0x502036A5, 0x4370C551, 0xB11B4652, 0x65D122B9, 0x97BAA1BA, 0x84EA524E, 0x7681D14D, 0x2892ED69, 0xDAF96E6A, 0xC9A99D9E, 0x3BC21E9D, 0xEF087A76, 0x1D63F975, 0x0E330A81, 0xFC588982,
        0xB21572C9, 0x407EF1CA, 0x532E023E, 0xA145813D, 0x758FE5D6, 0x87E466D5, 0x94B49521, 0x66DF1622, 0x38CC2A06, 0xCAA7A905, 0xD9F75AF1, 0x2B9CD9F2, 0xFF56BD19, 0x0D3D3E1A, 0x1E6DCDEE, 0xEC064EED,
        0xC38D26C4, 0x31E6A5C7, 0x22B65633, 0xD0DDD530, 0x0417B1DB, 0xF67C32D8, 0xE52CC12C, 0x1747422F, 0x49547E0B, 0xBB3FFD08, 0xA86F0EFC, 0x5A048DFF, 0x8ECEE914, 0x7CA56A17, 0x6FF599E3, 0x9D9E1AE0,
        0xD3D3E1AB, 0x21B862A8, 0x32E8915C, 0xC083125F, 0x144976B4, 0xE622F5B7, 0xF5720643, 0x07198540, 0x590AB964, 0xAB613A67, 0xB831C993, 0x4A5A4A90, 0x9E902E7B, 0x6CFBAD78, 0x7FAB5E8C, 0x8DC0DD8F,
        0xE330A81A, 0x115B2B19, 0x020BD8ED, 0xF0605BEE, 0x24AA3F05, 0xD6C1BC06, 0xC5914FF2, 0x37FACCF1, 0x69E9F0D5, 0x9B8273D6, 0x88D28022, 0x7AB90321, 0xAE7367CA, 0x5C18E4C9, 0x4F48173D, 0xBD23943E,
        0xF36E6F75, 0x0105EC76, 0x12551F82, 0xE03E9C81, 0x34F4F86A, 0xC69F7B69, 0xD5CF889D, 0x27A40B9E, 0x79B737BA, 0x8BDCB4B9, 0x988C474D, 0x6AE7C44E, 0xBE2DA0A5, 0x4C4623A6, 0x5F16D052, 0xAD7D5351
      };

      return (crc >> 8) ^ table[(crc ^ value) & 0xFF];
    }
  };

  //***************************************************************************
  /// CRC32 Poly 0x1EDC6F41 policy.
  /// Calculates CRC32 using polynomial 0x1EDC6F41
  //***************************************************************************
  template <const uint32_t INITIAL, const uint32_t XOR_OUT, const bool REFLECT = false>
  struct crc32_policy_poly_0x1edc6f41 : public crc32_table_poly_0x1edc6f41
  {
    typedef uint32_t value_type;

    //*************************************************************************
    ETL_CONSTEXPR uint32_t initial() const
    {
      return INITIAL;
    }

    //*************************************************************************
    uint32_t final(uint32_t crc) const
    {
      return crc ^ XOR_OUT;
    }
  };

  //***************************************************************************
  /// CRC32 Poly 0x1EDC6F41 reflected policy specialisation.
  //***************************************************************************
  template <const uint32_t INITIAL, const uint32_t XOR_OUT>
  struct crc32_policy_poly_0x1edc6f41<INITIAL, XOR_OUT, true> : public crc32_table_poly_0x1edc6f41_reflected
  {
    typedef uint32_t value_type;

    //*************************************************************************
    ETL_CONSTEXPR uint32_t initial() const
    {
      return INITIAL;
    }

    //*************************************************************************
    uint32_t final(uint32_t crc) const
    {
      return crc ^ XOR_OUT;
    }
  };

  //*************************************************************************
  /// CRC32 Poly 0x1EDC6F41
  //*************************************************************************
  template <const uint32_t INITIA, const uint32_t XOR_OUT, const bool REFLECT>
  class crc32_poly_0x1edc6f41 : public etl::frame_check_sequence<etl::crc32_policy_poly_0x1edc6f41<INITIA, XOR_OUT, REFLECT> >
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    crc32_poly_0x1edc6f41()
    {
      this->reset();
    }

    //*************************************************************************
    /// Constructor from range.
    /// \param begin Start of the range.
    /// \param end   End of the range.
    //*************************************************************************
    template<typename TIterator>
    crc32_poly_0x1edc6f41(TIterator begin, const TIterator end)
    {
      this->reset();
      this->add(begin, end);
    }
  };
}

#endif
