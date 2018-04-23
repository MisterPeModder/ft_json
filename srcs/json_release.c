/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_release.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 01:40:49 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/23 16:22:28 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/memory.h>
#include "json.h"

static void				json_release_value_helper(t_json_value *v)
{
	size_t				i;

	if (v->str.type == JSON_STRING && v->str.value)
		free(v->str.value);
	else if (v->obj.type == JSON_OBJECT && v->obj.data)
		hm_release(&v->obj.data, (void (*)(void *))&json_release_value_helper);
	else if (v->arr.type == JSON_ARRAY && v->arr.values)
	{
		i = 0;
		while (i < v->arr.values_num)
			if (v->arr.values[i++])
				json_release_value_helper(v->arr.values[i - 1]);
		free(v->arr.values);
	}
	free(v);
}

void					json_release_value(t_json_value **v)
{
	json_release_value_helper(*v);
	v = NULL;
}

void					json_release_file(t_json_parse_res **file)
{
	if ((*file)->err)
		free((*file)->err);
	ft_bzero(*file, sizeof(t_json_parse_res));
	free(*file);
	*file = NULL;
}
