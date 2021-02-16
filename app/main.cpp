#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <thread>

#include "page.hpp"
#include "blockid.hpp"
#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"

using namespace smartdb;
namespace fs = std::filesystem;

///////////////////////////////////////////////////////////////////////////////
//                               page.cpp test                               //
///////////////////////////////////////////////////////////////////////////////

void test_page_int() {
  page p1(sizeof(int));
  p1.set_int(0, 4);
  int num = p1.get_int(0);
  std::cout << num << std::endl;

  page p2(sizeof(int)-1);
  try {
    p2.set_int(0, 5);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  try {
    p2.get_int(0);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void test_page_bytes() {
  std::string s1 = "Hello World!";
  page p1(sizeof(int) + s1.size());
  std::vector<char> v1(s1.begin(), s1.end());
  p1.set_bytes(0, v1);
  std::vector <char> nv1 = p1.get_bytes(0);
  std::string ns1(nv1.begin(), nv1.end());
  std::cout << ns1 << std::endl;

  std::string s2 = "Tokyo City";
  page p2(sizeof(int)+s2.size()-1);
  std::vector<char> v2(s2.begin(), s2.end());
  try {
    p2.set_bytes(0, v2);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    p2.get_bytes(0);
  } catch (std::exception &e ) {
    std::cout << e.what() << std::endl;
  }
}

void test_page_string() {
  std::string s1 = "Test String";
  page p1(sizeof(int)+s1.size());
  p1.set_string(0, s1);
  std::cout << p1.get_string(0) << std::endl;

  std::string s2 = "Hogehoge";
  page p2(sizeof(int)+s2.size()-1);
  try {
    p2.set_string(0, s2);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << p2.get_string(0) << std::endl;
}

void test_page() {
  test_page_int();
  test_page_bytes();
  test_page_string();
}

///////////////////////////////////////////////////////////////////////////////
//                                blockid test                               //
///////////////////////////////////////////////////////////////////////////////

void test_blockid() {
  block_id block1("testfile", 2);
  block_id block2("testfile", 2);
  std::cout << block1.number() << std::endl;
  std::cout << block1.file_name() << std::endl;
  std::cout << block1.equals(block2) << std::endl;
  std::cout << block1.to_string() << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
//                              filemanager test                             //
///////////////////////////////////////////////////////////////////////////////

void test_filemanager_constructor() {
  const auto oldCurrentPath = fs::current_path();
  std::cout << oldCurrentPath <<  std::endl;
  const auto dirSandBox = oldCurrentPath / "sandbox";
  std::cout << dirSandBox << std::endl;
  file_manager fm(dirSandBox, 30);
  std::cout << fm.is_new() << std::endl; // 1
  std::cout << fm.block_size() << std::endl; // 30
  fs::remove_all(dirSandBox);
}

void test_filemanager_constructor2() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  fs::path binPath = dirSandBox / "temp.bin";
  fs::path binPath2 = dirSandBox / "hoge.bin";
  std::ofstream(binPath).put('a');
  std::ofstream(binPath2).put('a');
  file_manager fm(dirSandBox, 30);
  std::cout << fm.is_new() << std::endl; // 0
  for (auto &p: std::filesystem::directory_iterator(dirSandBox)) {
    std::cout << p.path() << std::endl; // path of hoge.bin
  }
  fs::remove_all(dirSandBox);
}

void test_filemanager_read() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string fileName = "hoge.bin";
  fs::path binPath = dirSandBox / fileName;
  std::fstream fileIO(binPath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
  std::string inputString = "Write this to file";
  fileIO.write(inputString.c_str(), inputString.size());
  fileIO.close();
  fileIO.flush();
  
  int blockSize = inputString.size();
  file_manager fm(dirSandBox, blockSize);
  block_id blockId(fileName, 0);
  page p(blockSize);
  fm.read(blockId, p);
  for (const auto &c : p.contents()) {
    std::cout << c;
  }
  std::cout << std::endl;
  fs::remove_all(dirSandBox);
}

void test_filemanager_read2() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string fileName = "hoge.bin";
  fs::path binPath = dirSandBox / fileName;
  std::fstream fileIO(binPath, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
  std::string inputString = "Write this to file";
  fileIO << inputString;
  fileIO.close();
  fileIO.flush();
  
  int blockSize = inputString.size();
  file_manager fm(dirSandBox, blockSize);
  block_id blockId(fileName, 1); // read less than a page
  page p(blockSize);
  fm.read(blockId, p);
  for (const auto &c : p.contents()) {
    std::cout << c;
  }
  std::cout << std::endl;
  std::cout << p.contents().size() << std::endl;
  fs::remove_all(dirSandBox);
}

void test_filemanager_write() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string fileName = "hoge.bin";
  std::string inputString = "Write this to file";

  int blockSize = inputString.size() + sizeof(int);
  file_manager fm(dirSandBox, blockSize);
  block_id blockId(fileName, 0);
  page p(blockSize);
  p.set_string(0, inputString);
  for (const auto &c: p.contents()) {
    std::cout << c;
  }
  std::cout << std::endl;
  fm.write(blockId, p);
  
  page pAfter(blockSize);
  fm.read(blockId, pAfter);
  for (const auto &c : pAfter.contents()) {
    std::cout << c;
  }
  std::cout << std::endl;
  fs::remove_all(dirSandBox);
}

void test_filemanager_write2() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  
  std::string fileName = "hoge.bin";
  std::string inputString = "Write this to file";

  int blockSize = inputString.size() + sizeof(int); // 22
  file_manager fm(dirSandBox, blockSize);
  block_id blockId(fileName, 1);
  page p(blockSize);
  p.set_string(0, inputString);
  for (const auto &c: p.contents()) {
    std::cout << c;
  }
  std::cout << std::endl;
  fm.write(blockId, p);
  
  page pAfter(blockSize);
  fm.read(blockId, pAfter);
  for (const auto &c : pAfter.contents()) {
    std::cout << c;
  }
  std::cout << std::endl;
  std::cout << fm.length(fileName) << std::endl; // 2
  fs::remove_all(dirSandBox);
}

void test_filemanager_append() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  
  std::string fileName = "hoge.bin";
  std::string inputString = "Write this to file"; // size 18

  int blockSize = inputString.size();
  file_manager fm(dirSandBox, blockSize);
  // std::cout << fm.length(fileName) << std::endl; // error
  fm.append(fileName);
  std::cout << fm.length(fileName) << std::endl; // 1
  fs::remove_all(dirSandBox);
}

void test_filemanager_multithread() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string fileName = "hoge.bin";
  std::string inputString = "Write this to file";

  int blockSize = inputString.size() + sizeof(int);

  file_manager fm(dirSandBox, blockSize);

  int numThreads = 5;
  
  std::vector<block_id> blockIds;
  std::vector<page> pages;
  
  for (int i = 0; i < numThreads; i ++) {
    block_id blockId(fileName, i);
    page p(blockSize);
    p.set_string(0, inputString);
    blockIds.emplace_back(blockId);
    pages.emplace_back(p);
  }

  std::vector<std::thread> threads;

  for (int i = 0; i < numThreads; i++) {
    threads.emplace_back(std::thread(&file_manager::write, &fm, std::ref(blockIds[i]), std::ref(pages[i])));
  }

  for (auto &th: threads) {
    th.join();
  }
  
  std::cout << fm.length(fileName) << std::endl;
  fs::remove_all(dirSandBox);
}


