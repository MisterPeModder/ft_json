/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_val_to_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:19:07 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/23 14:49:09 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/stringft.h>
#include <stdio.h>
#include "json.h"

static char				*json_sfy(char *str)
{
	char				*ret;

	if (!(ret = ft_strnew(ft_strlen(str) + 2)))
		return (NULL);
	*ret = '"';
	ft_strcat(ret, str);
	ft_strcat(ret, "\"");
	return (ret);
}

static int				json_obj_to_tab(const t_json_value *val, char **tab)
{
	t_hmiterator		it;
	char				**sub;
	size_t				j;

	if (!(*tab = ft_strdup("{")))
		return (0);
	hm_iter_init(val->obj.data, &it);
	printf("entering...\n");
	while (hm_iter_next(&it))
	{
		if (!(sub = json_to_tab(it.value)))
			return (0);
		printf("made sub array!\n");
		j = 0;
		while (sub[j])
		{
			if (!*tab)
			{
				printf("error: !tab.\n");
				ft_strtabdel(&sub);
				return (0);
			}
			++tab;
			*tab = !j ? ft_strconcat("\t", ft_strconcat(json_sfy(it.key), ft_strconcat(": ", *sub, 0), 2), 2) :
				ft_strconcat("\t\t", sub[j], 0);
			if (!sub[++j])
				*tab = ft_strconcat(*tab, ",", 1);
		}
		ft_strtabdel(&sub);
		++tab;
	}
	if (!(*tab = ft_strdup("}")))
		return (0);
	printf("end!\n");
	return (1);
}

static int				json_arr_to_tab(const t_json_value *val, char **tab)
{
	size_t				i;
	char				**sub;
	size_t				j;

	if (!(*tab = ft_strdup("[")))
		return (0);
	i = 0;
	while (i < val->arr.values_num)
	{
		if (!(sub = json_to_tab(val->arr.values[i])))
			return (0);
		j = 0;
		while (sub[j])
		{
			if (!*tab)
			{
				ft_strtabdel(&sub);
				return (0);
			}
			++tab;
			*tab = !j ? ft_strdup(*sub) : ft_strconcat("\t", sub[j], 0);
			if (!sub[++j])
				*tab = ft_strconcat(*tab, ",", 1);
		}
		ft_strtabdel(&sub);
		++tab;
		++i;
	}
	return (1);
}

int						json_val_to_tab(const t_json_value *val, char **tab)
{
	if (val->obj.type == JSON_OBJECT)
		return (json_obj_to_tab(val, tab));
	else if (val->obj.type == JSON_ARRAY)
		return (json_arr_to_tab(val, tab));
	else if (val->obj.type == JSON_STRING)
		*tab = ft_strconcat("\"", ft_strconcat(val->str.value, "\"", 0), 2);
	else if (val->obj.type == JSON_NUMBER)
		*tab = ft_itoa((int)val->num.value);
	else if (val->obj.type == JSON_BOOL)
		*tab = ft_strdup(val->bol.value ? "true" : "false");
	else if (val->obj.type == JSON_NULL)
		*tab = ft_strdup("null");
	else
		return (0);
	return (1);
}
