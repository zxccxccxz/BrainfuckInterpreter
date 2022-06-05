#pragma once
#include <string>
#include <memory>
#include "BrFMachine.h"

class BrainfuckHandler {
public:
	BrainfuckHandler(const char* code) : code_(code) {
		brfm_ = new BrFMachine(code_);
	}

	~BrainfuckHandler() {
		delete brfm_;
	}

	void Run() {
		brfm_->Run();
	}

	void PrintCode() {
		std::cout << code_ << std::endl;
		brfm_->PrintCode();
	}

private:
	std::string code_;
	BrFMachine* brfm_;
};