/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 16:13:11 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/23 16:21:50 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.h"

t_json_value			*json_obj_get_value(const t_json_object *obj,
		const char *key)
{
	return (!obj || !key ? NULL : hm_get(obj->data, key));
}

t_json_value			*json_arr_get_value(const t_json_array *arr, size_t i)
{
	return (!arr || i >= arr->values_num ? NULL : arr->values[i]);
}
