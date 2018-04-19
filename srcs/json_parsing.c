/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 06:09:03 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 07:13:47 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_internal.h"

static t_json_value		*json_parse_value(t_json_str_it *it, t_json_value *val,
		t_json_parse_res *res)
{
	if (val->obj.type == JSON_SPECIAL_CHAR && val->bol.value == '{')
		return (json_parse(it, 1, 0, res));
	else if (val->obj.type == JSON_SPECIAL_CHAR && val->bol.value == '[')
		return (json_parse(it, 1, 1, res));
	else
		return (val);
}

#include <stdio.h>

int						json_parse_obj_value(t_json_str_it *it,
		t_json_value *obj, t_json_value *first, t_json_parse_res *res)
{
	t_json_value		*values[2];
	t_json_value		*v;

	if (first->obj.type != JSON_STRING)
	{
		printf("err type: %d\n", first->obj.type);
		if (first->obj.type == JSON_SPECIAL_CHAR)
			printf("val: %c\n", first->bol.value);
		json_set_error(res, "non-string keys are not allowed!");
		return (0);
	}
	if (!(values[0] = json_lexing(it, res)))
		return (0);
	printf("two bis\n");
	if (values[0]->obj.type != JSON_SPECIAL_CHAR && values[1]->bol.value != ':')
	{
		json_set_error(res, "must have colon ':' after key");
		return (0);
	}
	if (!(values[1] = json_lexing(it, res)))
		return (0);
	printf("two ter\n");
	if (!(v = json_parse_value(it, values[1], res)))
		return (0);
	hm_put(obj->obj.data, values[0]->str.value, v);
	return (1);
}


t_json_value			*json_parse(t_json_str_it *it, char has_parent,
		char is_array, t_json_parse_res *res)
{
	t_json_value		*obj;
	t_json_value		*v;
	int					has_comma;

	if (!(obj = json_make_value(JSON_OBJECT)))
		return (NULL);
	has_comma = 0;
	while (1)
	{
		if (!(v = json_lexing(it, res)))
			return (NULL);
		printf("one\n");
		if (v->obj.type == JSON_SPECIAL_CHAR && (
					(is_array && v->bol.value == ']') ||
					(!is_array && v->bol.value == '}')))
			break;
		printf("two\n");
		if (!json_parse_obj_value(it, obj, v, res) ||
				!(v = json_lexing(it, res)))
			return (NULL);
		printf("three\n");
		if (v->obj.type != JSON_SPECIAL_CHAR && v->bol.value == ',')
			has_comma = 1;
		else
			has_comma = 0;
		printf("comma %d\n", has_comma);

	}
	if (has_comma)
		return (json_ret_error(res, "cannot have colon on last element"));
	(void)has_parent;
	return (obj);
}
