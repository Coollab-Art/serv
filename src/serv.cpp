#include "serv/serv.hpp"
#include <CivetServer.h>
#include <stdexcept>
#include <vector>

namespace serv {

auto Request::get(std::string_view param) const -> std::optional<std::string>
{
    std::string res{};
    if (CivetServer::getParam(_connection, param.data(), res))
        return res;
    return std::nullopt;
}

Server::Server(std::string const& port, std::vector<std::pair<std::string, RequestCallback>> const& callbacks)
    : _server{std::make_unique<CivetServer>(std::vector<std::string>{
          {"listening_ports", port,
           "num_threads", "1"
          }
      })}
{
    if (_server->getContext() == nullptr)
    {
        throw std::runtime_error{"Creation of the server failed"};
    }
    for (auto const& callback : callbacks)
        _handlers.emplace_back(callback.second);
    for (size_t i = 0; i < callbacks.size(); ++i)
        _server->addHandler(callbacks[i].first, &_handlers[i]);
}

} // namespace serv
