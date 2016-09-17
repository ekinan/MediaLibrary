#include <iostream>
#include <iomanip>
#include <fstream>
#include "HashTable.h"
#include "MediaLibrary.h"
#include "TwoThreeTree.h"

/*
Author: Enis K Inan
Date: 06/09/2015

The following program allows the user an opportunity to compare the implementations of a 2-3 Tree
and a Hash Table via their own media library. They can use either of the two data structures in adding,
removing, searching for, or displaying any entry. Once they are finished doing so, the contents of the library are
 output to an external file, with the bottom of the file detailing the relevant statistics gathered
from the data structure they used.

The user has the option to either open an existing media library, or create their own. Using this library,
they can either:

1. Add a new media item
2. Search for a media item
3. Remove a media item
4. Display the contents of the library
99. Exit to the main menu

They can choose to enter 99 in the main menu should they wish to exit the program.

*/

using namespace std;

const int INDENT = 10; //Indentation size for user output

//Pauses the program until the user enters a key to continue
void pause();


/*
Gives the user the choice to either open an existing library, or to build a new one from scratch
@post If the user chooses to open an existing library, constructLibrary is called
Regardless, the function proceeds to call libraryMenu
@param libraryPtr Pointer to the media library
*/
void libraryOptions(MediaLibraryInterface* libraryPtr);


/*
Constructs an existing library from an external file that stores its contents
@post libraryPtr stores the entire contents of the external file
@Param libraryPtr Pointer to the media library
*/
void constructLibrary(MediaLibraryInterface* libraryPtr);

/*
Gets the next media entry from the file storing the library.
Used in complement with constructLibrary
@post entry contains the contents of the next media entry in inFile
@param entry The entry to be modified
inFile The external file storing the library
*/
void getMediaEntry(MediaEntry& entry, ifstream& inFile);

/*
Gets the desired user entry into the media library. Used in complement with the
add, search and remove functions below.
@return User's desired media item
*/
MediaEntry getUserEntry();

/*
Displays a menu giving the user options to interact with the library.
There are five options: add, search, remove, display and exit
@post Calls add if the option chosen is add, search if it's search, remove if it's remove,
display if it's display, and exit if it's exit.
@param Pointer to the media library
*/
void libraryMenu(MediaLibraryInterface* libraryPtr);

/*
Adds a user-defined entry into the library.
@post A user-specified media item is created and stored in the library.
@param libraryPtr Pointer to the media library
*/
void add(MediaLibraryInterface* libraryPtr);

/*
Searches for an existing entry in the libary
@post The entry is displayed/outputted if it is in the library, otherwise a message is outputted
telling the user that the item is not found.
@param libraryPtr Pointer to the media library
*/
void search(MediaLibraryInterface* libraryPtr);

/*
Removes a user-defined entry from the library
@post The user-defined item is removed from the library if it exists, otherwise nothing happens
@param libraryPtr Pointer to the media library
*/
void remove(MediaLibraryInterface* libraryPtr);

/*
Gives the user four options:
1. Display all of the movies
2. Display all TV shows
3. Display all music
4. Display all entries
and then accordingly displays each.

@post The user-selected option is executed
@param libraryPtr Pointer to the media library
*/
void display(MediaLibraryInterface* libraryPtr);

/*
Stores the contents of the library and relevant data structure statistics
in a user-specified external file and then takes them back to the program menu.
@post Contents of the library and data structure statistics are stored in an external file.
@param libraryPtr Pointer to the media library
*/
void exit(MediaLibraryInterface* libraryPtr);




