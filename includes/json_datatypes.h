/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_datatypes.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 10:07:19 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/19 06:02:53 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_DATATYPES_H
# define JSON_DATATYPES_H

/*
** NOTE: the types below are not meant to be modified by the user.
*/

# include <stdlib.h>
# include <libft_containers/hashmap.h>

typedef enum			e_json_vtype
{
	JSON_OBJECT,
	JSON_STRING,
	JSON_NUMBER,
	JSON_ARRAY,
	JSON_BOOL,
	JSON_NULL,
	JSON_SPECIAL_CHAR
}						t_json_vtype;

union					u_json_value;

typedef struct			s_json_object
{
	t_json_vtype		type;
	t_hashmap			*data;
}						t_json_object;

typedef struct			s_json_string
{
	t_json_vtype		type;
	char				*value;
}						t_json_string;

typedef struct			s_json_number
{
	t_json_vtype		type;
	double				value;
}						t_json_number;

typedef struct			s_json_array
{
	t_json_vtype		type;
	union u_json_value	**values;
	size_t				values_num;
}						t_json_array;

typedef struct			s_json_bool
{
	t_json_vtype		type;
	char				value;
}						t_json_bool;

typedef union			u_json_value
{
	t_json_object		obj;
	t_json_string		str;
	t_json_number		num;
	t_json_array		arr;
	t_json_bool			bol;
}						t_json_value;

/*
** t_json_parse_res: the result of a JSON parsing.
**
** -err: a string representing the parsing error, will be NULL if no errors.
** -line: the line at witch the error occurred. is set to 0 if no errors.
** -obj: the parsed JSON object. NULL if the parsing failed.
*/
typedef struct			s_json_parse_res
{
	char				*err;
	int					line;
	int					col;
	t_json_object		*obj;
}						t_json_parse_res;

#endif
