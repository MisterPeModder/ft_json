/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_to_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:54:23 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/21 18:41:16 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/list.h>
#include <libft_base/memory.h>
#include <libft_base/stringft.h>
#include "json.h"

static t_list			*json_arr_to_lst(const t_json_value *val)
{
	(void)val;
	return (NULL);
}

static t_list				*json_to_lst(const t_json_value *val)
{
	t_list				*lst;

	if (val->obj.type == JSON_OBJECT)
		return (json_obj_to_lst(val));
	else if (val->obj.type == JSON_ARRAY)
		return (json_arr_to_lst(val));
	if (!(lst = ft_lstnew(NULL, 0)))
		return (NULL);
	if (val->obj.type == JSON_STRING)
		lst->content = ft_strconcat("\"",
				ft_strconcat(val->str.value, "\"", 1), 2);
	else if (val->obj.type == JSON_NUMBER)
		lst->content = ft_itoa((int)val->num.value);
	else if (val->obj.type == JSON_BOOL)
		lst->content = val->bol.value ? ft_strdup("true") : ft_strdup("false");
	else if (val->obj.type == JSON_NULL)
		lst->content = ft_strdup("null");
	else
	{
		free(lst);
		return (NULL);
	}
	return (lst);
}

char					**json_to_tab(const t_json_value *val)
{
	t_list				*lst;
	char				**tab;
	t_list				*curr;
	size_t				len;
	size_t				i;

	if (!val || !(lst = json_to_lst(val)))
		return (NULL);
	len = ft_lstlen(lst);
	if (!(tab = (char **)ft_memalloc(sizeof(char *) * (len + 1))))
		return (NULL);
	curr = lst;
	i = len - 1;
	while (curr)
	{
		lst = curr->next;
		tab[i--] = (char *)curr->content;
		free(curr);
		curr = lst;
	}
	return (tab);
}

char					*json_to_str(const t_json_value *val)
{
	char				**tab;
	size_t				i;
	size_t				len;
	char				*ret;

	if (!(tab = json_to_tab(val)))
		return (NULL);
	i = 0;
	len = 0;
	while (tab[i])
		len += ft_strlen(tab[i++]) + 1;
	if (!(ret = ft_strnew(len)))
	{
		ft_strtabdel(&tab);
		return (NULL);
	}
	i = 0;
	while (tab[i])
		ft_strcat(ret, ft_strconcat(tab[i++], "\n", 1));
	return (ret);
}
