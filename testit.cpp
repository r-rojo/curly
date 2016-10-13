#include <iostream>
#include <fstream>
#include <sstream>
#include "url.h"
#include "request.h"
#include "client.h"

using namespace std;
bool test_urlparse(const string &urlstring)
{
    auto url = url::parse(urlstring);
    cout << "Scheme: " << url.scheme() << endl;
    cout << "Host:" << url.host() << endl;
    cout << "Port:" << url.port() << endl;
    cout << "Resource:" << url.resource() << endl; 
    cout << "urlparse test: " << urlstring << " result: pass" << endl;
    return true;   
}

bool test_request(const string & urlstring)
{
    http_request r("GET", urlstring);
    auto buffer = r.buffer();
    
    http_client client;
    cout << "Buffer is: " << buffer << endl;
    auto response = client.send_request(r);
    if (response.status_code() == 200)
    {
        string content_type = response.header("Content-Type");
        if (content_type.find("text/") != string::npos)
        {
            cout << response.body() << endl;
        }
    
    }
    return true;
}

bool stress_it(const std::string &urlstring)
{
    http_client client;
    for (int i = 0; i < 100; i++)
    {
        auto response = client.get(urlstring);
        stringstream ss;
        ss << "bin/dump" << i << ".bin";
        auto filename = ss.str();
        cout << "saving to file: " << filename << std::endl;
        ofstream binfile(filename, ios::binary);
        binfile.write(response.body(), response.body_size());
        binfile.close();
    }
    return true;
}

int main(int argc, char * argv[])
{
    // test_urlparse("http://www.foo.com/one/two/three.jpg");
    // test_urlparse("http://www.foo.com:80/one/two/three.jpg");
    // test_urlparse("http://www.foo.com:8080/one/two/three.jpg");
    
    // test_request("http://localhost:8000/a.txt");
    // test_request("http://www.google.com");
    
    stress_it("http://www.httpbin.org/bytes/1024");
    
}