#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <asio.hpp>
#include <optional>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <thread>
#include <list>

/**
 *  Code heavily influenced by the tutorial at
 *  https://dens.website/tutorials/cpp-asio/tcp-chat-server
 */

using asio::ip::tcp;

using message_handler = std::function<void (std::string)>;
using error_handler = std::function<void ()>;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
private:
    tcp::socket socket;
    asio::streambuf streambuf;
    std::queue<std::string> outgoing;
    message_handler on_message;
    error_handler on_error;

    void async_read(){
        asio::async_read_until(socket, streambuf, "\n", std::bind(&ClientSession::on_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    void on_read(asio::error_code ec, std::size_t len){
        if (!ec){
            std::stringstream msg;
            msg << std::istream(&streambuf).rdbuf();
          //  msg << socket.remote_endpoint(ec) << ": " << std::istream(&streambuf).rdbuf();
            std::cout << "Received input from " << socket.remote_endpoint(ec) << ": " << msg.str();
            streambuf.consume(len);
            on_message(msg.str());
            async_read();
        } else {
            socket.close(ec);
            on_error();
        }
    }

    void async_write(){
        asio::async_write(socket, asio::buffer(outgoing.front()), std::bind(&ClientSession::on_write, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    void on_write(asio::error_code ec, std::size_t len){
        if (!ec){
            outgoing.pop();

            if (!outgoing.empty()){
                async_write();
            }
        } else {
            socket.close(ec);
            on_error();
        }
    }

public:
    ClientSession(tcp::socket&& socket) : socket(std::move(socket)){

    }

    void start(message_handler&& on_message, error_handler&& on_error){
        this->on_message = std::move(on_message);
        this->on_error = std::move(on_error);
        async_read();
    }

    void send(std::string const& msg){
        bool idle = outgoing.empty();
        outgoing.push(msg + "\n");

        if (idle){
            async_write();
        }
    }
};

class SimpleMessageSubscriber{
public:
    virtual void notify(std::string){ }
};

class ServerSocket{
private:
    const int port = 13337;

    asio::io_context ioContext;
    tcp::acceptor acceptor;
    std::optional<tcp::socket> socket;
    std::unordered_set<std::shared_ptr<ClientSession>> clients;
    std::thread thread;

    std::list<SimpleMessageSubscriber *> subs;

    void async_accept(){
        socket.emplace(ioContext);

        acceptor.async_accept(*socket, [&] (asio::error_code ec){
            auto client = std::make_shared<ClientSession>(std::move(*socket));
            clients.insert(client);

            std::cout << "New client connected. (now: " << clients.size() << " active clients)" << std::endl;
            client->send("Connection established.");

            client->start(
                [&](std::string msg)
                {
                    // Send received input to subscribers
                    for (SimpleMessageSubscriber *sub : subs){
                        sub->notify(msg);
                    }
                },
                [&, weak = std::weak_ptr(client)]
                {
                    if (auto shared = weak.lock(); shared && clients.erase(shared)){
                        std::cout << "A client disconnected. (now: " << clients.size() << " active clients)" << std::endl;
                    }
                }
            );

            async_accept();
        });
    }

public:
    ServerSocket() :
        ioContext(),
        acceptor(ioContext, tcp::endpoint(tcp::v4(), port)),
        thread([&] {
           async_accept();
           ioContext.run();
        })
    {
        /* Constructor body */
    }

    void send(std::string msg){
        for (auto& client : clients){
            client->send(msg);
        }
    }

    void subscribe(SimpleMessageSubscriber *sub){
       subs.push_back(sub);
    }

    void unsubscribe(SimpleMessageSubscriber *sub){
        subs.remove(sub);
    }
};

#endif