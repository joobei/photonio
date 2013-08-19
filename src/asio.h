#ifndef ASIO_H
#define ASIO_H

#include <boost/asio.hpp>
#include <boost/system/api_config.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "eventQueue.h"
#include <boost/array.hpp>

using namespace pho;
using boost::asio::ip::udp;

class udp_server {
public:
        udp_server(
            boost::asio::io_service& io_service,
            EventQueue* Queue,
            boost::mutex* mutex);
private:
        //create socket and start listening
        void start_receive();

        void handle_receive(const boost::system::error_code& error,
                std::size_t size /*bytes_transferred*/);

        void handle_send(boost::shared_ptr<std::string> /*message*/,
                const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/);

        EventQueue* queue;
        udp::socket socket_;

        udp::endpoint remote_endpoint_;
        boost::array<char, 300> recv_buffer_;
        //boost::array<char, 17> recv_buffer_;
        boost::mutex* io_mutex;
};


class Minicom_client
{
public:
        Minicom_client(
            boost::asio::io_service& io_service,
            unsigned int baud,
            const std::string& device,
            EventQueue* queue,
            boost::mutex* mutex
            );
		void shutDown();
private:
        void startRead();
        void read_complete(const boost::system::error_code& error, std::size_t bytes_transferred);
        float getFloat(int index);
		

        static const int max_read_length = 7*4+3; // maximum amount of data to read in one operation
        boost::asio::io_service& io_service_;
        boost::asio::serial_port serialPort; // the serial port this instance is connected to
        unsigned char serialBuffer[max_read_length]; // data read from the socket
        EventQueue* eventQueue;
        boost::mutex* io_mutex;
};


#endif // ASIO_H
