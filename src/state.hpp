#pragma once

struct State{
	typedef enum{
		running = 0,
		reset,
		exit
	} type;
};
