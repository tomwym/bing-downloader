#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
using namespace std;
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char** argv) {

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ;

    return 0;
}