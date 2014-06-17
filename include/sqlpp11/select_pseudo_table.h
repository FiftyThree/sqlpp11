/*
 * Copyright (c) 2013-2014, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_SELECT_PSEUDO_TABLE_H
#define SQLPP_SELECT_PSEUDO_TABLE_H

#include <sqlpp11/no_value.h>

namespace sqlpp
{
	// provide type information for sub-selects that are used as named expressions or tables
	template<typename Expr>
		struct select_column_spec_t
		{
			using _traits = make_traits<value_type_of<Expr>>;
			using _recursive_traits = make_recursive_traits<>;
			using _value_type = value_type_of<Expr>; // FIXME: column specs probably should use _traits, too

			using _name_t = typename Expr::_name_t;
			struct _column_type 
			{
				using _must_not_insert = std::true_type;
				using _must_not_update = std::true_type;
			};
		};

	template<
		typename Select,
		typename... NamedExpr
		>
		struct select_pseudo_table_t: public sqlpp::table_t<
																	select_pseudo_table_t<Select, NamedExpr...>, 
																	column_t<select_pseudo_table_t<Select, NamedExpr...>, select_column_spec_t<NamedExpr>>...
																								 >
	{
		using _traits = make_traits<no_value_t, tag::pseudo_table>;
		using _recursive_traits = make_recursive_traits<>;

		select_pseudo_table_t(Select select):
			_select(select)
		{
		}

		select_pseudo_table_t(const select_pseudo_table_t& rhs) = default;
		select_pseudo_table_t(select_pseudo_table_t&& rhs) = default;
		select_pseudo_table_t& operator=(const select_pseudo_table_t& rhs) = default;
		select_pseudo_table_t& operator=(select_pseudo_table_t&& rhs) = default;
		~select_pseudo_table_t() = default;

		Select _select;
	};

	template<typename Context, typename Select, typename... NamedExpr>
		struct serializer_t<Context, select_pseudo_table_t<Select, NamedExpr...>>
		{
			using T = select_pseudo_table_t<Select, NamedExpr...>;

			static Context& _(const T& t, Context& context)
			{
				serialize(t._select, context);
				return context;
			}
		};

}

#endif
