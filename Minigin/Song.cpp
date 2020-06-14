#include "MiniginPCH.h"
#include "Song.h"

Song::Song(const std::string& fileName)
{
	// Try to parse the song
	m_pSong = Mix_LoadMUS(fileName.c_str());

	// See if we get a nullptr
	if (!m_pSong) std::cout << "Illegal song with name: " << fileName << '\n';
}

void Song::Play() const
{
	if (m_pSong) Mix_PlayMusic(m_pSong, -1);
}
