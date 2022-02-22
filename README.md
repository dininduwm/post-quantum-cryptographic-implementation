# post-quantum-cryptographic-implementation
Latis based post quantum cryptographic algorithm implementation

## prerequisite
1. ### libsodium Library
    - installation:
        - #### on Ubuntu: sudo apt-get install -y libsodium-dev
        - #### on Mac:    brew install libsodium

## compilation
g++ input.cpp -o output -lsodium -DEIGEN_STACK_ALLOCATION_LIMIT=0

### -DEIGEN_STACK_ALLOCATION_LIMIT=0 flag required to set EIGEN_STACK_ALLOCATION_LIMIT for Eigen matrixes

## execution
./output
