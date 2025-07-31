#include "../include/ReorderBuffer.hpp"
#include <iostream>

void ROB::Connect(RS *rs, LSB *lsb, Decoder *decoder, RegisterFile *rf, ProgramCounter *pc, Predictor *predictor, ALU *alu) {
  this->rs = rs;
  this->lsb = lsb;
  this->decoder = decoder;
  this->rf = rf;
  this->pc = pc;
  this->predictor = predictor;
  this->alu = alu;
}

void ROB::Update() {
  predict_failed = task.predict_failed;
  task.predict_failed = false;
  if (predict_failed) {
    std::cerr << "ROB: get predict failed signal\n";
    alu_compute_type = InstructionType::NONE;
    decoder_submit_type = InstructionType::NONE;
    load_rob_ind = -1;
  } else {
    alu_compute_type = task.alu_compute_type;
    alu_given_value = task.alu_given_value;
    alu_given_dependence = task.alu_given_dependence;
    if (alu_compute_type != InstructionType::NONE) {
      std::cerr << "ROB: get value " << std::dec << alu_given_value << " of index " << alu_given_dependence << '\n';
    }
    decoder_submit_type = task.decoder_submit_type;
    decoder_given_rd = task.decoder_given_rd;
    decoder_given_value = task.decoder_given_value;
    decoder_given_ready = task.decoder_given_ready;
    if (decoder_submit_type != InstructionType::NONE) {
      std::cerr << "ROB: get task:  rd: " << decoder_given_rd << ", value: " << decoder_given_value << ", is_ready: " << (decoder_given_ready ? "true" : "false") << '\n';
    }
    load_rob_ind = task.load_rob_ind;
    load_value = task.load_value;
    if (load_rob_ind != -1) {
      std::cerr << "ROB: get loaded data:  ind: " << load_rob_ind << ", value: " << load_value << '\n';
    }
  }
  task.alu_compute_type = InstructionType::NONE;
  task.decoder_submit_type = InstructionType::NONE;
  task.load_rob_ind = -1;
}

void ROB::SetPredictFault() {
  task.predict_failed = true;
}

void ROB::SetFromALU(InstructionType operation, int32_t value, int32_t rob_index) {
  task.alu_compute_type = operation;
  task.alu_given_value = value;
  task.alu_given_dependence = rob_index;
}

void ROB::SetFromDecoder(InstructionType instruction_type, int32_t rd, int32_t value, bool is_ready) {
  task.decoder_submit_type = instruction_type;
  task.decoder_given_rd = rd;
  task.decoder_given_value = value;
  task.decoder_given_ready = is_ready;
}

void ROB::GetLoadedData(int32_t rob_ind, int32_t value) {
  task.load_rob_ind = rob_ind;
  task.load_value = value;
}

void ROB::Run() {
  if (predict_failed) {
    rob_structure.head = 0;
    rob_structure.tail = 0;
    rob_structure.size = 0;
    return;
  }
  if (alu_compute_type != InstructionType::NONE) {
    if (alu_compute_type == InstructionType::BEQ || alu_compute_type == InstructionType::BGE
        || alu_compute_type == InstructionType::BGEU || alu_compute_type == InstructionType::BLT
        || alu_compute_type == InstructionType::BLTU || alu_compute_type == InstructionType::BNE) {
      if (alu_given_value == (rob_structure.rob_entries[alu_given_dependence].value & 1)) {
        rob_structure.rob_entries[alu_given_dependence].value = rob_structure.rob_entries[alu_given_dependence].value << 1 | 1;
      } else {
        rob_structure.rob_entries[alu_given_dependence].value = rob_structure.rob_entries[alu_given_dependence].value << 1;
      }
      rob_structure.rob_entries[alu_given_dependence].is_ready = true;
    } else {
      rob_structure.rob_entries[alu_given_dependence].value = alu_given_value;
      rob_structure.rob_entries[alu_given_dependence].is_ready = true;
    }
  }
  if (decoder_submit_type != InstructionType::NONE) {
    rob_structure.rob_entries[rob_structure.tail].type = decoder_submit_type;
    rob_structure.rob_entries[rob_structure.tail].rd = decoder_given_rd;
    rob_structure.rob_entries[rob_structure.tail].value = decoder_given_value;
    rob_structure.rob_entries[rob_structure.tail].is_ready = decoder_given_ready;
    ++rob_structure.tail;
    rob_structure.tail %= 32;
    ++rob_structure.size;
  }
  if (load_rob_ind != -1) {
    rob_structure.rob_entries[load_rob_ind].value = load_value;
    rob_structure.rob_entries[load_rob_ind].is_ready = true;
  }
  int32_t rob_ind[32], value[32], num = 0;
  for (int i = rob_structure.head; i != rob_structure.tail; ++i, i %= 32) {
    if (rob_structure.rob_entries[i].is_ready) {
      rob_ind[num] = i;
      value[num] = rob_structure.rob_entries[i].value;
      ++num;
    }
  }
  rs->GetROBTable(rob_ind, value, num);
  lsb->GetROBTable(rob_ind, value, num);
  while (rob_structure.rob_entries[rob_structure.head].is_ready) {
    ROBEntry first_entry = rob_structure.rob_entries[rob_structure.head];
    std::cerr << std::dec << "ROB: commit instruction at index " << rob_structure.head << ":  reg: " << first_entry.rd
        << ", type: " << first_entry.type << ", value: " << first_entry.value << '\n';
    if (first_entry.type == InstructionType::EXIT) {
      int32_t return_value = rf->GetData(10);
      std::cout << (return_value & 0x000000FF) << '\n';
      exit(0);
    }
    if (first_entry.type == InstructionType::BEQ || first_entry.type == InstructionType::BGE
        || first_entry.type == InstructionType::BGEU || first_entry.type == InstructionType::BLT
        || first_entry.type == InstructionType::BLTU || first_entry.type == InstructionType::BNE) {
      if ((first_entry.value & 1) == 1) {
        predictor->SubmitResult(first_entry.value >> 2, (first_entry.value & 3) == 0 || (first_entry.value & 3) == 3);
      } else {
        predictor->SubmitResult(first_entry.value >> 2, (first_entry.value & 3) == 0 || (first_entry.value & 3) == 3);
        this->SetPredictFault();
        rs->PredictFailed();
        lsb->PredictFailed();
        decoder->PredictFailed();
        rf->PredictFailed();
        alu->SetPredictFault();
        pc->PredictFailed(first_entry.rd);
      }
    } else {
      rf->SetFromROB(first_entry.type, first_entry.rd, first_entry.value, rob_structure.head);
      decoder->CommitMessageFromROB(rob_structure.head, first_entry.value);
    }
    rob_structure.head++;
    rob_structure.head %= 32;
    --rob_structure.size;
  }
  decoder->PassRobTail(rob_structure.tail);
  rs->PassROBTail(rob_structure.tail);
  lsb->PassROBTail(rob_structure.tail);
  if (rob_structure.size == 32) {
    decoder->ROBFull();
    pc->ROBFull();
  }
}