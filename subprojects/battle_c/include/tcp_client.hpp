#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "battle_c.h"
#include "battle_c.pb.h"
#include <boost/asio.hpp>
#include <cstdint>
#include <functional>
#include <optional>
#include <queue>
#include <thread>
using namespace battle_c;
class TCPClient {

  std::thread connection_thread_;
  std::thread send_thread_;
  boost::asio::io_context io_context;

  std::optional<boost::asio::ip::tcp::socket> socket;

  void Handle();

  std::condition_variable cv_recvq;
  std::mutex cv_recvq_m;
  std::queue<ServerClientMessage> recvq = {};

  std::condition_variable cv_sendq;
  std::mutex cv_sendq_m;
  std::queue<ClientServerMessage> sendq = {};

  uint64_t seq_number = 64;

  BC_PlayerData local_player_data_;
  bool HandleMessage(ServerClientMessage sc_message);

public:
  TCPClient() {};

  void Connect(std::string host, uint16_t port);
  void SendQueue();

  void QueueMessage(ClientServerMessage message);

  ServerClientMessage
  WaitForMessage(std::function<bool(ServerClientMessage)> test);

  BC_PlayerData GetPlayerData() { return local_player_data_; }
};

#endif