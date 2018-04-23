/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 10:07:33 by yguaye            #+#    #+#             */
/*   Updated: 2018/04/23 16:14:10 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_H
# define JSON_H

# include "json_internal.h"

/*
** json_from_file: parses a file's contents into a JSON object.
**
** -path: the path of the JSON file.
**
** returns: a t_json_parse_res instance.
*/
t_json_parse_res		*json_from_file(const char *path);

/*
** json_from_str: parses a string into a JSON object.
**
** -src: the source string.
**
** returns: a t_json_parse_res instance.
*/
t_json_parse_res		*json_from_str(const char *src);

/*
** json_to_tab: converts the passed JSON value into an array.
**
** -val: the JSON value.
**
** returns: a string array containg each line of the serialized JSON value,
**          NULL if an error occured.
*/
char					**json_to_tab(const t_json_value *val);

/*
** json_to_str: converts the passed JSON value into a string.
**
** -val: the JSON value.
**
** returns: a string containg each line of the serialized JSON value,
**          NULL if an error occured.
*/
char					*json_to_str(const t_json_value *val);

/*
** json_to_file: puts the contents of a JSON object into a file.
**
** -obj: a pointer to a JSON object instance.
** -p: the file path, creates the file if it doesn't exists.
**
** returns: 1 if successful, 0 if not.
*/
int						json_to_file(const t_json_object *obj, const char *p);

/*
** json_release_value: frees the passed JSON value.
*/
void					json_release_value(t_json_value **val);

/*
** json_release_file: frees the passed JSON file.
*/
void					json_release_file(t_json_parse_res **file);

/*
** json_get_val: gets the value stored at key in a json OBJECT.
**
** -obj: the json object to search from.
** -key: the key;
**
** returns: a json value if found, null otherwise.
*/
t_json_value			*json_obj_get_value(const t_json_object *obj,
		const char *key);

/*
** json_get_val: gets the value stored at index i in a json ARRAY.
**
** -arr: the json array to search from.
** -i: the index;
**
** returns: a json value if found, null otherwise.
*/
t_json_value			*json_arr_get_value(const t_json_array *arr, size_t i);

#endif
