/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 03:35:29 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 07:06:28 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/stringft.h>
#include <libft_base/character.h>
#include <libft_base/list.h>
#include "json.h"

#include <stdio.h>

static t_json_value		*json_lex_str(t_json_str_it *it, t_json_parse_res *res)
{
	t_json_value		*v;
	char				c;
	t_list				*lst;
	t_list				*n;
	int					i;

	if (!(v = json_make_value(JSON_STRING)))
		return (NULL);
	i = 0;
	lst = NULL;
	while (!it->str.end)
	{
		c = json_it_next(it, res);
		if (c == '"')
		{
			if (!(v->str.value = ft_strnew(ft_lstlen(lst))))
				return (NULL);
			n = lst;
			i = 0;
			while (n)
			{
				v->str.value[i++] = *((char *)n->content);
				n = n->next;
			}
			return (v);
		}
		if (!(n = ft_lstnew(&c, sizeof(char))))
			return (NULL);
		if (!lst)
			lst = n;
		else
			ft_lst_pushback(lst, n);
	}
	json_release_value(&v);
	json_set_error(res, "unclosed string");
	return (NULL);
}

static char				*json_next_str(t_json_str_it *it, t_json_parse_res *res,
		char begin)
{
	t_list				*lst;
	t_list				*n;
	char				c;
	char				*str;
	int					i;

	lst = ft_lstnew(&begin, sizeof(char));
	while (!it->str.end && !ft_isspace(c = json_it_next(it, res)))
	{
		if (!(n = ft_lstnew(&c, sizeof(char))))
			return (NULL);
		ft_lst_pushback(lst, n);
	}
	if (!(str = ft_strnew(ft_lstlen(lst))))
		return (NULL);
	n = lst;
	i = 0;
	while (n)
	{
		str[i++] = *((char *)n->content);
		n = n->next;
	}
	return (str);
}

static t_json_value		*json_make_bool(char val)
{
	t_json_value		*r;

	if (!(r = json_make_value(JSON_BOOL)))
		return (NULL);
	r->bol.value = val;
	return (r);
}

static int				json_is_numeric(char *str)
{
	if (*str == '-')
		++str;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		++str;
	}
	return (1);
}

static int				json_is_special_char(char c)
{
	return (c == '{' || c == '}'
			|| c == '[' || c == ']'
			|| c == ':' || c == ',');
}

static t_json_value		*json_make_special_char(char c)
{
	t_json_value		*v;

	if (!(v = json_make_value(JSON_SPECIAL_CHAR)))
		return (NULL);
	v->bol.value = c;
	return (v);
}

t_json_value			*json_lexing(t_json_str_it *it, t_json_parse_res *res)
{
	t_json_value		*v;
	char				c;
	char				*str;

	while (!it->str.end && ft_isspace(c = json_it_next(it, res)))
		;
	if ((ft_isspace(c) || !c) && it->str.end)
		return (json_ret_error(res, "unexpected end of file"));
	else if (c == '"')
		return (json_lex_str(it, res));
	else if (json_is_special_char(c))
		return (json_make_special_char(c));
	else if (!(str = json_next_str(it, res, c)))
		return (NULL);
	if (ft_strcmp("true", str) == 0)
		return (json_make_bool(1));
	else if (ft_strcmp("false", str) == 0)
		return (json_make_bool(0));
	else if (ft_strcmp("null", str) == 0)
		return (json_make_value(JSON_NULL));
	else if (json_is_numeric(str))
	{
		if (!(v = json_make_value(JSON_NUMBER)))
			return (NULL);
		v->num.value = ft_atoi(str);
		return (v);
	}
	return (json_ret_error(res, "unexpected character"));
}
