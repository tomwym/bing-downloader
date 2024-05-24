Dependencies

sudo apt install libxml2-dev
sudo apt install libcurl4-openssl-dev

git clone https://github.com/nlohmann/json.git
cmake --build . --target install --config Debug --parallel 8

export BOOST_ROOT=/home/t0/boost/boost_1_82_0
./build/src/bing-downloader --threads 16