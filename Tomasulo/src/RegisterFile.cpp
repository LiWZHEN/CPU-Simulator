#include "../include/RegisterFile.hpp"
#include <iostream>

void RegisterFile::Connect(Decoder *decoder, LSB *lsb) {
  this->decoder = decoder;
  this->lsb = lsb;
}

void RegisterFile::Update() {
  predict_failed = task.predict_failed;
  ind_from_decoder = task.ind_from_decoder;
  dependence_from_decoder = task.dependence_from_decoder;
  task_from_rob_len = task.task_from_rob_len;
  for (int i = 0; i < task_from_rob_len; ++i) {
    task_from_rob[i] = task.task_from_rob[i];
  }
  task.task_from_rob_len = 0;
  task.predict_failed = false;
  task.ind_from_decoder = 0;
  task.dependence_from_decoder = -1;
}

void RegisterFile::SetNewDependence(int32_t rd, int32_t dependence) {
  task.ind_from_decoder = rd;
  task.dependence_from_decoder = dependence;
}

void RegisterFile::PredictFailed() {
  task.predict_failed = true;
}

void RegisterFile::Run() {
  for (int i = 0; i < task_from_rob_len; ++i) {
    for (int j = 0; j < 32; ++j) {
      if (dependence[j] == task_from_rob[i].rob_ind) {
        dependence[j] = -1;
      }
    }
    if (task_from_rob[i].type == InstructionType::SB
        || task_from_rob[i].type == InstructionType::SH
        || task_from_rob[i].type == InstructionType::SW) {
      lsb->SetStoreFromRF(task_from_rob[i].value, rf[task_from_rob[i].rd], task_from_rob[i].rob_ind);
    } else {
      rf[task_from_rob[i].rd] = task_from_rob[i].value;
    }
  }
  dependence[ind_from_decoder] = dependence_from_decoder;
  if (predict_failed) {
    for (int i = 0; i < 32; ++i) {
      dependence[i] = -1;
    }
  }
  decoder->PassRF(rf, dependence);
}

void RegisterFile::SetFromROB(InstructionType type, int32_t rd, int32_t value, int32_t rob_ind) {
  task.task_from_rob[task.task_from_rob_len++] = {type, rd, value, rob_ind};
}

int32_t RegisterFile::GetData(const int32_t ind) const {
  return rf[ind];
}