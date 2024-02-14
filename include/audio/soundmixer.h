#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include <SDL/SDL_mixer.h>
#include "sound.h"

namespace engine::audio {
	class Mixer;

	class Group {
		private:
		int id;
		int channelID;
		std::string name;
		double volume = 1.0;

		public:
		friend class Mixer;
		Group(const std::string &name, int groupID);

		int GetID() const { return id; }

		double GetVolume() const { return volume; }
		void SetVolume(double newVol) { volume = newVol; }
	};

	class Mixer {
		private:
		std::vector<Group> groups;

		void RemoveSoundFromGroup(Sound *sound);
		public:
		friend class Sound;

		Mixer() = default;
		~Mixer() = default;

		void CreateGroup(const std::string &name);
		const Group &GetGroup(const std::string &name) const;
		const Group &GetGroup(int id) const;

		void SetGroup(Sound *sound, const std::string &name);
	};
}