#include "../include/ReservationStation.hpp"
#include <iostream>

void RS::Connect(ALU *alu) {
  this->alu = alu;
}

void RS::SetFromDecoder(InstructionType type, int32_t V1, int32_t V2, int32_t Q1, int32_t Q2, int32_t destination) {
  if (type == InstructionType::NONE) {
    task.entry_from_decoder = {false, type, V1, V2, Q1, Q2, destination};
  } else {
    task.entry_from_decoder = {true, type, V1, V2, Q1, Q2, destination};
  }
}

void RS::SetFromALU(InstructionType type, int32_t result, int32_t rob_ind) {
  task.type_from_alu = type;
  task.value_from_alu = result;
  task.rob_ind_from_alu = rob_ind;
}

void RS::GetLoadedData(int32_t rob_ind, int32_t value) {
  task.loaded_ind = rob_ind;
  task.loaded_value = value;
}

void RS::PredictFailed() {
  task.predict_failed = true;
}

void RS::GetROBTable(int32_t rob_index[], int32_t val[], int32_t size) {
  task.rob_table_size = size;
  for (int i = 0; i < size; ++i) {
    task.rob_ind[i] = rob_index[i];
    task.rob_value[i] = val[i];
  }
}

void RS::PassROBTail(int32_t tail) {
  task.rob_tail = tail;
}

void RS::Update() {
  predict_failed = task.predict_failed;
  if (predict_failed) {
    std::cerr << "RS: get predict failed signal\n";
  }
  task.predict_failed = false;
  entry_from_decoder = task.entry_from_decoder;
  if (entry_from_decoder.type != InstructionType::NONE) {
    std::cerr << std::dec << "RS: get from decoder:  busy: " << (entry_from_decoder.busy ? "true" : "false") << ", type: " << Print(entry_from_decoder.type)
        << ", V1: " << (entry_from_decoder.Q1 == -1 ? entry_from_decoder.V1 : 111111111111111) << ", V2: " << (entry_from_decoder.Q2 == -1 ? entry_from_decoder.V2 : 111111111111111) << '\n';
  }
  type_from_alu = task.type_from_alu;
  value_from_alu = task.value_from_alu;
  rob_ind_from_alu = task.rob_ind_from_alu;
  if (type_from_alu != InstructionType::NONE) {
    std::cerr << std::dec << "RS: get from alu:  type: " << Print(type_from_alu)
        << ", value: " << value_from_alu << ", index: " << rob_ind_from_alu << '\n';
  }
  loaded_ind = task.loaded_ind;
  loaded_value = task.loaded_value;
  if (loaded_ind != -1) {
    std::cerr << std::dec << "RS: get loaded data:  ind: " << loaded_ind << ", value: " << loaded_value << '\n';
  }
  rob_table_size = task.rob_table_size;
  for (int i = 0; i < rob_table_size; ++i) {
    rob_ind[i] = task.rob_ind[i];
    rob_value[i] = task.rob_value[i];
  }
  rob_tail = task.rob_tail;
  std::cerr << std::dec << "RS: get rob tail: " << rob_tail << '\n';

  task.entry_from_decoder.type = InstructionType::NONE;
  task.type_from_alu = InstructionType::NONE;
  task.loaded_ind = -1;
  task.rob_table_size = 0;
}

void RS::Run() {
  if (predict_failed) {
    for (int i = 0; i < 32; ++i) {
      rs_entries[i].busy = false;
    }
    alu->SetTask(-1, InstructionType::NONE, 0, 0);
    return;
  }
  if (entry_from_decoder.type != InstructionType::NONE) {
    if (entry_from_decoder.Q1 != -1) {
      for (int i = 0; i < rob_table_size; ++i) {
        if (rob_ind[i] == entry_from_decoder.Q1) {
          entry_from_decoder.Q1 = -1;
          entry_from_decoder.V1 = rob_value[i];
          break;
        }
      }
    }
    if (entry_from_decoder.Q2 != -1) {
      for (int i = 0; i < rob_table_size; ++i) {
        if (rob_ind[i] == entry_from_decoder.Q2) {
          entry_from_decoder.Q2 = -1;
          entry_from_decoder.V2 = rob_value[i];
          break;
        }
      }
    }
    entry_from_decoder.destination = rob_tail;
    for (int i = 0; i < 32; ++i) {
      if (!rs_entries[i].busy) {
        rs_entries[i] = entry_from_decoder;
        break;
      }
    }
  }

  if (loaded_ind != -1) {
    for (int i = 0; i < 32; ++i) {
      if (!rs_entries[i].busy) {
        continue;
      }
      if (rs_entries[i].Q1 == loaded_ind) {
        rs_entries[i].Q1 = -1;
        rs_entries[i].V1 = loaded_value;
      }
      if (rs_entries[i].busy == loaded_ind) {
        rs_entries[i].Q2 = -1;
        rs_entries[i].V2 = loaded_value;
      }
    }
  }
  for (int i = 0; i < 32; ++i) {
    if (!rs_entries[i].busy) {
      continue;
    }
    if (rs_entries[i].Q1 == -1 && rs_entries[i].Q2 == -1) {
      alu->SetTask(rs_entries[i].destination, rs_entries[i].type, rs_entries[i].V1, rs_entries[i].V2);
      rs_entries[i].busy = false;
      return;
    }
  }
  alu->SetTask(0, InstructionType::NONE, 0, 0);
  
}