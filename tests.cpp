#include "gtest/gtest.h"
#include "client.h"


class GetTest : public ::testing::TestWithParam<const char*>{

};

TEST_P(GetTest, client_get) {
  http_client c;
  auto r = c.get(GetParam());
  EXPECT_EQ(200, r.status_code());
}

INSTANTIATE_TEST_CASE_P(GetTest, GetTest,
    ::testing::Values("http://www.httpbin.org/html",
                      "http://www.httpbin.org/deny"));

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
