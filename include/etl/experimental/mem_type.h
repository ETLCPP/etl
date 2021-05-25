#pragma once
template <size_t Size_>
class mem_type
{
public:

  static constexpr size_t Size = Size_;

  //***********************************
  template <size_t Other_Size>
  mem_type(const mem_type<Other_Size& other)
  {
    ETL_STATIC_ASSERT(Size >= Other_Size, "Other size is too large");

    memcpy(buffer, other.buffer, Size_);
  }

  //***********************************
  template <typename T>
  T& get()
  {
    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <typename T>
  const T& get() const
  {
    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <typename T>
  operator T() const
  {
    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <size_t Other_Size>
  mem_type& operator =(const mem_type<Other_Size>& rhs)
  {
    ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");

    memcpy(buffer, rhs.buffer, Size_);

    return *this;
  }

  //***********************************
  constexpr size_t size() const
  {
    return Size;
  }

  //***********************************
  constexpr char* data() const
  {
      return buffer;
  }

private:

  char buffer[Size]
};

template <size_t Size_>
class mem_type_ptr
{
public:

  static constexpr size_t Size = Size_;

  //***********************************
  mem_type_ptr()
    : pbuffer(ETL_NULLPTR)
  {
  }

  //***********************************
  mem_type_ptr(char* pbuffer_)
    : pbuffer(pbuffer_)
  {
  }

  //***********************************
  template <size_t Other_Size>
  mem_type_ptr(const mem_type_ptr<Other_Size& other, char* pbuffer_)
    : pbuffer(pbuffer_)
  {
      ETL_STATIC_ASSERT(Size >= Other_Size, "Other size is too large");

      memcpy(buffer, other.buffer, Size_);
  }

  //***********************************
  template <typename T>
  T& get()
  {
    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  template <typename T>
  const T& get() const
  {
    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  template <typename T>
  operator T() const
  {
    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  template <size_t Other_Size>
  mem_type_ptr& operator =(const mem_type_ptr<Other_Size>& rhs)
  {
      ETL_STATIC_ASSERT(Size >= Other_Size, "RHS size is too large");

      memcpy(pbuffer, rhs.pbuffer, Size_);

      return *this;
  }

  //***********************************
  constexpr size_t size() const
  {
    return Size;
  }

  //***********************************
  constexpr char* data() const
  {
      return pbuffer;
  }

private:

  char* pbuffer;
};
