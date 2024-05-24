#pragma once

#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;



class Options {
    bool _valid;
    std::string _archive;
    std::string _supplemental;
    unsigned int _thread_count;
    bool _dry_run;
public:
    Options() = delete;
    Options(const int argc, const char** argv)
    : _valid{true}
    , _archive{"archive"}
    , _supplemental{"suppl"}
    , _thread_count{0}
    , _dry_run{false}
    {
        // Declare the supported options.
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,H", "produce help message")
            ("archive,A", po::value<std::string>(), "[Optional] Directory for downloaded images.")
            ("suppl,S", po::value<std::string>(), "[Optional] Directory for supplemental files.")
            ("threads,T", po::value<int>(), "[Optional] Number of download threads.")
            ("dry,D", po::bool_switch(), "[Optional] Dry run. Execute without downloading online resources.")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            _valid = false;
            return;
        }
        if (vm.count("archive")) {
            _archive = vm["archive"].as<std::string>();
        }
        if (vm.count("suppl")) {
            _supplemental = vm["suppl"].as<std::string>();
        }
        if (vm.count("threads")) {
            _thread_count = vm["threads"].as<int>();
        }
        if (vm.count("dry")) {
            _dry_run = vm["dry"].as<bool>();
        }
    }

public:
    auto Valid() const -> bool {
        return _valid;
    }
    auto Archive() const -> std::string {
        return _archive;
    }
    auto Supplemental() const -> std::string {
        return _supplemental;
    }
    auto ThreadCount() const -> unsigned int {
        return _thread_count;
    }
    auto DryRun() const -> bool {
        return _dry_run;
    }
};