int main()
{
	MediaLibraryInterface* libraryPtr = NULL;
	int choice;

	system("clear");
	cout << "Welcome! ";
	do
	{
		do //Prompts user to select an option until a valid input is received
		{
			cout << "Please select your choice from the three options below:" << endl;

			cout << setw(INDENT) << "1. Use a 2-3 Tree to store your media library" << endl;
			cout << setw(INDENT) << "2. Use a Hash Table to store your media library" << endl;
			cout << setw(INDENT) << "99. Exit the program" << endl << endl;
			cout << "Choice: ";

			cin >> choice;
			cin.clear();
			cin.ignore(1000, '\n'); //Clean the input
			cout << endl << endl;

		} while ((choice != 99) && (choice != 1) && (choice != 2));

		if (choice != 99)
		{
			if (choice == 1)
				libraryPtr = new MediaLibrary<TwoThreeTree>;
			else
				libraryPtr = new MediaLibrary<HashTable>;

			libraryOptions(libraryPtr);

			libraryPtr = NULL;
		}
	} while (choice != 99);

	cout << "Thank you for using the program. Have a good day!" << endl << endl;

	return 0;
}

void pause()
{
	char entry;
	cout << "Enter any key to continue: "; //Pauses program until user enters a key
	cin >> entry;
	cin.clear();
	cin.ignore(1000, '\n');
}

void libraryOptions(MediaLibraryInterface* libraryPtr)
{
	system("clear");
	int choice;
	do //Prompts user to select one of two options until valid input is received
	{
		cout << "Please select from the two options below:" << endl;
		cout << setw(INDENT) << "1. Open an existing file containing your media library" << endl;
		cout << setw(INDENT) << "2. Create a new media library." << endl << endl;
		cout << "Choice: ";

		cin >> choice;
		cin.clear();
		cin.ignore(1000, '\n');
		cout << endl << endl;
	} while ((choice != 1) && (choice != 2));

	if (choice == 1)
		constructLibrary(libraryPtr);

	libraryMenu(libraryPtr);
}

void constructLibrary(MediaLibraryInterface* libraryPtr)
{
	system("clear");
	ifstream inFile;

	do //Asks user to enter the file name of the library
	{  //Does so until a valid file name is entered
		char fileName[50];
		cout << "Please enter the file name with a .txt after: ";
		cin.get(fileName, 50);
		cin.clear();
		cin.ignore(1000, '\n');
		cout << endl << endl;

		inFile.open(fileName);
	}
	while (!inFile);

	while ((inFile.peek() != '\n')) //Constructs the library until the new line character
	{				//designating the end of the file is reached
		MediaEntry temp;
		getMediaEntry(temp, inFile);
		libraryPtr->addEntry(temp);
	}

	inFile.close();

	cout << "The contents of your media library were successfully imported." << endl;
	pause();
}

void getMediaEntry(MediaEntry& entry, ifstream& inFile)
{
	char mediaTitle[200]; char mediaType; //Readsthe title of the entry and type
	inFile.getline(mediaTitle, 200);      //from the input file
	inFile >> mediaType;
	inFile.ignore(1000, '\n');

	entry.setTitle(mediaTitle);
	entry.setMediaType(mediaType);
}

void libraryMenu(MediaLibraryInterface* libraryPtr)
{
	int choice = 0;

	do //Cycles until the user decides to exit and close their library
	{
		system("clear");
		do //Prompts user to select five options until a valid input is received
		{
			cout << "Please select your choice from the five options below:" << endl;

			cout << setw(INDENT) << "1. Add a new entry" << endl;
			cout << setw(INDENT) << "2. Search for an entry" << endl;
			cout << setw(INDENT) << "3. Remove an entry" << endl;
			cout << setw(INDENT) << "4. Display" << endl;
			cout << setw(INDENT) << "99. Main Menu" << endl << endl;;
			cout << "Choice: ";

			cin >> choice;
			cin.clear();
			cin.ignore(1000, '\n');
			cout << endl << endl;

		} while ((choice != 99) && (choice != 1) && (choice != 2) &&
				(choice != 3) && (choice != 4));

		switch (choice) //Calls corresponding functions depending on their choice
		{
		case 1:
			add(libraryPtr);
			break;
		case 2:
			search(libraryPtr);
			break;
		case 3:
			remove(libraryPtr);
			break;
		case 4:
			display(libraryPtr);
			break;
		}
	} while (choice != 99);

	exit(libraryPtr);
}

