/*
 ============================================================================
 Name        : CWK2Q5.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement an algorithm in C which given a file containing a block of text as
 input, redacts all words from a given set of “redactable” words (also from a
 file), and outputs the result to a file called “result.txt”. For example,
 given the block of text:
    The quick brown fox jumps over the lazy dog

 and the redactable set of words:
    the, jumps, lazy

 the output text in “result.txt” should be
    *** quick brown fox ***** over *** **** dog

 Note that the number of stars in the redacted text is the same as the number
 of letters in the word that has been redacted, and that capitalization is
 ignored. You should not use any of the string libraries to answer this
 question. You should also test your program using the example files
 provided.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int BUFFER_SIZE = 64;						// The maximum length of a word that can be parsed by the program.
char **restricted_words;				// Array of all words to be redacted.
int restricted_words_count = 0; // Number of words to be redacted.

/* FUNCTIONS */
char *strcpy(char *destination, const char *source);
int strcmp(const char *str1, const char *str2);
void to_lower_case(char *str);
void redact_words(const char *text_filename, const char *redact_words_filename);
void parse_word(const char *buffer, char **parsed);
int get_redacted(const char *redact_words_filename);
int matches_redactable(const char *letters);
char *char_pointers_to_string(const char **letters);

int main(int argc, char *argv[])
{
	const char *input_file = "./debate.txt";
	const char *redact_file = "./redact.txt";
	redact_words(input_file, redact_file);
	return EXIT_SUCCESS;
}

/*
* Redacts words from one file using words provided by another before outputting the result to result.txt.
*
* @param redact_words_filename The filename of the input file containing words to be redacted.
* @param text_filename The filename of the text file to be redacted.
*/
void redact_words(const char *text_filename, const char *redact_words_filename)
{
	get_redacted(redact_words_filename);
	FILE *inp;
	FILE *out;
	char buf[BUFFER_SIZE];
	inp = fopen(text_filename, "r");
	out = fopen("result.txt", "w");
	if (!inp || !out)
		return;

	int exists = fscanf(inp, "%s", buf);
	while (exists == 1)
	{
		char *parsed;
		parse_word(buf, &parsed);
		if (matches_redactable(parsed))
		{
			int counter = 0;
			while (buf[counter])
			{
				if (isalpha(buf[counter]))
					buf[counter] = '*';
				counter++;
			}
		}

		fputs(buf, out);
		fputc(' ', out);

		exists = fscanf(inp, "%s", buf);
	}
	free(buf);
	fclose(inp);
	fclose(out);
}

/*
* Extracts all redactable words from an input file.
*
* @param redact_words_filename The filename of the input file containing words to be redacted.
* @return 1 if successful, 0 otherwise.
*/
int get_redacted(const char *redact_words_filename)
{
	FILE *inp;
	char buf[BUFFER_SIZE];
	inp = fopen(redact_words_filename, "r");
	if (!inp)
		return 0;

	int exists = fscanf(inp, "%s", buf);
	while (exists == 1)
	{
		restricted_words_count += 1;
		char *parsed;
		parse_word(buf, &parsed);
		restricted_words = realloc((void *)restricted_words, restricted_words_count * sizeof(char *));
		restricted_words[restricted_words_count - 1] = parsed;

		exists = fscanf(inp, "%s", buf);
	}
	free(buf);
	fclose(inp);
	return 1;
}

/*
* Converts a string to a consistent format.
*
* @param buffer The string to be converted.
* @param parsed The result of the conversion applied to the string.
*/
void parse_word(const char *buffer, char **parsed)
{
	char letters[BUFFER_SIZE];
	char buffer_copy[BUFFER_SIZE];
	strcpy(buffer_copy, buffer);
	to_lower_case(buffer_copy);

	int buffer_counter = 0;
	int letters_counter = 0;
	while (buffer_copy[buffer_counter])
	{
		if (isalpha(buffer_copy[buffer_counter]))
		{
			letters[letters_counter] = buffer_copy[buffer_counter];
			letters_counter++;
		}
		buffer_counter++;
	}
	letters[letters_counter] = '\0';

	*parsed = malloc(letters_counter + 1 * sizeof(char));
	if (*parsed != NULL)
	{
		strcpy(*parsed, letters);
	}
}

/*
* Reimplementation of the strcpy function from string.h
*
* @param destination A pointer to where the string is to be copied to.
* @param source A pointer to where the string is to be copied from.
* @return A pointer to the destination.
*/
char *strcpy(char *destination, const char *source)
{
	int counter = 0;
	while (source[counter])
	{
		destination[counter] = source[counter];
		counter++;
	}
	destination[counter] = '\0';
	return destination;
}

/*
* Converts a string to lowercase.
*
* @param str The string to be made lower case.
*/
void to_lower_case(char *str)
{
	for (char *p = str; *p; p++)
	{
		*p = tolower(*p);
	}
}

/*
* Checks whether a string matches any of the words to be redacted.
*
* @param parsed A parsed version of the string to be checked.
* @return 1 if the string matches, 0 otherwise.
*/
int matches_redactable(const char *parsed)
{
	for (int i = 0; i < restricted_words_count; i++)
	{
		if (strcmp(restricted_words[i], parsed) == 0)
			return 1;
	}
	return 0;
}

/*
* Reimplementation of the strcmp function from string.h
*
* @param str1, str2 Strings to be compared to each other.
* @return The numerical difference between the first two letters that do not match.
*/
int strcmp(const char *str1, const char *str2)
{
	while (*str1)
	{
		if (*str1 != *str2)
		{
			break;
		}
		str1++;
		str2++;
	}
	return *str1 - *str2;
}