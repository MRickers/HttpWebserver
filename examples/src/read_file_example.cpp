#include <logging/logging.h>
#include <util/util.h>


int main() {
    try {
        lLog(lInfo) << "Read file example";
        const std::string filename = "test_file.html";

        lLog(lInfo) << "Read whole file";
        {
            const auto file = webserver::util::ReadFile(filename);

            lLog(lInfo) << "Read file " << filename;
            lLog(lInfo) << std::string{file.cbegin(), file.cend()};
        }

        lLog(lInfo) << "Read 3000 bytes";
        {
            const auto file = webserver::util::ReadFile(filename, 3000);
            lLog(lInfo) << "Read file " << filename << " with " << file.size() << " bytes";
            lLog(lInfo) << std::string{file.cbegin(), file.cend()};
        }

    } catch(const webserver::util::ServerException& e) {
        lLog(lError) << e.what();
    }

    return 0;
}