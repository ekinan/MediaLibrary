#include "MediaEntry.h"
#include <cstring>
#include <cassert>

std::ostream& operator<<(std::ostream& os, const MediaEntry& entry)
{
	for (int i = 0; i < entry.titleLength; i++) //Output the title of the media entry
		os << entry.title[i];

	return os;
}

MediaEntry::MediaEntry()
{
	title = NULL;
	type = 'S'; //Default type is S (Song)
	titleLength = 0;
}

MediaEntry::MediaEntry(const char* mediaTitle, char mediaType)
{
	title = NULL;
	setTitle(mediaTitle);
	setMediaType(mediaType);
}

MediaEntry::MediaEntry(const MediaEntry& otherEntry)
{
	copyEntry(otherEntry);
}

MediaEntry::~MediaEntry()
{
	if (title != NULL) //Destructor, deallocate the dynamic memory of title
	{
		delete [] title;
		title = NULL;
	}
}

void MediaEntry::copyEntry(const MediaEntry& otherEntry)
{
	titleLength = otherEntry.titleLength;
	if (otherEntry.title != NULL) //Copy the title of otherEntry if it exists
	{
		title = new char[titleLength+1];
		for (int i = 0; i < titleLength; i++)
			title[i] = otherEntry.title[i];
	}
	else
		title = NULL;

	type = otherEntry.type; //Copy its type as well
}

int MediaEntry::length() const
{
	return titleLength;
}

char MediaEntry::getMediaType() const
{
	return type;
}

void MediaEntry::setTitle(const char* mediaTitle)
{
	if (title != NULL) //Deallocate and remove the existing title, if any
	{
		delete [] title;
		title = NULL;
		titleLength = 0;
	}

	titleLength = std::strlen(mediaTitle);
	title = new char[titleLength+1];
	for (int i = 0; i < titleLength; i++) //Copy the contents of mediaTitle into title
		title[i] = mediaTitle[i];
}

void MediaEntry::setMediaType(char mediaType)
{
	switch (mediaType)
	{
	case 'T': //T = TV
	case 'M': //M = movie
	case 'S': //S = Music/Song
		type = mediaType;
		break;
	default:
		type = 'S'; //Assume song by default
		break;
	}
}

const MediaEntry& MediaEntry::operator=(const MediaEntry& otherEntry)
{
	if (this != &otherEntry)
	{
		if (title != NULL) //Delete the existing title
		{
			delete [] title;
			title = NULL;
		}
		copyEntry(otherEntry); //Copy the contents of otherEntry
	}

	return *this;
}

char& MediaEntry::operator[](int index) //Override [] operator to return the character corresponding
{					//to index
	assert(0 <= index && index < titleLength); //Terminate program if index is out of bounds
	return title[index];
}

const char& MediaEntry::operator[](int index) const
{
	assert(0 <= index && index < titleLength);
	return title[index];
}

int MediaEntry::compareTitles(const MediaEntry& otherEntry) const
{
	int value = 0; //Assume equal by default

	if (title == NULL && otherEntry.title != NULL) //By default, an empty title is the minimal element
		value = -1;			       //in the set
	else if (title != NULL && otherEntry.title == NULL) //This checks if any of the titles are empty
		value = 1;				    //Prior to comparison
	else //Both titles are nonempty or they are empty.
	{    //Regardless, function returns correct value for both

		int thisIndex = 0;
		int otherIndex = 0;

		int thisAlphLength = 0;
		int otherAlphLength = 0;

		while ((value == 0) && ((thisIndex < titleLength) && (otherIndex < otherEntry.titleLength)))
		{ 	//Compare every alphabetical character (in upper case) in both titles
			//until the first different character is encountered.
			int thisChar = toupper(getNextChar(title, titleLength, thisIndex, thisAlphLength));
			int otherChar = toupper(getNextChar(otherEntry.title, otherEntry.titleLength, otherIndex,
							otherAlphLength));

			if (thisChar < otherChar)
				value = -1; //-1 = less than
			else if (thisChar > otherChar)
			{
				value = 1; //1 = greater than
			}
		}

		if (value == 0) //Titles are either equal, or one title is a subset of the other
		{		//(i.e. deals with the case of "abc" and "abcd")

			while (thisIndex < titleLength) //Finish computing the non-blank lengths
				getNextChar(title, titleLength, thisIndex, thisAlphLength);
			while (otherIndex < otherEntry.titleLength)
				getNextChar(otherEntry.title, otherEntry.titleLength, otherIndex, otherAlphLength);

			if (thisAlphLength < otherAlphLength) //Shorter titles are "less"
				value = -1;
			else if (thisAlphLength > otherAlphLength)
				value = 1;
		}

	}

	return value;
}

char MediaEntry::getNextChar(char* phrase, int phraseLength, int& index, int& alphLength) const
{
	char nextChar = phrase[index];

	//Note the ASCII value of A = 65 and Z = 90
	for (; (index < phraseLength) && ((toupper(nextChar) < 65) || (toupper(nextChar) > 90)); index++)
		nextChar = phrase[index];

	index++; //Increment the index to avoid repeats next time the function is called

	if ((65 <= toupper(nextChar)) && (toupper(nextChar) <= 90)) //Ensure that the character is a part
		alphLength++;					    //of the alphabet
	else //No alphabetical character was found, so return the null character
		nextChar = '\0';

	return nextChar;
}

bool MediaEntry::operator==(const MediaEntry& otherEntry) const
{
	//Two media entries are equal if they have the same type, and if they have
	//the same title (i.e. name).
	if (type == otherEntry.type)
	{
		if (compareTitles(otherEntry) == 0)
			return true;
	}
	else
		return false;
}

int MediaEntry::getPrecedence(char type) const //Songs have highest precedence, followed by TV, then movie
{
	switch (type)
	{
	case 'M':
		return 0;
		break;
	case 'T':
		return 1;
		break;
	default: //Recall that default is song
		return 2;
		break;
	}
}

bool MediaEntry::operator<=(const MediaEntry& otherEntry) const
{
	int compValue = compareTitles(otherEntry);
	if (compValue == -1) //Alphabetical order is first precedence.
		return true;
	else if (compValue == 0) //Then we look at the types and their precedence
	{
		if (getPrecedence(type) < getPrecedence(otherEntry.type))
			return true;
		else if (getPrecedence(type) == getPrecedence(otherEntry.type))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool MediaEntry::operator>=(const MediaEntry& otherEntry) const
{
	int compValue = compareTitles(otherEntry);
	if (compValue == 1) //Alphabetical order is first precedence.
		return true;
	else if (compValue == 0) //Then we look at the types and their precedence
	{
		if (getPrecedence(type) > getPrecedence(otherEntry.type))
			return true;
		else if (getPrecedence(type) == getPrecedence(otherEntry.type))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool MediaEntry::operator<(const MediaEntry& otherEntry) const
{
	return (!(*this >= otherEntry)); //Since >= is the complement of <
}

bool MediaEntry::operator>(const MediaEntry& otherEntry) const
{
	return (!(*this <= otherEntry)); //Since <= is the complement of >
}

bool MediaEntry::operator!=(const MediaEntry& otherEntry) const
{
	return !(*this == otherEntry); //Since != is the complement of ==
}

void MediaEntry::writeToFile(std::ostream& outFile) const
{
	outFile << (*this) << std::endl; //Write the title and type of the media entry to the file
	outFile << type << std::endl; //The extra line is to facilitate input for the media library
}
