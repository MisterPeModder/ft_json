/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_obj_to_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/21 17:25:22 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/21 19:30:12 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/list.h>
#include <libft_base/stringft.h>
#include "json.h"

static void				json_obj_to_lst_values(t_list **lst, char **tab)
{
	size_t				i;
	t_list				*tmp;

	i = 1;
	while (tab[i])
	{
		if (!(tmp = ft_lstnew(NULL, 0)))
			return ;
		tmp->content = ft_strconcat("\t", tab[i++], 2);
		ft_lstadd(lst, tmp);
	}
	free(tab);
}

static void				*json_obj_error(t_list **lst)
{
	ft_lstdel(lst, (void (*)(void *, size_t))&free);
	return (NULL);
}

static t_list			*json_obj_put_end(t_list *lst)
{
	t_list				*tmp;

	if (!(tmp = ft_lstnew("}", 2)))
	{
		ft_lstdel(&lst, (void (*)(void *, size_t))&free);
		return (NULL);
	}
	ft_lstadd(&lst, tmp);
	return (lst);
}

t_list					*json_obj_to_lst(const t_json_value *val)
{
	t_hmiterator		it;
	t_list				*lst;
	t_list				*tmp;
	char				**tab;
	int					res;

	hm_iter_init(val->obj.data, &it);
	if (!(lst = ft_lstnew("{", 2)) || !(res = hm_iter_next(&it)))
		return (NULL);
	while (res)
	{
		if (!(tmp = ft_lstnew(NULL, 0)))
			return (json_obj_error(&lst));
		if (!(tab = json_to_tab(it.value)) || !*tab)
			return (json_obj_error(&lst));
		tmp->content = ft_strconcat("\t\"", ft_strconcat(it.key,
					ft_strconcat("\": ", *tab, 0), 1), 2);
		ft_lstadd(&lst, tmp);
		res = hm_iter_next(&it);
		json_obj_to_lst_values(&lst, tab);
		if (res)
			lst->content = ft_strconcat(lst->content, ",", 1);
	}
	return (json_obj_put_end(lst));
}
