#include "materialize/aggregationfn.hpp"
#include "plan/plan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class group_by_plan : public plan {
public:
  group_by_plan(transaction *pTx, const std::shared_ptr<plan> &pP,
                const std::vector<std::string> &pGroupFields,
                const std::vector<std::shared_ptr<aggregation_fn>> &pAffFns);
  std::shared_ptr<scan> open() override;
  int blocks_accessed() override;
  int records_output() override;
  int distinct_values(const std::string &pFldName) override;
  schema get_schema() override;

private:
  std::shared_ptr<plan> mP;
  std::vector<std::string> mGroupFields;
  std::vector<std::shared_ptr<aggregation_fn>> mAggFns;
  schema mSch;
};

} // namespace simpledb
