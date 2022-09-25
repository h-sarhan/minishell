/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 22:18:16 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 17:09:21 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\f'
		|| c == '\n' || c == '\r'
		|| c == '\t' || c == '\v');
}

int	ft_atoi(const char *str)
{
	int	num;
	int	i;
	int	sign;

	num = 0;
	i = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (ft_strncmp(&str[i], "-2147483648", 11) == 0)
		return (INT_MIN);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num += str[i++] - '0';
	}
	return (num * sign);
}
long	ft_atol(const char *str, bool *check)
{
	long	num;
	long	temp;
	int		i;
	int		sign;

	num = 0;
	i = 0;
	sign = 1;
	*check = true;
	while (is_space(str[i]))
		i++;
	if (ft_strncmp(&str[i], "-9223372036854775808", 20) == 0)
		return (LONG_MIN);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		temp = num;
		num *= 10;
		num += str[i++] - '0';
		if (temp > num)
		{
			*check = false;
			return (-1);
		}
	}
	return (num * sign);
}
