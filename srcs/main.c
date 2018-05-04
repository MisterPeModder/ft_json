/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 01:56:49 by yguaye            #+#    #+#             */
/*   Updated: 2018/05/04 16:59:07 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json.h"
#include <fcntl.h>
#include <unistd.h>
#include <libft.h>
#include <stdio.h>

int						main(int argc, char **argv)
{
	int					fd;
	t_json_parse_res	*res;
	char				*str;

	if (argc != 2)
		return (42);
	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror("JSON parser: couldn't open file");
		return (11);
	}
	res = json_from_file(fd, 1);
	if (!res || !res->obj)
	{
		if (res)
		{
			ft_putendl(res->err ? res->err : "CRITICAL FAILURE");
			json_release_file(&res, 0);
		}
		close(fd);
		return (43);
	}
	ft_putstr(str = json_to_str(res->obj));
	if (!str)
		ft_putendl("(null)\nSomething went wrong!");
	else
		free(str);
	json_release_file(&res, 1);
	close(fd);
	return (0);
}
