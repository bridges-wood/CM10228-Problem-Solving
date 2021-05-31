/*
 ============================================================================
 Name        : CWK2Q3.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement your own Hash Table in C for storing and searching names, i.e. char
 arrays. In the event of collisions, you should use linear probing with an
 interval of 1. The hash function should be the sum of the ASCII values of the
 string modulo the size of the underlying data structure. Your Hash Table
 implementation should have the following interface:
	int hash_function(const char *key)
	void resize_map(int new_size)
	void add_to_map(const char *name)
	int remove_from_map(const char *name)
	int search_map(const char *name)
	void print_map()

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **hash_map;						// Location where names are stored.
float load = 0;							// Measures the load factor of the map.
int entries = 0;						// Counts the number of entries in the map.
int map_size = 0;						// The total size of the map data structure.
int PROBING_INTERVAL = 1;		// The probing interval.
char *DUMMY = "FL7IuApGjF"; // Dummy value to maintain search effectiveness.

/* FUNCTIONS */
void update_load();
int hash_function(const char *key);
void resize_map(int new_size);
void add_to_map(const char *name);
int remove_from_map(const char *name);
int search_map(const char *name);
void print_map();

int main(int argc, char *argv[])
{
	char *stringOne = "#Hello world";
	char *stringTwo = "How are you?";
	char *stringThree = "Be the best you...!!";
	char *stringFour = "Be kind to yourself";
	char *stringFive = "Principles of Programming 2";

	add_to_map(stringOne);
	add_to_map(stringTwo);
	add_to_map(stringOne);
	add_to_map(stringThree);
	add_to_map(stringFour);
	add_to_map(stringFive);
	print_map();

	int ret = search_map(stringOne);
	if (ret)
		printf("Found %s!\n", stringOne);

	remove_from_map(stringThree);
	print_map();

	ret = search_map(stringFive);
	if (ret)
		printf("Found %s!\n", stringFive);
	print_map();

	add_to_map(stringThree);
	add_to_map("Manuel");
	print_map();

	return EXIT_SUCCESS;
}

/*
* Resizes the hash map to allow for new entries.
*
* @param new_size The new size of the map.
*/
void resize_map(int new_size)
{
	if (new_size == 0 || new_size < entries)
		return;

	char **old_map = hash_map; // Keep reference to previous map
	int old_size = map_size;
	map_size = new_size;
	hash_map = (char **)malloc(map_size * sizeof(char *)); // Resize map
	if (hash_map)
	{
		int remaining_entries = entries;
		entries = 0;
		int counter = 0;
		while (remaining_entries > 0 && counter <= old_size)
		{ // For all the entries in the old map...
			if (old_map[counter] && strcmp(old_map[counter], DUMMY) != 0)
			{
				char *stored = old_map[counter];
				add_to_map(stored);
				free(old_map[counter]);
				remaining_entries -= 1;
			}
			counter++;
		}
	}

	free(old_map);
	update_load();
}

/*
* Updates the load value of the hash map.
*/
void update_load()
{
	load = (float)entries / (float)map_size;
	if (load > 0.7)
		resize_map(map_size * 2);
}

/*
* Inserts a name into the hash map.
*
* @param name The name to be inserted into the map.
*/
void add_to_map(const char *name)
{
	if (map_size == 0)
		resize_map(2);

	int hash = hash_function(name);

	// Avoid redundant inserts
	if (hash_map[hash] && strcmp(name, hash_map[hash]) == 0)
		return;

	while (hash_map[hash] && strcmp(hash_map[hash], DUMMY) != 0)
	{																							 // Find space in hash_map
		hash = (hash + PROBING_INTERVAL) % map_size; // Wrap around
	}
	char *dest = (char *)malloc((strlen(name) + 1) * sizeof(char));
	if (dest)
	{
		strcpy(dest, name);
		hash_map[hash] = dest;
		entries += 1;
		update_load();
	}
}

/*
* Computes the hash of the given key.
*
* @param key The key to be hashed.
*/
int hash_function(const char *key)
{
	char letter = *key;
	int sum = 0;
	while (*key)
	{
		sum += *key;
		key++;
	}
	return sum % map_size;
}

/*
* Removes the given name from the hash map if it exists.
*
* @param name The name to be removed from the hash map.
* @return 1 if the name was removed, 0 otherwise.
*/
int remove_from_map(const char *name)
{
	int hash = hash_function(name);
	int location = hash;
	do
	{
		if (strcmp(name, hash_map[location]) == 0)
		{
			hash_map[location] = realloc(hash_map[location], (strlen(DUMMY) + 1 * sizeof(char)));
			strcpy(hash_map[location], DUMMY); // Replace with dummy.
			entries -= 1;											 // Load is only affected by non-dummy values.
			update_load();
			return 1;
		}
		location = (location + PROBING_INTERVAL) % map_size;
	} while (!location);
	return 0;
}

/*
* Searches the hash map for a given name.
*
* @param name The name to be searched for.
* @return 1 if the name is found, 0 otherwise.
*/
int search_map(const char *name)
{
	int hash = hash_function(name);
	int location = hash;
	do
	{
		if (strcmp(name, hash_map[location]) == 0)
			return 1;
		location = (location + PROBING_INTERVAL) % map_size; // Wrap around
	} while (hash_map[location]);
	return 0;
}

/*
* Prints the contents of the hash map.
*/
void print_map()
{
	char **location = hash_map;
	printf("---BEGIN MAP---\n");
	printf("Load: %f \n", load);
	printf("Entries: %d \n", entries);
	printf("Map Size: %d \n", map_size);
	while (location - hash_map < map_size)
	{
		if (*location)
			printf("%ld : %p : %s \n", location - hash_map, location, *location);
		location += PROBING_INTERVAL;
	}
	printf("---END MAP---\n");
}