MediaEntry getUserEntry()
{
	char mediaTitle[200]; char type;
	cout << "Please enter the name of the media file: "; //Prompts user to enter the name of their media file
	cin.get(mediaTitle, 200);
	cin.clear();
	cin.ignore(1000, '\n');
	cout << endl << endl;

	int choice;
	do //Prompts user to select the file genre until a valid input is received
	{
		cout << "Please indicate the type of media file this is:" << endl;


		cout << setw(INDENT) << "1. Movie" << endl;
		cout << setw(INDENT) << "2. TV Show" << endl;
		cout << setw(INDENT) << "3. Music/Song" << endl << endl;
		cout << "Choice: ";

		cin >> choice;
		cin.clear();
		cin.ignore(1000, '\n');
		cout << endl << endl;

	} while ((choice != 1) && (choice != 2) && (choice != 3));

	switch (choice) //Adjusts the type of the entry accordingly
	{
	case 1:
		type = 'M';
		break;
	case 2:
		type = 'T';
		break;
	default:
		type = 'S';
		break;
	}

	return MediaEntry(mediaTitle, type);
}


void add(MediaLibraryInterface* libraryPtr)
{
	system("clear");
	libraryPtr->addEntry(getUserEntry());

	cout << "Entry has been added to the library." << endl;
	pause();
}

void search(MediaLibraryInterface* libraryPtr)
{
	system("clear");
	MediaEntry userEntry = getUserEntry();

	if (libraryPtr->contains(userEntry)) //Outputs the entry if it is found
	{
		cout << "Entry: " <<  userEntry << endl;
		cout << "Type: " << userEntry.getMediaType() << endl;
	}
	else //Entry is not in the library
		cout << "Entry is not in your library." << endl;
	cout << endl;

	pause();
}

void remove(MediaLibraryInterface* libraryPtr)
{
	system("clear");
	if (libraryPtr->removeEntry(getUserEntry()))
		cout << "Removal was successful" << endl;
	else
		cout << "Entry is not in your library." << endl;
	pause();
}

void display(MediaLibraryInterface* libraryPtr)
{
	system("clear");

	int choice;
	do //Prompts user to select from four display options below until a valid input
	{
		cout << "Please select from the four options below: " << endl;


		cout << setw(INDENT) << "1. Display movies" << endl;
		cout << setw(INDENT) << "2. Display TV shows" << endl;
		cout << setw(INDENT) << "3. Display music" << endl;
		cout << setw(INDENT) << "4. Display all media" << endl << endl;
		cout << "Choice: ";

		cin >> choice;
		cin.clear();
		cin.ignore(1000, '\n');
		cout << endl << endl;

	} while ((choice != 1) && (choice != 2) && (choice != 3) && (choice != 4));

	switch (choice) //Selects the corresponding function depending on their choice
	{
	case 1:
		libraryPtr->displayAllMovies();
		break;
	case 2:
		libraryPtr->displayAllTv();
		break;
	case 3:
		libraryPtr->displayAllMusic();
		break;
	default:
		libraryPtr->displayAll();
		break;
	}

	cout << endl << endl;
	pause();
}

void exit(MediaLibraryInterface* libraryPtr)
{
	system("clear");
	ofstream outFile;

	char fileName[50]; //Asks user to enter the file name of the file
	cout << "Please enter the file name to save your library, with a .txt after: ";
	cin.get(fileName, 50);
	cin.clear();
	cin.ignore(1000, '\n');
	cout << endl << endl;

	outFile.open(fileName);

	libraryPtr->writeToFile(outFile); //Saves contents of the library to the File
	outFile << "Here are the statistics for the data structure: " << endl;
	libraryPtr->displayStatistics(outFile);
	outFile << endl << endl;

	outFile.close();

	delete libraryPtr;

	cout << "Your media library was successfully exported." << endl;
	pause();
	system("clear"); //Takes user back to the program menu
}
