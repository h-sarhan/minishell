/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:09:20 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/01 13:13:35 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_checkformat(char c, va_list lst)
{
	int	len;

	len = 0;
	if (c == 'c')
		len += ft_putchar(va_arg(lst, int));
	else if (c == 's')
		len += ft_putstr(va_arg(lst, char *));
	else if (c == 'p')
		len += ft_hexaddr(va_arg(lst, unsigned long));
	else if (c == 'd' || c == 'i')
		len += ft_intdec(va_arg(lst, int));
	else if (c == 'u')
		len += ft_unint(va_arg(lst, unsigned int));
	else if (c == 'x')
		len += ft_hexalow(va_arg(lst, unsigned int));
	else if (c == 'X')
		len += ft_hexacap(va_arg(lst, unsigned int));
	else if (c == '%')
		len += ft_putchar(c);
	return (len);
}

int	ft_stderr(const char *str, ...)
{
	va_list	lst;
	int		i;
	int		len;

	i = 0;
	len = 0;
	va_start(lst, str);
	while (str[i])
	{
		if (str[i] == '%')
			len += ft_checkformat(str[++i], lst);
		else
			len += ft_putchar(str[i]);
		i++;
	}
	va_end(lst);
	return (len);
}
