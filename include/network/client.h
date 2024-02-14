#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <enet/enet.h>

namespace engine { namespace network {
	class Client {
		private:
		ENetHost *client = nullptr;
		ENetPeer *serverPeer = nullptr;
		std::function<void(ENetPacket *)> onPacketCallback;
		std::atomic<bool> running = false;
		std::thread runThread;

		public:
		Client(uint8_t maxConn = 1, uint8_t channelCount = 2, uint8_t incomingBandwith = 0, uint8_t outgoingBandwith = 0);
		~Client();

		void Connect(const std::string &ip, uint16_t port, std::function<void(ENetPacket *)> callback = std::function<void(ENetPacket*)>());
		void SendPacket(const std::string &packet, uint32_t channel = 0);
		void SendPacket(const std::vector<uint8_t> &data, uint32_t channel = 0);
	};
}}