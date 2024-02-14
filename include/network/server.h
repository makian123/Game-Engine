#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <atomic>
#include <thread>

#include <enet/enet.h>

namespace engine { namespace network{
	class Server {
		private:
		ENetAddress address;
		std::vector<ENetPeer*> peers;
		ENetHost *host = nullptr;
		std::atomic<bool> running = false;
		std::thread runThread;

		public:
		Server(const std::string &addr, uint16_t port, uint8_t maxConn = 32, uint8_t channelCount = 2, uint32_t incomingBandwith = 0, uint32_t outgoingBandwith = 0);
		~Server();

		void Start();
		void Stop();
		void SendPacket(ENetPeer *peer, const std::string &packet, uint8_t channel = 0, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
		void SendPacket(ENetPeer *peer, const std::vector<uint8_t> &data, uint8_t channel = 0, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
		void Broadcast(const std::string &packet, uint8_t channel = 0, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
		void Broadcast(const std::vector<uint8_t> &data, uint8_t channel = 0, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);

		ENetPeer *GetPeer(const std::string &ip);
	};
}}