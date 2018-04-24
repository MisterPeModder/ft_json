/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_number.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/24 14:53:29 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/24 21:37:40 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/stringft.h>
#include <libft_base/character.h>
#include <math.h>
#include "json.h"

static void				*json_number_check(const char *src, t_json_parse_res *r)
{
	int					i;

	i = *src == '-' ? 1 : 0;
	if (!src[i])
		return (json_ret_error(r, "incomplete number"));
	if (src[i] == '0' && src[i + 1] && ft_isdigit(src[i + 1]))
		return (json_ret_error(r, "trailing zero(s)"));
	else if (src[i] != '0')
		while (src[++i] && ft_isdigit(src[i]) && src[i] != '0')
			;
	if (!src[i])
		return (r);
	if (src[i] == '.')
		while (src[++i] && ft_isdigit(src[i]))
			;
	if (!src[i])
		return (r);
	if (src[i] == 'e' || src[i] == 'E')
	{
		if (!src[++i])
			return (json_ret_error(r, "missing number after exponent"));
		if (src[i] == '+' || src[i] == '-')
			++i;
		while (src[i] && ft_isdigit(src[i++]))
			;
	}
	return (src[i] ? json_ret_error(r, "trailing character(s)") : (void *)r);
}

static double			json_number_to_dec(double r)
{
	r = r < 0 ? -r : r;
	while (r > 1)
		r /= 10.;
	return (r);
}

#include <stdio.h>

static double			json_num_get_exp(const char *src)
{
	printf("exp: '%c'\n", *src);
	while (*src && (*src != 'E' || *src != 'e'))
		++src;
	if (!*src || !*(src + 1))
		return (1);
	return (pow(10., (double)atoi(src + 1)));
}

/*
** TODO: for some reason, the string src doesn't contain the full number...
**       Find out why.
*/

t_json_value			*json_make_number(const char *src, t_json_parse_res *r)
{
	int					e_part;
	t_json_value		*num;

	printf("source: \"%s\"\n", src);
	if (!json_number_check(src, r))
		return (NULL);
	e_part = ft_atoi(src);
	while (*src || *src == '.')
		++src;
	if (*(src - 1) == '.')
	{
		if (!(num = json_make_value(JSON_DOUBLE)))
			return (NULL);
		num->n_d.value = ((double)e_part) + json_number_to_dec(ft_atoi(src));
		num->n_d.value *= json_num_get_exp(src);
	}
	else
	{
		if (!(num = json_make_value(JSON_INT)))
			return (NULL);
		num->n_i.value = e_part;
		printf("int: e_part = %i\n", e_part);
		num->n_i.value *= json_num_get_exp(src);
	}
	return (num);
}

char					*json_double_to_str(double num)
{
	(void)num;
	return (NULL);
}
