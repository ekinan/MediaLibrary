# MediaLibrary
------------------------------------------------------------------------------------------------------------------
Below is a brief summary of the project. For more details, specifically
on the implementation, please see the project write up "CS 163 - Design 
Write Up FINAL".

This program was a text-based media library that I wrote for Portland 
State's CS 163 course. It stores the user's songs, movies and TV shows.
The media library can either be created from scratch, or loaded from a
text file (see "my_library.txt" for an example of what this file looks
like). Upon execution, the user has the option of choosing either a
2-3 tree or a hash table to store their library. They can do all of the
basic CRUD operations (except for update) for every media entry. When
they are done browsing through their library, they can choose to save
it to a separate file so that they may reload it again in the future.
The file will also contain data-structure specific statistics (e.g.
the number of collisions if a hash table was used, the height of the
tree if a tree was used, etc.). Note that the user can switch to a
different data structure when they reload their library.

------------------------------------------------------------------------------------------------------------------
To compile the program, please type
	make clean
	make
into the command prompt.

To run the program, please type
	./a.out
into the command prompt.

