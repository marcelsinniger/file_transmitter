/*
 FileTransmitter is a program which allows to transfer files between computers over the network
 Version 0.1
 Copyright 2014 Marcel Sinniger

 This file is part of FileTransmitter.

 FileTransmitter is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 FileTransmitter is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with FileTransmitter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "server.hpp"

using namespace boost::asio::ip;

server::server() :
        io_service(), socket(io_service), endpoint(0), acceptor(0) {

}

void server::bind(std::string src_port) {
    endpoint = new tcp::endpoint(tcp::v4(), std::atoi(src_port.c_str()));
}

void server::accept() {
    acceptor = new tcp::acceptor(io_service, *endpoint);
    acceptor->accept(socket);
}

void server::receive(std::string filename) {
    std::ofstream file(filename);
    while (true) {
        try {
            boost::asio::read(socket, boost::asio::buffer(buffer, BUFFER_SIZE));
            memcpy(&length, buffer, sizeof(int));
            if (length > 0) {
                file.write((buffer + sizeof(int)), length);
            } else {
                break;
            }
        } catch (boost::system::system_error &e) {
            std::cerr << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

void server::close() {
    socket.close();
}

server::~server() {
    delete acceptor;
    delete endpoint;
}
