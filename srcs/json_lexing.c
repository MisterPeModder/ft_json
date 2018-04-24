/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 03:35:29 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/24 18:39:26 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/stringft.h>
#include <libft_base/character.h>
#include <libft_base/list.h>
#include "json.h"

static int				json_lex_str_setback(t_json_parse_res *res, int lc)
{
	res->col = lc;
	--res->line;
	return (1);
}

static t_json_value		*json_lex_str(t_json_str_it *it, t_json_parse_res *res,
		int lc)
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
		lc = res->col;
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
			ft_lstdel(&lst, (void (*)(void *, size_t))&free);
			return (v);
		}
		if (c == '\n' && json_lex_str_setback(res, lc))
			break ;
		if (!(n = ft_lstnew(&c, sizeof(char))))
			return (NULL);
		if (!lst)
			lst = n;
		else
			ft_lst_pushback(lst, n);
	}
	if (lst)
		ft_lstdel(&lst, (void (*)(void *, size_t))&free);
	json_release_value(&v);
	json_set_error(res, "unclosed string");
	return (NULL);
}

static int				json_is_special_char(char c)
{
	return (c == '{' || c == '}'
			|| c == '[' || c == ']'
			|| c == ':' || c == ',');
}

static char				*json_next_str(t_json_str_it *it,
		t_json_parse_res *res, char begin, int lc)
{
	t_list				*lst;
	t_list				*n;
	char				c;
	char				*str;
	int					i;

	lst = ft_lstnew(&begin, sizeof(char));
	lc = res->col;
	while (!it->str.end && !ft_isspace(c = json_it_next(it, res)))
	{
		if (!(n = ft_lstnew(&c, sizeof(char))))
			return (NULL);
		ft_lst_pushback(lst, n);
		lc = res->col;
		if (json_is_special_char(c = json_it_peek(it)))
			break ;
	}
	if (!it->str.end && c == '\n')
		json_lex_str_setback(res, lc);
	if (!(str = ft_strnew(ft_lstlen(lst))))
		return (NULL);
	n = lst;
	i = 0;
	while (n)
	{
		str[i++] = *((char *)n->content);
		n = n->next;
	}
	if (lst)
		ft_lstdel(&lst, (void (*)(void *, size_t))&free);
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

static t_json_value		*json_make_special_char(char c)
{
	t_json_value		*v;

	if (!(v = json_make_value(JSON_SPECIAL_CHAR)))
		return (NULL);
	v->bol.value = c;
	return (v);
}

static void				*json_unexpected_char(t_json_parse_res *res, char *str)
{
	if (str)
		res->col -= (int)ft_strlen(str);
	return (json_ret_error(res, "unexpected value"));
}

#include <stdio.h>

t_json_value			*json_lexing(t_json_str_it *it, t_json_parse_res *res)
{
	t_json_value		*v;
	char				c;
	char				*str;

	c = 0;
	v = NULL;
	while (!it->str.end && ft_isspace(c = json_it_next(it, res)))
		;
	if ((ft_isspace(c) || !c) && it->str.end)
		return (json_ret_error(res, "unexpected end of file"));
	else if (c == '"')
		return (json_lex_str(it, res, 0));
	else if (json_is_special_char(c))
		return (json_make_special_char(c));
	else if (!(str = json_next_str(it, res, c, 0)) || !*str)
		return (NULL);
	if (ft_strcmp("true", str) == 0)
		v = json_make_bool(1);
	else if (ft_strcmp("false", str) == 0)
		v = json_make_bool(0);
	else if (ft_strcmp("null", str) == 0)
		v = json_make_value(JSON_NULL);
	else if (ft_isdigit(*str) || *str == '-')
		v = json_make_number(str, res);
	free(str);
	return (v ? v : json_unexpected_char(res, str));
}
