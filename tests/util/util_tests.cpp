#include <gtest/gtest.h>
#include <util/util.h>

TEST(SplitTests, Split1) {
    std::string data = "hello;how;are;you;?";

    const auto items = webserver::util::Split(data, ";");

    ASSERT_STREQ("hello", items.at(0).c_str());
    ASSERT_STREQ("how", items.at(1).c_str());
    ASSERT_STREQ("are", items.at(2).c_str());
    ASSERT_STREQ("you", items.at(3).c_str());
    ASSERT_STREQ("?", items.at(4).c_str());
}

TEST(SplitTests, Split2) {
    std::string data = "hello\n\rhow\n\rare\n\ryou\n\r?";

    const auto items = webserver::util::Split(data, "\n\r");

    ASSERT_STREQ("hello", items.at(0).c_str());
    ASSERT_STREQ("how", items.at(1).c_str());
    ASSERT_STREQ("are", items.at(2).c_str());
    ASSERT_STREQ("you", items.at(3).c_str());
    ASSERT_STREQ("?", items.at(4).c_str());
}

TEST(SplitTests, Split3) {
    std::string data = "QWERoOTZUIoOasdfoO";

    const auto items = webserver::util::Split(data, "oO");

    ASSERT_STREQ("QWER", items.at(0).c_str());
    ASSERT_STREQ("TZUI", items.at(1).c_str());
    ASSERT_STREQ("asdf", items.at(2).c_str());
}

TEST(SplitTests, Split4) {
    std::string data = "";

    const auto items = webserver::util::Split(data, " ");

    ASSERT_EQ(0, items.size());
}

TEST(SplitTests, Split5) {
    std::string data = "hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo|hallo";

    const auto items = webserver::util::Split(data, "|");
    
    for(const auto& item : items) {
        ASSERT_STREQ("hallo", item.c_str());
    }
}

TEST(TrimTests, Trim1) {
    using namespace webserver::util;

    std::string data = " hello ";

    const auto trimmed = Trim(data);

    ASSERT_STREQ("hello", trimmed.c_str());
}

TEST(TrimTests, Trim2) {
    using namespace webserver::util;
    
    std::string data = " \nhello\r ";

    const auto trimmed = Trim(data);

    ASSERT_STREQ("hello", trimmed.c_str());
}

TEST(TrimTests, Trim3) {
    using namespace webserver::util;
    
    std::string data = " \nhello there\t\t ";

    const auto trimmed = Trim(data);

    ASSERT_STREQ("hello there", trimmed.c_str());
}

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}