#include <audio/sound.h>
#include <audio/soundmixer.h>

#include <algorithm>

namespace engine::audio {
	Sound::Sound(Mixer &mixer, const std::string &file, bool inMemory): mixer(mixer), bigFile(!inMemory) {
		if (!bigFile) {
			soundImpl.chunk = Mix_LoadWAV(file.c_str());
			if (!soundImpl.chunk) {
				throw std::exception("Bad file");
			}
		}
		else {
			soundImpl.music = Mix_LoadMUS(file.c_str());
		}
	}
	Sound::~Sound() {
		if (!bigFile) {
			Mix_FreeChunk(soundImpl.chunk);
		}
		else {
			Mix_FreeMusic(soundImpl.music);
		}
	}

	bool Sound::Play() {
		double groupMulti = (groupID >= 0 ? mixer.GetGroup(groupID).GetVolume() : 1.0);
		auto newVolume = std::clamp<uint8_t>(128 * volume * groupMulti, 0, 128);
		if (!bigFile) {
			soundImpl.chunk->volume = newVolume;
			currentChannelPlaying = Mix_PlayChannel(-1, soundImpl.chunk, (looping ? -1 : 0));

			return currentChannelPlaying != -1;
		}

		Mix_VolumeMusic(newVolume);
		currentChannelPlaying = Mix_PlayMusic(soundImpl.music, (looping ? -1 : 0));
		return currentChannelPlaying != -1;
	}
	void Sound::Stop() {
		if (currentChannelPlaying >= 0) {
			if (!bigFile)
				Mix_HaltChannel(currentChannelPlaying);
			else
				Mix_HaltMusic();
		}
	}
}