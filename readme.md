Dependencies

sudo apt install libxml2-dev
sudo apt install libcurl4-openssl-dev

git clone https://github.com/nlohmann/json.git
cmake --build . --target install --config Debug --parallel 8
