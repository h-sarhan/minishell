/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:06:31 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/01 13:13:44 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

int	ft_stderr(const char *str, ...);
int	ft_putchar(int c);
int	ft_putstr(char *str);
int	ft_hexaddr(unsigned long n);
int	ft_intdec(int n);
int	ft_unint(unsigned int n);
int	ft_hexalow(unsigned int n);
int	ft_hexacap(unsigned int n);

#endif