/*@file MediaLibrary.h*/
#ifndef _MEDIA_LIBRARY_H
#define _MEDIA_LIBRARY_H

#include "MediaLibraryInterface.h"


template <template <class MediaEntry> class DataStructure>
class MediaLibrary : public MediaLibraryInterface
{
private:
	DataStructure<MediaEntry> library;

	/*
	Displays the media entry if it is a movie, music/song, or TV Show for the first three functions,
	or the entry itself for the last one.
	@post All entries satisfying the criteria in the function title are displayed
	*/
	static void displayMovie(MediaEntry& media);
	static void displayMusic(MediaEntry& media);
	static void displayTv(MediaEntry& media);
	static void displayEntry(MediaEntry& media);

public:
	MediaLibrary();

	//Refer to MediaLibraryInterface.h for details on these functions
	bool addEntry(const MediaEntry& newMedia);
	bool removeEntry(const MediaEntry& newMedia);
	MediaEntry getEntry(const MediaEntry& media) const;
	bool contains(const MediaEntry& media) const;
	void displayAllMovies() const;
	void displayAllMusic() const;
	void displayAllTv() const;
	void displayAll() const;
	void writeToFile(std::ostream& outFile) const;
	int getNumberOfItems() const;
	void displayStatistics(std::ostream& os) const;
};

#include "MediaLibrary.cpp"

#endif
