#pragma once

#include <memory>

#include "query/constant.hpp"
#include "query/predicate.hpp"
#include "query/scan.hpp"
#include "query/updatescan.hpp"

namespace simpledb {
class select_scan : public update_scan {
public:
  select_scan(const std::shared_ptr<scan> &pS, const predicate &pP);

  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

  void set_int(const std::string &pFldName, int pVal) override;
  void set_string(const std::string &pFldName,
                  const std::string &pVal) override;
  void set_val(const std::string &pFldName, const constant &pVal) override;
  void insert() override;
  void remove() override;
  rid get_rid() override;
  void move_to_rid(const rid &pRID) override;

private:
  std::shared_ptr<scan> mS;
  predicate mP;
};
} // namespace simpledb
