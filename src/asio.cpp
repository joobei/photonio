#include "asio.h"

udp_server::udp_server(boost::asio::io_service& io_service, EventQueue* eventQueue, boost::mutex* mutex) :
    socket_(io_service, udp::endpoint(udp::v4(), 9999)), queue(eventQueue), io_mutex(mutex) {
    start_receive();
}

//create socket and start listening
void udp_server::start_receive() {
    socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
                               remote_endpoint_, boost::bind(&udp_server::handle_receive,
                                                             this, boost::asio::placeholders::error,
                                                             boost::asio::placeholders::bytes_transferred));

}

void udp_server::handle_receive(const boost::system::error_code& error,
                                std::size_t size /*bytes_transferred*/) {
    //lock so no thread problems
    boost::mutex::scoped_lock lock(*io_mutex);
    //push the message into the event queue
    keimote::PhoneEvent tempEvent;

    tempEvent.ParseFromArray(&recv_buffer_[0], size);
    queue->push(tempEvent);
    recv_buffer_.empty();
    //unlock mutex on EventQueue
    lock.unlock();
    if (!error) {
        start_receive();
    }

    if (error) {
        std::cout << error.message();
    }

    if (error == boost::asio::error::message_size) {
        std::cout << "Message too large - Network Event";
    }
}

void udp_server::handle_send(boost::shared_ptr<std::string> /*message*/,
                             const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) {
}
