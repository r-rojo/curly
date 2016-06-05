#include <iostream>
#include "url.h"
#include "request.h"
#include "client.h"

using namespace std;
bool test_urlparse(const string &urlstring)
{
    Url url = Url::Parse(urlstring);
    cout << "Scheme: " << url.Scheme() << endl;
    cout << "Host:" << url.Host() << endl;
    cout << "Port:" << url.Port() << endl;
    cout << "Resource:" << url.Resource() << endl; 
    cout << "urlparse test: " << urlstring << " result: pass" << endl;
    return true;   
}

bool test_request(const string & urlstring)
{
    HttpRequest r("GET", urlstring);
    auto buffer = r.GetBuffer();
    
    HttpClient client;
    string str(buffer.begin(), buffer.end());
    cout << "Buffer is: " << str << endl;
    HttpResponse response = client.Send(r);
    if (response.StatusCode() == 200)
    {
        string content_type = response.Header("Content-Type");
        if (content_type.find("text/") != string::npos)
        {
            cout << response.GetBody() << endl;
        }
    
    }
    return true;
}


int main(int argc, char * argv[])
{
    test_urlparse("http://www.foo.com/one/two/three.jpg");
    test_urlparse("http://www.foo.com:80/one/two/three.jpg");
    test_urlparse("http://www.foo.com:8080/one/two/three.jpg");
    
    test_request("http://localhost:8000/a.txt");
    test_request("http://www.google.com");

    
}