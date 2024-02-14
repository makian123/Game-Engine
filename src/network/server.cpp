#include <network/server.h>
#include <iostream>
#include <future>

namespace engine { namespace network{
	Server::Server(const std::string &addr, uint16_t port, uint8_t maxConn, uint8_t channelCount, uint32_t incomingBandwith, uint32_t outgoingBandwith) {
		enet_address_set_host_ip(&address, addr.c_str());
		address.port = port;

		host = enet_host_create(&address, maxConn, channelCount, incomingBandwith, outgoingBandwith);
		if (!host) {
			std::cout << "Couldn't create host";
		}
	}
	Server::~Server() {
		Stop();

		enet_host_destroy(host);
	}

	void Server::Start() {
		if (running.load()) return;
		running.store(true);

		runThread = std::thread([&]() {
			std::string ip(32, 0);
			enet_address_get_host_ip(&address, ip.data(), ip.size());
			std::cout << "[SERVER] Server listening on " << ip << ":" << address.port << "\n";
			while (running.load()) {
				ENetEvent event;

				enet_host_service(host, &event, 1000);
				switch (event.type) {
					case ENET_EVENT_TYPE_CONNECT:
						std::cout << "[SERVER] New connection\n";
						peers.emplace_back(event.peer);
						break;
					case ENET_EVENT_TYPE_DISCONNECT:
						if (peers.size()) {
							peers.erase(std::find(peers.begin(), peers.end(), event.peer));
						}
						break;
					case ENET_EVENT_TYPE_RECEIVE:
						std::cout << "[SERVER] Packet received: '" << event.packet->data << "'\n";
						enet_packet_destroy(event.packet);
						break;
				}
			}

			std::cout << "[SERVER] Server stopped\n";

			return true;
		});
	}
	void Server::Stop() {
		if (!running.load()) return;
		running.store(false);

		runThread.join();

		for (auto &peer : peers) {
			enet_peer_disconnect_now(peer, 0);
		}
		enet_host_flush(host);
	}

	void Server::SendPacket(ENetPeer *peer, const std::string &packet, uint8_t channel, ENetPacketFlag flag) {
		if (!peer) return;
		auto enetPacket = enet_packet_create(packet.c_str(), packet.size() + 1, flag);

		enet_peer_send(peer, channel, enetPacket);
		enet_host_flush(host);
	}
	void Server::SendPacket(ENetPeer *peer, const std::vector<uint8_t> &data, uint8_t channel, ENetPacketFlag flag) {
		if (!peer) return;
		auto enetPacket = enet_packet_create(data.data(), data.size(), flag);

		enet_peer_send(peer, channel, enetPacket);
		enet_host_flush(host);
	}
	void Server::Broadcast(const std::string &packet, uint8_t channel, ENetPacketFlag flag) {
		auto enetPacket = enet_packet_create(packet.c_str(), packet.size() + 1, flag);

		enet_host_broadcast(host, channel, enetPacket);
		ENetEvent event;
		enet_host_service(host, &event, 1000);
		//enet_host_flush(host);
	}
	void Server::Broadcast(const std::vector<uint8_t> &data, uint8_t channel, ENetPacketFlag flag) {
		auto enetPacket = enet_packet_create(data.data(), data.size(), flag);

		enet_host_broadcast(host, channel, enetPacket);
		enet_host_flush(host);
	}

	ENetPeer *Server::GetPeer(const std::string &ip) {
		ENetAddress addr;
		enet_address_set_host_ip(&addr, ip.c_str());
		for (auto &peer : peers) {
			if (peer->address.host == addr.host) {
				return peer;
			}
		}
		return nullptr;
	}
}}