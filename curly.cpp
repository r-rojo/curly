#include "client.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    
    if (argc > 1)
    {
        HttpClient client;
        // assume first argument is the url
        HttpResponse response = client.Get(argv[1]);
        auto content_type = response.Header("Content-Type");
        if (content_type.find("text/") != string::npos ||
            content_type == "application/json" ||
            content_type == "application/xml")
        {
            // if 2 arguments, use the second as the filename to save it to
            if (argc >= 3)
            {
                ofstream txtfile(argv[2]);
                txtfile.write(response.GetBody(), response.GetBodyLength());
            }
            else
            {
                // if no filename is supplied, print out the text
                cout << response.GetBody() << endl;
            }
        }
        else
        {
            string filename = argc >= 3 ? argv[2] : "curly.bin";

            if (content_type.find("image/png") != string::npos)
            {
                auto body = response.GetBody();
                auto size = response.GetBodyLength();
                ofstream binfile(filename, ios::binary);
                binfile.write(body, size);
                binfile.close();
            }
            
        }
        
    }
}