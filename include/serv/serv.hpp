#pragma once
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "CivetServer.h"

namespace serv {

class Request {
public:
    explicit Request(mg_connection* connection)
        : _connection{connection}
    {}
    auto get(std::string_view param) const -> std::optional<std::string>;
    auto route() const -> std::string;

private:
    mg_connection* _connection{};
};

using RequestCallback = std::function<void(Request const&)>;

namespace internal {

class CivetRAII { // NOLINT(*special-member-functions)
public:
    CivetRAII() { mg_init_library(0); }
    ~CivetRAII() { mg_exit_library(); }
};

class Handler : public CivetHandler {
public:
    explicit Handler(RequestCallback callback)
        : _callback{std::move(callback)}
    {}

    auto handleGet(CivetServer*, mg_connection*) -> bool override;

private:
    RequestCallback _callback;
};

} // namespace internal

class Server {
public:
    explicit Server(int port, std::vector<std::pair<std::string, RequestCallback>> const& callbacks);

    auto port() const -> int { return _server->getListeningPorts()[0]; }

private:
    std::unique_ptr<internal::CivetRAII> _raii{std::make_unique<internal::CivetRAII>()};
    std::unique_ptr<CivetServer>         _server; // We must not copy a CivetServer because it is buggy (not implemented, and the default behavior is wrong)
    std::vector<internal::Handler>       _handlers{};
};

} // namespace serv
