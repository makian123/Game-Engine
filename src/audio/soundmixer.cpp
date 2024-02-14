#include <audio/soundmixer.h>

namespace engine::audio {
	Group::Group(const std::string &name, int groupID): name(name), id(groupID) {}

	void Mixer::CreateGroup(const std::string &name) {
		groups.emplace_back(name, groups.size());
	}
	const Group &Mixer::GetGroup(const std::string &name) const {
		return *std::find_if(groups.begin(), groups.end(), [name](const Group &group) { return group.name == name; });
	}
	const Group &Mixer::GetGroup(int groupID) const {
		return groups[groupID];
	}

	void Mixer::RemoveSoundFromGroup(Sound *sound) {
		sound->groupID = -1;
	}

	void Mixer::SetGroup(Sound *sound, const std::string &name) {
		sound->groupID = GetGroup(name).id;
	}
}