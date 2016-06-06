#include "client.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    
    if (argc > 1)
    {
        http_client client;
        // assume first argument is the url
        http_response response = client.get(argv[1]);
        auto content_type = response.header("Content-Type");
        if (content_type.find("text/") != string::npos ||
            content_type == "application/json" ||
            content_type == "application/xml")
        {
            // if 2 arguments, use the second as the filename to save it to
            if (argc >= 3)
            {
                ofstream txtfile(argv[2]);
                txtfile.write(response.body(), response.body_size());
            }
            else
            {
                // if no filename is supplied, print out the text
                cout << response.body() << endl;
            }
        }
        else
        {
            string filename = argc >= 3 ? argv[2] : "curly.bin";

            if (content_type.find("image/png") != string::npos)
            {
                auto body = response.body();
                auto size = response.body_size();
                ofstream binfile(filename, ios::binary);
                binfile.write(body, size);
                binfile.close();
            }
            
        }
        
    }
}