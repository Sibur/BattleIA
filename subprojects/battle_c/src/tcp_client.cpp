#include "tcp_client.hpp"
#include "battle_c.pb.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>

void TCPClient::Handle() {
  for (;;) {
    uint8_t magic;
    uint32_t message_size;
    size_t len;
    boost::system::error_code error;
    do {
      len = socket.value().read_some(boost::asio::buffer(&magic, sizeof(magic)),
                                     error);

      if (error == boost::asio::error::eof)
        break;
      else if (error)
        throw boost::system::system_error(error);
    } while (magic != 0b10101010);

    len = socket.value().read_some(
        boost::asio::buffer(&message_size, sizeof(message_size)), error);

    if (error == boost::asio::error::eof)
      break;
    else if (error)
      throw boost::system::system_error(error);
    std::cout << "reading message_size = " << message_size << std::endl;

    std::vector<uint8_t> buffer(message_size);
    socket.value().read_some(boost::asio::buffer(buffer), error);
    {
      ServerClientMessage message;
      message.ParseFromArray(buffer.data(), buffer.size());
      if (this->HandleMessage(message)) {
        std::lock_guard guard(this->cv_recvq_m);
        this->recvq.push(message);
      }
    }
    this->cv_recvq.notify_one();
  }
}

bool TCPClient::HandleMessage(ServerClientMessage sc_message) {

  if (sc_message.has_player_data()) {
    auto player_data = sc_message.player_data();
    this->local_player_data_.position.x = player_data.position().x();
    this->local_player_data_.position.y = player_data.position().y();
    this->local_player_data_.position.z = player_data.position().z();

    this->local_player_data_.id = player_data.id();
    this->local_player_data_.speed.x = player_data.speed().x();
    this->local_player_data_.speed.y = player_data.speed().y();
    this->local_player_data_.speed.z = player_data.speed().z();

    return false;
  }
  return true;
}

void TCPClient::Connect(std::string host, uint16_t port) {
  boost::asio::ip::tcp::resolver resolver(io_context);
  boost::asio::ip::tcp::resolver::results_type endpoints =
      resolver.resolve(host, std::to_string(port));

  this->socket = boost::asio::ip::tcp::socket(io_context);

  boost::asio::connect(this->socket.value(), endpoints);
  this->connection_thread_ = std::thread(&TCPClient::Handle, this);
  this->send_thread_ = std::thread(&TCPClient::SendQueue, this);
}

void TCPClient::SendQueue() {
  boost::system::error_code error;
  std::unique_lock<std::mutex> lk(this->cv_sendq_m);

  while (true) {
    while (this->sendq.empty()) {
      cv_sendq.wait(lk);
    }

    ClientServerMessage message = this->sendq.front();

    std::string output;
    message.SerializeToString(&output);

    uint8_t magic = 0b10101010;
    socket.value().write_some(boost::asio::buffer(&magic, sizeof(uint8_t)));

    uint32_t packet_size = output.size();
    socket.value().write_some(
        boost::asio::buffer(&packet_size, sizeof(uint32_t)));
    socket.value().write_some(boost::asio::buffer(output));

    this->sendq.pop();
    std::cout << "Message sent: 1 (magic) + 4 (size) + " << packet_size
              << " (data), seq_number = " << message.message_id() << std::endl;
  }
}

void TCPClient::QueueMessage(ClientServerMessage message) {
  message.set_message_id(this->seq_number++);
  {
    std::lock_guard guard(this->cv_sendq_m);
    this->sendq.push(message);
  }
  this->cv_sendq.notify_one();
}

ServerClientMessage
TCPClient::WaitForMessage(std::function<bool(ServerClientMessage)> test) {
  std::unique_lock<std::mutex> lk(this->cv_recvq_m);

  cv_recvq.wait(lk, [test, this] {
    return !this->recvq.empty() && test(this->recvq.front());
  });
  ServerClientMessage message = this->recvq.front();
  this->recvq.pop();
  return message;
}