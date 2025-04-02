/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include "unit_test_framework.h"

#include "etl/scaled_rounding.h"

#include <array>

namespace
{
  //                  Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
  std::array<int, 40> source = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69, 
  //                  Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
                                -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

  SUITE(test_scaled_rounding)
  {
    //*************************************************************************
    TEST(round_ceiling_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  70,  70,  70,  70,  70,  70,  70,  70,  70,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -50, -50, -50, -50, -50, -50, -50, -50, -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_ceiling_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_ceiling_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_ceiling_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_ceiling_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_ceiling_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_ceiling_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_ceiling_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_ceiling_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_ceiling_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_ceiling_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_ceiling_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_ceiling_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_ceiling_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_ceiling_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_ceiling_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_ceiling_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_ceiling_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_ceiling_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_ceiling_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_ceiling_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_ceiling_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_ceiling_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_ceiling_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_ceiling_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_ceiling_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_ceiling_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_ceiling_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_ceiling_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_ceiling_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_ceiling_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_ceiling_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_ceiling_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_ceiling_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_ceiling_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_ceiling_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_ceiling_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_ceiling_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_ceiling_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_ceiling_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_ceiling_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_ceiling_unscaled)
    {
      //                    Index  =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  14  16  17  18  19
      //                    Source =  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69
      std::array<int, 40> expected = { 5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,
      //                    Index  =  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  34  36  37  38  39
      //                    Source = -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 -60 -61 -62 -63 -64 -65 -66 -67 -68 -69
                                      -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_ceiling_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_ceiling_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_ceiling_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_ceiling_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_ceiling_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_ceiling_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_ceiling_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_ceiling_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_ceiling_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_ceiling_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_ceiling_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_ceiling_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_ceiling_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_ceiling_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_ceiling_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_ceiling_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_ceiling_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_ceiling_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_ceiling_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_ceiling_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_ceiling_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_ceiling_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_ceiling_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_ceiling_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_ceiling_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_ceiling_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_ceiling_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_ceiling_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_ceiling_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_ceiling_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_ceiling_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_ceiling_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_ceiling_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_ceiling_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_ceiling_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_ceiling_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_ceiling_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_ceiling_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_ceiling_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_ceiling_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_ceiling_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_ceiling_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_ceiling_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_ceiling_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_ceiling_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_ceiling_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_ceiling_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_ceiling_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_ceiling_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_ceiling_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_ceiling_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_ceiling_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_ceiling_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_ceiling_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_ceiling_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_ceiling_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_ceiling_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_ceiling_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_ceiling_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_ceiling_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_ceiling_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_ceiling_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_ceiling_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_ceiling_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_ceiling_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_ceiling_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_ceiling_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_ceiling_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_ceiling_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_ceiling_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_ceiling_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_ceiling_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_ceiling_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_ceiling_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_ceiling_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_ceiling_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_ceiling_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_ceiling_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_ceiling_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_ceiling_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_ceiling_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_ceiling_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69, 
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_ceiling_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_ceiling_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_ceiling_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_ceiling_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_ceiling_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_ceiling_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_ceiling_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_ceiling_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_ceiling_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_ceiling_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_ceiling_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_ceiling_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_ceiling_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_ceiling_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_ceiling_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_ceiling_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_ceiling_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_ceiling_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_ceiling_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_ceiling_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_ceiling_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_ceiling_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_ceiling_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_ceiling_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_ceiling_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_ceiling_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_ceiling_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_ceiling_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_ceiling_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_ceiling_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_ceiling_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_ceiling_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_ceiling_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_ceiling_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_ceiling_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_ceiling_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_ceiling_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_ceiling_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_ceiling_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_ceiling_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_floor_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60, -70, -70, -70, -70, -70, -70, -70, -70, -70 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_floor_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_floor_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_floor_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_floor_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_floor_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_floor_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_floor_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_floor_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_floor_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_floor_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_floor_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_floor_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_floor_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_floor_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_floor_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_floor_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_floor_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_floor_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_floor_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_floor_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_floor_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_floor_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_floor_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_floor_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_floor_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_floor_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_floor_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_floor_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_floor_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_floor_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_floor_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_floor_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_floor_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_floor_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_floor_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_floor_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_floor_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_floor_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_floor_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_floor_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_floor_unscaled)
    {
      //                    Index  =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  14  16  17  18  19
      //                    Source =  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69
      std::array<int, 40> expected = { 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
      //                    Index  =  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  34  36  37  38  39
      //                    Source = -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 -60 -61 -62 -63 -64 -65 -66 -67 -68 -69
                                      -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7, -7, -7, -7, -7, -7 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_floor_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_floor_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_floor_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_floor_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_floor_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_floor_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_floor_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_floor_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_floor_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_floor_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_floor_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_floor_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_floor_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_floor_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_floor_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_floor_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_floor_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_floor_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_floor_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_floor_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_floor_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_floor_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_floor_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_floor_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_floor_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_floor_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_floor_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_floor_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_floor_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_floor_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_floor_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_floor_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_floor_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_floor_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_floor_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_floor_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_floor_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_floor_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_floor_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_floor_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_floor_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_floor_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_floor_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_floor_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_floor_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_floor_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_floor_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_floor_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_floor_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_floor_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_floor_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_floor_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_floor_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_floor_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_floor_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_floor_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_floor_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_floor_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_floor_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_floor_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_floor_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_floor_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_floor_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_floor_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_floor_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_floor_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_floor_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_floor_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_floor_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_floor_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_floor_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_floor_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_floor_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_floor_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_floor_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_floor_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_floor_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_floor_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_floor_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_floor_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_floor_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_floor_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69, 
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_floor_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_floor_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_floor_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_floor_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_floor_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_floor_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_floor_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_floor_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_floor_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_floor_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_floor_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_floor_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_floor_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_floor_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_floor_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_floor_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_floor_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_floor_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_floor_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_floor_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_floor_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_floor_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_floor_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_floor_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_floor_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_floor_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_floor_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_floor_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_floor_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_floor_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_floor_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_floor_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_floor_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_floor_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_floor_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_floor_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_floor_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_floor_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_floor_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_floor_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_up_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  50,  50,  50,  50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  70,  70,  70,  70,  70,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -50, -50, -50, -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60, -70, -70, -70, -70, -70 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_up_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_up_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_up_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_up_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_up_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_up_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_up_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_up_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_up_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_up_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_up_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_up_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_up_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_up_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_up_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_up_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_up_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_up_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_up_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_up_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_up_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_up_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_up_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_up_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_up_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_up_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_up_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_up_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_up_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_up_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_up_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_up_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_up_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_up_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_up_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_up_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_up_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_up_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_up_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_up_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_up_unscaled)
    {
      //                    Index  =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  14  16  17  18  19
      //                    Source =  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69
      std::array<int, 40> expected = { 5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,
      //                    Index  =  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  34  36  37  38  39
      //                    Source = -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 -60 -61 -62 -63 -64 -65 -66 -67 -68 -69
                                      -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7, -7 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_up_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_up_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_up_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_up_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_up_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_up_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_up_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_up_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_up_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_up_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_up_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_up_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_up_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_up_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_up_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_up_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_up_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_up_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_up_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_up_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_up_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_up_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_up_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_up_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_up_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_up_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_up_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_up_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_up_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_up_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_up_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_up_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_up_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_up_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_up_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_up_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_up_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_up_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_up_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_up_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_up_scaled_with_scaling_of_2)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  52,  52,  54,  54,  56,  56,  58,  58,  60,  60,  62,  62,  64,  64,  66,  66,  68,  68,  70, 
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -52, -52, -54, -54, -56, -56, -58, -58, -60, -60, -62, -62, -64, -64, -66, -66, -68, -68, -70 };

      const size_t Scale = 2;

      CHECK_EQUAL(expected[0],  etl::round_half_up_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_up_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_up_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_up_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_up_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_up_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_up_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_up_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_up_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_up_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_up_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_up_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_up_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_up_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_up_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_up_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_up_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_up_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_up_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_up_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_up_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_up_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_up_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_up_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_up_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_up_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_up_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_up_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_up_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_up_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_up_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_up_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_up_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_up_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_up_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_up_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_up_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_up_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_up_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_up_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_up_unscaled_with_scaling_of_2)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 25,  26,  26,  27,  27,  28,  28,  29,  29,  30,  30,  31,  31,  32,  32,  33,  33,  34,  34,  35, 
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -25, -26, -26, -27, -27, -28, -28, -29, -29, -30, -30, -31, -31, -32, -32, -33, -33, -34, -34, -35 };

      const size_t Scale = 2;

      CHECK_EQUAL(expected[0],  etl::round_half_up_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_up_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_up_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_up_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_up_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_up_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_up_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_up_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_up_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_up_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_up_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_up_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_up_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_up_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_up_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_up_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_up_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_up_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_up_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_up_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_up_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_up_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_up_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_up_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_up_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_up_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_up_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_up_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_up_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_up_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_up_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_up_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_up_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_up_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_up_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_up_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_up_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_up_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_up_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_up_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_down_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  50,  50,  50,  50,  50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  70,  70,  70,  70,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -50, -50, -50, -50, -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60, -70, -70, -70, -70 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_down_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_down_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_down_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_down_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_down_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_down_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_down_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_down_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_down_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_down_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_down_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_down_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_down_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_down_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_down_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_down_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_down_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_down_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_down_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_down_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_down_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_down_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_down_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_down_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_down_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_down_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_down_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_down_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_down_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_down_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_down_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_down_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_down_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_down_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_down_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_down_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_down_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_down_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_down_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_down_scaled<Scale>(source[39]));
    }

    ////*************************************************************************
    TEST(round_half_down_unscaled)
    {
      //                    Index  =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  14  16  17  18  19
      //                    Source =  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69
      std::array<int, 40> expected = { 5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,
      //                    Index  =  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  34  36  37  38  39
      //                    Source = -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 -60 -61 -62 -63 -64 -65 -66 -67 -68 -69
                                      -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_down_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_down_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_down_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_down_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_down_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_down_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_down_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_down_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_down_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_down_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_down_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_down_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_down_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_down_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_down_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_down_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_down_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_down_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_down_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_down_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_down_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_down_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_down_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_down_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_down_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_down_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_down_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_down_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_down_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_down_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_down_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_down_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_down_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_down_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_down_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_down_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_down_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_down_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_down_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_down_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_down_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_half_down_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_down_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_down_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_down_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_down_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_down_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_down_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_down_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_down_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_down_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_down_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_down_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_down_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_down_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_down_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_down_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_down_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_down_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_down_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_down_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_down_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_down_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_down_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_down_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_down_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_down_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_down_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_down_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_down_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_down_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_down_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_down_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_down_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_down_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_down_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_down_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_down_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_down_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_down_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_down_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_down_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_half_down_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_down_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_down_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_down_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_down_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_down_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_down_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_down_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_down_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_down_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_down_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_down_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_down_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_down_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_down_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_down_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_down_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_down_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_down_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_down_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_down_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_down_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_down_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_down_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_down_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_down_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_down_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_down_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_down_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_down_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_down_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_down_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_down_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_down_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_down_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_down_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_down_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_down_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_down_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_down_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_zero_scaled)
    {
    //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
    //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
    std::array<int, 40> expected = { 50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,
    //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
    //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                    -50, -50, -50, -50, -50, -50, -50, -50, -50, -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_zero_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_zero_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_zero_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_zero_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_zero_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_zero_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_zero_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_zero_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_zero_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_zero_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_zero_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_zero_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_zero_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_zero_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_zero_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_zero_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_zero_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_zero_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_zero_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_zero_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_zero_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_zero_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_zero_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_zero_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_zero_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_zero_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_zero_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_zero_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_zero_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_zero_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_zero_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_zero_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_zero_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_zero_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_zero_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_zero_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_zero_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_zero_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_zero_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_zero_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_zero_unscaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = {  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                       -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_zero_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_zero_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_zero_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_zero_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_zero_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_zero_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_zero_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_zero_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_zero_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_zero_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_zero_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_zero_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_zero_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_zero_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_zero_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_zero_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_zero_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_zero_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_zero_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_zero_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_zero_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_zero_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_zero_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_zero_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_zero_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_zero_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_zero_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_zero_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_zero_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_zero_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_zero_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_zero_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_zero_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_zero_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_zero_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_zero_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_zero_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_zero_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_zero_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_zero_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_zero_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_zero_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_zero_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_zero_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_zero_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_zero_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_zero_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_zero_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_zero_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_zero_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_zero_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_zero_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_zero_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_zero_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_zero_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_zero_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_zero_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_zero_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_zero_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_zero_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_zero_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_zero_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_zero_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_zero_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_zero_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_zero_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_zero_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_zero_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_zero_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_zero_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_zero_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_zero_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_zero_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_zero_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_zero_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_zero_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_zero_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_zero_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_zero_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_zero_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_zero_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_zero_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_zero_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_zero_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_zero_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_zero_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_zero_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_zero_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_zero_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_zero_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_zero_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_zero_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_zero_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_zero_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_zero_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_zero_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_zero_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_zero_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_zero_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_zero_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_zero_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_zero_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_zero_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_zero_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_zero_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_zero_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_zero_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_zero_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_zero_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_zero_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_zero_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_zero_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_zero_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_zero_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_zero_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_zero_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_zero_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_zero_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_zero_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_zero_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_zero_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_zero_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_infinity_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  70,  70,  70,  70,  70,  70,  70,  70,  70,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60, -70, -70, -70, -70, -70, -70, -70, -70, -70 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_infinity_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_infinity_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_infinity_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_infinity_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_infinity_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_infinity_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_infinity_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_infinity_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_infinity_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_infinity_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_infinity_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_infinity_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_infinity_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_infinity_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_infinity_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_infinity_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_infinity_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_infinity_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_infinity_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_infinity_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_infinity_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_infinity_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_infinity_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_infinity_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_infinity_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_infinity_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_infinity_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_infinity_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_infinity_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_infinity_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_infinity_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_infinity_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_infinity_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_infinity_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_infinity_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_infinity_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_infinity_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_infinity_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_infinity_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_infinity_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_infinity_unscaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = {  5,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,   7,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                       -5,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6,  -7,  -7,  -7,  -7,  -7,  -7,  -7,  -7,  -7 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_infinity_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_infinity_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_infinity_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_infinity_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_infinity_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_infinity_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_infinity_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_infinity_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_infinity_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_infinity_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_infinity_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_infinity_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_infinity_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_infinity_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_infinity_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_infinity_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_infinity_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_infinity_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_infinity_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_infinity_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_infinity_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_infinity_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_infinity_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_infinity_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_infinity_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_infinity_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_infinity_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_infinity_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_infinity_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_infinity_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_infinity_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_infinity_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_infinity_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_infinity_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_infinity_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_infinity_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_infinity_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_infinity_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_infinity_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_infinity_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_infinity_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_infinity_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_infinity_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_infinity_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_infinity_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_infinity_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_infinity_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_infinity_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_infinity_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_infinity_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_infinity_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_infinity_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_infinity_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_infinity_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_infinity_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_infinity_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_infinity_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_infinity_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_infinity_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_infinity_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_infinity_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_infinity_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_infinity_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_infinity_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_infinity_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_infinity_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_infinity_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_infinity_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_infinity_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_infinity_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_infinity_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_infinity_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_infinity_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_infinity_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_infinity_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_infinity_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_infinity_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_infinity_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_infinity_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_infinity_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_infinity_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_infinity_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_infinity_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_infinity_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_infinity_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_infinity_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_infinity_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_infinity_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_infinity_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_infinity_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_infinity_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_infinity_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_infinity_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_infinity_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_infinity_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_infinity_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_infinity_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_infinity_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_infinity_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_infinity_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_infinity_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_infinity_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_infinity_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_infinity_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_infinity_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_infinity_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_infinity_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_infinity_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_infinity_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_infinity_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_infinity_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_infinity_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_infinity_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_infinity_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_infinity_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_infinity_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_infinity_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_infinity_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_infinity_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_infinity_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_infinity_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_infinity_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_even_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  50,  50,  50,  50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  70,  70,  70,  70,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -50, -50, -50, -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60, -60, -70, -70, -70, -70 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_even_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_even_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_even_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_even_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_even_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_even_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_even_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_even_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_even_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_even_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_even_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_even_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_even_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_even_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_even_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_even_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_even_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_even_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_even_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_even_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_even_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_even_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_even_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_even_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_even_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_even_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_even_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_even_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_even_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_even_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_even_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_even_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_even_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_even_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_even_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_even_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_even_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_even_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_even_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_even_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_even_unscaled)
    {
      //                    Index  =    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  14  16  17  18  19
      //                    Source =   50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69
      std::array<int, 40> expected = {  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,
      //                    Index  =   20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  34  36  37  38  39
      //                    Source =  -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 -60 -61 -62 -63 -64 -65 -66 -67 -68 -69
                                       -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_even_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_even_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_even_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_even_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_even_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_even_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_even_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_even_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_even_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_even_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_even_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_even_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_even_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_even_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_even_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_even_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_even_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_even_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_even_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_even_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_even_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_even_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_even_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_even_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_even_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_even_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_even_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_even_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_even_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_even_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_even_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_even_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_even_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_even_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_even_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_even_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_even_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_even_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_even_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_even_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_even_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_half_even_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_even_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_even_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_even_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_even_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_even_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_even_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_even_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_even_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_even_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_even_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_even_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_even_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_even_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_even_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_even_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_even_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_even_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_even_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_even_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_even_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_even_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_even_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_even_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_even_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_even_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_even_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_even_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_even_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_even_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_even_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_even_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_even_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_even_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_even_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_even_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_even_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_even_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_even_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_even_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_even_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_half_even_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_even_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_even_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_even_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_even_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_even_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_even_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_even_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_even_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_even_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_even_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_even_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_even_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_even_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_even_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_even_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_even_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_even_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_even_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_even_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_even_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_even_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_even_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_even_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_even_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_even_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_even_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_even_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_even_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_even_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_even_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_even_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_even_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_even_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_even_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_even_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_even_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_even_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_even_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_even_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_odd_scaled)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  50,  50,  50,  50,  50,  60,  60,  60,  60,  60,  60,  60,  60,  60,  70,  70,  70,  70,  70,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -50, -50, -50, -50, -50, -60, -60, -60, -60, -60, -60, -60, -60, -60, -70, -70, -70, -70, -70 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_odd_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_odd_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_odd_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_odd_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_odd_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_odd_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_odd_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_odd_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_odd_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_odd_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_odd_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_odd_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_odd_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_odd_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_odd_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_odd_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_odd_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_odd_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_odd_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_odd_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_odd_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_odd_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_odd_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_odd_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_odd_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_odd_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_odd_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_odd_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_odd_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_odd_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_odd_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_odd_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_odd_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_odd_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_odd_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_odd_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_odd_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_odd_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_odd_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_odd_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_odd_unscaled)
    {
      //                    Index  =    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  14  16  17  18  19
      //                    Source =   50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69
      std::array<int, 40> expected = {  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,
      //                    Index  =   20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  34  36  37  38  39
      //                    Source =  -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 -60 -61 -62 -63 -64 -65 -66 -67 -68 -69
                                       -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7, -7 };

      const size_t Scale = 10;

      CHECK_EQUAL(expected[0],  etl::round_half_odd_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_odd_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_odd_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_odd_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_odd_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_odd_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_odd_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_odd_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_odd_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_odd_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_odd_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_odd_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_odd_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_odd_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_odd_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_odd_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_odd_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_odd_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_odd_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_odd_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_odd_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_odd_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_odd_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_odd_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_odd_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_odd_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_odd_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_odd_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_odd_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_odd_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_odd_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_odd_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_odd_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_odd_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_odd_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_odd_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_odd_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_odd_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_odd_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_odd_unscaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_odd_scaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_half_odd_scaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_odd_scaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_odd_scaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_odd_scaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_odd_scaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_odd_scaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_odd_scaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_odd_scaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_odd_scaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_odd_scaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_odd_scaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_odd_scaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_odd_scaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_odd_scaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_odd_scaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_odd_scaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_odd_scaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_odd_scaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_odd_scaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_odd_scaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_odd_scaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_odd_scaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_odd_scaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_odd_scaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_odd_scaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_odd_scaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_odd_scaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_odd_scaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_odd_scaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_odd_scaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_odd_scaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_odd_scaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_odd_scaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_odd_scaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_odd_scaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_odd_scaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_odd_scaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_odd_scaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_odd_scaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_odd_scaled<Scale>(source[39]));
    }

    //*************************************************************************
    TEST(round_half_odd_unscaled_with_scaling_of_1)
    {
      //                    Index  =    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   14   16   17   18   19
      //                    Source =   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69
      std::array<int, 40> expected = { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
      //                    Index  =   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   34   36   37   38   39
      //                    Source =  -50  -51  -52  -53  -54  -55  -56  -57  -58  -59  -60  -61  -62  -63  -64  -65  -66  -67  -68  -69
                                      -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69 };

      const size_t Scale = 1;

      CHECK_EQUAL(expected[0],  etl::round_half_odd_unscaled<Scale>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_odd_unscaled<Scale>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_odd_unscaled<Scale>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_odd_unscaled<Scale>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_odd_unscaled<Scale>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_odd_unscaled<Scale>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_odd_unscaled<Scale>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_odd_unscaled<Scale>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_odd_unscaled<Scale>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_odd_unscaled<Scale>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_odd_unscaled<Scale>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_odd_unscaled<Scale>(source[11]));
      CHECK_EQUAL(expected[12], etl::round_half_odd_unscaled<Scale>(source[12]));
      CHECK_EQUAL(expected[13], etl::round_half_odd_unscaled<Scale>(source[13]));
      CHECK_EQUAL(expected[14], etl::round_half_odd_unscaled<Scale>(source[14]));
      CHECK_EQUAL(expected[15], etl::round_half_odd_unscaled<Scale>(source[15]));
      CHECK_EQUAL(expected[16], etl::round_half_odd_unscaled<Scale>(source[16]));
      CHECK_EQUAL(expected[17], etl::round_half_odd_unscaled<Scale>(source[17]));
      CHECK_EQUAL(expected[18], etl::round_half_odd_unscaled<Scale>(source[18]));
      CHECK_EQUAL(expected[19], etl::round_half_odd_unscaled<Scale>(source[19]));

      CHECK_EQUAL(expected[20], etl::round_half_odd_unscaled<Scale>(source[20]));
      CHECK_EQUAL(expected[21], etl::round_half_odd_unscaled<Scale>(source[21]));
      CHECK_EQUAL(expected[22], etl::round_half_odd_unscaled<Scale>(source[22]));
      CHECK_EQUAL(expected[23], etl::round_half_odd_unscaled<Scale>(source[23]));
      CHECK_EQUAL(expected[24], etl::round_half_odd_unscaled<Scale>(source[24]));
      CHECK_EQUAL(expected[25], etl::round_half_odd_unscaled<Scale>(source[25]));
      CHECK_EQUAL(expected[26], etl::round_half_odd_unscaled<Scale>(source[26]));
      CHECK_EQUAL(expected[27], etl::round_half_odd_unscaled<Scale>(source[27]));
      CHECK_EQUAL(expected[28], etl::round_half_odd_unscaled<Scale>(source[28]));
      CHECK_EQUAL(expected[29], etl::round_half_odd_unscaled<Scale>(source[29]));
      CHECK_EQUAL(expected[30], etl::round_half_odd_unscaled<Scale>(source[30]));
      CHECK_EQUAL(expected[31], etl::round_half_odd_unscaled<Scale>(source[31]));
      CHECK_EQUAL(expected[32], etl::round_half_odd_unscaled<Scale>(source[32]));
      CHECK_EQUAL(expected[33], etl::round_half_odd_unscaled<Scale>(source[33]));
      CHECK_EQUAL(expected[34], etl::round_half_odd_unscaled<Scale>(source[34]));
      CHECK_EQUAL(expected[35], etl::round_half_odd_unscaled<Scale>(source[35]));
      CHECK_EQUAL(expected[36], etl::round_half_odd_unscaled<Scale>(source[36]));
      CHECK_EQUAL(expected[37], etl::round_half_odd_unscaled<Scale>(source[37]));
      CHECK_EQUAL(expected[38], etl::round_half_odd_unscaled<Scale>(source[38]));
      CHECK_EQUAL(expected[39], etl::round_half_odd_unscaled<Scale>(source[39]));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(round_constexpr_scaled_rounding)
    {
      constexpr int round_ceiling          = etl::round_ceiling_scaled<10>(10);
      constexpr int round_floor            = etl::round_floor_scaled<10>(10);
      constexpr int round_half_down_scaled = etl::round_half_down_scaled<10>(10);
      constexpr int round_half_even_scaled = etl::round_half_even_scaled<10>(10);
      constexpr int round_half_odd_scaled  = etl::round_half_odd_scaled<10>(10);
      constexpr int round_half_up_scaled   = etl::round_half_up_scaled<10>(10);
      constexpr int round_infinity         = etl::round_infinity_scaled<10>(10);
      constexpr int round_zero             = etl::round_zero_scaled<10>(10);

      // Ensure that the value are constexpr.
      std::array<char, round_ceiling>          a{};
      std::array<char, round_floor>            b{};
      std::array<char, round_half_down_scaled> c{};
      std::array<char, round_half_even_scaled> d{};
      std::array<char, round_half_odd_scaled>  e{};
      std::array<char, round_half_up_scaled>   f{};
      std::array<char, round_infinity>         g{};
      std::array<char, round_zero>             h{};

      (void)a;
      (void)b;
      (void)c;
      (void)d;
      (void)e;
      (void)f;
      (void)g;
      (void)h;

      CHECK_EQUAL(10, round_ceiling);
      CHECK_EQUAL(10, round_floor);
      CHECK_EQUAL(10, round_half_down_scaled);
      CHECK_EQUAL(10, round_half_even_scaled);
      CHECK_EQUAL(10, round_half_odd_scaled);
      CHECK_EQUAL(10, round_half_up_scaled);
      CHECK_EQUAL(10, round_infinity);
      CHECK_EQUAL(10, round_zero);
    }
#endif
  };
}
