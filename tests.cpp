#include "gtest/gtest.h"
#include "client.h"

TEST(client, get_html) {
  http_client c;
  auto r = c.get("http://www.httpbin.org/html");
  EXPECT_EQ(200, r.status_code());

}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
