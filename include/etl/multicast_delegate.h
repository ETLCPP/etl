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

/******************************************************************************

	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

******************************************************************************/

//#pragma once
//#include "delegate.h"
//#include <list>
//#include <functional>
//
//namespace SA {
//
//	template<typename RET, typename ...PARAMS>
//	class multicast_delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
//	public:
//
//		multicast_delegate() = default;
//		~multicast_delegate() {
//			for (auto& element : invocationList) delete element;
//			invocationList.clear();
//		} //~multicast_delegate
//
//		bool isNull() const { return invocationList.size() < 1; }
//		bool operator ==(void* ptr) const {
//			return (ptr == nullptr) && this->isNull();
//		} //operator ==
//		bool operator !=(void* ptr) const {
//			return (ptr != nullptr) || (!this->isNull());
//		} //operator !=
//
//		size_t size() const { return invocationList.size(); }
//
//		multicast_delegate& operator =(const multicast_delegate&) = delete;
//		multicast_delegate(const multicast_delegate&) = delete;
//
//		bool operator ==(const multicast_delegate& another) const {
//			if (invocationList.size() != another.invocationList.size()) return false;
//			auto anotherIt = another.invocationList.begin();
//			for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
//				if (**it != **anotherIt) return false;
//			return true;
//		} //==
//		bool operator !=(const multicast_delegate& another) const { return !(*this == another); }
//
//		bool operator ==(const delegate<RET(PARAMS...)>& another) const {
//			if (isNull() && another.isNull()) return true;
//			if (another.isNull() || (size() != 1)) return false;
//			return (another.invocation == **invocationList.begin());
//		} //==
//		bool operator !=(const delegate<RET(PARAMS...)>& another) const { return !(*this == another); }
//
//		multicast_delegate& operator +=(const multicast_delegate& another) {
//			for (auto& item : another.invocationList) // clone, not copy; flattens hierarchy:
//				this->invocationList.push_back(new typename delegate_base<RET(PARAMS...)>::InvocationElement(item->object, item->stub));
//			return *this;
//		} //operator +=
//
//		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
//		multicast_delegate& operator +=(const LAMBDA & lambda) {
//			delegate<RET(PARAMS...)> d = delegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
//			return *this += d;
//		} //operator +=
//
//		multicast_delegate& operator +=(const delegate<RET(PARAMS...)>& another) {
//			if (another.isNull()) return *this;
//			this->invocationList.push_back(new typename delegate_base<RET(PARAMS...)>::InvocationElement(another.invocation.object, another.invocation.stub));
//			return *this;
//		} //operator +=
//
//		// will work even if RET is void, return values are ignored:
//		// (for handling return values, see operator(..., handler))
//		void operator()(PARAMS... arg) const {
//			for (auto& item : invocationList)
//				(*(item->stub))(item->object, arg...);
//		} //operator()
//
//		template<typename HANDLER>
//		void operator()(PARAMS... arg, HANDLER handler) const {
//			size_t index = 0;
//			for (auto& item : invocationList) {
//				RET value = (*(item->stub))(item->object, arg...);
//				handler(index, &value);
//				++index;
//			} //loop
//		} //operator()
//
//		void operator()(PARAMS... arg, delegate<void(size_t, RET*)> handler) const {
//			operator()<decltype(handler)>(arg..., handler);
//		} //operator()
//		void operator()(PARAMS... arg, std::function<void(size_t, RET*)> handler) const {
//			operator()<decltype(handler)>(arg..., handler);
//		} //operator()
//
//	private:
//
//		std::list<typename delegate_base<RET(PARAMS...)>::InvocationElement *> invocationList;
//
//	}; //class multicast_delegate
//
//} /* namespace SA */
//
