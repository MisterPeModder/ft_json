/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/05 12:49:19 by yguaye            #+#    #+#             */
/*   Updated: 2018/05/05 16:55:58 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <json.h>
#include <libft.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

/*
** library test file: parses JSON code then serializes it back.
**
** It takes 2 arguments, arg 1 must be either 'file' or 'string'.
**
** if 'file' passed, then arg 2 must be a filename.
** if 'string' passed, then arg 2 must be json code.
*/

static int				process_file(t_json_parse_res *res)
{
	int out;
	if (!res)
	{
		perror("couldn't parse JSON");
		return (4);
	}
	if (!res->obj)
	{
		ft_putendl_fd(res->err ? res->err : "critical error !", 2);
		json_release_file(&res, 1);
		return (5);
	}
	out = open("./out", O_WRONLY | O_CREAT, 0644);
	json_to_file(res->obj, out);
	close(out);
	json_release_file(&res, 1);
	return (0);
}

int						main(int ac, char **av)
{
	int					fd;
	t_json_parse_res	*res;

	if (ac <= 2)
	{
		ft_putendl_fd("wrong number of arguments", 2);
		return (1);
	}
	if (ft_strequ(av[1], "string"))
		return (process_file(json_from_str(av[2], JFLAG_ALLOW_COMMENTS)));
	else if (ft_strequ(av[1], "file"))
	{
		if ((fd = open(av[2], O_RDONLY)) <= 0)
		{
			perror("couldn't open file");
			return (3);
		}
		res = json_from_file(fd, JFLAG_ALLOW_COMMENTS);
		close(fd);
		return (process_file(res));
	}
	else
	{
		ft_putendl_fd("1st argument must be either 'string' or 'file'", 2);
		return (2);
	}
	return (0);
}
