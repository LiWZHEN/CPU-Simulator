#include "../include/ReservationStation.hpp"
#include <iostream>
#include <iomanip>

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
  task.predict_failed = false;
  entry_from_decoder = task.entry_from_decoder;
  type_from_alu = task.type_from_alu;
  value_from_alu = task.value_from_alu;
  rob_ind_from_alu = task.rob_ind_from_alu;
  loaded_ind = task.loaded_ind;
  loaded_value = task.loaded_value;
  rob_table_size = task.rob_table_size;
  for (int i = 0; i < rob_table_size; ++i) {
    rob_ind[i] = task.rob_ind[i];
    rob_value[i] = task.rob_value[i];
  }
  rob_tail = task.rob_tail;
  new_dependence_rd = task.new_dependence_rd;
  new_dependence_rob_ind = task.new_dependence_rob_ind;
  task.new_dependence_rd = -1;
  task.new_dependence_rob_ind = -1;
  rs1 = task.rs1;
  rs2 = task.rs2;
  task.rs1 = -1;
  task.rs2 = -1;
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
    if (rs1 != -1) {
      if (rs1 == new_dependence_rd) {
        entry_from_decoder.Q1 = new_dependence_rob_ind;
      }
    }
    if (rs2 != -1) {
      if (rs2 == new_dependence_rd) {
        entry_from_decoder.Q2 = new_dependence_rob_ind;
      }
    }
    for (int i = 0; i < 32; ++i) {
      if (!rs_entries[i].busy) {
        rs_entries[i] = entry_from_decoder;
        break;
      }
    }
  }

  if (type_from_alu != InstructionType::NONE) {
    for (int i = 0; i < 32; ++i) {
      if (!rs_entries[i].busy) {
        continue;
      }
      if (rs_entries[i].Q1 == rob_ind_from_alu) {
        rs_entries[i].V1 = value_from_alu;
        rs_entries[i].Q1 = -1;
      }
      if (rs_entries[i].Q2 == rob_ind_from_alu) {
        rs_entries[i].V2 = value_from_alu;
        rs_entries[i].Q2 = -1;
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
      if (rs_entries[i].Q2 == loaded_ind) {
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

void RS::SetNewDependence(int32_t rd, int32_t dependence) {
  task.new_dependence_rd = rd;
  task.new_dependence_rob_ind = dependence;
}

void RS::PassRS(int32_t rs1, int32_t rs2) {
  task.rs1 = rs1;
  task.rs2 = rs2;
}

void RS::Print() {
  std::cerr << "RS:\n";
  std::cerr << '|' << std::setw(8) << "type" << '|' << std::setw(8) << "target" << '|' << std::setw(8) << "V1"
      << '|' << std::setw(8) << "V2" << '|' << std::setw(4) << "Q1" << '|' << std::setw(4) << "Q2" << "|\n";
  for (int i = 0; i < 32; ++i) {
    if (!rs_entries[i].busy) {
      continue;
    }
    std::cerr << '|' << std::setw(8) << PrintType(rs_entries[i].type) << '|' << std::setw(8) << rs_entries[i].destination
        << '|' << std::setw(8) << rs_entries[i].V1 << '|' << std::setw(8) << rs_entries[i].V2 << '|' << std::setw(4) << rs_entries[i].Q1
        << '|' << std::setw(4) << rs_entries[i].Q2 << "|\n";
  }
  std::cerr << '\n';
}