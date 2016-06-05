#include "client.h"
#include <iostream>


using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        HttpClient client;
        HttpResponse response = client.Get(argv[1]);
        auto content_type = response.Header("Content-Type");
        if (content_type.find("text/") != string::npos ||
            content_type == "application/json" ||
            content_type == "application/xml")
        {
            cout << response.GetBody() << endl;
        }
        
    }
}