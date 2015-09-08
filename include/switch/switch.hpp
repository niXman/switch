
// Copyright (c) 2015 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of SWITCH(https://github.com/niXman/switch) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _switch__switch_hpp_included_
#define _switch__switch_hpp_included_

#include <type_traits>
#include <cassert>
#include <cstring>

#ifdef __has_include
#	if __has_include(<optional>)
#		include <optional>
#		define _STD_OPTIONAL_NS ::std
#	elif __has_include(<experimental/optional>)
#		include <experimental/optional>
#		define _STD_OPTIONAL_NS ::std::experimental
#	endif
#endif

/***************************************************************************/

namespace switch_ {

template<
	 typename R
	,typename K
>
struct switch_impl {
	switch_impl(const K key)
		:key(key)
		,ret()
		{}
		switch_impl(const switch_impl &r) = default;
		switch_impl(switch_impl &&r) = default;

		template<typename Kt>
		switch_impl& case_(const Kt &k, const R &val) {
			if ( key_cmp(key, k) )
				ret.emplace(val);

			return *this;
		}
		template<typename Kt>
		switch_impl& case_(const Kt &k, R &&val) {
			if ( key_cmp(key, k) )
				ret.emplace(std::move(val));

			return *this;
		}
		template<typename Kt, typename F>
		switch_impl& case_(const Kt &k, const F &f, typename std::result_of<F()>::type* = 0) {
			if ( key_cmp(key, k) )
				ret.emplace(f());

			return *this;
		}

		R default_(const R &r) const {
			return (ret ? ret.value() : r);
		}

		operator R() const {
			if ( !ret ) {
				#define __SWITCH_STR(x) #x
				#define _SWITCH_STR(x) __SWITCH_STR(x)

				throw std::runtime_error(
					__FILE__ "(" _SWITCH_STR(__LINE__) "): "
					"Fell off the end of a switch"
				);

				#undef _SWITCH_STR
				#undef __SWITCH_STR
			}

			return ret.value();
		}

private:
	const K key;
	_STD_OPTIONAL_NS::optional<R> ret;

private:
	static bool key_cmp(std::size_t l, std::size_t r) { return l == r; }
	static bool key_cmp(const char *l, const char *r) { return std::strcmp(l, r) == 0; }
	static bool key_cmp(const std::string &l, const char *r) { return key_cmp(l.c_str(), r); }
	static bool key_cmp(const char *l, const std::string &r) { return key_cmp(l, r.c_str()); }
	static bool key_cmp(const std::string &l, const std::string &r) { return key_cmp(l.c_str(), r.c_str()); }
};

template<
	 typename R
	,typename K
	,typename KT = typename std::conditional<
		 std::is_integral<K>::value
		,const std::size_t
		,typename std::conditional<
			 std::is_same<K, std::string>::value
			,const std::string&
			,const char*
		>::type
	>::type
>
switch_impl<R, KT> switch_(const K &key) {
	using type = typename std::decay<K>::type;
	static_assert(
		std::is_same<type, std::string>::value ||
		std::is_same<type, char*>::value ||
		std::is_integral<type>::value
		,"key may be one of integtegral, cstring or std::string type"
	);
	return switch_impl<R, KT>(key);
}

} // ns switch_

/***************************************************************************/

#endif // _switch__switch_hpp_included_