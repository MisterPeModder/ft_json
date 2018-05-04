/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 06:09:03 by yguaye            #+#    #+#             */
/*   Updated: 2018/05/04 22:59:16 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/list.h>
#include <libft_base/memory.h>
#include "json.h"
#include "json_internal.h"

static int				json_is_special(t_json_value *val, char c)
{
	return (val->obj.type == JSON_SPECIAL_CHAR && val->bol.value == c);
}

static t_json_value		*json_parse_value(t_json_str_it *it, t_json_value *val,
		t_json_parse_res *res)
{
	t_json_value		*ret;

	if (json_is_special(val, '{'))
		return (json_parse_object(it, 1, res));
	else if (json_is_special(val, '['))
		return (json_parse_array(it, res));
	if (!(ret = ft_memdup(val, sizeof(t_json_value))))
		return (NULL);
	ft_bzero(val, sizeof(t_json_value));
	val->obj.type = JSON_NULL;
	return (ret);
}

static int				json_parse_obj_value(t_json_str_it *it,
		t_json_value *obj, t_json_value *first, t_json_parse_res *res)
{
	t_json_value		second;
	t_json_value		*v;

	if (first->obj.type != JSON_STRING)
		return (json_ret_errorv(res, "non-string keys are not allowed!"));
	if (!json_lexing(&second, it, res, 1))
		return (0);
	if (!json_is_special(&second, ':') && json_rd(&second))
		return (json_ret_errorv(res, "must have colon ':' after key"));
	if (!json_lexing(&second, it, res, 0))
		return (0);
	if (!(v = json_parse_value(it, &second, res)) && json_rd(&second))
		return (0);
	if (v->obj.type == JSON_SPECIAL_CHAR && json_release(&v))
		return (json_ret_errorv(res, "unexpected value"));
	if (hm_get(obj->obj.data, first->str.value) && json_release(&v))
		return (json_ret_errorv(res, "cannot use the same key twice!"));
	hm_put(obj->obj.data, first->str.value, v);
	return (1);
}

static int				json_parse_arr_value(t_json_str_it *it,
		t_list **lst, t_json_value *first, t_json_parse_res *res)
{
	t_list				*nlst;
	t_json_value		*v;

	if (!(nlst = ft_lstnew(NULL, 0)))
		return (0);
	if (!(v = json_parse_value(it, first, res)))
	{
		ft_lstdelone(&nlst, NULL);
		return (0);
	}
	nlst->content = v;
	ft_lstadd(lst, nlst);
	return (1);
}

t_json_value			*json_parse_object(t_json_str_it *it, int has_parent,
		t_json_parse_res *res)
{
	t_json_value		*obj;
	t_json_value		v;
	int					is_begin;

	if (!json_lexing(&v, it, res, 1))
		return (NULL);
	if (!(obj = json_make_value(JSON_OBJECT)))
		return (NULL);
	if (has_parent && json_is_special(&v, '}'))
		return (obj);
	is_begin = 1;
	while (!json_is_special(&v, '}'))
	{
		if (is_begin && !has_parent && json_is_special(&v, '{') && json_rd(&v))
		{
			is_begin = 0;
			if (!json_lexing(&v, it, res, 0) && json_release(&obj))
				return (NULL);
		}
		else if (is_begin && !has_parent && json_release(&obj))
			return (json_ret_error(res, "main object must start with '{'"));
		if (!json_is_special(&v, '}') && (!json_parse_obj_value(it, obj, &v, res)
					|| !json_lexing(&v, it, res, 0)) &&
				json_release(&obj) && json_rd(&v))
			return (NULL);
		if (!json_is_special(&v, ','))
		{
			if (json_is_special(&v, '}'))
				return (obj);
			json_release(&obj);
			--res->line;
			json_rd(&v);
			return (json_ret_error(res, "missing comma ','"));
		}
		if (!json_lexing(&v, it, res, 0) && json_rd(&v) && json_release(&obj))
			return (NULL);
	}
	json_release(&obj);
	json_rd(&v);
	return (json_ret_error(res, "cannot have comma ',' on last element"));
}

static t_json_value		*json_convert_lst2arr(t_list *elems)
{
	t_json_value		*arr;
	t_list				*n;
	size_t				i;

	if (!(arr = json_make_value(JSON_ARRAY)))
	{
		ft_lstdel(&elems, &json_rel4lst);
		return (NULL);
	}
	arr->arr.values_num = ft_lstlen(elems);
	if (!(arr->arr.values = malloc(sizeof(t_json_value) * arr->arr.values_num)))
	{
		json_release(&arr);
		ft_lstdel(&elems, &json_rel4lst);
		return (NULL);
	}
	n = elems;
	i = arr->arr.values_num - 1;
	while (n)
	{
		arr->arr.values[i--] = n->content;
		n = n->next;
	}
	ft_lstdel(&elems, NULL);
	return (arr);
}

t_json_value			*json_parse_array(t_json_str_it *it,
		t_json_parse_res *res)
{
	t_json_value		v;
	t_list				*elems;

	if (!json_lexing(&v, it, res, 1))
		return (NULL);
	if (json_is_special(&v, ']'))
		return (json_make_value(JSON_ARRAY));
	elems = NULL;
	while (!json_is_special(&v, ']'))
	{
		if (!json_is_special(&v, ']') &&
				(!json_parse_arr_value(it, &elems, &v, res) ||
				 !json_lexing(&v, it, res, 0)) && json_rd(&v))
		{
			ft_lstdel(&elems, &json_rel4lst);
			return (NULL);
		}
		if (!json_is_special(&v, ','))
		{
			if (json_is_special(&v, ']'))
				return (json_convert_lst2arr(elems));
			ft_lstdel(&elems, &json_rel4lst);
			--res->line;
			json_rd(&v);
			return (json_ret_error(res, "missing comma ','"));
		}
		if (!json_lexing(&v, it, res, 0) && json_rd(&v))
		{
			ft_lstdel(&elems, &json_rel4lst);
			return (NULL);
		}
	}
	ft_lstdel(&elems, &json_rel4lst);
	return (json_ret_error(res, "cannot have comma ',' on last element"));
}
