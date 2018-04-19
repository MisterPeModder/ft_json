/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_to_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:54:23 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 16:56:53 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/stringft.h>
#include "json.h"

char					*json_to_str(const t_json_value *val)
{
	if (!val)
		return (NULL);
	if (val->obj.type == JSON_STRING)
		return (ft_strdup(val->str.value));
	return (NULL);
}
