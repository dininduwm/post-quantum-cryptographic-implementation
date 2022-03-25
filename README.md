# post-quantum-cryptographic-implementation
Latis based post quantum cryptographic algorithm implementation

## prerequisite
1. ### libsodium Library
    - installation:
        - #### on Ubuntu: sudo apt-get install -y libsodium-dev
        - #### on Mac:    brew install libsodium
2. ### boost
    - installation:
        - #### on Ubuntu : sudo apt-get install libboost-all-dev
        - #### on Mac: brew install boost
3. ### crypto++
    - installation:
        - #### on ubuntu : sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils
## compilation

g++ input.cpp -o output -lsodium -DEIGEN_STACK_ALLOCATION_LIMIT=0
g++ RegevsCryptoSystemWithAES.cpp -pthread -lsodium -std=c++11 ../Matrix/Matrix.cpp ../cryptopp/libcryptopp.a
#### if including helper functions
g++ input.cpp -o output ../Helper/Helper.cpp
## For Regev Hybrid Crypto system compilation
g++ -DNDEBUG=1 -g3 -O2 -Wextra AESRegevsCryptoSystemMultibits.cpp -o hybrid -lcryptopp -lsodium -DEIGEN_STACK_ALLOCATION_LIMIT=0

### -DEIGEN_STACK_ALLOCATION_LIMIT=0 flag required to set EIGEN_STACK_ALLOCATION_LIMIT for Eigen matrixes

##For openssl implementation

g++ -O2 ssl.cpp -lm -o output -lcrypto -lsodium -DEIGEN_STACK_ALLOCATION_LIMIT=0

## execution
./output
