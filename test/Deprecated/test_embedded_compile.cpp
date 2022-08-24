

#include "etl/algorithm.h"
#include "etl/alignment.h"
#include "etl/array.h"
#include "etl/bitset.h"
#include "etl/container.h"
#include "etl/crc8_ccitt.h"
#include "etl/crc16.h"
#include "etl/crc16_ccitt.h"
#include "etl/crc16_kermit.h"
#include "etl/crc32.h"
#include "etl/crc64_ecma.h"
#include "etl/cyclic_value.h"
#include "etl/deque.h"
#include "etl/io_port.h"
#include "etl/vector.h"
#include "etl/variant.h"
#include "etl/list.h"
#include "etl/map.h"
#include "etl/integral_limits.h"
#include "etl/constant.h"

#include <algorithm>

#if !defined(ETL_COMPILER_IAR) & !defined(ETL_COMPILER_TI)
#include "etl/stm32f4xx.h"
#endif

#if defined(COMPILER_KEIL)
  #pragma diag_suppress 550
  #pragma diag_suppress 177
#endif

#if defined(COMPILER_IAR)
#pragma diag_suppress = pe177
#endif

struct Test
{
  Test(int i, double d)
    : i(i),
      d(d)
  {
  }

  int i;
  double d;
};

//*****************************************************************************
// algorithm
//*****************************************************************************
void test_algorithm()
{
  int data[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  std::pair<int*, int*> result1;
  std::pair<int, int> result2;
  int  x = 0;
  int  y = 1;
  int* p;
  bool b;

  // minmax_element
  result1 = etl::minmax_element(etl::begin(data), etl::end(data));
  result1 = etl::minmax_element(etl::begin(data), etl::end(data), std::greater<int>());

  // minmax
  result2 = etl::minmax(x, y);
  result2 = etl::minmax(x, y, std::greater<int>());

  // is_sorted_until
  p = etl::is_sorted_until(etl::begin(data), etl::end(data));
  p = etl::is_sorted_until(etl::begin(data), etl::end(data), std::greater<int>());

  // is_sorted
  b = etl::is_sorted(etl::begin(data), etl::end(data));
  b = etl::is_sorted(etl::begin(data), etl::end(data), std::greater<int>());

  // copy_n
  p = etl::copy_n(etl::begin(data), 5, etl::begin(data2));

  // copy_if
  p = etl::copy_if(etl::begin(data), etl::end(data), etl::begin(data2), std::bind2nd(std::greater<int>(), 4));

  // find_if_not
  p = etl::find_if_not(etl::begin(data), etl::end(data), std::bind2nd(std::greater<int>(), 4));

  // all_of
  b = etl::all_of(etl::begin(data), etl::end(data), std::bind2nd(std::greater<int>(), 4));

  // any_of
  b = etl::any_of(etl::begin(data), etl::end(data), std::bind2nd(std::greater<int>(), 4));

  // none_of
  b = etl::none_of(etl::begin(data), etl::end(data), std::bind2nd(std::greater<int>(), 4));

  // is_permutation
  b = etl::is_permutation(etl::begin(data), etl::end(data), etl::begin(data2));
  b = etl::is_permutation(etl::begin(data), etl::end(data), etl::begin(data2), std::equal_to<int>());
  b = etl::is_permutation(etl::begin(data), etl::end(data), etl::begin(data2), etl::end(data2));
  b = etl::is_permutation(etl::begin(data), etl::end(data), etl::begin(data2), etl::end(data2), std::equal_to<int>());

  // is_partitioned
  b = etl::is_partitioned(etl::begin(data), etl::end(data), std::bind2nd(std::greater<int>(), 4));

  // partition_point
  p = etl::partition_point(etl::begin(data), etl::end(data), std::bind2nd(std::greater<int>(), 4));

  // partition_copy
  result1 = etl::partition_copy(etl::begin(data), etl::end(data), etl::begin(data2), etl::begin(data3), std::bind2nd(std::greater<int>(), 4));
}

//*****************************************************************************
// alignment
//*****************************************************************************
etl::aligned_storage<100, 8>::type data9;
etl::aligned_storage_as<100, double>::type data10;

void test_alignment()
{
  int a = static_cast<int&>(data9);

  etl::aligned_storage<1, 1>::type  data1;
  etl::aligned_storage<1, 2>::type  data2;
  etl::aligned_storage<1, 4>::type  data3;
  etl::aligned_storage<1, 8>::type  data4;

  etl::aligned_storage_as<1, char>::type   data5;
  etl::aligned_storage_as<1, short>::type  data6;
  etl::aligned_storage_as<1, int>::type    data7;
  etl::aligned_storage_as<1, double>::type data8;
}

//*****************************************************************************
// array
//*****************************************************************************
void test_array()
{
  etl::array<int, 10> a;

  int i = a[4];
  int s = a.size();
  a.fill(45);
}

//*****************************************************************************
// bitset
//*****************************************************************************
void test_bitset()
{
  etl::bitset<7>  b7;
  etl::bitset<8>  b8;
  etl::bitset<9>  b9;
  etl::bitset<15> b15;
  etl::bitset<16> b16;
  etl::bitset<17> b17;
  etl::bitset<31> b31;
  etl::bitset<32> b32;
  etl::bitset<33> b33;
  etl::bitset<63> b63;
  etl::bitset<64> b64;
  etl::bitset<65> b65;

  b65.set();
  b65.set(4, true);
  b65.reset();
  b65.reset(37);
  b65 = ~b65;
  bool b = b65[4];
  b = b65[64];
  b65.flip();
  b65.flip(5);

  etl::ibitset& ib = b65;

  ib.set();
  ib.set(4, true);
  ib.reset();
  ib.reset(37);
  b = ib[4];
  b = ib[64];
  ib.flip();
  ib.flip(5);
}

//*****************************************************************************
// crc
//*****************************************************************************
void test_crc()
{
  char data[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  uint8_t crc1 = etl::crc8_ccitt(etl::begin(data), etl::end(data));
  uint8_t crc2 = etl::crc8_ccitt(etl::begin(data), etl::end(data));

  uint16_t crc3 = etl::crc16(etl::begin(data), etl::end(data));
  uint16_t crc4 = etl::crc16(etl::begin(data), etl::end(data));

  uint16_t crc5 = etl::crc16_ccitt(etl::begin(data), etl::end(data));
  uint16_t crc6 = etl::crc16_ccitt(etl::begin(data), etl::end(data));

  uint16_t crc7 = etl::crc16_kermit(etl::begin(data), etl::end(data));
  uint16_t crc8 = etl::crc16_kermit(etl::begin(data), etl::end(data));

  uint32_t crc9  = etl::crc32(etl::begin(data), etl::end(data));
  uint32_t crc10 = etl::crc32(etl::begin(data), etl::end(data));

  uint64_t crc11 = etl::crc64_ecma(etl::begin(data), etl::end(data));
  uint64_t crc12 = etl::crc64_ecma(etl::begin(data), etl::end(data));
}

//*****************************************************************************
// deque
//*****************************************************************************
void test_cyclic_value()
{
  etl::cyclic_value<int, 1, 10> cv1;

  etl::cyclic_value<int> cv2;
  cv2.set(3, 8);

  cv1.advance(3);
  cv1.to_first();
  cv1.to_last();

  --cv1;
  ++cv1;
  int f = cv1.first();
  int l = cv1.last();

  int v = cv1;
  cv1   = v;
  cv1   = cv2;

  bool b;
  b	= cv1 == cv2;
  b = cv1 != cv2;
}

template <uintptr_t ADDRESS>
struct serial_port
{
  etl::io_port_ro<uint8_t,  ADDRESS>     rxdata;
  etl::io_port_wo<uint8_t,  ADDRESS + 1> txdata;
  etl::io_port_rw<uint16_t, ADDRESS + 2> control;
  etl::io_port_ro<uint16_t, ADDRESS + 4> status;
  etl::io_port_wos<uint8_t, ADDRESS + 6> control2;
};

struct dynamic_serial_port
{
  dynamic_serial_port(uint8_t* base)
    : rxdata(base),
      txdata(base + 1),
      control(base + 2),
      status(base + 4),
      control2(base + 6)
  {
  }

  etl::io_port_ro<uint8_t>  rxdata;
  etl::io_port_wo<uint8_t>  txdata;
  etl::io_port_rw<uint16_t> control;
  etl::io_port_ro<uint16_t> status;
  etl::io_port_wos<uint8_t> control2;
};

//*****************************************************************************
// io_port
//*****************************************************************************
void test_io_port()
{
  serial_port<0x1234U> port1;

  uint8_t rxdata  = port1.rxdata;
  port1.txdata    = 0x34U;
  port1.control   = 0x5678U; // Little endian.
  uint16_t status = port1.status;
  port1.control2  = 0xDEU;
  int control2    = port1.control2;

  uint8_t memory[7];
  dynamic_serial_port port2(memory);

  uint8_t rxdata2  = port2.rxdata;
  port2.txdata     = 0x34U;
  port2.control    = 0x5678U; // Little endian.
  uint16_t status2 = port2.status;
  port2.control2   = 0xDEU;
  int control22    = port2.control2;
}

//*****************************************************************************
// variant
//*****************************************************************************
void test_variant()
{
  typedef etl::variant<int, double, Test> Data;

  Data data;

  data = int(1);
  int i = data;

  data = double(2.2);
  double d = data;

  data = Test(3, 3.3);
  Test test(data);
}

//*****************************************************************************
// deque
//*****************************************************************************
void test_deque()
{
  typedef etl::deque<Test, 10> Data;

  Data data;

  data.push_back(Test(1, 1.1));
  data.push_back(Test(2, 2.2));

  Data::iterator it = data.begin();
  data.erase(it);
}

//*****************************************************************************
// vector
//*****************************************************************************
void test_vector()
{
  typedef etl::vector<Test, 10> Data;

  Data data;

  data.push_back(Test(1, 1.1));
  data.push_back(Test(2, 2.2));

  Data::iterator it = data.begin();
  data.erase(it);
}

//*****************************************************************************
// list
//*****************************************************************************
void test_list()
{
  typedef etl::list<Test, 10> Data;
  typedef etl::list<int, 10>  Data2;

  Data  data;
  Data2 data2;

  data.push_back(Test(1, 1.1));
  data.push_front(Test(3, 3.3));
  data.reverse();

  Data::iterator it = data.begin();
  data.erase(it);

  data2.push_back(1);
  data2.push_front(3);
  data2.reverse();

  Data2::iterator it2 = data2.begin();
  data2.erase(it2);
}

//*****************************************************************************
// map
//*****************************************************************************
void test_map()
{
  typedef etl::map<int, int, 10> Data;

  Data data;

  data.insert(std::pair<int, int>(1, 2));
  data.insert(std::pair<int, int>(3, 4));

  Data::iterator it = data.begin();
  data.erase(it);
}

//*****************************************************************************
// integral_limits
//*****************************************************************************
void test_integral_limits()
{
  static unsigned int imax = etl::integral_limits<unsigned int>::max;
  static unsigned int cmin = etl::integral_limits<char>::min;
}

//*****************************************************************************
// constant
//*****************************************************************************
void test_constant()
{
  typedef etl::constant<unsigned int, 0x12345678UL> C1;

  unsigned int i1 = C1::value;

  C1 c1;
  unsigned int i2 = c1.value;
}

//*****************************************************************************
// main
//*****************************************************************************
int main()
{
  test_algorithm();
  test_alignment();
  test_array();
  test_bitset();
  test_crc();
  test_cyclic_value();
  test_deque();
  test_vector();
  test_list();
  test_io_port();
}
