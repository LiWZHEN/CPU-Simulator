#include "../include/LoadStoreBuffer.hpp"
#include <iostream>

void LSB::Connect(Memory *memory) {
  this->memory = memory;
}

void LSB::SetFromDecoder(InstructionType type, int32_t address, int32_t cycle, int32_t rob_index, bool is_committed) {
  task.type_from_decoder = type;
  task.address_from_decoder = address;
  task.cycle_from_decoder = cycle;
  task.rob_index_from_decoder = rob_index;
  task.committed_from_decoder = is_committed;
}

void LSB::SetFromALU(InstructionType type, int32_t result, int32_t rob_ind) {
  task.type_from_alu = type;
  task.result_from_alu = result;
  task.rob_ind_from_alu = rob_ind;
}

void LSB::SetStoreFromRF(int32_t address, int32_t value, int32_t rob_ind) {
  task.store_from_rf[task.store_rf_num++] = {address, value, rob_ind};
}

void LSB::GetLoadedData(int32_t rob_ind, int32_t value) {
  task.loaded_rob_ind = rob_ind;
  task.loaded_value = value;
}

void LSB::PredictFailed() {
  task.predict_failed = true;
}

void LSB::GetROBTable(int32_t rob_ind[], int32_t value[], int32_t size) {
  task.size = size;
  for (int i = 0; i < size; ++i) {
    task.rob_ind[i] = rob_ind[i];
    task.value[i] = value[i];
  }
}

void LSB::PassROBTail(int32_t tail) {
  task.rob_tail = tail;
}

void LSB::Update() {
  predict_failed = task.predict_failed;
  if (predict_failed) {
    std::cerr << "LSB: get predict failed signal\n";
  }
  task.predict_failed = false;
  type_from_decoder = task.type_from_decoder;
  address_from_decoder = task.address_from_decoder;
  cycle_from_decoder = task.cycle_from_decoder;
  committed_from_decoder = task.committed_from_decoder;
  if (type_from_decoder != InstructionType::NONE) {
    std::cerr << std::dec << "LSB: get from decoder:  type: " << Print(type_from_decoder)
        << ", address: " << address_from_decoder << ", cycle: " << cycle_from_decoder
        << ", committed: " << (committed_from_decoder ? "true" : "false") << '\n';
  }
  type_from_alu = task.type_from_alu;
  result_from_alu = task.result_from_alu;
  rob_ind_from_alu = task.rob_ind_from_alu;
  if (type_from_alu != InstructionType::NONE) {
    std::cerr << std::dec << "LSB: get from alu:  type: " << Print(type_from_alu)
        << ", result: " << result_from_alu << ", rob index: " << rob_ind_from_alu << '\n';
  }
  store_rf_num = task.store_rf_num;
  for (int i = 0; i < store_rf_num; ++i) {
    store_from_rf[i] = task.store_from_rf[i];
  }
  loaded_rob_ind = task.loaded_rob_ind;
  loaded_value = task.loaded_value;
  if (loaded_rob_ind != -1) {
    std::cerr << std::dec << "LSB: get loaded data:  rob index: " << loaded_rob_ind << ", value: " << loaded_value << '\n';
  }
  size = task.size;
  for (int i = 0; i < size; ++i) {
    rob_ind[i] = task.rob_ind[i];
    value[i] = task.value[i];
  }
  rob_tail = task.rob_tail;

  task.type_from_decoder = InstructionType::NONE;
  task.type_from_alu = InstructionType::NONE;
  task.store_rf_num = 0;
  task.size = 0;
  task.loaded_rob_ind = -1;
  task.rob_tail = 0;
}

void LSB::Run() {
  if (predict_failed) {
    task.type_from_decoder = InstructionType::NONE;
    task.type_from_alu = InstructionType::NONE;
    task.store_rf_num = 0;
    task.size = 0;
    task.loaded_rob_ind = -1;
    task.rob_tail = 0;
    for (int i = lsb_structure.head; i != lsb_structure.tail; ++i, i %= 32) {
      if (!lsb_structure.lsb_entries[i].committed) {
        lsb_structure.tail = i;
        break;
      }
    }
  }
  if (type_from_decoder != InstructionType::NONE) {
    lsb_structure.lsb_entries[lsb_structure.tail] = {type_from_decoder, -1, -1, rob_tail, false};
    lsb_structure.tail = (lsb_structure.tail + 1) % 32;
  }
  if (type_from_alu != InstructionType::NONE) {
    for (int i = lsb_structure.head; i != lsb_structure.tail; ++i, i %= 32) {
      if (!lsb_structure.lsb_entries[i].committed && lsb_structure.lsb_entries[i].rob_ind == rob_ind_from_alu) {
        lsb_structure.lsb_entries[i].address = result_from_alu;
        lsb_structure.lsb_entries[i].committed = true;
      }
    }
  }
  for (int i = 0; i < store_rf_num; ++i) {
    StoreFromRF unit_task = store_from_rf[i];
    for (int i = lsb_structure.head; i != lsb_structure.tail; ++i, i %= 32) {
      if (!lsb_structure.lsb_entries[i].committed && lsb_structure.lsb_entries[i].rob_ind == unit_task.rob_ind) {
        lsb_structure.lsb_entries[i].address = unit_task.address;
        lsb_structure.lsb_entries[i].rob_ind = unit_task.value;
        lsb_structure.lsb_entries[i].committed = true;
      }
    }
  }
  if (lsb_structure.head == lsb_structure.tail || !lsb_structure.lsb_entries[lsb_structure.head].committed) {
    return;
  }

  if (lsb_structure.lsb_entries[lsb_structure.head].cycle == -1) {
    lsb_structure.lsb_entries[lsb_structure.head].cycle = 2;
  } else {
    --lsb_structure.lsb_entries[lsb_structure.head].cycle;
    if (lsb_structure.lsb_entries[lsb_structure.head].cycle == 0) {
      std::cerr << "task set!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << Print(lsb_structure.lsb_entries[lsb_structure.head].type) << "\n";
      memory->SetTask(lsb_structure.lsb_entries[lsb_structure.head].type,
            lsb_structure.lsb_entries[lsb_structure.head].address, lsb_structure.lsb_entries[lsb_structure.head].rob_ind);
      lsb_structure.head = (lsb_structure.head + 1) % 32;
    }
  }
}