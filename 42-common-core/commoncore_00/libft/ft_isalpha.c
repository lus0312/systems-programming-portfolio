int	ft_isalpha(int n)
{
	if ((n >= 65 && n <= 90) || (n >= 97 && n <= 122))
		return (1);
	else
		return (0);
}
