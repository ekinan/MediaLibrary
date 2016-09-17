#ifndef _MEDIA_LIBRARY_CPP
#define _MEDIA_LIBRARY_CPP

#include "MediaLibrary.h"
#include <iostream>

template <template <class MediaEntry> class DataStructure>
MediaLibrary<DataStructure>::MediaLibrary()
{
}

template <template <class MediaEntry> class DataStructure>
bool MediaLibrary<DataStructure>::addEntry(const MediaEntry& newMedia)
{
	return library.add(newMedia);
}

template <template <class MediaEntry> class DataStructure>
bool MediaLibrary<DataStructure>::removeEntry(const MediaEntry& newMedia)
{
	return library.remove(newMedia);
}

template <template <class MediaEntry> class DataStructure>
MediaEntry MediaLibrary<DataStructure>::getEntry(const MediaEntry& media) const
{
	return library.getEntry(media);
}

template <template <class MediaEntry> class DataStructure>
bool MediaLibrary<DataStructure>::contains(const MediaEntry& media) const
{
	return library.contains(media);
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayAllMovies() const
{
	library.traverse(displayMovie);
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayAllMusic() const
{
	library.traverse(displayMusic);
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayAllTv() const
{
	library.traverse(displayTv);
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayAll() const
{
	library.traverse(displayEntry);
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayMovie(MediaEntry& media)
{
	if (media.getMediaType() == 'M')
		std::cout << media << std::endl;
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayMusic(MediaEntry& media)
{
	if (media.getMediaType() == 'S')
		std::cout << media << std::endl;
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayTv(MediaEntry& media)
{
	if (media.getMediaType() == 'T')
		std::cout << media << std::endl;
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayEntry(MediaEntry& media)
{
	std::cout << media << std::endl;
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::writeToFile(std::ostream& outFile) const
{
	library.writeToFile(outFile);
	outFile << std::endl << std::endl;
}

template <template <class MediaEntry> class DataStructure>
int MediaLibrary<DataStructure>::getNumberOfItems() const
{
	return library.getNumberOfItems();
}

template <template <class MediaEntry> class DataStructure>
void MediaLibrary<DataStructure>::displayStatistics(std::ostream& os) const
{
	library.displayStatistics(os);
}

#endif
