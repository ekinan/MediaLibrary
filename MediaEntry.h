/*@file MediaEntry.h*/
#ifndef _MEDIA_ENTRY_H
#define _MEDIA_ENTRY_H

#include <iostream>

class MediaEntry
{
	//Override the stream extraction operator
	friend std::ostream& operator<<(std::ostream&, const MediaEntry&);
private:
	char* title; //Name of the media entry
	char type; //Type of the media entry (either Movie, TV Show, or Music/Song)
	int titleLength; //Length of the media entry's name


	void copyEntry(const MediaEntry& otherEntry); //Copies the contents of another media entry

	/*
	Compares two titles of two different media entries. Uses the alphabetical ordering, with respect
	to upper case letters. See design-write up for more details
	@param otherEntry The other media entry to be compared
	@return -1 if title < otherEntry.title; 1 if title > otherEntry.title;
	and 0 if title = otherEntry.title
	*/
	int compareTitles(const MediaEntry& otherEntry) const;

	/*
	Returns the next alphabet character in a phrase, used in compliment with compareTitles,
	or the blank character if index > length.
	@post The index is incremented to the next value and the alphLength of the phrase
	(see design-write up) is also adjusted accordingly.
	@param phrase Pointer to the phrase
	phraseLength Length of the phrase
	index The starting index of the phrase
	alphLength The number of alphabetical characters in the phrase
	*/
	char getNextChar(char* phrase, int phraseLength, int& index, int& alphLength) const;

	/*
	Returns the precedence of a media entry genre depending on the type.
	Songs have highest precedence, followed by TV shows and then movies.
	@return Precedence of type, which is either 'M', 'T', or 'S'
	*/
	int getPrecedence(char type) const;

public:
	MediaEntry();
	MediaEntry(const char* mediaTitle, char mediaType = 'S');
	MediaEntry(const MediaEntry& otherEntry); //Copy constructor
	~MediaEntry();

	/*
	Returns the length of the media entry's title
	@return Length of the media title
	*/
	int length() const;

	/*
	Returns the type of the media entry
	@return Type of the media entry
	*/
	char getMediaType() const;

	/*
	Sets the title of the media entry to mediaTitle.
	@post title has the same phrase as mediaTitle
	@param mediaTitle The new title of the mediaEntry
	*/
	void setTitle(const char* mediaTitle);

	/*
	Sets the type of the media entry to mediaType
	@post type is the same as mediaType
	@param mediaType The new type of the mediaEntry
	*/
	void setMediaType(char mediaType);


	/*
	Writes the contents of the media entry, specifically its title and genre,
	to the file opened by outFile.
	@post Contents of the media entry are written to outFile
	@param outFile Ostream variable containing the file
	*/
	void writeToFile(std::ostream& outFile) const;

	/*
	Override the assignment operator
	*/
	const MediaEntry& operator=(const MediaEntry&);

	/*
	Override the array index operator
	*/
	char& operator[](int index);
	const char& operator[](int index) const;


	/*
	Override the boolean operators. Used in complement with compareTitles
	*/
	bool operator==(const MediaEntry& otherEntry) const;
	bool operator<=(const MediaEntry& otherEntry) const;
	bool operator>=(const MediaEntry& otherEntry) const;
	bool operator<(const MediaEntry& otherEntry) const;
	bool operator>(const MediaEntry& otherEntry) const;
	bool operator!=(const MediaEntry& otherEntry) const;
};


#endif
