#include "serv/serv.hpp"
#include <CivetServer.h>
#include <stdexcept>
#include <vector>

namespace serv {

auto internal::Handler::handleGet(CivetServer*, mg_connection* connection) -> bool
{
    _callback(Request{connection});
    mg_printf(connection,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n"
              "Connection: close\r\n"
              "Content-Length:  0\r\n"
              "Access-Control-Allow-Origin: *\r\n" // Allow CORS from any origin
              "\r\n");
    return true;
}

auto Request::get(std::string_view param) const -> std::optional<std::string>
{
    std::string res{};
    if (CivetServer::getParam(_connection, param.data(), res))
        return res;
    return std::nullopt;
}

auto Request::route() const -> std::string
{
    return mg_get_request_info(_connection)->local_uri;
}

Server::Server(int port, std::vector<std::pair<std::string, RequestCallback>> const& callbacks)
    : _server{std::make_unique<CivetServer>(std::vector<std::string>{
          {"listening_ports", std::to_string(port),
           "num_threads", "1"
          }
      })}
{
    for (auto const& callback : callbacks)
        _handlers.emplace_back(callback.second);
    for (size_t i = 0; i < callbacks.size(); ++i)
        _server->addHandler(callbacks[i].first, &_handlers[i]);
}

} // namespace serv
