/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexacap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:33:23 by mkhan             #+#    #+#             */
/*   Updated: 2022/05/19 14:50:59 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_hexacap(unsigned int n)
{
	int		len;
	char	*s;

	len = 0;
	s = "0123456789ABCDEF";
	if (n > 15)
		len += ft_hexacap(n / 16);
	len += ft_putchar(s[n % 16]);
	return (len);
}
