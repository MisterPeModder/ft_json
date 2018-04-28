/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 06:09:03 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/28 19:20:45 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/list.h>
#include "json.h"

static int				json_is_special(t_json_value *val, char c)
{
	return (val->obj.type == JSON_SPECIAL_CHAR && val->bol.value == c);
}

static t_json_value		*json_parse_value(t_json_str_it *it, t_json_value *val,
		t_json_parse_res *res)
{
	if (json_is_special(val, '{'))
	{
		json_release_value(&val);
		return (json_parse_object(it, 1, res));
	}
	else if (json_is_special(val, '['))
	{
		json_release_value(&val);
		return (json_parse_array(it, res));
	}
	else
		return (val);
}

static int				json_parse_obj_value(t_json_str_it *it,
		t_json_value *obj, t_json_value *first, t_json_parse_res *res)
{
	t_json_value		*second;
	t_json_value		*v;

	if (first->obj.type != JSON_STRING)
	{
		json_set_error(res, "non-string keys are not allowed!");
		json_release_value(&first);
		return (0);
	}
	if (!(second = json_lexing(it, res)))
	{
		json_release_value(&first);
		return (0);
	}
	if (second->obj.type != JSON_SPECIAL_CHAR || second->bol.value != ':')
	{
		json_release_value(&first);
		json_release_value(&second);
		json_set_error(res, "must have colon ':' after key");
		return (0);
	}
	json_release_value(&second);
	if (!(second = json_lexing(it, res)))
	{
		json_release_value(&first);
		return (0);
	}
	if (!(v = json_parse_value(it, second, res)))
	{
		json_release_value(&first);
		return (0);
	}
	if (hm_get(obj->obj.data, first->str.value))
	{
		json_set_error(res, "cannot use the same key twice!");
		json_release_value(&first);
		json_release_value(&v);
		return (0);
	}
	hm_put(obj->obj.data, first->str.value, v);
	json_release_value(&first);
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
	t_json_value		*v;
	int					is_begin;

	if (!(obj = json_make_value(JSON_OBJECT)))
		return (NULL);
	if (!(v = json_lexing(it, res)))
		return (NULL);
	if (has_parent && json_is_special(v, '}'))
	{
		json_release_value(&v);
		return (obj);
	}
	is_begin = 1;
	while (!json_is_special(v, '}'))
	{
		if (is_begin && !has_parent && json_is_special(v, '{'))
		{
			json_release_value(&v);
			is_begin = 0;
			if (!(v = json_lexing(it, res)))
				return (NULL);
		}
		else if (is_begin && !has_parent)
		{
			json_release_value(&v);
			json_release_value(&obj);
			return (json_ret_error(res, "main object must start with '{'"));
		}
		if (!json_is_special(v, '}') && (!json_parse_obj_value(it, obj, v, res)
					|| !(v = json_lexing(it, res))))
		{
			json_release_value(&obj);
			return (NULL);
		}
		if (!json_is_special(v, ','))
		{
			if (json_is_special(v, '}'))
			{
				json_release_value(&v);
				return (obj);
			}
			json_release_value(&obj);
			json_release_value(&v);
			--res->line;
			return (json_ret_error(res, "missing comma ','"));
		}
		json_release_value(&v);
		if (!(v = json_lexing(it, res)))
			return (NULL);
	}
	json_release_value(&obj);
	json_release_value(&v);
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
		json_release_value(&arr);
		ft_lstdel(&elems, &json_rel4lst);
		return (NULL);
	}
	n = elems;
	i = arr->arr.values_num - 1;;
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
	t_json_value		*v;
	t_list				*elems;

	v = NULL;
	if (!(v = json_lexing(it, res)))
		return (NULL);
	if (json_is_special(v, ']'))
	{
		json_release_value(&v);
		return (json_make_value(JSON_ARRAY));
	}
	elems = NULL;
	while (!json_is_special(v, ']'))
	{
		if (!json_is_special(v, ']') &&
				(!json_parse_arr_value(it, &elems, v, res) ||
				 !(v = json_lexing(it, res))))
		{
			ft_lstdel(&elems, &json_rel4lst);
			return (NULL);
		}
		if (!json_is_special(v, ','))
		{
			if (json_is_special(v, ']'))
			{
				json_release_value(&v);
				return (json_convert_lst2arr(elems));
			}
			ft_lstdel(&elems, &json_rel4lst);
			json_release_value(&v);
			--res->line;
			return (json_ret_error(res, "missing comma ','"));
		}
		json_release_value(&v);
		if (!(v = json_lexing(it, res)))
			return (NULL);
	}
	json_release_value(&v);
	ft_lstdel(&elems, &json_rel4lst);
	return (json_ret_error(res, "cannot have comma ',' on last element"));
}
