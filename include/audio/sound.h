#pragma once

#include <string>

#include <SDL/SDL_mixer.h>

namespace engine::audio {
	class Mixer;

	class Sound {
		private:
		union {
			Mix_Chunk *chunk;
			Mix_Music *music;
		} soundImpl;

		int currentChannelPlaying = -1;
		Mixer &mixer;
		double volume = 1.0;
		bool looping = false;
		bool bigFile;
		int groupID = -1;

		public:
		friend class Mixer;
		friend class Group;

		Sound(Mixer &mixer, const std::string &file, bool inMemory = true);
		~Sound();

		bool Play();
		void Stop();

		double GetVolume() const { return volume; }
		void SetVolume(double newVol) { volume = newVol; }

		bool IsLooping() const { return looping; }
		void SetLooping(bool loop) { looping = loop; }
	};
}