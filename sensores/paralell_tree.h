#ifndef PARALELL_H
#define PARALELL_H

#include "sensor_tree.h"

class ParalellTree : public SensorTree {
 public:
  int contadorEstaciones;
  ParalellTree(const std::vector<double>&);
  double calculateMaxAverageInternal(SensorTree* node_ptr) override;
  double calculateMaxAverage() override;
  void insertInternal(SensorTree*, const std::vector<double>&) override;
  void insert(const std::vector<double>&) override;
};

#endif  // PARALELL_H
