#include "paralell_tree.h"

ParalellTree::ParalellTree(const std::vector<double>& data)
    : SensorTree(data), contadorEstaciones(1) {}

double ParalellTree::calculateMaxAverage() {
  return calculateMaxAverageInternal(this);
}

double ParalellTree::calculateMaxAverageInternal(SensorTree* node_ptr) {
  if(node_ptr == nullptr) return 0.0;

  // suma los datos del sensor en el nodo actual
  double sum = 0.0;
  int cont = 0;
  for(int value : node_ptr->sensor_data) {
    if(value > 0.0) {
      sum += value;
      cont += 1;
    }
  }
  // obtnemos promedio
  double current_avg = 0.0;
  if(cont > 0) current_avg = sum / (double)cont;

  double max_avg_left = 0.0, max_avg_right = 0.0;

#pragma omp parallel sections
  {
#pragma omp section
    {
      if(node_ptr->left != nullptr) {
        max_avg_left = calculateMaxAverageInternal(node_ptr->left);
      }
    }

#pragma omp section
    {
      if(node_ptr->right != nullptr) {
        max_avg_right = calculateMaxAverageInternal(node_ptr->right);
      }
    }
  }

  // retornamos el máximo del promedio del nodo y sus hijos
  return std::max(std::max(current_avg, max_avg_left), max_avg_right);
}

void ParalellTree::insert(const std::vector<double>& data) {
  insertInternal(this, data);
  contadorEstaciones++;
}

void ParalellTree::insertInternal(SensorTree* node_ptr,
                                  const std::vector<double>& data) {
  if(node_ptr == nullptr) {
    node_ptr = new ParalellTree(data);
    return;
  } else if(node_ptr->left == nullptr) {
    node_ptr->left = new ParalellTree(data);
    return;
  } else if(node_ptr->right == nullptr) {
    node_ptr->right = new ParalellTree(data);
    return;
  }

  if(node_ptr->left != nullptr) insertInternal(node_ptr->left, data);
  if(node_ptr->right != nullptr) insertInternal(node_ptr->right, data);
}
