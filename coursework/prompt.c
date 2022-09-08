#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prompt.h"

// Read in a variable-length user string
// prompting the user with the given message.
// Caller should free the return value when finished with it.

char *prompt();
int main()
{
	char word[20];
	char *p = prompt();
	strcpy(word, p);
	return 0;
}

char *prompt()
{
	int cur_array_len = 2;
	int pos = 0;
	char ch;
	char *text = NULL;

	// printf("%s", mesg);

	text = malloc(sizeof(char) * cur_array_len);
	do
	{
		scanf("%c", &ch);
		if (ch == '\n')
		{
			text[pos] = '\0';
		}
		else
		{
			text[pos] = ch;
			pos++;

			if (pos == cur_array_len - 1)
			{
				// expand array
				char *t2 = malloc(sizeof(char) * cur_array_len * 2);

				int i;
				for (i = 0; i < cur_array_len; i++)
				{
					t2[i] = text[i];
				}

				cur_array_len *= 2;
				free(text);
				text = t2;
			}
		}
	} while (ch != '\n');

	return text;
}
