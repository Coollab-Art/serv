#include "../include/serv/serv.hpp"
#include <iostream>
#include "CivetServer.h"

namespace serv {

class Handler : public CivetHandler {
private:
    RequestHandler _handler;

    bool handleAll(mg_connection* conn)
    {
        mg_printf(conn,
                  "HTTP/1.1 200 OK\r\nContent-Type: "
                  "text/html\r\n\r\n\r\n");
        std::string s = "";
        if (CivetServer::getParam(conn, "param", s))
        {
            mg_printf(conn, "<p>param set to %s</p>", s.c_str());
        }
        else
        {
            mg_printf(conn, "<p>param not set</p>");
        }
        return true;
    }

public:
    bool handleGet(CivetServer*, mg_connection* conn)
    {
        std::string s;
        if (CivetServer::getParam(conn, "command", s))
        {
            _handler(s);
        }
        else
        {
            _handler("No command");
        }
        return handleAll(conn);
    }

    bool handlePost(CivetServer*, mg_connection* conn)
    {
        std::string s;
        if (CivetServer::getParam(conn, "command", s))
        {
            _handler(s);
        }
        else
        {
            _handler("No command");
        }
        return handleAll(conn);
    }

    explicit Handler(RequestHandler handler)
        : _handler{handler}
    {}
};

void init(RequestHandler handler)
{
    mg_init_library(0);

    static CivetServer server(
        {"listening_ports", "6002",
         "num_threads", "1"}
    );

    static Handler myHandler{handler};
    server.addHandler("/", myHandler);
}

void shut_down()
{
    mg_exit_library();
}

} // namespace serv
