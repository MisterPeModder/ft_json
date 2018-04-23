/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_datatypes.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 10:07:19 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/23 16:35:32 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_DATATYPES_H
# define JSON_DATATYPES_H

# include <stdlib.h>
# include <libft_containers/hashmap.h>

/*
** t_json_vtype: stores the available types in json.
** NOTE: JSON_SPECIAL_CHAR is (as its name implies) special, it will NEVER
** appear in a json_object and therfore should NEVER be used by the user.
*/
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

/*
** t_json_object: the json object structure.
**
** the contents of this structure should NEVER be manipulated directly.
** User json_obj_get/json_obj_set function instead.
*/
typedef struct			s_json_object
{
	t_json_vtype		type;
	t_hashmap			*data;
}						t_json_object;

/*
** t_json_string: holds a json string.
**
** the 'value' string is safe to use and modify, but keep in mind that you
** should not free it if you don't know what you are doing.
*/
typedef struct			s_json_string
{
	t_json_vtype		type;
	char				*value;
}						t_json_string;

/*
** t_json_number: holds a json number that is represented by a double.
**
** the 'value' double is safe to use and modify.
*/
typedef struct			s_json_number
{
	t_json_vtype		type;
	double				value;
}						t_json_number;

/*
** t_json_array: the json object structure.
**
** the contents of this structure should NEVER be manipulated directly.
** User json_arr_get/json_arr_set function instead.
*/
typedef struct			s_json_array
{
	t_json_vtype		type;
	union u_json_value	**values;
	size_t				values_num;
}						t_json_array;

/*
** t_json_number: holds a json boolean that is represented by a char.
**
** the 'value' char is safe to use and modify.
*/
typedef struct			s_json_number
typedef struct			s_json_bool
{
	t_json_vtype		type;
	char				value;
}						t_json_bool;

/*
** t_json_value: contains all the structures corresponding to each type.
**
** NOTE: the JSON_NULL type has no structure representing it.
** You can only query the type using something like:
** my_value->obj.type == JSON_NULL
*/
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
