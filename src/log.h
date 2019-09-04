#pragma once

#include "kcc.h"

namespace kcc {
	template<class... Args>
	void log(const char* fmt, Args&&... args) {
		print(fmt, args...);
	}
}