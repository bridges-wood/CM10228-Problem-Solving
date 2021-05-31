/*
 ============================================================================
 Name        : CWK2Q7.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement a Columnar Transposition Cipher in C to encrypt a message of any
 length. A Columnar Transposition Cipher is transposition cipher that follows
 a simple rule for mixing up the characters in the plaintext to form the
 ciphertext.

 As an example, to encrypt the message ATTACKATDAWN with the keyword KEYS,
 we first write out our message as shown below,
    K	E	Y	S
    A	T	T	A
    C	K	A	T
    D	A	W	N

 Note: if the message to encode does not fit into the grid, you should pad
 the message with x's or random characters for example, ATTACKNOW with the
 keyword KEYS might look like below,
    K	E	Y	S
    A	T	T	A
    C	K	N	O
    W	X	X	X

 Once you have constructed your table, the columns are now reordered such
 that the letters in the keyword are ordered alphabetically,
    E	K	S	Y
    T	A	A	T
    K	C	T	A
    A	D	N	W

 The ciphertext is now read off along the columns, so in our example above,
 the ciphertext is TAATKCTAADNW.

 You should demonstrate your implementation by encrypting the file in the
 folder Q7 using the keyword - LOVELACE.

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void encrypt_columnar(const char *message_filename, const char *key, char **result);
void read_file(const char *message_filename, char **result);
void sort_grid(int rows, int columns, char (*grid)[rows][columns]);

int main(int argc, char *argv[])
{
   const char *example_message = "./text.txt";
   const char *example_key = "LOVELACE";
   char *encrypted_message = NULL;
   encrypt_columnar(example_message, example_key, &encrypted_message);
   printf("Encrypted message = %s\n", encrypted_message);
   free(encrypted_message);
   return EXIT_SUCCESS;
}

/*
* Encrypts the text stored in a file using the Columnar Transposition Cipher.
*
* @param message_filename The name of the file to be encrypted.
* @param key The key to be used to encrypt the message.
* @param result The result of the encryption.
*/
void encrypt_columnar(const char *message_filename, const char *key, char **result)
{
   read_file(message_filename, result);
   int key_length = strlen(key);
   int text_length = strlen(*result);
   int rows = ((text_length + key_length - 1) / key_length) + 1;
   char grid[rows][key_length];

   // Write key into the cipher grid.
   for (int i = 0; i < key_length; i++)
   {
      grid[0][i] = key[i];
   }

   // Write plaintext onto the cipher grid.
   for (int i = 0; i < (rows - 1) * key_length; i++)
   {
      if (i >= text_length) // Insert padding
      {
         grid[rows - 1][i % key_length] = 'X';
         continue;
      }
      grid[(i / key_length) + 1][i % key_length] = (*result)[i];
   }

   // Encrypt with the sorted key.
   sort_grid(rows, key_length, &grid);

   // Read ciphertext out of grid.
   *result = malloc(((rows - 1) * key_length) + 1);
   if (*result)
   {
      for (int row = 1; row < rows; row++)
      {
         for (int column = 0; column < key_length; column++)
         {
            (*result)[8 * (row - 1) + column] = grid[row][column];
         }
      }
   }
   (*result)[(rows - 1) * key_length] = '\0'; // Terminate string
}

/*
* Reads a text file into a string.
*
* @param message_filename The name of the file to be read from.
* @param result The string to be written to.
*/
void read_file(const char *message_filename, char **result)
{
   long letters = 0;
   FILE *in = fopen(message_filename, "r");
   if (in != NULL)
   {
      fseek(in, 0, SEEK_END);  // Go to the end of the file.
      long bytes = ftell(in);  // Count bytes in file.
      rewind(in);              // Return to start.
      *result = malloc(bytes); // Allocates maximum possible space needed.

      if (*result != NULL)
      {
         for (long i = 0; i < bytes / sizeof(char); i++)
         {
            char next = fgetc(in);
            if (isalpha(next))
            {
               (*result)[letters] = toupper(next);
               letters += 1;
            }
         }
         *result = realloc(*result, letters + 1);
      }
   }
   fclose(in);
}

/*
* Sorts a rows x columns array of characters by the values of the first row using Bubble Sort.
*
* @param rows The number of rows in the array.
* @param columns The number of rows in the array.
* @param grid The array to be sorted.
*/
void sort_grid(int rows, int columns, char (*grid)[rows][columns])
{
   int changed = 1, max = columns;
   while (changed)
   {
      changed = 0;
      for (int i = 1; i < columns; i++)
      {
         if ((*grid)[0][i] < (*grid)[0][i - 1])
         {
            // Swap items for all rows.
            for (int row = 0; row < rows; row++)
            {
               char temp = (*grid)[row][i];
               (*grid)[row][i] = (*grid)[row][i - 1];
               (*grid)[row][i - 1] = temp;
            }
            changed = 1;
         }
      }
      max--; // After each iteration, the next from last element should be in the correct place.
   }
}
