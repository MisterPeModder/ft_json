/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_str_iterator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 02:25:32 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 04:03:19 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <libft_base/memory.h>
#include "json_internal.h"
#include <stdio.h>

void					json_init_iterator(t_json_str_it *it, int file,
		char *path_or_str)
{
	it->str.i = 0;
	it->str.is_file = file;
	if (!file)
	{
		it->str.str = path_or_str;
		it->str.end = path_or_str && *path_or_str ? 0 : 1;
	}
	else
	{
		if ((it->file.fd = open(path_or_str, 0)) == -1)
			it->file.end = 1;
		else if ((it->file.data_size = read(it->file.fd, it->file.data,
						JRD_PACKET)) == -1 || it->file.data_size == 0)
			it->file.end = 1;
		else
			it->file.end = 0;
	}
}

static char				json_it_inc(t_json_parse_res *res, char c)
{
	if (c == '\n')
	{
		++res->line;
		res->col = 0;
	}
	else
		++res->col;
	return (c);
}

char					json_it_next(t_json_str_it *it, t_json_parse_res *res)
{
	if (!it->str.is_file)
	{
		if (!it->str.str[it->str.i + 1])
			it->str.end = 1;
		return (json_it_inc(res, it->str.str[it->str.i++]));
	}
	else
	{
		if (it->file.i >= it->file.data_size)
		{
			if ((it->file.data_size = read(it->file.fd, it->file.data,
							JRD_PACKET)) == -1)
			{
				it->file.end = 1;
				return (json_it_inc(res, 0));
			}
			it->file.i = 0;
		}
		if (it->file.data_size < JRD_PACKET &&
				it->file.i >= it->file.data_size - 1)
			it->file.end = 1;
		return (json_it_inc(res, it->file.data[it->file.i++]));
	}
}

void					json_close_file(t_json_str_it *it)
{
	if (it->file.is_file && it->file.fd != -1)
		close(it->file.fd);
	it->str.end = 1;
}
