
#include "algorithm.h"
#include "alignment.h"
#include "array.h"
#include "bitset.h"
#include "container.h"
#include "crc8_ccitt.h"
#include "crc16.h"
#include "crc16_ccitt.h"
#include "crc16_kermit.h"
#include "crc32.h"
#include "crc64_ecma.h"
#include "cyclic_value.h"
#include "deque.h"

#if !defined(COMPILER_IAR)
#include "variant.h"
#endif

#if defined(COMPILER_KEIL)
  #pragma diag_suppress 550
  #pragma diag_suppress 177
#endif

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
etl::align_at<int, 16> data9;
etl::align_at<char, 16> data10;

void test_alignment()
{
  data9.value = 0;
  data10.value = 0;
  
	etl::align_at<char, 1>  data1;
	etl::align_at<char, 2>  data2;
	etl::align_at<char, 4>  data3;
	etl::align_at<char, 8>  data4;
	
	etl::align_as<char, char>   data5;
	etl::align_as<char, short>  data6;
	etl::align_as<char, int>    data7;
	etl::align_as<char, double> data8;
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
	etl::bitset<7>  b7;  // uint8_t
	etl::bitset<8>  b8;  // uint8_t
	etl::bitset<9>  b9;  // uint16_t
	etl::bitset<15> b15; // uint16_t
	etl::bitset<16> b16; // uint16_t
	etl::bitset<17> b17; // uint32_t
	etl::bitset<31> b31; // uint32_t
	etl::bitset<32> b32; // uint32_t
	etl::bitset<33> b33; // uint64_t
	etl::bitset<63> b63; // uint64_t
	etl::bitset<64> b64; // uint64_t
	etl::bitset<65> b65; // 2 * uint64_t
	
	b65.set();
	b65.set(4, true);
	b65.reset();
	b65.reset(37);
	b65 = ~b65;
	bool b = b65[4];
	b = b65[64];
	b65.flip();
	b65.flip(5);
	
	etl::bitset<7>::iterator b1 = b7.begin();
	etl::bitset<7>::iterator e1 = b7.end();
	etl::bitset<7>::const_iterator b2 = b7.cbegin();
	etl::bitset<7>::const_iterator e2 = b7.cend();
	
	++b1;
	--e1;
	b2 += 2;
	e2 -= 2;
	
	*b1 = true;

	const etl::bitset<7>::iterator b3 = b7.begin();
	bool t = *b3;
}

//*****************************************************************************
// crc
//*****************************************************************************
void test_crc()
{
	int data[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	
	uint8_t crc1 = etl::crc8_ccitt<>(etl::begin(data), etl::end(data));
	uint8_t crc2 = etl::crc8_ccitt<etl::endian::big>(etl::begin(data), etl::end(data));

	uint16_t crc3 = etl::crc16<>(etl::begin(data), etl::end(data));
	uint16_t crc4 = etl::crc16<etl::endian::big>(etl::begin(data), etl::end(data));
	
	uint16_t crc5 = etl::crc16_ccitt<>(etl::begin(data), etl::end(data));
	uint16_t crc6 = etl::crc16_ccitt<etl::endian::big>(etl::begin(data), etl::end(data));

	uint16_t crc7 = etl::crc16_kermit<>(etl::begin(data), etl::end(data));
	uint16_t crc8 = etl::crc16_kermit<etl::endian::big>(etl::begin(data), etl::end(data));
	
	uint32_t crc9  = etl::crc32<>(etl::begin(data), etl::end(data));
	uint32_t crc10 = etl::crc32<etl::endian::big>(etl::begin(data), etl::end(data));
	
	uint64_t crc11 = etl::crc64_ecma<>(etl::begin(data), etl::end(data));
	uint64_t crc12 = etl::crc64_ecma<etl::endian::big>(etl::begin(data), etl::end(data));	
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

//*****************************************************************************
// cyclic_value
//*****************************************************************************
void test_deque()
{
	
}

//*****************************************************************************
// main
//*****************************************************************************
int main()
{
  test_alignment();
}
