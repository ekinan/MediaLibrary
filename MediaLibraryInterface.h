/*@file MediaLibraryInterface.h*/
#ifndef _MEDIA_LIBRARY_INTERFACE_H
#define _MEDIA_LIBRARY_INTERFACE_H

#include <iostream>
#include "MediaEntry.h"

class MediaLibraryInterface
{
public:
	/*
	Adds a new media entry into the library
	@post If successful, newMedia is stored in the library
	@param newMedia The media item to be added as the new entry
	@return True if addition was successful, otherwise false
	*/
	virtual bool addEntry(const MediaEntry& newMedia) = 0;

	/*
	Removes a media entry from the library.
	@post If it exists, newMedia is removed in the library. Otherwise, nothing happens.
	@param newMedia The media item to be removed
	@return True if the removal was successful, false otherwise
	*/
	virtual bool removeEntry(const MediaEntry& newMedia) = 0;

	/*
	Retrieves a media entry from the library
	@param media Entry to be retrieved
	@return The entry if it is in the library, otherwise an exception (in the data structure)
	is thrown.
	*/
	virtual MediaEntry getEntry(const MediaEntry& media) const = 0;

	/*
	Checks if a media item is in the library
	@param media Entry to be checked
	@return True if the entry exists in the library, false otherwise
	*/
	virtual bool contains(const MediaEntry& media) const = 0;

	/*
	Display either all of the movies, music, TV or everything in the library.
	@post Either all of the movies, music, TV or the contents of the entire library
	are outputted
	*/
	virtual void displayAllMovies() const = 0;
	virtual void displayAllMusic() const = 0;
	virtual void displayAllTv() const = 0;
	virtual void displayAll() const = 0;


	/*
	Writes the contents of the library to an external file opened by outFile.
	@post The contents of the library are written to outFile
	*/
	virtual void writeToFile(std::ostream& outFile) const = 0;

	/*
	Returns the number of entries in the media library
	@return The number of entries in the library
	*/
	virtual int getNumberOfItems() const = 0;

	/*
	Displays the relevant statistics of the data structure used to implement the library
	using the ostream variable os to output them.
	@post The relevant statistics are outputted
	@param os The ostream variable for the output
	*/
	virtual void displayStatistics(std::ostream& os) const = 0;
};

#endif
