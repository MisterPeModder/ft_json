/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 03:35:29 by yguaye            #+#    #+#             */
/*   Updated: 2018/05/03 13:38:10 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/memory.h>
#include <libft_base/stringft.h>
#include <libft_base/character.h>
#include "json.h"

static int				json_is_special_char(char c)
{
	return (c == '{' || c == '}'
			|| c == '[' || c == ']'
			|| c == ':' || c == ',');
}

static int				json_next_token(char *str, t_json_str_it *it,
		t_json_parse_res *res, char begin)
{
	int					s;
	char				c;

	s = 1;
	ft_bzero(str, 31);
	*str = begin;
	while (!it->str.end && ft_istoken(c = json_it_peek(it)))
	{
		str[s++] = c;
		if (s >= 30)
			return (json_ret_errorv(res, "unexpected value"));
		json_it_next(it, res);
	}
	return (1);
}

static int				json_mksimple(t_json_value *v, char c, t_json_vtype t)
{
	v->bol.type = t;
	v->bol.value = c;
	return (1);
}

static int				json_from_token(t_json_value *v, const char *str)
{
	if (ft_strcmp("true", str) == 0)
		return (json_mksimple(v, 1, JSON_BOOL));
	else if (ft_strcmp("false", str) == 0)
		return (json_mksimple(v, 0, JSON_BOOL));
	else if (ft_strcmp("null", str) == 0)
		return (json_mksimple(v, 0, JSON_NULL));
	return (0);
}

static int				json_unexpected_char(t_json_parse_res *res, char *str)
{
	if (str)
		res->col -= (int)ft_strlen(str);
	return (json_ret_errorv(res, "unexpected value"));
}

int						json_lexing(t_json_value *v, t_json_str_it *it,
		t_json_parse_res *res, int init)
{
	char				c;
	char				str[31];

	c = 0;
	if (!init)
		json_rd(v);
	while (!it->str.end && ft_isspace(c = json_it_next(it, res)))
		;
	if ((ft_isspace(c) || !c) && it->str.end)
		return (json_ret_errorv(res, "unexpected end of file"));
	else if (c == '"')
		return (json_lex_str(v, it, res, 0));
	else if (json_is_special_char(c))
		return (json_mksimple(v, c, JSON_SPECIAL_CHAR));
	else if (!json_next_token(str, it, res, c))
		return (0);
	else if (json_from_token(v, str))
		return (1);
	else if (ft_isdigit(*str) || *str == '-')
		return (json_make_number(v, str, res));
	return (json_unexpected_char(res, str));
}
