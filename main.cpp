#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

int main( int argc, char* argv[] )
{
    try {
        if ( argc != 3  )
           {
           cerr << "Usage: ued2k <host> <query>" << std::endl;
           return 1;
           }

        string server {argv[1]};
        size_t separator_pos = server.find(':');

        string host, port;

        if ( separator_pos == string::npos )
           {
           host = server;
           port = "4661";
           }
        else {
             host = server.substr(0, separator_pos);
             port = server.substr(++separator_pos);
             }

        cout << "Connecting: Host=" << host << "; Port=" << port << "\n";

        io_context io_context;

        tcp::resolver               resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(host, port);
        tcp::socket                 socket(io_context);

        connect(socket, endpoints);

        // socket.send();

        for (;;)
            {
            boost::array<char, 128> buf;
            system::error_code      error;

            size_t len = socket.read_some(buffer(buf), error);

            if ( error == error::eof )
               break; // Connection closed cleanly by peer.
            else if ( error )
                 throw system::system_error(error); // Some other error.

            cout.write(buf.data(), len);
            }
        }
    catch ( std::exception& e )
          {
          cerr << e.what() << endl;
          }

    return 0;
}
