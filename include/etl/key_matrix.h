

#include "platform.h"
#include "array.h"
#include "binary.h"
#include "debounce_multi.h"
#include "smallest.h"

#include <stdint.h>

//******************************************************************************
// A key matrix decoder.
// \tparam N_Rows              The number of rows in the key matrix. Rows are read from the key matrix.
// \tparam N_Columns           The number of columns in the key matrix. Columns are written to the key matrix.
// \tparam Key_Matrix_Polarity The polarity of the key matrix. Active_High = pressed key reads as '1', Active_Low = pressed key reads as '0'.
// \tparam TCount              The type of the per-bit counters. Must be able to hold the largest of Valid_Count, Hold_Count and Repeat_Count.
// \tparam TRead               The type of the read value (uint8_t/16_t/32_t/64_t).
// \tparam TWrite              The type of the write value (uint8_t/16_t/32_t/64_t).
// \tparam Valid_Count         The number of consecutive samples a bit must differ from its previous debounced state before the state changes (>= 1).
// \tparam Hold_Count          The number of samples a bit must remain set (after becoming valid) before a 'held' event is generated. 0 = disabled.
// \tparam Repeat_Count        Once held, a 'repeat' event is generated every Repeat_Count samples. 0 = disabled.
//******************************************************************************
template <size_t N_Rows, size_t N_Columns, etl::bit_polarity::type Polarity, typename TCount, typename TRead, typename TWrite, uint32_t Valid_Count,
          uint32_t Hold_Count = 0, uint32_t Repeat_Count = 0>
class key_matrix
{
public:

  typedef typename TRead                                                 read_type;
  typedef typename TWrite                                                write_type;
  typedef typename TCount                                                count_type;
  typedef typename etl::smallest_uint_for_bits<N_Rows + N_Columns>::type return_type;

  //***************************************************************************
  /// Constructor.
  //***************************************************************************
  key_matrix()
    : column_bit(1)
    ,
  {
    write_type initial_value = static_cast<write_type>(Key_Matrix_Polarity == key_matrix_polarity::Active_High ? 0 : ~0);

    for (size_t i = 0; i < N_Columns; ++i)
    {
      rows[i].reset(initial_value);
    }
  }

  //***************************************************************************
  /// Destructor.
  //***************************************************************************
  virtual ~key_matrix() {}

  //***************************************************************************
  /// Write a value to the key matrix.
  ///\param write_value The value to write to the key matrix columns.
  //***************************************************************************
  virtual void write_column(write_type write_value) = 0;

  //***************************************************************************
  /// Read a value from the key matrix.
  ///\return The value read from the key matrix rows.
  //***************************************************************************
  virtual read_type read_row() = 0;

  //***************************************************************************
  /// Reset the key matrix.
  //***************************************************************************
  void reset()
  {
    column_bit = 1;

    for (size_t i = 0; i < N_Columns; ++i)
    {
      rows[i].reset();
    }
  }

  //***************************************************************************
  /// Scan the key matrix.
  /// This function must be called repeatedly to scan the key matrix.
  /// Returns 'true' if all of the rows have been processed, otherwise 'false'.
  bool scan_incremental()
  {
    write_column(column_bit);

    read_type read_value = read_row();

    bool all_rows_processed = rows[column_bit].update_incremental(read_value);

    ++column_index;

    if (column_index == N_Columns)
    {
      column_index = 0;
    }

    return all_rows_processed;
  }

private:

  typedef etl::debounce_multi<read_type, TCount, N_Rows, Valid_Count, Hold_Count, Repeat_Count> row_debouncer_type;

  etl::array<row_debouncer_type, N_Columns> rows;

  write_type initial_value;
  write_type column_bit;
};
