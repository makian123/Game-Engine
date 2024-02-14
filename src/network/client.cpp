#include <network/client.h>
#include <iostream>

namespace engine { namespace network {
	Client::Client(uint8_t maxConn, uint8_t channelCount, uint8_t incomingBandwith, uint8_t outgoingBandwith){
		client = enet_host_create(nullptr, maxConn, channelCount, incomingBandwith, outgoingBandwith);
		if (!client) {
			std::cout << "Couldn't create client\n";
		}
	}

	Client::~Client() {
		if (!serverPeer) return;
		running.store(false);
		runThread.join();

		enet_peer_disconnect(serverPeer, 0);

		ENetEvent event;
		while (enet_host_service(client, &event, 3000) > 0) {
			switch (event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << "[CLIENT] Disconnection succeeded.\n";
					enet_host_destroy(client);
					return;
			}
		}

		enet_host_destroy(client);
	}

	void Client::Connect(const std::string &ip, uint16_t port, std::function<void(ENetPacket *)> callback) {
		this->onPacketCallback = callback;

		ENetAddress serverAddr;
		enet_address_set_host_ip(&serverAddr, ip.c_str());
		serverAddr.port = port;

		serverPeer = enet_host_connect(client, &serverAddr, client->channelLimit, 0);
		if (!serverPeer) {
			std::cout << "Couldn't connect to " << ip << ":" << port << "\n";
		}

		ENetEvent event;
		if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
			std::cout << "[CLIENT] Connection to " << ip << ":" << port << " succeeded\n";
			running.store(true);

			runThread = std::thread([&]() {
				while (running.load()) {
					ENetEvent event;

					enet_host_service(client, &event, 1000);

					switch (event.type) {
						case ENET_EVENT_TYPE_DISCONNECT:
							std::cout << "[CLIENT] Disconnected from server\n";
							running.store(false);
							break;
						case ENET_EVENT_TYPE_RECEIVE:
							if (onPacketCallback) {
								onPacketCallback(event.packet);
							}
							enet_packet_destroy(event.packet);
							break;
					}
				}
			});
		}
		else {
			enet_peer_reset(serverPeer);
			serverPeer = nullptr;

			std::cout << "Connection to " << ip << ":" << port << " failed\n";
		}
	}

	void Client::SendPacket(const std::string &packet, uint32_t channel) {
		auto *enetPacket = enet_packet_create(packet.c_str(), packet.size() + 1, ENET_PACKET_FLAG_RELIABLE);

		enet_peer_send(serverPeer, channel, enetPacket);

		ENetEvent event;
		//enet_host_flush(client);
		enet_host_service(client, &event, 1000);
	}
	void Client::SendPacket(const std::vector<uint8_t> &data, uint32_t channel) {
		auto *enetPacket = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);

		enet_peer_send(serverPeer, channel, enetPacket);

		ENetEvent event;
		//enet_host_flush(client);
		enet_host_service(client, &event, 1000);
	}
}}