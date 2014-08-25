mkdir -p lib/{Debug,Release}
mkdir -p TMD/bin/{Debug,Release}
make cleanup && make debug
cd tests
make clean && make server && make client
cd ../TMD
make clean && make all
