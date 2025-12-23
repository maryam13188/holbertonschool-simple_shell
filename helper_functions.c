#include "shell.h"

/**
 * _strlen - Returns length of string
 * @s: String
 * Return: Length
 */
int _strlen(char *s)
{
	int len = 0;

	if (!s)
		return (0);

	while (s[len])
		len++;

	return (len);
}

/**
 * _strdup - Duplicates a string
 * @str: String to duplicate
 * Return: Duplicated string
 */
char *_strdup(char *str)
{
	char *dup;
	int i, len;

	if (!str)
		return (NULL);

	len = _strlen(str);
	dup = malloc(len + 1);

	if (!dup)
		return (NULL);

	for (i = 0; i <= len; i++)
		dup[i] = str[i];

	return (dup);
}
