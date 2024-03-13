/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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

#include "etl/message.h"

SUITE(test_message)
{
  //*************************************************************************
  TEST(test_message_traits)
  {
    // imessage
    //   ^
    // MessageBase 
    //   ^
    // etl::message<I, MessageBase>
    //   ^
    // Message

    //             | is_imessage | is_message | is_message_type | is_message_base | is_user_message_base
    // imessage    |     T       |     T      |     F           |     T           |     F
    // MessageBase |     F       |     T      |     F           |     T           |     T
    // Message     |     F       |     T      |     T           |     F           |     F
    // int         |     F       |     F      |     F           |     F           |     F

    struct MessageBase : public etl::imessage {};
    struct Message : public etl::message<1, MessageBase> {};

#if ETL_USING_CPP17
    // Is an imessage?
    CHECK_TRUE(etl::is_imessage_v<etl::imessage>);
    CHECK_FALSE(etl::is_imessage_v<MessageBase>);
    CHECK_FALSE(etl::is_imessage_v<Message>);
    CHECK_FALSE(etl::is_imessage_v<int>);

    // Is an imessage, or derived from imessage?
    CHECK_TRUE(etl::is_message_v<etl::imessage>);
    CHECK_TRUE(etl::is_message_v<MessageBase>);
    CHECK_TRUE(etl::is_message_v<Message>);
    CHECK_FALSE(etl::is_message_v<int>);

    // Is a concrete message?
    CHECK_FALSE(etl::is_message_type_v<etl::imessage>);
    CHECK_FALSE(etl::is_message_type_v<MessageBase>);
    CHECK_TRUE(etl::is_message_type_v<Message>);
    CHECK_FALSE(etl::is_message_type_v<int>);

    // Is a message base?
    CHECK_TRUE(etl::is_message_base_v<etl::imessage>);
    CHECK_TRUE(etl::is_message_base_v<MessageBase>);
    CHECK_FALSE(etl::is_message_base_v<Message>);
    CHECK_FALSE(etl::is_message_base_v<int>);

    // Is a user message base?
    CHECK_FALSE(etl::is_user_message_base_v<etl::imessage>);
    CHECK_TRUE(etl::is_user_message_base_v<MessageBase>);
    CHECK_FALSE(etl::is_user_message_base_v<Message>);
    CHECK_FALSE(etl::is_user_message_base_v<int>);
#else
    // Is an imessage?
    CHECK_TRUE(etl::is_imessage<etl::imessage>::value);
    CHECK_FALSE(etl::is_imessage<MessageBase>::value);
    CHECK_FALSE(etl::is_imessage<Message>::value);
    CHECK_FALSE(etl::is_imessage<int>::value);

    // Is an imessage, or derived from imessage?
    CHECK_TRUE(etl::is_message<etl::imessage>::value);
    CHECK_TRUE(etl::is_message<MessageBase>::value);
    CHECK_TRUE(etl::is_message<Message>::value);
    CHECK_FALSE(etl::is_message<int>::value);

    // Is a concrete message?
    CHECK_FALSE(etl::is_message_type<etl::imessage>::value);
    CHECK_FALSE(etl::is_message_type<MessageBase>::value);
    CHECK_TRUE(etl::is_message_type<Message>::value);
    CHECK_FALSE(etl::is_message_type<int>::value);

    // Is a message base?
    CHECK_TRUE(etl::is_message_base<etl::imessage>::value);
    CHECK_TRUE(etl::is_message_base<MessageBase>::value);
    CHECK_FALSE(etl::is_message_base<Message>::value);
    CHECK_FALSE(etl::is_message_base<int>::value);

    // Is a user message base?
    CHECK_FALSE(etl::is_user_message_base<etl::imessage>::value);
    CHECK_TRUE(etl::is_user_message_base<MessageBase>::value);
    CHECK_FALSE(etl::is_user_message_base<Message>::value);
    CHECK_FALSE(etl::is_user_message_base<int>::value);
#endif
  }

  //*************************************************************************
  TEST(test_message_base_type)
  {
    struct Message1 : public etl::message<1> {};
    
    struct MessageBase : public etl::imessage {};
    struct Message2 : public etl::message<2, MessageBase> {};

    CHECK_TRUE((std::is_same_v<etl::imessage, Message1::base_type>));
    CHECK_TRUE((std::is_same_v<MessageBase,   Message2::base_type>));

    CHECK_FALSE((std::is_same_v<MessageBase,   Message1::base_type>));
    CHECK_FALSE((std::is_same_v<etl::imessage, Message2::base_type>));
  }
};
