/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_str_iterator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 02:25:32 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/24 01:30:55 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <libft_base/memory.h>
#include "json_internal.h"
#include <stdio.h>

void					json_init_iterator(t_json_str_it *it, int file,
		const char *path_or_str)
{
	it->str.i = 0;
	it->str.is_file = file;
	it->str.tabc = 0;
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
		it->file.peek = 1;
	}
}

static char				json_it_inc(t_json_parse_res *res, char c, char *tabc)
{
	if (c == '\n')
	{
		++res->line;
		res->col = 0;
		*tabc = 4;
	}
	else if (c == '\t')
	{
		res->col += *tabc;
		*tabc = 4;
	}
	else
	{
		++res->col;
		*tabc = --*tabc <= 0 ? 4 : *tabc;
	}
	return (c);	
}

char					json_it_peek(t_json_str_it *it)
{
	if (it->str.end)
		return (0);
	if (!it->str.is_file)
		return (it->str.str[it->str.i]);
	else
	{
		if (it->file.i >= it->file.data_size)
		{
			if ((it->file.data_size = read(it->file.fd, it->file.data,
							JRD_PACKET)) == -1)
			{
				it->file.end = 1;
				return (0);
			}
			it->file.i = 0;
		}
		it->file.peek = 1;
		return (it->file.data[it->file.i]);
	}
}

char					json_it_next(t_json_str_it *it, t_json_parse_res *res)
{
	if (it->str.end)
		return (0);
	if (!it->str.is_file)
	{
		if (!it->str.str[it->str.i + 1])
			it->str.end = 1;
		return (json_it_inc(res, it->str.str[it->str.i++], &it->str.tabc));
	}
	else
	{
		if (it->file.i >= it->file.data_size)
		{
			if ((it->file.data_size = read(it->file.fd, it->file.data,
							JRD_PACKET)) == -1)
			{
				it->file.end = 1;
				return (json_it_inc(res, 0, &it->str.tabc));
			}
			it->file.i = 0;
		}
		if (it->file.data_size < JRD_PACKET &&
				it->file.i >= it->file.data_size - 1)
			it->file.end = 1;
		if (!it->file.peek)
			return (json_it_inc(res, it->file.data[it->file.i++],
						&it->str.tabc));
		it->file.peek = 0;
		return (json_it_inc(res, it->file.data[it->file.i], &it->str.tabc));
	}
}

void					json_close_file(t_json_str_it *it)
{
	if (it->file.is_file && it->file.fd != -1)
		close(it->file.fd);
	it->str.end = 1;
}
