/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 04:08:33 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 06:13:11 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/stringft.h>
#include "json.h"

void					json_set_error(t_json_parse_res *res, char *msg)
{
	res->err = ft_strconcat(ft_strconcat(ft_strconcat(ft_strconcat(ft_strconcat(
							"JSON parsing error at line ",
							ft_itoa(res->line), 2), " col ", 1),
					ft_itoa(res->col), 1), ": ", 1), msg, 1);
}

t_json_value			*json_ret_error(t_json_parse_res *res, char *msg)
{
	json_set_error(res, msg);
	return (NULL);
}
