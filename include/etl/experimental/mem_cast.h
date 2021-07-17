#pragma once
template <size_t Size_>
class uni_type
{
public:

  static constexpr size_t Size = Size_;

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

  template <typename T>
  operator T()
  {
    return *reinterpret_cast<T*>(buffer);
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