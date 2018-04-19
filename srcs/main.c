/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 01:56:49 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 16:58:13 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.h"
#include <libft.h>
#include <stdio.h>

int						main(int argc, char **argv)
{
	t_json_str_it		it;
	t_json_parse_res	res;
	t_json_value		*obj;
	t_hmiterator		hmit;

	if (argc != 2)
		return (42);
	json_init_iterator(&it, 0, argv[1]);
	res.err = NULL;
	res.line = 0;
	res.col = 0;
	res.obj = NULL;
	obj = json_parse(&it, 0, 0, &res);
	if (!obj)
	{
		printf("(nil)\n");
		if (res.err)
			printf("%s\n", res.err);
		return (43);
	}
	hm_iter_init(obj->obj.data, &hmit);
	printf("number of elements: %zu\n", hm_length(obj->obj.data));
	while (hm_iter_next(&hmit))
	{
		printf("\"%s\": ", hmit.key);
		printf("%s,\n", json_to_str((t_json_value *)hmit.value));
	}
	return (0);
}