void test_filemanager() {
  test_filemanager_read();
  test_filemanager_read2();
  test_filemanager_write();
  test_filemanager_write2();
  test_filemanager_append();
  test_filemanager_multithread();
}

///////////////////////////////////////////////////////////////////////////////
//                                 File test                                 //
///////////////////////////////////////////////////////////////////////////////

// todo
void test_file() {
  return;
}

///////////////////////////////////////////////////////////////////////////////
//                              Logmanager test                              //
///////////////////////////////////////////////////////////////////////////////

void print_log_records(log_manager &lm) {
  log_manager::log_iterator iter = lm.iterator();
  while (iter.has_next()) {
    std::vector<char> rec = iter.next();
    page p(rec);
    std::string s = p.get_string(0);
    int npos = s.size();
    int val = p.get_int(npos);
    std::cout << "[" + s + "," + std::to_string(val) + "]";
  }
  std::cout << std::endl;
}

std::vector<char> create_log_record(std::string s, int n) {
  int npos = s.size();
  std::vector<char> rec(npos+sizeof(int), 0);
  page p(rec);
  p.set_string(0, s);
  p.set_int(npos, n);
  rec = p.contents();
  return rec;
}

void create_log_records(log_manager &lm, int start, int end) {
  std::cout << "creating records: ";
  for (int i = start; i < end; i++) {
    std::vector<char> rec = create_log_record("record"+std::to_string(i), i+100);
    int lsn = lm.append(rec);
    std::cout << lsn << " ";
  }
  std::cout << std::endl;
}

