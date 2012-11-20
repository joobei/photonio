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

//******************************** MINICOM CLIENT *******************************************************
//******************************** MINICOM CLIENT *******************************************************
//******************************** MINICOM CLIENT *******************************************************

Minicom_client::Minicom_client(boost::asio::io_service& io_service, unsigned int baud, const std::string& device, EventQueue* queue,boost::mutex* mutex)
    : serialPort(io_service_, device), eventQueue(queue), io_service_(io_service),io_mutex(mutex)
{

    if (!serialPort.is_open())
    {
        std::cout << "Serial Port Failed to open" << std::endl;
        return;
    }
    boost::asio::serial_port_base::baud_rate baud_option(baud);
    serialPort.set_option(baud_option); // set the baud rate after the port has
    serialPort.set_option( boost::asio::serial_port_base::character_size(8));
    serialPort.set_option( boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serialPort.set_option( boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serialPort.set_option( boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    std::string msg = "C";  //command to make polhemus send data constantly
    boost::asio::write(serialPort,boost::asio::buffer(msg.c_str(),msg.size()));

    startRead();
}

void Minicom_client::startRead() {

    //std::string msg = "P";  //command to make polhemus send binary data constantly
    //boost::asio::write(serialPort,boost::asio::buffer(msg.c_str(),msg.size()));

    boost::asio::async_read(serialPort,boost::asio::buffer(serialBuffer,max_read_length),boost::asio::transfer_all(),
                            boost::bind(&Minicom_client::read_complete,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred)
                            );

}

void Minicom_client::read_complete(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    boost::array<float, 7> topush;

    for (int i=0;i<7;i++) {
        topush[i] = getFloat(i);
    }

    //try {
    boost::mutex::scoped_lock lock(*io_mutex);
    eventQueue->push(topush);
    lock.unlock();
    /*}
    catch (Ogre::Exception &e)
    {
        MessageBox(NULL,e.getFullDescription().c_str(),"Error",MB_OK);
    }*/

    //send P and wait for more
    startRead();
}


float Minicom_client::getFloat(int index)
{ 	float temp = 0.0;
        memcpy(&temp,serialBuffer+3*sizeof(char)+index*sizeof(float),sizeof(float));
            return temp; }

void Minicom_client::shutDown() {

	std::string msg = "c";  //command to make polhemus stop sending data
    boost::asio::write(serialPort,boost::asio::buffer(msg.c_str(),msg.size()));

}