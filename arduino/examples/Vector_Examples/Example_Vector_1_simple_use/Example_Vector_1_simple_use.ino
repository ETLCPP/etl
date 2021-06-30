// make sure that we do not rely on the STL
#define ETL_NO_STL

#include "Embedded_Template_Library.h"
#include "etl/vector.h"

template <typename T>
void print_vector(etl::ivector<T> const & vec_in)
{
  Serial.print(F("print vector content | size ")); Serial.print(vec_in.size()); Serial.print(F(" | capacity ")); Serial.println(vec_in.capacity());
  Serial.print(F("content | "));

  for (T const & elem : vec_in) 
  {
    Serial.print(elem);
    Serial.print(F(" | "));
  }

  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println(F("booted"));

  etl::vector<int, 12> vec_int;

  Serial.println(F("initialized vec_int"));
  print_vector(vec_int);

  vec_int.push_back(1);
  vec_int.push_back(2);
  Serial.println(F("pushed to vec_int"));
  print_vector(vec_int);

  vec_int.pop_back();
  Serial.println(F("pop from vec_int; returns no value"));
  print_vector(vec_int);
}

void loop()
{
  
}
