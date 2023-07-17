
#include <iostream>
#include <string>
#include <random>
//#include <rocksdb/db.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <cstring>
 #include <fcntl.h>

#define unlikely(x)     __builtin_expect((x),0)

static constexpr size_t MAX_SIZE = 1000;

std::vector<std::pair<std::string, std::string>> generateRandomKVs(int keys, unsigned int seed) {
  std::vector<std::pair<std::string, std::string>> result(keys);
  std::mt19937_64 gen(seed);  
  
  uint64_t rawVal;
  int i = 0;
  for (auto& [key, val] : result) {
    val.resize(sizeof(uint64_t) + 1);
    rawVal = gen();
    std::memcpy((char*)val.data(), &rawVal, sizeof(uint64_t));
    val[sizeof(uint64_t) + 1] = '\x00';
    key = std::to_string(i);
    ++i;
  }
  return result;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <iterations> <seed>" << std::endl;
    return 1;
  }

  //auto start = std::chrono::steady_clock::now();
  int iterations = std::stoi(argv[1]);
  unsigned int seed = 1337;// std::stoi(argv[2]);
  std::string data{argv[2]};
  std::cout << argv[0] << ", data: " << data << std::endl; 
  /*  rocksdb::DB* db;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "/home/yflum/rock", &db);
  if (!status.ok()) {
    std::cerr << "Failed to open database: " << status.ToString() << std::endl;
    return 1;
  }*/

  /*std::cout << "Creating " << iterations << " kv pairs" << std::endl; 
  std::vector<std::pair<std::string, std::string>> kvs = generateRandomKVs(iterations, 1337);

  auto writeOptions = rocksdb::WriteOptions();
  auto end = std::chrono::steady_clock::now();
  auto ms_diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  auto diff = (5e6 - (ms_diff * 1000));
  std::cout << "Setup took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;*/


  // Sleep for five seconds for perf accuracy
  //usleep((5e6 - (ms_diff * 1000)));
  /*std::cout << "Starting measurements" << std::endl; 
  {
  std::ofstream fifo("./wat");
  if(! fifo.is_open() ){
        std::cout << " error : cannot open file " << std :: endl; 
        return 1;
   }
   fifo << "enable";
  }*/

  auto fd = open("testfile", O_WRONLY | O_CREAT);
  auto start = std::chrono::steady_clock::now();
  // Insert random string values
  /*for (auto& [key, value] : kvs) {
    rocksdb::Status putStatus = db->Put(writeOptions, key, value);
    if (unlikely(!putStatus.ok())) {
      std::cerr << "Failed to insert key " << key << ": " << putStatus.ToString() << std::endl;
      return 1;
    }
  }*/
  
  for (int i = 0; i < iterations; i++) {
     write(fd, (char*)data.data(), data.size());
  }

  auto end = std::chrono::steady_clock::now();
  std::cout << "Write benchmark took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
  start = std::chrono::steady_clock::now();
  fsync(fd);
  end = std::chrono::steady_clock::now();
  std::cout << "fsync took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

  close(fd);
  /*auto readOptions = rocksdb::ReadOptions();
  std::string returnedVal;
  for (auto& [k,v] : kvs) {
    auto getStatus = db->Get(readOptions, k, &returnedVal);
    if (!getStatus.ok() || status.IsNotFound()) {
      std::cerr << "Failed to retrieve key " << k << ": " << getStatus.ToString() << std::endl;
      return 1;
    }

    if (std::memcmp(v.data(), returnedVal.data(), v.size()) != 0) {
        std::cerr << "size: " << v.size() <<". inconsistent value, expected: " << ((uint8_t*)v.data())[0] << ", actual: " << ((uint8_t*)returnedVal.data())[0] << std::endl;
        return 1;
    }

  }*/

  //delete db;

  return 0;
}