void test_logmanager() {
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string fileName = "hoge.bin";
  std::shared_ptr<file_manager> fm(new file_manager(dirSandBox, 300));
  log_manager lm(fm, fileName);
  create_log_records(lm, 1, 35);
  std::cout << "records 34 to 1" << std::endl;
  print_log_records(lm);
  create_log_records(lm, 36, 70);
  lm.flush(65);
  std::cout << "records 69 to 1" << std::endl;
  print_log_records(lm);
  fs::remove_all(dirSandBox);
}

void test_logmanager_append() {
  // check if flush is occured if more than a page is appended
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string fileName = "hoge.bin";

  std::string s1 = "hogehoge01";
  std::string s2 = "fugafuga02";
  std::string s3 = "fooobaar03";

  int recsize = s1.size() + sizeof(int);

  page p1(recsize);
  page p2(recsize);
  page p3(recsize);

  p1.set_string(0, s1);
  p2.set_string(0, s2);
  p3.set_string(0, s3);

  std::vector<char> rec1 = p1.contents();
  std::vector<char> rec2 = p2.contents();
  std::vector<char> rec3 = p3.contents();

  std::shared_ptr<file_manager> fm(new file_manager(dirSandBox, (recsize + sizeof(int))*2 + sizeof(int)));

  std::cout << "blocksize: " << fm->block_size() << std::endl;
  
  log_manager lm(fm, fileName);
  lm.append(rec1);
  std::cout << fm->length(fileName) << std::endl;
  lm.append(rec2);
  std::cout << fm->length(fileName) << std::endl;
  page pBefore(fm->block_size());
  block_id blockIdBefore(fileName, 0);
  fm->read(blockIdBefore, pBefore);
  for (auto &c: pBefore.contents()) {
    std::cout << c ;
  }
  std::cout << std::endl;
  // log_manager::log_iterator iter = lm.iterator();
  // while (iter.has_next()) {
  //   std::vector<char> rec = iter.next();
  //   page p(rec);
  //   std::string s = p.get_string(0);
  //   std::cout << "[" + s  + "]";
  // }
  // std::cout << std::endl;
  lm.append(rec3);
  std::cout << fm->length(fileName) << std::endl;
  std::cout << "block 0" << std::endl;
  page pAfter1(fm->block_size());
  block_id blockIdAfter1(fileName, 0);
  fm->read(blockIdAfter1, pAfter1);
  for (auto &c: pAfter1.contents()) {
    std::cout << c ;
  }
  std::cout << std::endl;

  std::cout << "block 1" << std::endl;
  page pAfter2(fm->block_size());
  block_id blockIdAfter2(fileName, 1);
  fm->read(blockIdAfter2, pAfter2);
  for (auto &c: pAfter2.contents()) {
    std::cout << c ;
  }
  std::cout << std::endl;
  fs::remove_all(dirSandBox);
}

