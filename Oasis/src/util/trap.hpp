#pragma once

#define OASIS_TRAP(cond) if(!cond) *((unsigned int*)0) = 0xDEAD;