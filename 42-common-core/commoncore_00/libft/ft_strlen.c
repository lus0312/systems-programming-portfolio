int	ft_strlen(const char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (*str && str[i] != '\0')
	{
		i++;
		len++;
	}
	return (len);
}
