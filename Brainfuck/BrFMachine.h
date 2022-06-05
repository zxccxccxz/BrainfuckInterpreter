#pragma once
#include <cstring>
#include <iostream>
#include <memory>
#include <forward_list>

class LoopExecutor {
public:
	LoopExecutor(char** data_ptr, char** instructions_ptr);
private:
	char** data_ptr_;
	char** instructions_ptr_;
};

class Command {
public:
	// Command() : ptr_(nullptr) {};
	Command(char** ptr) : ptr_(ptr) {}//*ptr_ = *ptr_ + 8; **ptr = 52; }

	void SetPtr(char** ptr) {
		ptr_ = ptr;
	}

	// virtual void Execute() = 0;
	void MoveToNext() {
		++* ptr_;
	}

	void MoveToPrev() {
		--* ptr_;
	}

	void IncrementData() {
		++** ptr_;
	}

	void DecrementData() {
		--** ptr_;
	}

	void PrintData() {
		putchar(**ptr_);
		//std::cout << **ptr_;
	}

	void GetData() {
		**ptr_ = std::move(getchar());
	}

	void LoopOpen(char** instructions_ptr) {
		loops_.emplace_front(ptr_, instructions_ptr);
	}

protected:
	char** ptr_;
	std::forward_list<LoopExecutor> loops_;
};

// Command loopCmds;

LoopExecutor::LoopExecutor(char** data_ptr, char** instructions_ptr) : data_ptr_(data_ptr), instructions_ptr_(instructions_ptr) {
	// if 0, goto next instruction after ']'
	if (!**data_ptr_) {
		char* start_ptr = *instructions_ptr;
		size_t other_loops_count = 0;

		std::string instructs(*instructions_ptr);

		size_t found_open = instructs.find_first_of("[", 1);
		size_t found_close = instructs.find_first_of("]", 1);

		while (found_open != std::string::npos) {
			if (found_open > found_close)
				break;

			other_loops_count++;

			found_close = instructs.find_first_of("]", found_close + 1);

			found_open = instructs.find_first_of("[", found_open + 1);
		}

		// if (std::regex_match(instructs, std::regex("(\\[.*\\])+[^\\]]+\\]")))

		// std::cout << "Other loops: " << other_loops_count << std::endl;

		*instructions_ptr += found_close;
	}
	else {
		char* instructions_ptr_return = *instructions_ptr;
		*instructions_ptr += 1;

		Command loopCmds(data_ptr);
		// loopCmds.SetPtr(data_ptr);

		while (**instructions_ptr != '\0') {
			switch (**instructions_ptr) {
			case '>':
				loopCmds.MoveToNext();
				break;
			case '<':
				loopCmds.MoveToPrev();
				break;
			case '+':
				loopCmds.IncrementData();
				break;
			case '-':
				loopCmds.DecrementData();
				break;
			case '.':
				loopCmds.PrintData();
				break;
			case ',':
				loopCmds.GetData();
				break;
			case '[':
				loopCmds.LoopOpen(instructions_ptr);
				break;
			case ']':
				if (**data_ptr == 0) {
					// ++* instructions_ptr;
					return;
				}
				else {
					*instructions_ptr = instructions_ptr_return;
				}

				break;
			default:
				break;
			}

			++* instructions_ptr;
		}
	}
}

class BrFMachine {
public:
	//code_ = new char[strlen(code) + 1];  std::memcpy(code_, code, strlen(code) + 1); }

	BrFMachine(std::string& code) : code_(code) {
		memset(mem_, 0, 30000);
	}

	~BrFMachine() noexcept {  }

	void Run() {
		std::cout << "run()" << std::endl;
		instruct_ptr = &code_[0];
		Command cmd(&ptr_);
		//loopCmds.SetPtr(&ptr_);

		while (*instruct_ptr != '\0') {
			//std::cout << *cur_cmd;

			switch (*instruct_ptr) {
				case '>':
					cmd.MoveToNext();
					break;
				case '<':
					cmd.MoveToPrev();
					break;
				case '+':
					cmd.IncrementData();
					break;
				case '-':
					cmd.DecrementData();
					break;
				case '.':
					cmd.PrintData();
					break;
				case ',':
					cmd.GetData();
					break;
				case '[':
					cmd.LoopOpen(&instruct_ptr);
					break;
				default:
					break;
			}

			++instruct_ptr;
		}
		std::cout << std::endl;
	}

	void PrintCode() {
		// std::cout << code_ << std::endl;
		std::cout << *ptr_ << std::endl;

		// Command cmd(&ptr_);
		// std::cout << *ptr_ << std::endl;
	}


private:
	std::string& code_;
	char mem_[30000] = { 0 };
	char* ptr_ = mem_;
	char* instruct_ptr;

};