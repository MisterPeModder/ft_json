/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 01:26:24 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/24 11:55:03 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_INTERNAL_H
# define JSON_INTERNAL_H

# include <sys/types.h>
# include <libft_base/list.h>
# include "json_datatypes.h"

# define JRD_PACKET 128

typedef struct			s_json_str_it_str
{
	char				is_file;
	ssize_t				i;
	char				end;
	char				tabc;
	const char			*str;
}						t_json_str_it_str;

typedef struct			s_json_str_it_file
{
	char				is_file;
	ssize_t				i;
	char				end;
	char				tabc;
	char				peek;
	int					fd;
	ssize_t				data_size;
	char				data[JRD_PACKET];
}						t_json_str_it_file;

typedef union			u_json_str_it
{
	t_json_str_it_str	str;
	t_json_str_it_file	file;
}						t_json_str_it;

t_json_value			*json_make_value(t_json_vtype type);

void					json_init_iterator(t_json_str_it *it, int file,
		const char *path_or_str, int fd);

char					json_it_peek(t_json_str_it *it);

char					json_it_next(t_json_str_it *it, t_json_parse_res *res);

t_json_value			*json_ret_error(t_json_parse_res *res, char *msg);

t_json_value			*json_parse_object(t_json_str_it *it, int has_parent,
		t_json_parse_res *res);

t_json_value			*json_parse_array(t_json_str_it *it,
		t_json_parse_res *res);

t_json_value			*json_lexing(t_json_str_it *it, t_json_parse_res *res);

void					json_set_error(t_json_parse_res *res, char *msg);

int						json_val_to_tab(const t_json_value *val, char **tab);

void					json_rel4lst(void *value, size_t);

#endif
