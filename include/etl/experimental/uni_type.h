#pragma once
template <size_t Size_>
class uni_type
{
public:

  static constexpr size_t Size = Size_;

    //***********************************
  template <typename T>
  uni_type(const uni_type& other)
  {
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
  operator T()
  {
    return *reinterpret_cast<T*>(buffer);
  }

  //***********************************
  template <typename T>
  uni_type& operator =(const uni_type& rhs)
  {
    memcpy(buffer, rhs.buffer, Size_);

    return *this;
  }

  //***********************************
  constexpr size_t size() const
  {
    return Size;
  }

private:

  char buffer[Size]
};

template <size_t Size_>
class uni_type_ptr
{
public:

  static constexpr size_t Size = Size_;

  //***********************************
  uni_type_ptr()
    : pbuffer(ETL_NULLPTR)
  {
  }

  //***********************************
  uni_type_ptr(void* pbuffer_)
    : pbuffer(reinterpret_cast<char*>(pbuffer))
  {
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

  template <typename T>
  operator T()
  {
    return *reinterpret_cast<T*>(pbuffer);
  }

  //***********************************
  constexpr size_t size() const
  {
    return Size;
  }

private:

  char* pbuffer;
};