///////////////////////////////////////////////////////////////////////////////
//                                buffer test                                //
///////////////////////////////////////////////////////////////////////////////



void test_buffermanager() {
  int blockSize = 30;
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string logFileName = "logfile";
  std::string testFileName ="testfile";
  std::shared_ptr<file_manager> fileManager(new file_manager(dirSandBox, blockSize));
  std::shared_ptr<log_manager> logManager(new log_manager(fileManager, logFileName));

  // 環境づくり
  for (int i = 0; i < 5; i++) {
    fileManager->append(testFileName);
  }

  std::cout << fileManager->length(testFileName) << std::endl;

  // pinしてmodifyしてunpinする
  buffer_manager bufferManager(fileManager, logManager, 3);
  std::shared_ptr<buffer> buff1 = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 1)));

  std::shared_ptr<page> p1 = buff1->contents();
  p1->set_int(10, 100);
  buff1->set_modified(1, 0);
  bufferManager.unpin(buff1);


  // one of these will flush buffer1 to disk
  std::shared_ptr<buffer> buff2 = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 2)));
  std::shared_ptr<buffer> buff3 = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 3)));
  std::shared_ptr<buffer> buff4 = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 4)));


  page pAfter(fileManager->block_size());
  block_id blockIdAfter(testFileName, 1);
  fileManager->read(blockIdAfter, pAfter);
  std::cout << pAfter.get_int(10) << std::endl;

  bufferManager.unpin(buff2);
  buff2 = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 1)));
  std::shared_ptr<page> p2 = buff2->contents();
  p2->set_int(10, 6);
  buff2->set_modified(1, 0);
  bufferManager.unpin(buff2);

  fs::remove_all(dirSandBox);
}

void test_buffermanager2() {
  int blockSize = 30;
  const auto oldCurrentPath = fs::current_path();
  const auto dirSandBox = oldCurrentPath / "sandbox";
  fs::remove_all(dirSandBox);
  fs::create_directory(dirSandBox);
  std::string logFileName = "logfile";
  std::string testFileName ="testfile";
  std::shared_ptr<file_manager> fileManager(new file_manager(dirSandBox, blockSize));
  std::shared_ptr<log_manager> logManager(new log_manager(fileManager, logFileName));

  // 環境づくり
  for (int i = 0; i < 5; i++) {
    fileManager->append(testFileName);
  }

  std::cout << fileManager->length(testFileName) << std::endl;

  buffer_manager bufferManager(fileManager, logManager, 3);
  std::vector<std::shared_ptr<buffer>> buff(6);
  buff[0] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 0)));
  buff[1] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 1)));
  buff[2] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 2)));
  bufferManager.unpin(buff[0]);
  buff[0] = std::shared_ptr<buffer>(nullptr);
  buff[3] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 0)));
  buff[4] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 1)));
  std::cout << "available: "<< bufferManager.available() << std::endl;

  try {
    std::cout << "Attempting to pin block3" << std::endl;
    buff[5] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 3)));
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  bufferManager.unpin(buff[2]);
  buff[2] = std::shared_ptr<buffer>(nullptr);
  buff[5] = bufferManager.pin(std::shared_ptr<block_id>(new block_id(testFileName, 3)));

  std::cout << "final buffer allocation" << std::endl;
  for (int i = 0; i < buff.size(); i++) {
    std::shared_ptr<buffer> b = buff[i];
    if (b) {
      std::cout << "buff[" << i <<  "] pinned to block" + std::to_string(b->block()->number()) << std::endl;
    }
  }
  fs::remove_all(dirSandBox);
}


int main() {
  // test_blockid();
  // test_page();
  // test_filemanager()
  // test_logmanager_append();
  test_buffermanager2();
  
}
