#include "SimpleFoxServer.h"

int main () {
    SimpleFoxServer serve("./test-server/res/fox.jpg");
    serve.MainThreadSpinner();
//     std::this_thread::sleep_for(std::chrono::seconds(100));
}